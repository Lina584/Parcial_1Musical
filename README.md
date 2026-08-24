# Sala Musical Domótica con ESP32

## Descripción del proyecto

Este proyecto corresponde al desarrollo de una parte de una **casa domótica**, específicamente una **sala musical interactiva**. El sistema utiliza un **ESP32** como controlador principal y permite interactuar con diferentes instrumentos musicales mediante comandos de voz enviados desde una aplicación móvil, a través de una red **WiFi** creada por el propio ESP32.

La sala musical está diseñada para reproducir diferentes notas y ritmos utilizando **servomotores**, los cuales realizan el movimiento necesario para golpear los instrumentos. Además, el sistema cuenta con una **pantalla OLED** que permite visualizar el instrumento y la acción que se está ejecutando.

La comunicación con el sistema se realiza mediante **WiFi**: el ESP32 crea su propia red inalámbrica y aloja un pequeño servidor web que recibe los comandos. Estos comandos pueden enviarse desde una **aplicación móvil con reconocimiento de voz** (desarrollada en MIT App Inventor) o, para pruebas, directamente desde el **Monitor Serial** del computador.

---

## Objetivo

Desarrollar un sistema musical automatizado utilizando un ESP32, integrando comunicación WiFi, control por voz mediante una aplicación móvil, una pantalla OLED y servomotores para controlar diferentes instrumentos musicales.

El proyecto busca demostrar la aplicación de conceptos de **microcontroladores, comunicación inalámbrica, servidores web embebidos, reconocimiento de voz, control de servomotores, interfaces de usuario y automatización** dentro de una aplicación de domótica.

---

## Funcionamiento

El ESP32 crea su propia red WiFi y aloja un servidor web interno. Cuando recibe una petición HTTP con un comando de texto (proveniente de la aplicación móvil o del Monitor Serial), el programa interpreta la instrucción y determina qué instrumento o nota debe reproducirse.

Dependiendo del comando, el sistema puede:

* Reproducir las notas **DO, RE, MI y FA** en el xilófono.
* Activar el **bombo** de la batería.
* Activar el **redoblante**.
* Reproducir una melodía automática en el xilófono.
* Reproducir un ritmo automático en la batería.
* Ejecutar una secuencia completa combinando xilófono y batería.
* Mostrar en la pantalla OLED el instrumento y la acción que se está ejecutando.
* Responder con un mensaje de confirmación que la aplicación muestra en pantalla.

Los servomotores realizan un movimiento desde una posición de reposo hasta una posición de golpe y posteriormente regresan a su posición inicial.

---

## Instrumentos implementados

### Xilófono

El xilófono cuenta con cuatro servomotores encargados de reproducir las notas:

* DO
* RE
* MI
* FA

Cada servomotor corresponde a una nota diferente y realiza el movimiento necesario para producir el sonido.

### Batería

La batería está controlada mediante dos servomotores:

* Bombo
* Redoblante

Estos permiten generar diferentes patrones rítmicos.

---

## Comunicación WiFi

El ESP32 crea su propia red WiFi en modo **Access Point** (no requiere router ni conexión a internet) y aloja un servidor web interno que escucha peticiones HTTP.

Datos de la red:

| Parámetro | Valor |
| --- | --- |
| Nombre de red (SSID) | `AsistenteMusical_WiFi` |
| Contraseña | `musica123` |
| IP del ESP32 | `192.168.4.1` |
| Ruta de comandos | `/comando?texto=...` |

El dispositivo móvil (o el computador) debe conectarse a esta red para poder enviar comandos. Una vez conectado, los comandos pueden llegar de dos formas:

1. **Aplicación móvil con reconocimiento de voz** (ver sección "Aplicación de control por voz").
2. **Página web de respaldo**: el ESP32 también sirve una página en `http://192.168.4.1` con un campo de texto, útil como alternativa si la app no está disponible.

También es posible enviar los comandos directamente mediante el **Monitor Serial** del Arduino IDE, sin necesidad de WiFi, útil para pruebas rápidas o simulación en Wokwi.

---

## Comandos disponibles

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

El programa normaliza el texto recibido (quita tildes y lo convierte a mayúsculas) y luego analiza el contenido para determinar la acción correspondiente. Esto permite reconocer comandos aunque el reconocimiento de voz los transcriba con acentos, por ejemplo "xilófono" o "batería".

---

## Secuencias automáticas

Además de controlar cada instrumento individualmente, el sistema cuenta con secuencias automáticas.

### Melodía del xilófono

El sistema reproduce una secuencia formada por:

```text
DO → RE → MI → FA → MI → RE
```

Esta secuencia se repite automáticamente dos veces.

### Ritmo de batería

La batería ejecuta un patrón compuesto por:

```text
BOMBO → REDOBLANTE → BOMBO → BOMBO → REDOBLANTE
```

Este patrón también se repite automáticamente dos veces.

### Comando completo

También existe un modo que combina los dos instrumentos, ejecutando primero la melodía del xilófono y luego el ritmo de la batería:

```text
XILÓFONO + BATERÍA
```

---

## Pantalla OLED

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

## Componentes utilizados

* ESP32
* Pantalla OLED SSD1306 de 128 × 64
* 6 servomotores
* Xilófono
* Bombo
* Redoblante
* Computador para programación y pruebas
* Dispositivo móvil con la aplicación de control por voz
* Cables de conexión
* Fuente de alimentación

---

## Configuración de pines

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

## Librerías utilizadas

El programa utiliza las siguientes librerías:

* `WiFi.h`
* `WebServer.h`
* `Wire.h`
* `Adafruit_GFX.h`
* `Adafruit_SSD1306.h`
* `ESP32Servo.h`

Estas librerías permiten crear la red WiFi y el servidor web, controlar la comunicación I2C, la pantalla OLED y los servomotores del ESP32.

---

## Explicación del código

El programa está organizado en las siguientes partes:

### Configuración de red

```cpp
const char* WIFI_SSID = "AsistenteMusical_WiFi";
const char* WIFI_PASSWORD = "musica123";
WebServer server(80);
```

Define el nombre y la contraseña de la red que crea el ESP32, y crea el objeto `server`, encargado de escuchar peticiones HTTP en el puerto 80.

### Funciones de hardware

| Función | Descripción |
| --- | --- |
| `mostrarOLED(instrumento, accion)` | Limpia la pantalla y escribe el instrumento y la acción actual en dos líneas grandes. |
| `golpear(Servo &servo)` | Mueve un servo a la posición de golpe, espera, y lo regresa a reposo. Recibe el servo por referencia para reutilizarse con cualquiera de los seis servomotores. |
| `tocarDO / tocarRE / tocarMI / tocarFA / tocarBombo / tocarRedoblante` | Actualizan la OLED, ejecutan el golpe del servo correspondiente e imprimen un mensaje de depuración en el Monitor Serial. |
| `tocarXilofono()` | Reproduce el patrón de melodía `DO-RE-MI-FA-MI-RE`, repetido dos veces. |
| `tocarBateria()` | Reproduce el patrón de ritmo `BOMBO-REDOBLANTE-BOMBO-BOMBO-REDOBLANTE`, repetido dos veces. |
| `tocarCompleto()` | Ejecuta `tocarXilofono()` seguido de `tocarBateria()`. |
| `mostrarAyuda()` | Imprime en el Monitor Serial la lista de comandos disponibles. |
| `quitarTildes(texto)` | Reemplaza vocales acentuadas y la ñ por su versión sin tilde, para que el reconocimiento de voz no falle por acentos. |

### `interpretar(frase)`

Es la función central del sistema. Recibe el texto del comando, le quita las tildes, lo convierte a mayúsculas, y compara su contenido con palabras clave para decidir qué acción ejecutar. Devuelve un `String` con un mensaje de confirmación (por ejemplo, `"Tocando bombo"`), que es lo que la aplicación muestra en pantalla después de cada comando.

El orden en que se revisan las palabras clave es importante: por ejemplo, `"COMPLETO"`/`"TODO"` se revisa antes que la nota `"DO"`, porque la palabra "TODO" contiene la subcadena "DO" y, de revisarse en otro orden, activaría la nota por error.

### Servidor web

| Ruta | Función asociada | Descripción |
| --- | --- | --- |
| `/` | `handleRaiz()` | Sirve la página web de respaldo con el campo de texto. |
| `/comando?texto=...` | `handleComando()` | Lee el parámetro `texto`, lo pasa a `interpretar()`, y devuelve la respuesta. Es la ruta que utiliza la aplicación móvil. |
| Cualquier otra | `handleNoEncontrado()` | Responde con error 404. |

### `setup()` y `loop()`

`setup()` se ejecuta una sola vez al encender la placa: inicializa el puerto serial, crea la red WiFi, inicializa la pantalla OLED, configura los servomotores y arranca el servidor web. `loop()` se ejecuta de forma continua, llamando a `server.handleClient()` para atender cualquier petición HTTP entrante.

---

## Aplicación de control por voz

El control por voz se implementó mediante una **aplicación móvil nativa para Android**, desarrollada con **MIT App Inventor**. Se optó por una aplicación nativa en lugar de una página web porque los navegadores exigen conexión HTTPS para autorizar el acceso al micrófono, restricción que no aplica al permiso de micrófono de una aplicación instalada.

### Componentes de la aplicación

| Componente | Función |
| --- | --- |
| `Hablar` (Button) | Botón que activa el reconocimiento de voz. |
| `Label1` | Muestra el texto reconocido y luego la respuesta del ESP32. |
| `SpeechRecognizer1` | Activa el reconocimiento de voz nativo de Android. |
| `Web1` | Realiza la petición HTTP hacia el ESP32. |

### Lógica de la aplicación (bloques)

**Al tocar el botón, se activa el micrófono:**

```text
when Hablar.Click do
  call SpeechRecognizer1.GetText
```

**Al terminar de reconocer la voz, se envía el comando al ESP32:**

```text
when SpeechRecognizer1.AfterGettingText (result, partial) do
  set Label1.Text to result
  set Web1.Url to join("http://192.168.4.1/comando?texto=", result)
  call Web1.Get
```

**Al recibir la respuesta del ESP32, se muestra en pantalla:**

```text
when Web1.GotText (url, responseCode, responseContent) do
  set Label1.Text to responseContent
```

### Flujo completo

```text
Voz del usuario
  → SpeechRecognizer (motor nativo de Android)
  → texto transcrito
  → petición HTTP GET al ESP32 (misma red WiFi)
  → ESP32 recibe en /comando, ejecuta interpretar()
  → responde con un mensaje de confirmación
  → la aplicación muestra la respuesta
```

El celular debe estar conectado a la red `AsistenteMusical_WiFi` para que la aplicación pueda alcanzar al ESP32 en la dirección `192.168.4.1`.

---

## Relación con la casa domótica

La sala musical hace parte de una propuesta de **casa domótica**, en la cual diferentes espacios pueden incorporar sistemas automatizados e interactivos.

En este caso, la automatización se aplica al área musical, permitiendo controlar instrumentos mediante comandos de voz y generar diferentes acciones de manera automática, sin necesidad de cableado físico entre el usuario y el sistema.

---

## Video de funcionamiento

[Ver el video en Youtube](https://youtu.be/z1HGR-rDO_s)

---

## Autora

**Lina María Moreno Ospina, Ingeniería Mecatrónica**

Parcial 1 Micros y Lab — Sala Musical Domótica con ESP32.
