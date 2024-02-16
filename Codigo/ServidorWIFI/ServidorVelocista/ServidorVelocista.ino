#include <WiFi.h>
#include <WebServer.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED
#endif

// Set these to your desired credentials.
const char *ssid = "Velocista";
const char *password = "12345678";

WebServer server(80);

void setup() {
  Serial.begin(115200);

  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();

  server.on("/", handleRoot);
  server.on("/Constantes", constantes);
  server.begin();
}

void loop() {
  server.handleClient();
}

void constantes() {
  String KP = server.arg("KP");
  Serial.print("KP: ");
  Serial.println(KP);
  String KD = server.arg("KD");
  Serial.print("KD: ");
  Serial.println(KD);
  String KI = server.arg("KI");
  Serial.print("KI: ");
  Serial.println(KI);
  server.send(200, "text/plain", "Constantes");
}

void handleRoot() {
  const String Pagina =  R"====(<!DOCTYPE html><html><head><meta charset="UTF-8"><title>Servidor velocista</title></head><style>body{display:flex;flex-direction:column;align-items:center}.Input_Constantes{display:flex;height:30px;margin:10px;align-items:center}.Input_title{padding-right:10px}.Boton{padding:5px 5px;margin:10px 20px}</style><body><h1>Servidor velocista</h1><div class="Input_Constantes"><h4 class="Input_title">KP</h4><input id="KP" type="number"></div><div class="Input_Constantes"><h4 class="Input_title">KD</h4><input id="KD" type="number"></div><div class="Input_Constantes"><h4 class="Input_title">KI</h4><input id="KI" type="number"></div><div><button class="Boton" onclick="envierConstantes()">Enviar constantes</button><button class="Boton">Iniciar robot</button></div></body><script>function envierConstantes() {
      const KP = document.getElementById("KP").value;
      const KD = document.getElementById("KD").value;
      const KI = document.getElementById("KI").value;

      const href = window.location.href;
      const url = href + 'Constantes?' + 'KP=' + KP + '&' + 'KD=' + KD + '&' + 'KI=' + KI
      
      fetch(url)
        .then(response => {
          if (!response.ok) {
              throw new Error('La solicitud no fue exitosa');
          }
        })
    }</script></html>)====";
  server.send(200, "text/html", Pagina);
}
