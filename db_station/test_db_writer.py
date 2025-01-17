import mysql.connector
from mysql.connector import Error
import random
import datetime

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

def insert_data(connection):
    """Insert example data into the table."""
    insert_query = """
    INSERT INTO data_measurements (timestamp, temp_panel, temp_env, voltage, current)
    VALUES (%s, %s, %s, %s, %s);
    """

    for i in range(1737017914, 1737019000):

        # In ein datetime-Objekt konvertieren
        dt_object = datetime.datetime.fromtimestamp(i)

        # In das SQL-Format (YYYY-MM-DD HH:MM:SS) umwandeln
        sql_timestamp = dt_object.strftime('%Y-%m-%d %H:%M:%S')

        temp_panel = random.uniform(21, 23.25)
        temp_env = 0
        voltage = random.uniform(0, 43.000)
        current = random.uniform(0, 30.000)

        example_data = [
            (sql_timestamp, temp_panel, temp_env, voltage, current),
        ]

        cursor = connection.cursor()
        try:
            cursor.executemany(insert_query, example_data)
            connection.commit()
            print("Data inserted successfully")
        except Error as e:
            print(f"Error: '{e}'")

if __name__ == "__main__":
    # Change these credentials to match your MariaDB setup
    host_name = "raspberrypi"
    user_name = "root"
    user_password = "example"
    db_name = "pv_measurements_db"

    connection = create_connection(host_name, user_name, user_password, db_name)

    if connection is not None:
        insert_data(connection)
        connection.close()
    else:
        print("Failed to create database connection.")