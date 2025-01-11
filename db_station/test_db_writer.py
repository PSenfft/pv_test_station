import influxdb_client, os, time
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS


# Replace the following with your InfluxDB connection details
url = "http://raspberrypi.local:8086"                                                                    # InfluxDB URL
token = "ZMaJlSEokqxFana4X3JT8cSP5T3iUQ1VQn67ql4mGhA8Rb8Wmtr4PQn-Rpu8cYKJj7uSuqHcfY7hX8yEIagwQw=="      # InfluxDB token
org = "pv_test_station"                                                                                 # Your organization name                                                                                      # Your bucket name
bucket="testbucket"

# Initialize InfluxDB client
client = InfluxDBClient(url=url, token=token, org=org)

write_api = client.write_api(write_options=SYNCHRONOUS)
   
for value in range(5):
  point = (
    Point("measurement1")
    .tag("tagname1", "tagvalue1")
    .field("field1", value)
  )
  write_api.write(bucket=bucket, org="pv_test_station", record=point)
  time.sleep(1) # separate points by 1 second