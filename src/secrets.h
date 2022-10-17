#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "EvryHub-Blinds"
 
const char WIFI_SSID[] = "HOME";
const char WIFI_PASSWORD[] = "AkJ197219992001";
const char AWS_IOT_ENDPOINT[] = "ah1nte2vfjrs0-ats.iot.ca-central-1.amazonaws.com";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUefp4EttX/jDAEA3DkvAO5I7XEbgwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIyMTAxNzIyMzQ1
OFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAO0GcpRWV38/2D2T7ktR
qCDKX/ISmBZEF4ivdQRAXICc79onDVcwMIttuVRlf/Xpam0sKm7140bWkp0VAUSJ
eEM4s2nyceOzuaYdlNqAdMDeND0i/4LttXBkcrN5qCYuTZI9DswW1jWyShIcuTEN
FJdVYyR8oXo/JXyWL5PBkz0R1CIglbOEUEGxtVzXmCE26ufOS2onKkexm0A02Wnv
wjRxewNKWj4gwwcJcU0fcAz+fvkYTrIerdL5vQZDhqAlRpuG7dl7D5tJBKuuRzdD
cY4kCU9TNmJ4kUss45MF3WKNALugXjoOuNCRb3/LykMyMfFDByUDtV1OCfN6/lca
xuUCAwEAAaNgMF4wHwYDVR0jBBgwFoAUjHElPwXQx07pz2qWaGPa+LtONoYwHQYD
VR0OBBYEFG664NFIMkmrowyw9ldXWSLrySJhMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCa87D+/NnvgWgilWcER9x2M2Cw
KFycX0B5YaAyjrMUDIX14ZZewx0JACYPLEcqt0TxLcdpMdbuvS4z+l2PSumqox2r
YT+4Uq1hTF8fuIOVwEXDYFkAcYDfjQsBSL4jSFL+s8ycSAnA7ZVx6H9bXX9GGquA
bacNfCLMGxshmseF8qGsgH/U3BQ9LNHOKuSBZj+SdixHt/QZJH1MXkZl45CgcG9i
8f6Yo6vPOLadY+xdGrehABvDRo8O9lG+9BHqXKjPIAFErWMe4aVnfDfBLBrxS1kY
Wmy+2eoCWvdaDzQoJFN4TJVumwy0nwnsnmr4Kqv+nDUnJxyh6ReRVBvJ40AX
-----END CERTIFICATE-----
)KEY";
 
// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEA7QZylFZXfz/YPZPuS1GoIMpf8hKYFkQXiK91BEBcgJzv2icN
VzAwi225VGV/9elqbSwqbvXjRtaSnRUBRIl4QzizafJx47O5ph2U2oB0wN40PSL/
gu21cGRys3moJi5Nkj0OzBbWNbJKEhy5MQ0Ul1VjJHyhej8lfJYvk8GTPRHUIiCV
s4RQQbG1XNeYITbq585LaicqR7GbQDTZae/CNHF7A0paPiDDBwlxTR9wDP5++RhO
sh6t0vm9BkOGoCVGm4bt2XsPm0kEq65HN0NxjiQJT1M2YniRSyzjkwXdYo0Au6Be
Og640JFvf8vKQzIx8UMHJQO1XU4J83r+VxrG5QIDAQABAoIBAQDWQTLHpfi3vRWS
LCuGiAyrf2QVZPt3fRus4bWsHyW+2J3zm8NyYcwv7QJPKOTvfIxInVjKLpjAiuvC
9J7NvloooZOEnLSYePjDERBiJyXixbmA3icLyiZtr08Dr1/xC3/xRiIfGDGQHlbY
YoCAt2s6lOPmsoYhA/21zei57oxS8RJw6ajUMCdjNCCKCChV2yqVY6pKXLmDuWqt
P0GTZOdtEetiiUZ4N1QIiYC0SiXWdDQWohtR75VrjErH5TJ7JBGTFcSeKkSRdQez
UeBoJfBtUqB4PgqM30lgYLZQhEv5lGG3OGCfaC2ZhpLRgKRSCjCge9utqwZac3Ri
UKTmCFCBAoGBAPfnL0Zr7cDlztkVaz+Ix3cIF+ioXSCL6mjcNAbqBHBm79QODkGg
tZ8S2P9m8b6myg3QVZRi8w4fL4e5ur5J1pJsx7oQruCEOVxyjRJylGGmgxYbxFeL
KX2Mc4aVaze0+xR1M4hNpW0voRovWUUAz0Yaqe9ejJrULGxRMLs0wRUhAoGBAPTE
TwTVGDKMU3m4wXqib+Q54VmOh44L4gfTtKI110PYUAdC3DxsdW2vXG9eI6iFR4Sz
c/RU41A+010aGIewRVc659dj6HAeYQBNqGnCu8F2fd/pWrMu2ePjQZg0cYcP2Tgl
fM/xlbXFPvIIBRRK3fJY/N8ownEQX15IwrfraHVFAoGBALMatyAk2AhWwEQ84lhr
xNit40A3a9kY3UnXVktshbqD1GAbq5ckujNslZSo1p2MuUyTqVDk0H/ybg+cU1wp
0rnFl+NJLF+PiDXPmTary6JlgC3X79nj9Bz9/XVNvMHposFcD9g5aPxivf1CGjft
ZtNabk4/vEwcYN/9JL0MVzehAoGBANZ1gn7Lb04q4aEPHoci+ELR6/8aAO+Wvc75
0ZUm85wAVe5myK0wopseQTFbHIc1b/p3eaulDR5sTamPidqpube6ZxSz9x71nywy
3tL7yqKx/YxI3C2PaV5g6kJAARDD9CFbx0YzeGcxkftix4CGMH19NuMYxXVnUQg9
XE7ltjq5AoGAPwhBGaj4/8LuYQliBhhGGLtkxTLjEMllqCFwY5OHCQY4Hjh5+kky
XJ2QKqGPHmRXqsbYn1+dja2yJ6tObGLH5gS3LIdtF9mBtfdmyWdi42Q0pMlksf0Y
MbiE1sU6PNRu53XzWBfRMfVO83tfrEKkrL1rVnY7GJwahtqwgq96HmA=
-----END RSA PRIVATE KEY-----
)KEY";