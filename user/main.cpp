/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Create a WiFi access point and provide a web server on it. */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "ESPap";

static void handleGpioOn(uint8_t pin);
static void handleGpioOn16(void);
static void handleGpioOff(uint8_t pin);
static void handleGpioOff16(void);

ESP8266WebServer server(80);

static void handleGpioOn16(void)
{
  handleGpioOn(16);
}

static void handleGpioOn(uint8_t pin)
{
  String response;
  digitalWrite(pin, HIGH);
  response += "<h1>Pin ";
  response += pin;
  response += ": ON</h1>";
  response += "<a href=\"/gpio/off/";
  response += pin;
  response += "\">off</a>";
  server.send(200, "text/html", response);
}

static void handleGpioOff(uint8_t pin)
{
  String response;
  digitalWrite(pin, LOW);
  response += "<h1>Pin ";
  response += pin;
  response += ": OFF</h1>";
  response += "<a href=\"/gpio/on/";
  response += pin;
  response += "\">on</a>";
  server.send(200, "text/html", response);
}

static void handleGpioOff16(void)
{
  handleGpioOff(16);
}

void setup()
{
  delay(1000);
  pinMode(16, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAPConfig(IPAddress(192, 168, 1, 254), IPAddress(192, 168, 1, 254), IPAddress(255, 255, 255, 0));
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/gpio/on/16", handleGpioOn16);
  server.on("/gpio/off/16", handleGpioOff16);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}
