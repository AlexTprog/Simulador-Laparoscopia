import requests
import json
import serial

# Constantes
URL = "https://demo.thingsboard.io/api/v1/jJnRqwuzhBWo0iIeRGgD/telemetry"
HEAD = {"Content-Type": "application/json"}
# conexion con puertos seriales
GAS = serial.Serial('COM5', 9600, timeout=1)
DISTANCE = serial.Serial('COM6', 9600, timeout=1)
HEARTBEAT = serial.Serial('COM7', 9600, timeout=1)
TEMPERATURE = serial.Serial('COM8', 9600, timeout=1)


def genBody():
    body = {}
    distance, gas, heartbit, temperature = getData()

    if (distance != 0):
        body["Distancia"] = distance

    if (gas != 0):
        body["CO2"] = gas

    if (heartbit != 0):
        body["Latidos"] = heartbit

    if (temperature != 0):
        body["Temperatura"] = temperature

    print(body)
    return body


def readSensor(sensor):
    data = sensor.readline().decode()
    if data == "":
        return 0
    else:
        return float(data)
    # return 0 if sensor.readline().decode() == "" else float(sensor.readline().decode())


def getData():
    gas = readSensor(GAS)
    distancia = readSensor(DISTANCE)/10
    latidos = readSensor(HEARTBEAT)
    temperatura = readSensor(TEMPERATURE)
    return distancia, gas, latidos, temperatura


if __name__ == "__main__":
    while True:
        r = requests.post(URL, json.dumps(genBody()), HEAD)
        print(f"Status ->{r.status_code}")
