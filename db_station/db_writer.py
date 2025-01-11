import struct
import datetime
import time
from influxdb_client import InfluxDBClient, Point, WritePrecision, WriteOptions

# Import Python System Libraries
import time
# Import Blinka Libraries
import busio
from digitalio import DigitalInOut, Direction, Pull
import board
# Import the SSD1306 module.
import adafruit_ssd1306
# Import the RFM69 radio module.
import adafruit_rfm9x


# Replace the following with your InfluxDB connection details
url = "http://raspberrypi.local:8086"                                                                    # InfluxDB URL
token = "ZMaJlSEokqxFana4X3JT8cSP5T3iUQ1VQn67ql4mGhA8Rb8Wmtr4PQn-Rpu8cYKJj7uSuqHcfY7hX8yEIagwQw=="      # InfluxDB token
org = "pv_test_station"                                                                                 # Your organization name
bucket = "testbucket"                                                                                        # Your bucket name

# Initialize InfluxDB client
client = InfluxDBClient(url=url, token=token, org=org)

# Use synchronous write for simplicity
write_api = client.write_api(write_options=WriteOptions(batch_size=1))


def write_to_db(package):
    # Parse the packet
    # Adjust the format string if data types or endianness differ
    temp_panel, temp_env, voltage, current, timestamp = struct.unpack('>ffHHI', package)

    # Convert timestamp to datetime object
    timestamp_datetime = datetime.datetime.utcfromtimestamp(timestamp)

    # Create points for each measurement
    points = []

    # TempPanel
    point_temp_panel = Point("TempPanel") \
        .field("value", temp_panel) \
        .time(timestamp_datetime, WritePrecision.S)
    points.append(point_temp_panel)

    # TempEnv
    point_temp_env = Point("TempEnv") \
        .field("value", temp_env) \
        .time(timestamp_datetime, WritePrecision.S)
    points.append(point_temp_env)

    # Voltage
    point_voltage = Point("Voltage") \
        .field("value", voltage) \
        .time(timestamp_datetime, WritePrecision.S)
    points.append(point_voltage)

    # Current
    point_current = Point("Current") \
        .field("value", current) \
        .time(timestamp_datetime, WritePrecision.S)
    points.append(point_current)

    # Write the points to InfluxDB
    write_api.write(bucket=bucket, org=org, record=points)

    # Close the client
    client.close()

    print("Data written to InfluxDB successfully.")


##Button init (unnecessary)
# Button A
btnA = DigitalInOut(board.D5)
btnA.direction = Direction.INPUT
btnA.pull = Pull.UP

# Button B
btnB = DigitalInOut(board.D6)
btnB.direction = Direction.INPUT
btnB.pull = Pull.UP

# Button C
btnC = DigitalInOut(board.D12)
btnC.direction = Direction.INPUT
btnC.pull = Pull.UP

# Create the I2C interface.
i2c = busio.I2C(board.SCL, board.SDA)

# 128x32 OLED Display
reset_pin = DigitalInOut(board.D4)
display = adafruit_ssd1306.SSD1306_I2C(128, 32, i2c, reset=reset_pin)
# Clear the display.
display.fill(0)
display.show()
width = display.width
height = display.height

# Configure LoRa Radio
CS = DigitalInOut(board.CE1)
RESET = DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, 434.0)
rfm9x.tx_power = 23
prev_packet = None


def write_to_db(temp_env, temp_panel, ampere, voltage, timestamp):
    # Create a point with fields
    point = (
        Point("solar_measurements")
        .field("temp_env", temp_env)
        .field("temp_panel", temp_panel)
        .field("ampere", ampere)
        .field("voltage", voltage)
        .time("timestamp", timestamp)
    )
    
    # Write the point to the bucket
    write_api.write(bucket=bucket, org=org, record=point)


while True:
    package = None
    # draw a box to clear the image
    display.fill(0)
    display.text('RasPi LoRa', 35, 0, 1)

    # check for packet rx
    package = rfm9x.receive()
    if package is None:
        display.show()
        display.text('- Waiting for PKT -', 15, 20, 1)
    else:
        # Display the packet text and rssi
        display.fill(0)
        prev_packet = package
        packet_text = prev_packet.hex()
        display.text('RX: ', 0, 0, 1)
        display.text(packet_text, 25, 0, 1)

        # Parse the packet
        # Adjust the format string if data types or endianness differ
        temp_panel, temp_env, voltage, current, timestamp = struct.unpack('>ffHHI', package)

        # Convert timestamp to datetime object
        timestamp_datetime = datetime.datetime.utcfromtimestamp(timestamp)

        # Create points for each measurement
        points = []

                # TempPanel
        point_temp_panel = Point("TempPanel") \
            .field("value", temp_panel) \
            .time(timestamp_datetime, WritePrecision.S)
        points.append(point_temp_panel)

        # TempEnv
        point_temp_env = Point("TempEnv") \
            .field("value", temp_env) \
            .time(timestamp_datetime, WritePrecision.S)
        points.append(point_temp_env)

        # Voltage
        point_voltage = Point("Voltage") \
            .field("value", voltage) \
            .time(timestamp_datetime, WritePrecision.S)
        points.append(point_voltage)

        # Current
        point_current = Point("Current") \
            .field("value", current) \
            .time(timestamp_datetime, WritePrecision.S)
        points.append(point_current)

        # Write the points to InfluxDB
        write_api.write(bucket=bucket, org=org, record=points)

        # Close the client
        client.close()  
        time.sleep(1)

    display.show()
    time.sleep(0.1)
