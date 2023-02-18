package main

import (
    "fmt"
    "time"

    MQTT "github.com/eclipse/paho.mqtt.golang"
    ui "github.com/gizak/termui/v3"
    "github.com/gizak/termui/v3/widgets"
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

    // Create temperature and humidity widgets
    tempParagraph := widgets.NewParagraph()
    tempParagraph.Title = "Temperatura"
    tempParagraph.Text = " Aguardando dados..."
    tempParagraph.SetRect(0, 0, 29, 3)

    humParagraph := widgets.NewParagraph()
    humParagraph.Title = "Umidade"
    humParagraph.Text = " Aguardando dados..."
    humParagraph.SetRect(0, 3, 29, 6)

    // Create a paragraph widget to display the LED status
    ledParagraph := widgets.NewParagraph()
    ledParagraph.Title = "LED"
    ledParagraph.Text = " Desligado"
    ledParagraph.SetRect(0, 6, 29, 9)

    // Create a paragraph widget to display the keyboard events
    keyParagraph := widgets.NewParagraph()
    keyParagraph.Text = " Quit with 'q' or 'Ctrl-C'"
    keyParagraph.SetRect(0, 9, 29, 12)

    // Initialize the user interface
    ui.Init()
    defer ui.Close()

    ui.Render(tempParagraph, humParagraph, ledParagraph, keyParagraph)

    // Publish "1" and "0" messages to the "topic_on_off_led" topic, with a 5-second interval
    go func() {
        for {
            client.Publish(TOPIC_ON_OFF_LED, 0, false, "1")
            ledParagraph.Text = " Ligado"
            ui.Render(ledParagraph)
            time.Sleep(5 * time.Second)

            client.Publish(TOPIC_ON_OFF_LED, 0, false, "0")
            ledParagraph.Text = " Desligado"
            ui.Render(ledParagraph)
            time.Sleep(5 * time.Second)
        }
    }()

    // Subscribe to the "topic_sensor_temperature" topic and update the temperature widget
    client.Subscribe(TOPIC_PUBLISH_TEMPERATURE, 0, func(client MQTT.Client, msg MQTT.Message) {
        tempParagraph.Text = fmt.Sprintf(" %s", string(msg.Payload()))
        ui.Render(tempParagraph)
    })

    // Subscribe to the "topic_sensor_humidity" topic and update the humidity widget
    client.Subscribe(TOPIC_PUBLISH_HUMIDITY, 0, func(client MQTT.Client, msg MQTT.Message) {
        humParagraph.Text = fmt.Sprintf(" %s", string(msg.Payload()))
        ui.Render(humParagraph)
    })

    // Handle keyboard events
    uiEvents := ui.PollEvents()

    for {
        e := <-uiEvents
        switch e.ID {
        case "q", "<C-c>":
            // Quit the program when the user presses 'q' or 'Ctrl-C'
            return
        }
    }
}
