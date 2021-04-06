#ifndef WORDCLOCK_SERVER_H
#define WORDCLOCK_SERVER_H

#include <ESP8266WebServer.h>
/*Change the waiting time from 5000 to 2000 and 200 in library definition
#define HTTP_MAX_DATA_WAIT 200 //ms to wait for the client to send the request was 5000
#define HTTP_MAX_POST_WAIT 2000 //ms to wait for POST data to arrive was 5000
#define HTTP_MAX_SEND_WAIT 2000 //ms to wait for data chunk to be ACKed was 5000
*/
class HttpServer {
  public:
    static ESP8266WebServer web;
    static String ip;
    static void setup();
    static void loop();
};

#endif
