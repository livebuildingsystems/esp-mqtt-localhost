from flask import Flask, request, jsonify, render_template
import paho.mqtt.client as mqtt_client

# Initialize Flask app
app = Flask(__name__)

# Route to serve HTML form
@app.route('/')
def index():
    return render_template('localhosthing.html')

# Route to receive credentials
@app.route('/send_creds', methods=['POST'])
def send_credentials():
    data = request.json
    print("Received data:", data)  # Debug print

    if data is None:
        print("Error: No JSON data received")
        return jsonify({"error": "No JSON data received"}), 400
    
    mqtt_creds = {
        "broker": data.get('mqttBroker'),
        "port": int(data.get('mqttPort', 1883)),  # Ensure port is an integer
        "topic": data.get('topic'),
        "username": data.get('username'),
        "password": data.get('password'),
    }

    # MQTT client setup
    mqtt_client_instance = mqtt_client.Client()

    try:
        # mqtt_client_instance.username_pw_set(mqtt_creds['username'], mqtt_creds['password'])
        
        mqtt_client_instance.connect(mqtt_creds['broker'], mqtt_creds['port'], 60)
        mqtt_client_instance.loop_start()
    except Exception as e:
        print(f"Error connecting to MQTT broker: {e}")
        return jsonify({"error": "Failed to connect to MQTT broker."}), 500

    # Publish credentials to the MQTT broker
    message = data.get('controlCommand')  # Convert the data dictionary to a string
    print("Publishing message:", message)  # Debug print

    result = mqtt_client_instance.publish(mqtt_creds['topic'], message)
    print("Publish result:", result.rc)  # Log the result of the publish attempt

    if result.rc == 0:
        print("Message published successfully!")
    else:
        print("Failed to publish message.")

    return jsonify({"message": "Credentials sent to broker!"})

# Start the Flask app
if __name__ == "__main__":
    app.run(host="localhost", port=5000, debug=True)
