#  Sala Musical Domótica con ESP32

## Descripción del proyecto

Este proyecto corresponde al desarrollo de una parte de una **casa domótica**, específicamente una **sala musical interactiva**. El sistema utiliza un **ESP32** como controlador principal y permite interactuar con diferentes instrumentos musicales mediante comandos enviados desde un computador o un dispositivo móvil.

La sala musical está diseñada para reproducir diferentes notas y ritmos utilizando **servomotores**, los cuales realizan el movimiento necesario para golpear los instrumentos. Además, el sistema cuenta con una **pantalla OLED** que permite visualizar el instrumento y la acción que se está ejecutando.

La comunicación con el sistema puede realizarse mediante **Bluetooth**, permitiendo enviar comandos desde un celular, o mediante el **Monitor Serial** para realizar pruebas directamente desde el computador.

---

##  Objetivo

Desarrollar un sistema musical automatizado utilizando un ESP32, integrando comunicación Bluetooth, una pantalla OLED y servomotores para controlar diferentes instrumentos musicales.

El proyecto busca demostrar la aplicación de conceptos de **microcontroladores, comunicación inalámbrica, control de servomotores, interfaces de usuario y automatización** dentro de una aplicación de domótica.

---

##  Funcionamiento

El ESP32 recibe una instrucción mediante Bluetooth o mediante el puerto serial. El programa interpreta el comando recibido y determina qué instrumento o nota debe reproducirse.

Dependiendo del comando, el sistema puede:

* Reproducir las notas **DO, RE, MI y FA** en el xilófono.
* Activar el **bombo** de la batería.
* Activar el **redoblante**.
* Reproducir una melodía automática en el xilófono.
* Reproducir un ritmo automático en la batería.
* Ejecutar una secuencia completa combinando xilófono y batería.
* Mostrar en la pantalla OLED el instrumento y la acción que se está ejecutando.

Los servomotores realizan un movimiento desde una posición de reposo hasta una posición de golpe y posteriormente regresan a su posición inicial.

---

##  Instrumentos implementados

###  Xilófono

El xilófono cuenta con cuatro servomotores encargados de reproducir las notas:

* DO
* RE
* MI
* FA

Cada servomotor corresponde a una nota diferente y realiza el movimiento necesario para producir el sonido.

###  Batería

La batería está controlada mediante dos servomotores:

* Bombo
* Redoblante

Estos permiten generar diferentes patrones rítmicos.

---

##  Comunicación Bluetooth

El ESP32 utiliza Bluetooth para recibir comandos desde un dispositivo móvil.

El nombre configurado para el dispositivo Bluetooth es:

`AsistenteMusical`

Una vez establecida la conexión, se pueden enviar instrucciones escritas desde el celular para controlar los instrumentos.

También es posible enviar los comandos directamente mediante el **Monitor Serial** del Arduino IDE.

---

##  Comandos disponibles

Algunos ejemplos de comandos que puede interpretar el sistema son:

```text
Toca DO
Toca RE
Toca MI
Toca FA
Toca el bombo
Toca el redoblante
Toca el xilofono
Toca la bateria
Toca todo
AYUDA
```

El programa convierte las instrucciones recibidas a mayúsculas y analiza el texto para determinar la acción correspondiente.

---

##  Secuencias automáticas

Además de controlar cada instrumento individualmente, el sistema cuenta con secuencias automáticas.

### Melodía del xilófono

El sistema reproduce una secuencia formada por:

```text
DO → RE → MI → FA → MI → RE
```

Esta secuencia se repite automáticamente.

### Ritmo de batería

La batería ejecuta un patrón compuesto por:

```text
BOMBO → REDOBLANTE → BOMBO → BOMBO → REDOBLANTE
```

### Comandos completos

También existe un modo que combina los dos instrumentos:

```text
XILÓFONO + BATERÍA
```

---

##  Pantalla OLED

El proyecto utiliza una pantalla OLED de **128 × 64 píxeles** para mostrar información relacionada con la acción que está realizando el sistema.

Por ejemplo:

```text
XILOFONO
DO
```

o:

```text
BATERIA
BOMBO
```

La pantalla permite visualizar de manera sencilla qué instrumento y qué acción se encuentran activos.

---

##  Componentes utilizados

* ESP32
* Pantalla OLED SSD1306 de 128 × 64
* 6 servomotores
* Xilófono
* Bombo
* Redoblante
* Computador para programación y pruebas
* Dispositivo móvil para comunicación Bluetooth
* Cables de conexión
* Fuente de alimentación

---

##  Configuración de pines

### Pantalla OLED

| Función | Pin ESP32 |
| ------- | --------: |
| SDA     |   GPIO 21 |
| SCL     |   GPIO 22 |

### Servomotores

| Instrumento | Pin ESP32 |
| ----------- | --------: |
| Xilófono DO |   GPIO 13 |
| Xilófono RE |   GPIO 14 |
| Xilófono MI |   GPIO 27 |
| Xilófono FA |   GPIO 26 |
| Bombo       |   GPIO 25 |
| Redoblante  |   GPIO 33 |

---

##  Librerías utilizadas

El programa utiliza las siguientes librerías:

* `Wire.h`
* `Adafruit_GFX.h`
* `Adafruit_SSD1306.h`
* `ESP32Servo.h`
* `BluetoothSerial.h`

Estas librerías permiten controlar la comunicación I2C, la pantalla OLED, los servomotores y la comunicación Bluetooth del ESP32.

---



## Relación con la casa domótica

La sala musical hace parte de una propuesta de **casa domótica**, en la cual diferentes espacios pueden incorporar sistemas automatizados e interactivos.

En este caso, la automatización se aplica al área musical, permitiendo controlar instrumentos mediante comandos y generar diferentes acciones de manera automática.

---
## Video de funcionamiento
[Ver el video en Youtube](https://youtu.be/8uXLc7dYh_g)


##  Autora

**Lina María Moreno Ospina, Ingeniería Mecatrónica**

Parcial 1 Micros y Lab — Sala Musical Domótica con ESP32.
