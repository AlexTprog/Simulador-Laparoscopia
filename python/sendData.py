import requests
import serial
import time

# Constantes
URL = ""
HEAD = {"Content-Type": "application/json"}

# Puertos seriales
sensors = {
    "gas": serial.Serial('COM5', 9600, timeout=1),
    "distance": serial.Serial('COM6', 9600, timeout=1),
    "heartbeat": serial.Serial('COM7', 9600, timeout=1),
    "temperature": serial.Serial('COM8', 9600, timeout=1)
}


def genBody():
    body = {}
    data = getData()

    for key, value in data.items():
        if value != 0:
            body[key] = value

    print(body)
    return body


def readSensor(sensor):
    try:
        data = sensor.readline().decode().strip()
        return float(data) if data else 0
    except:
        return 0


def getData():
    data = {}
    for key, sensor in sensors.items():
        data[key] = readSensor(sensor)
    return data


if __name__ == "__main__":
    while True:
        try:
            r = requests.post(URL, json=genBody(), headers=HEAD)
            print(f"Status ->{r.status_code}")
        except Exception as e:
            print(f"Error: {str(e)}")

        time.sleep(1)
