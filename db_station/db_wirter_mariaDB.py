import struct
import mysql.connector
from mysql.connector import Error
import random
import datetime

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


def create_connection(host_name, user_name, user_password, db_name):
    """Create a database connection to a MariaDB database."""
    connection = None
    try:
        connection = mysql.connector.connect(
            host=host_name,
            user=user_name,
            password=user_password,
            database=db_name,
            port=3306
        )
        print("Connection to MariaDB successful")
    except Error as e:
        print(f"Error: '{e}'")

    return connection


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


def insert_data(connection, temp_panel, temp_env, voltage, current, timestamp):
    """Insert example data into the table."""
    insert_query = """
    INSERT INTO timestamp (timestamp, temp_panel, temp_env, voltage, current)
    VALUES (%s, %s, %s, %s, %s);
    """

    # In ein datetime-Objekt konvertieren
    dt_object = datetime.datetime.fromtimestamp(i)

    # Convert to SQL-Format (YYYY-MM-DD HH:MM:SS)
    sql_timestamp = dt_object.strftime('%Y-%m-%d %H:%M:%S')

    measure_package = [(sql_timestamp, temp_panel, temp_env, voltage, current)]

    cursor = connection.cursor()
    try:
        cursor.executemany(insert_query, measure_package)
        connection.commit()
        print("Data inserted successfully")
    except Error as e:
        print(f"Error: '{e}'")



if __name__ == "__main__":
    # Change these credentials to match your MariaDB setup
    host_name = "localhost"
    user_name = "root"
    user_password = "example"
    db_name = "data_measurements"

    connection = create_connection(host_name, user_name, user_password, db_name)

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

            if connection is not None:
                insert_data(connection, temp_panel, temp_env, voltage, current, timestamp)
                connection.close()
            else:
                print("Failed to create database connection.")
            
        display.show()
        time.sleep(0.1)
