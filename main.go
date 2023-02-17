package main

import (
    "fmt"
    "time"

    MQTT "github.com/eclipse/paho.mqtt.golang"
)

const (
    TOPIC_ON_OFF_LED          = "topic_on_off_led"
    TOPIC_PUBLISH_TEMPERATURE = "topic_sensor_temperature"
    TOPIC_PUBLISH_HUMIDITY    = "topic_sensor_humidity"
)

func main() {
    // Create a new MQTT client with the broker address and a client ID
    opts := MQTT.NewClientOptions().AddBroker("tcp://broker.hivemq.com:1883")
    opts.SetClientID("go-mqtt-publisher")
    client := MQTT.NewClient(opts)

    // Connect to the MQTT broker and check for errors
    if token := client.Connect(); token.Wait() && token.Error() != nil {
        panic(token.Error())
    }

    // Print a message when the client is connected
    fmt.Println("Connected to broker.hivemq.com")

    // Publish "1" and "0" messages to the "topic_on_off_led" topic, with a 5-second interval
    go func() {
        for {
            client.Publish(TOPIC_ON_OFF_LED, 0, false, "1")
            fmt.Println("LED ligado")
            time.Sleep(5 * time.Second)

            client.Publish(TOPIC_ON_OFF_LED, 0, false, "0")
            fmt.Println("LED desligado")
            time.Sleep(5 * time.Second)
        }
    }()

    // Subscribe to the "topic_sensor_temperature" topic and print received messages to the console
    go func() {
        client.Subscribe(TOPIC_PUBLISH_TEMPERATURE, 0, func(client MQTT.Client, msg MQTT.Message) {
            fmt.Printf("Temperatura: %s\n", string(msg.Payload()))
        })
    }()

    // Subscribe to the "topic_sensor_humidity" topic and print received messages to the console
    go func() {
        client.Subscribe(TOPIC_PUBLISH_HUMIDITY, 0, func(client MQTT.Client, msg MQTT.Message) {
            fmt.Printf("Humidade: %s\n", string(msg.Payload()))
        })
    }()

    // Wait for events to occur (infinite loop)
    select {}
}
