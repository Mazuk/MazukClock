#ifndef WORDCLOCK_GUI_H
#define WORDCLOCK_GUI_H

class Gui {
  static String pad(int value);
  static String htmlOption(const String& label, const String& value, const String& store);
  //static String getTimeForm();
  static String createStyleSheet();
  static String createNav(int part);
  static String createScript();
  static String createColor();
  static String createTime();
  static String createWeather();
  static String createDND();
  static String createWIFI();
  //static String createContent();
  static String createFooter();
 
  public:
    static String index(int part);
    
};
#endif
