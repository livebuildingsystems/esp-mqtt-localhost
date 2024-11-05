import paho.mqtt.client as mqtt

# MQTT broker details
broker = "broker.emqx.io"  # Replace with your broker
port = 1883
topic_credentials = "test/test"  # The topic you want to subscribe to

# Callback function for when the client receives a CONNACK response from the server
def on_connect(client, userdata, flags, rc):
    print(f"Connected to MQTT Broker with result code: {rc}")
    client.subscribe(topic_credentials)  # Subscribe to the topic

# Callback function for when a message is received
def on_message(client, userdata, message):
    print(f"Received message: {message.payload.decode()} on topic {message.topic}")

# Create MQTT client and set up callbacks
mqtt_client = mqtt.Client()
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

# Connect to the MQTT broker
mqtt_client.connect(broker, port, 60)
mqtt_client.loop_start()  # Start the loop to process callbacks

# Keep the script running
try:
    while True:
        pass  # Keep the program running to listen for messages
except KeyboardInterrupt:
    print("Exiting...")
finally:
    mqtt_client.loop_stop()  # Stop the loop
    mqtt_client.disconnect()  # Disconnect from the broker
