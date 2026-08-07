from flask import Flask, render_template
from flask_socketio import SocketIO, emit

import json
import paho.mqtt.client as mqtt

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*", async_mode="threading")

@app.route('/')
def index():
    return render_template('dashboard.html')

def on_message(client, userdata, message):
    payload = message.payload.decode()
    data = json.loads(payload)
    print(data)
    socketio.emit("sensor_update",data)

subscriber = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

subscriber.on_message = on_message

subscriber.connect(host= '10.163.98.34',port= 1883)

subscriber.subscribe(topic= 'airsense/data')

subscriber.loop_start()

if __name__ == '__main__':
    socketio.run(app, debug=True)


