import influxdb_client
from influxdb_client import Point, WritePrecision  # Add WritePrecision import
from influxdb_client.client.write_api import SYNCHRONOUS
import time
# Import Python System Libraries
import time
# Import Blinka Libraries
import busio
from digitalio import DigitalInOut, Direction, Pull
import board
# Import the SSD1306 module.
import adafruit_ssd1306
# Import RFM9x
import adafruit_rfm9x

# Initialize InfluxDBClient
token = os.environ.get("INFLUXDB_TOKEN")
org = "pv_test_station"
url = "http://raspberrypi.local:8086"

client = influxdb_client.InfluxDBClient(url=url, token=token, org=org)

# Define bucket
bucket = "pv_db"

# Write data
write_api = client.write_api(write_options=SYNCHRONOUS)


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
rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, 915.0)
rfm9x.tx_power = 23
prev_packet = None

display.fill(0)             #clear display 


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
    packet = None

    # check for packet rx
    packet = rfm9x.receive()
    if packet is None:
        break
    else:
        data = packet.split(",")
        temp_env = data[0]
        temp_panel = data[1]
        ampere = data[2]
        voltage = data[3]
        timestamp = data[4]

        write_to_db(temp_env, temp_panel, ampere, voltage, timestamp)

    display.show()
    time.sleep(0.1)
