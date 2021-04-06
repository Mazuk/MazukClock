#ifndef WORDCLOCK_CONTROLLER_H
#define WORDCLOCK_CONTROLLER_H

class Controller {
  public:
    static void index();
    static void saveColor();    
    static void saveTime();    
    static void saveDnd();  
    static void saveWeather();
    static void deleteWiFi();
    static String getLogoSvg();
    static void getManifest();
    static void notFound();
};

#endif
