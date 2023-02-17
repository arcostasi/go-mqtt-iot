# Go MQTT IoT: Embedded Circuit Simulation with Wokwi

This project is an embedded circuit simulation that uses the Go programming language and MQTT technology to create an IoT (Internet of Things) solution. The project is based on an Franzininho WiFi (ESP32 S2) and uses a DHT22 temperature sensor and an LED.

The simulation is done through the Wokwi simulator, which allows the creation and testing of electronic circuits in a virtual environment. The goal of the project is to demonstrate how it is possible to create an IoT solution using modern technologies and practical programming.

The project includes a Go application that publishes and subscribes to MQTT topics to control the LED and receive data from the temperature sensor. The directory structure is organized and scalable, allowing the project code to be easily maintained and modified.

This project is open source and available on GitHub under the MIT license, which means that anyone can contribute code, report bugs or suggest improvements. The goal of the project is to help the developer community create IoT solutions with the Go programming language and MQTT technology in a simple and practical way.


## Portuguese translation

Este projeto é uma simulação de circuito embarcado que utiliza a linguagem de programação Go e a tecnologia MQTT para criar uma solução IoT (Internet das Coisas). O projeto é baseado em uma placa Franzininho WiFi (ESP32 S2) e utiliza um sensor de temperatura DHT22 e um LED.

A simulação é realizada por meio do simulador Wokwi, que permite a criação e testes de circuitos eletrônicos em um ambiente virtual. O objetivo do projeto é demonstrar como é possível criar uma solução IoT usando tecnologias modernas e práticas de programação.

O projeto inclui um aplicativo Go que publica e subscreve tópicos MQTT para controlar o LED e receber dados do sensor de temperatura. A estrutura de diretórios é organizada e escalável, permitindo que o código do projeto seja facilmente mantido e modificado.

Este projeto é open source e está disponível no GitHub sob a licença MIT, o que significa que qualquer pessoa pode contribuir com código, reportar bugs ou sugerir melhorias. O objetivo do projeto é ajudar a comunidade de desenvolvedores a criar soluções IoT com a linguagem de programação Go e tecnologia MQTT de maneira simples e prática.


## Requirements

To run this project, you will need:

- Install the latest version of Go
- A web browser or VSCode (Wokwi Extension)
- A Wokwi account (free)
- Basic knowledge of the MQTT protocol


## Getting Started

To start using the project:

To run the Go application, follow these steps:

1. Make sure you have Go installed on your system. If you don't have it installed, you can download it from the [official Go website](https://golang.org/dl/).

2. Clone the project from GitHub:

```shell
$ git clone https://github.com/arcostasi/go-mqtt-iot.git
```

3. Change to the project directory:

```shell
$ cd go-mqtt-iot
```

4. Install the Paho MQTT library:

```shell
$ go get github.com/eclipse/paho.mqtt.golang
```

5. Start the application:

```shell
$ go run cmd/main.go
```

6. The application will start running and you should see output in the console indicating that the LED is turning on and off and that temperature and humidity data is being received from the DHT22 sensor.

```shell
Connected to broker.hivemq.com
LED ligado
Temperatura: 25.6
Humidade: 56.3
LED desligado
Temperatura: 25.6
Humidade: 56.3
```

You can stop the application by pressing `Ctrl + C`.

7. To run the Wokwi simulation, open the `https://wokwi.com/projects/322524997423727188` file in a web browser.

The Wokwi simulation should now load in the web browser and you can interact with the circuit as desired.

Note: The Wokwi simulation requires an active internet connection in order to communicate with the MQTT broker.


## Acknowledgments

This project was developed as a learning exercise for embedded systems and MQTT communication. We acknowledge the use of the Wokwi simulator with the Franzininho WiFi (ESP32 S2) board, which provided a fast and easy way to test the circuit without the need for physical hardware.


## License

This project is licensed under the terms of the MIT license. See the [LICENSE](https://github.com/arcostasi/go-mqtt-iot/blob/main/LICENSE) file for details.

