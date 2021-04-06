#ifndef WORDCLOCK_WIFI_H
#define WORDCLOCK_WIFI_H


class Wifi {  
  public:
    //static MDNSResponder mdns;
    //static DNSServer dnsServer;
    static void setup();
    static void reset();
    static void loop();
    static void mDNSsetup(); 

    
};

#endif
