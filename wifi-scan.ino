#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

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

#define REPOSO 70
#define GOLPE 110

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

// Cuantas veces se repite el patron de cada instrumento
#define REPETICIONES_XILOFONO 2
#define REPETICIONES_BATERIA 2

void tocarXilofono() {

  mostrarOLED("XILOFONO", "CANCION");

  Serial.println(">> TOCANDO CANCION XILOFONO");

  // Patron tipo melodia, se repite REPETICIONES_XILOFONO veces
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

  // Patron tipo ritmo, se repite REPETICIONES_BATERIA veces
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
  Serial.println("Quiero escuchar DO");
  Serial.println("Toca la bateria     -> ritmo repetitivo");
  Serial.println("Toquese el xilofono -> melodia repetitiva");
  Serial.println("Toca el comando completo -> xilofono + bateria");
  Serial.println("==============================");
  Serial.println();
}

// =====================================================
// INTERPRETAR FRASE
// (misma logica que la version fisica, solo que aqui
//  la unica entrada es el Monitor Serial)
// =====================================================

void interpretar(String frase) {

  frase.trim();
  frase.toUpperCase();

  if (frase.length() == 0) {
    return;
  }

  Serial.print("Comando recibido: ");
  Serial.println(frase);

  // ---------------------------------------------------
  // AYUDA
  // ---------------------------------------------------

  if (frase == "AYUDA") {

    mostrarAyuda();
    return;
  }

  // ---------------------------------------------------
  // COMPLETO / TODO
  // (se revisa PRIMERO porque "TODO" contiene "DO" y se
  //  confundiria con la nota DO si se revisara despues)
  // ---------------------------------------------------

  if (frase.indexOf("COMPLETO") >= 0 || frase.indexOf("TODO") >= 0) {

    tocarCompleto();
    return;
  }

  // ---------------------------------------------------
  // BOMBO
  // ---------------------------------------------------

  if (frase.indexOf("BOMBO") >= 0) {

    tocarBombo();
    return;
  }

  // ---------------------------------------------------
  // REDOBLANTE
  // ---------------------------------------------------

  if (frase.indexOf("REDOBLANTE") >= 0) {

    tocarRedoblante();
    return;
  }

  // ---------------------------------------------------
  // NOTAS
  // ---------------------------------------------------

  if (frase.indexOf("DO") >= 0) {

    tocarDO();
    return;
  }

  if (frase.indexOf("RE") >= 0) {

    tocarRE();
    return;
  }

  if (frase.indexOf("MI") >= 0) {

    tocarMI();
    return;
  }

  if (frase.indexOf("FA") >= 0) {

    tocarFA();
    return;
  }

  // ---------------------------------------------------
  // XILOFONO
  // ---------------------------------------------------

  if (frase.indexOf("XILOFONO") >= 0) {

    tocarXilofono();

    return;
  }

  // ---------------------------------------------------
  // BATERIA
  // ---------------------------------------------------

  if (frase.indexOf("BATERIA") >= 0) {

    tocarBateria();

    return;
  }

  // ---------------------------------------------------
  // NO RECONOCIDO
  // ---------------------------------------------------

  mostrarOLED("ERROR", "COMANDO");

  Serial.println("No entendi el comando.");
  Serial.println("Escribe AYUDA para ver ejemplos.");
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

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
  // MENSAJE INICIAL
  // ---------------------------------------------------

  mostrarOLED("MUSICAL", "LISTO");

  Serial.println();
  Serial.println("================================");
  Serial.println("  ASISTENTE MUSICAL (SIMULACIÓN)");
  Serial.println("================================");
  Serial.println("Escribe una frase en el Monitor Serial.");
  Serial.println("Ejemplo: Toca el bombo");
  Serial.println();
}

// =====================================================
// LOOP
// =====================================================

void loop() {

  // ---------------------------------------------------
  // COMANDOS POR EL MONITOR SERIAL (unica entrada aqui)
  // ---------------------------------------------------

  if (Serial.available() > 0) {

    String frase = Serial.readStringUntil('\n');

    interpretar(frase);
  }
}
