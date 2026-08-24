#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

// =====================================================
// WIFI (la ESP32 crea su propia red, modo Access Point)
// =====================================================

const char* WIFI_SSID = "AsistenteMusical_WiFi";
const char* WIFI_PASSWORD = "musica123"; // minimo 8 caracteres

WebServer server(80);

// =====================================================
// OLED
// =====================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// =====================================================
// SERVOS
// =====================================================

#define SERVO_DO 13
#define SERVO_RE 14
#define SERVO_MI 27
#define SERVO_FA 26

#define SERVO_BOMBO 25
#define SERVO_REDOBLANTE 33

Servo servoDO;
Servo servoRE;
Servo servoMI;
Servo servoFA;

Servo servoBombo;
Servo servoRedoblante;

// =====================================================
// ANGULOS
// =====================================================

#define REPOSO 90
#define GOLPE 300

// =====================================================
// PAGINA WEB (HTML + JS con reconocimiento de voz)
// =====================================================

const char paginaHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Asistente Musical</title>
<style>
  body {
    font-family: sans-serif;
    text-align: center;
    background: #111;
    color: #eee;
    padding: 40px 20px;
  }
  h1 { font-size: 22px; }
  #campoTexto {
    width: 85%;
    max-width: 400px;
    font-size: 20px;
    padding: 14px;
    border-radius: 10px;
    border: none;
    outline: none;
  }
  #btnEnviar {
    font-size: 18px;
    padding: 14px 28px;
    margin-top: 15px;
    border-radius: 10px;
    border: none;
    background: #2ecc71;
    color: white;
    cursor: pointer;
    display: block;
    margin-left: auto;
    margin-right: auto;
  }
  #instruccion {
    font-size: 14px;
    color: #aaa;
    margin-bottom: 15px;
  }
  #respuesta {
    margin-top: 20px;
    color: #3498db;
    font-size: 16px;
    min-height: 24px;
  }
  #ayuda {
    margin-top: 30px;
    font-size: 13px;
    color: #888;
    text-align: left;
    display: inline-block;
  }
</style>
</head>
<body>

<h1>Asistente Musical</h1>
<p id="instruccion">
  Toca el campo de abajo, luego toca el icono de
  microfono de tu teclado para dictar, y presiona Enviar.
</p>
<input type="text" id="campoTexto" placeholder="Ej: toca el bombo" autofocus>
<button id="btnEnviar">Enviar comando</button>
<div id="respuesta"></div>

<div id="ayuda">
  Ejemplos:<br>
  "toca DO" / "toca RE" / "toca MI" / "toca FA"<br>
  "toca el bombo" / "toca el redoblante"<br>
  "toca el xilofono" (melodia repetitiva)<br>
  "toca la bateria" (ritmo repetitivo)<br>
  "toca todo" (xilofono + bateria)
</div>

<script>
const campoTexto = document.getElementById('campoTexto');
const btnEnviar = document.getElementById('btnEnviar');
const respuesta = document.getElementById('respuesta');

btnEnviar.addEventListener('click', function () {
  const texto = campoTexto.value.trim();
  if (texto.length > 0) {
    enviarComando(texto);
  }
});

// Tambien permite enviar presionando Enter/Listo en el teclado
campoTexto.addEventListener('keydown', function (event) {
  if (event.key === 'Enter') {
    const texto = campoTexto.value.trim();
    if (texto.length > 0) {
      enviarComando(texto);
    }
  }
});

function enviarComando(texto) {
  respuesta.textContent = 'Enviando...';
  fetch('/comando?texto=' + encodeURIComponent(texto))
    .then(function (r) { return r.text(); })
    .then(function (data) {
      respuesta.textContent = data;
      campoTexto.value = '';
      campoTexto.focus();
    })
    .catch(function (err) { respuesta.textContent = 'Error enviando comando: ' + err; });
}
</script>

</body>
</html>
)rawliteral";

// =====================================================
// OLED
// =====================================================

void mostrarOLED(String instrumento, String accion) {

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(5, 5);
  display.println(instrumento);

  display.setTextSize(2);
  display.setCursor(10, 35);
  display.println(accion);

  display.display();
}

// =====================================================
// GOLPE
// =====================================================

void golpear(Servo &servo) {

  servo.write(GOLPE);

  delay(150);

  servo.write(REPOSO);

  delay(400);
}

// =====================================================
// INSTRUMENTOS
// =====================================================

void tocarDO() {

  mostrarOLED("XILOFONO", "DO");

  golpear(servoDO);

  Serial.println(">> TOCANDO DO");
}

void tocarRE() {

  mostrarOLED("XILOFONO", "RE");

  golpear(servoRE);

  Serial.println(">> TOCANDO RE");
}

void tocarMI() {

  mostrarOLED("XILOFONO", "MI");

  golpear(servoMI);

  Serial.println(">> TOCANDO MI");
}

void tocarFA() {

  mostrarOLED("XILOFONO", "FA");

  golpear(servoFA);

  Serial.println(">> TOCANDO FA");
}

void tocarBombo() {

  mostrarOLED("BATERIA", "BOMBO");

  golpear(servoBombo);

  Serial.println(">> TOCANDO BOMBO");
}

void tocarRedoblante() {

  mostrarOLED("BATERIA", "REDOBLANTE");

  golpear(servoRedoblante);

  Serial.println(">> TOCANDO REDOBLANTE");
}

// =====================================================
// SECUENCIAS TIPO CANCION (REPETITIVAS)
// =====================================================

#define REPETICIONES_XILOFONO 2
#define REPETICIONES_BATERIA 2

void tocarXilofono() {

  mostrarOLED("XILOFONO", "CANCION");

  Serial.println(">> TOCANDO CANCION XILOFONO");

  for (int i = 0; i < REPETICIONES_XILOFONO; i++) {

    tocarDO();
    tocarRE();
    tocarMI();
    tocarFA();
    tocarMI();
    tocarRE();
  }
}

void tocarBateria() {

  mostrarOLED("BATERIA", "RITMO");

  Serial.println(">> TOCANDO RITMO BATERIA");

  for (int i = 0; i < REPETICIONES_BATERIA; i++) {

    tocarBombo();
    tocarRedoblante();
    tocarBombo();
    tocarBombo();
    tocarRedoblante();
  }
}

void tocarCompleto() {

  mostrarOLED("TODO", "CANCION");

  Serial.println(">> TOCANDO CANCION COMPLETA (XILOFONO + BATERIA)");

  tocarXilofono();
  tocarBateria();
}

// =====================================================
// AYUDA
// =====================================================

void mostrarAyuda() {

  Serial.println();
  Serial.println("========== EJEMPLOS ==========");
  Serial.println("Toca DO");
  Serial.println("Toca RE");
  Serial.println("Toca MI");
  Serial.println("Toca FA");
  Serial.println("Toca el bombo");
  Serial.println("Toca el redoblante");
  Serial.println("Toca la bateria     -> ritmo repetitivo");
  Serial.println("Toquese el xilofono -> melodia repetitiva");
  Serial.println("Toca el comando completo -> xilofono + bateria");
  Serial.println("==============================");
  Serial.println();
}

// =====================================================
// QUITAR TILDES
// El reconocimiento de voz de Android transcribe con
// tildes ("xilofono" -> "xilófono"), pero las comparaciones
// de interpretar() no las tienen en cuenta. Esta funcion
// normaliza el texto antes de comparar.
// =====================================================

String quitarTildes(String texto) {

  texto.replace("á", "a");
  texto.replace("é", "e");
  texto.replace("í", "i");
  texto.replace("ó", "o");
  texto.replace("ú", "u");
  texto.replace("Á", "A");
  texto.replace("É", "E");
  texto.replace("Í", "I");
  texto.replace("Ó", "O");
  texto.replace("Ú", "U");
  texto.replace("ñ", "n");
  texto.replace("Ñ", "N");

  return texto;
}

// =====================================================
// INTERPRETAR FRASE
// Ahora devuelve un String: es la respuesta que se
// muestra en la pagina web despues de cada comando.
// =====================================================

String interpretar(String frase) {

  frase.trim();
  frase = quitarTildes(frase);
  frase.toUpperCase();

  if (frase.length() == 0) {
    return "Comando vacio";
  }

  Serial.print("Comando recibido: ");
  Serial.println(frase);

  // ---------------------------------------------------
  // AYUDA
  // ---------------------------------------------------

  if (frase == "AYUDA") {

    mostrarAyuda();
    return "Revisa el monitor serial para ver los comandos disponibles.";
  }

  // ---------------------------------------------------
  // COMPLETO / TODO
  // (se revisa PRIMERO porque "TODO" contiene "DO")
  // ---------------------------------------------------

  if (frase.indexOf("COMPLETO") >= 0 || frase.indexOf("TODO") >= 0) {

    tocarCompleto();
    return "Tocando cancion completa: xilofono + bateria";
  }

  // ---------------------------------------------------
  // BOMBO
  // ---------------------------------------------------

  if (frase.indexOf("BOMBO") >= 0) {

    tocarBombo();
    return "Tocando bombo";
  }

  // ---------------------------------------------------
  // REDOBLANTE
  // ---------------------------------------------------

  if (frase.indexOf("REDOBLANTE") >= 0) {

    tocarRedoblante();
    return "Tocando redoblante";
  }

  // ---------------------------------------------------
  // NOTAS
  // ---------------------------------------------------

  if (frase.indexOf("DO") >= 0) {

    tocarDO();
    return "Tocando nota DO";
  }

  if (frase.indexOf("RE") >= 0) {

    tocarRE();
    return "Tocando nota RE";
  }

  if (frase.indexOf("MI") >= 0) {

    tocarMI();
    return "Tocando nota MI";
  }

  if (frase.indexOf("FA") >= 0) {

    tocarFA();
    return "Tocando nota FA";
  }

  // ---------------------------------------------------
  // XILOFONO
  // ---------------------------------------------------

  if (frase.indexOf("XILOFONO") >= 0) {

    tocarXilofono();
    return "Tocando melodia del xilofono";
  }

  // ---------------------------------------------------
  // BATERIA
  // ---------------------------------------------------

  if (frase.indexOf("BATERIA") >= 0) {

    tocarBateria();
    return "Tocando ritmo de bateria";
  }

  // ---------------------------------------------------
  // NO RECONOCIDO
  // ---------------------------------------------------

  mostrarOLED("ERROR", "COMANDO");

  Serial.println("No entendi el comando.");

  return "No entendi el comando: " + frase;
}

// =====================================================
// RUTAS DEL SERVIDOR WEB
// =====================================================

void handleRaiz() {

  server.send_P(200, "text/html", paginaHTML);
}

void handleComando() {

  if (!server.hasArg("texto")) {

    server.send(400, "text/plain", "Falta el parametro 'texto'");
    return;
  }

  String frase = server.arg("texto");

  String respuesta = interpretar(frase);

  server.send(200, "text/plain", respuesta);
}

void handleNoEncontrado() {

  server.send(404, "text/plain", "Ruta no encontrada");
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // ---------------------------------------------------
  // WIFI (Access Point)
  // ---------------------------------------------------

  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

  IPAddress ip = WiFi.softAPIP();

  Serial.println();
  Serial.println("================================");
  Serial.println("   ASISTENTE MUSICAL (WIFI)");
  Serial.println("================================");
  Serial.print("Red WiFi: ");
  Serial.println(WIFI_SSID);
  Serial.print("Contrasena: ");
  Serial.println(WIFI_PASSWORD);
  Serial.print("Abre en el navegador: http://");
  Serial.println(ip);
  Serial.println();

  // ---------------------------------------------------
  // OLED
  // ---------------------------------------------------

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C)) {

    Serial.println("OLED NO ENCONTRADA");

    while (true);
  }

  // ---------------------------------------------------
  // SERVOS
  // ---------------------------------------------------

  servoDO.setPeriodHertz(50);
  servoRE.setPeriodHertz(50);
  servoMI.setPeriodHertz(50);
  servoFA.setPeriodHertz(50);

  servoBombo.setPeriodHertz(50);
  servoRedoblante.setPeriodHertz(50);

  servoDO.attach(SERVO_DO, 500, 2400);
  servoRE.attach(SERVO_RE, 500, 2400);
  servoMI.attach(SERVO_MI, 500, 2400);
  servoFA.attach(SERVO_FA, 500, 2400);

  servoBombo.attach(SERVO_BOMBO, 500, 2400);
  servoRedoblante.attach(SERVO_REDOBLANTE, 500, 2400);

  // ---------------------------------------------------
  // POSICION INICIAL
  // ---------------------------------------------------

  servoDO.write(REPOSO);
  servoRE.write(REPOSO);
  servoMI.write(REPOSO);
  servoFA.write(REPOSO);

  servoBombo.write(REPOSO);
  servoRedoblante.write(REPOSO);

  // ---------------------------------------------------
  // SERVIDOR WEB
  // ---------------------------------------------------

  server.on("/", handleRaiz);
  server.on("/comando", handleComando);
  server.onNotFound(handleNoEncontrado);

  server.begin();

  Serial.println("Servidor web iniciado.");

  // ---------------------------------------------------
  // MENSAJE INICIAL EN OLED
  // ---------------------------------------------------

  mostrarOLED("MUSICAL", "WIFI LISTO");
}

// =====================================================
// LOOP
// =====================================================

void loop() {

  server.handleClient();
}
