// ---------------------------------------------------------------------------------------------------------------
// Este código foi criado por mim para fins de aprendizado e aprimoramento, se você está utilizando o mesmo, saiba
// que fico feliz em poder ajuda-lo(a) de alguma forma. - Fabiano Jaureguy
// ---------------------------------------------------------------------------------------------------------------

// Bibliotecas utilizadas
#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"

#define DHTTYPE DHT11

//informar o pino DATA e modelo do Sensor (no caso foi utilizaro o DHT11 com o sensor de dados no pino Analógico 1)
DHT dht(A1, DHT11); 

//Definicoes de IP, mascara de rede e gateway
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 200);       //Define o endereco IP
IPAddress gateway(192, 168, 1, 255);  //Define o gateway
IPAddress subnet(255, 255, 255, 0); //Define a máscara de rede
EthernetServer server(60080);

void setup()
{
  Serial.begin(9600);     // Start dos serviços
  Ethernet.begin(mac, ip);
  server.begin();
  dht.begin();
}
void loop() {
  
float h = dht.readHumidity();   //Variaveis
float t = dht.readTemperature();

  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c == '\n' && currentLineIsBlank) {

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 2");                     // Taxa de atualização da pagina web (atualização automatica a cada 2s)
          client.println();

          // Pagina HTML

          client.println("<!DOCTYPE html>"); 
          client.println("<html>");
          client.println("<head>");
          client.println("<title>DataCenter - SE Company </title>");                        // Titulo do Site
          client.println("</head>");
          client.println("<body>");
          client.println("<center>");
          client.print("<img src=\"https://i.imgur.com/itJca42.jpg\">");                    // Imagem Estab. - link web
          client.println ("<font face=Helvetica>");                                         // Fonte do Site
          client.println("<h1><font color=#FF8C00>MONITOR DE TEMPERATURA </font></h1>");    // Texto
          client.println("<h1><font color=#FF8C00>DATA CENTER </font></h1>");               // Texto
          client.println("<center/>");
          client.println("<hr/>");
          client.println("<h1>Temperatura</h1>");
          client.print("<b>");
          client.println ("<font color=#FF8C00>");
          client.print (t,0);                                                               // Var de Temperatura
          client.println("*C");
          client.println ("</font>");
          client.print("<b/>");
          client.println("<h1>Umidade</h1>");
          client.println ("<font color=#FF8C00>");
          client.print (h,0);                                                               // Var de Umidade
          client.println("%");
          client.println ("</font>");
          client.println("<br/>");
          client.println("<hr/>");
          client.print ("Arduino UNO R3 - MAC: DE:AD:BE:EF:FE:ED");                         // Info de equipamento
          client.println("</body>");
          client.println("</html>");
          break;
        }

        if (c == '\n')
        {
          currentLineIsBlank = true;
        }
        else if (c != '\r')
        {
          currentLineIsBlank = false;
        }
      }
    }

    delay(1);
    client.stop();
  }
}
