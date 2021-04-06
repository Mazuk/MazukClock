#include <Arduino.h>

#include "gui.h"
#include "color.h"
#include "config.h"
#include "led.h"
#include "httpServer.h"
#include "controller.h"
#include "weather.h"


String Gui::pad(int value) {
  if(value < 10) {
    return "0" + String(value);
  }
  return String(value);
}


String Gui::htmlOption(const String& label, const String& value, const String& store) {
    String content = "<option value=\"" + value + "\"";
    if (store == value) {
        content += " selected";
    }

    content += ">" + label + "</option>";
    return content;
}

String Gui::createStyleSheet() {
  String content = "";

  content += "body { font-family: Arial, 'Helvetica Neue', Helvetica, sans-serif; background-color: #eee; color: #333;}";
  content += "nav { position: fixed; top: 0; left: 0; right: 0; background-color: #3d72a8; list-style: none; margin: 0; display: flex;}";
  content += "nav li  {margin: 0; padding: 10px; flex: 1; text-align: center; color: #eee; cursor: pointer; }";
  content += "nav li.icon { padding: 6px; width: 50px; flex-grow: 0; flex-shrink: 0; flex-basis: 50px; cursor: inherit; }";
  content += "nav li.icon img { vertical-align: middle; }";
  content += "nav li.active { color: #ccc; }";
  content += "nav li svg path { fill: #eee; }";
  content += "nav li.active svg path { fill: #ccc; }";
  content += "nav.disabled li:not(.icon) { display: none; }";
  content += "main { margin-top: 60px; }";
  content += "table { width: 100%;} table td { width: 25%;}";
  content += "section { display: none; }";
  content += "section.active { display: block; }";
  content += "section div { color: #3d72a8; margin-bottom: 20px; }";
  content += "section div label { display: block; font-size: 0.8em; margin-bottom: 5px; }";
  content += "section div input { display: block; background-color:#fff; width: 100%; border: 0; padding: 0; margin: 0; border-bottom: 1px solid #3d72a8; height: 30px; }";
  content += "section div select { display: block; background-color:#fff; width: 100%; border: 0; padding: 0; margin: 0; border-bottom: 1px solid #3d72a8; height: 30px; }";
  content += "section div input[type='color'] { border: 0; }";
  content += "section div div.time { display: flex; }";
  content += "section div div.time span { flex-basis: 20px; text-align: center; font-weight: bold; font-size: 1.3em; }";
  content += "section div div.time select { display: inline-block; flex: 1; }";
  content += "#reset_wifi { background-color: #3d72a8; border: 0; width: 100%; color: #eee; padding: 10px; cursor: pointer; }";
  content += "#reset_wifi_message { display: none; text-align: center;}";
  content += "footer { position: fixed; bottom: 0; left: 0; right: 0; background-color: #3d72a8; text-align: right; }";
  content += "footer button { background-color: #3d72a8; color: #eee; border: 0; padding: 15px; text-transform: uppercase; font-weight: bold; }";
  content += "footer button:hover,footer button:active { color: #ccc; cursor: pointer; }";

  return content;
}


String Gui::createNav(int part) {
  String content = "";
  if (part == 1) {
      content += "<nav>";
      content += "<li class='icon'>" + Controller::getLogoSvg() + "</li>";// <img src = '/logo.svg' width = '32'> < / li>";
      content += "<li class='active' name='color'>";
      content += "<svg style='width:24px;height:24px' viewBox='0 0 24 24'>";
      content += "<path fill='#eee' d='M17.5,12A1.5,1.5 0 0,1 16,10.5A1.5,1.5 0 0,1 17.5,9A1.5,1.5 0 0,1 19,10.5A1.5,1.5 0 0,1 17.5,12M14.5,8A1.5,1.5 0 0,1 13,6.5A1.5,1.5 0 0,1 14.5,5A1.5,1.5 0 0,1 16,6.5A1.5,1.5 0 0,1 14.5,8M9.5,8A1.5,1.5 0 0,1 8,6.5A1.5,1.5 0 0,1 9.5,5A1.5,1.5 0 0,1 11,6.5A1.5,1.5 0 0,1 9.5,8M6.5,12A1.5,1.5 0 0,1 5,10.5A1.5,1.5 0 0,1 6.5,9A1.5,1.5 0 0,1 8,10.5A1.5,1.5 0 0,1 6.5,12M12,3A9,9 0 0,0 3,12A9,9 0 0,0 12,21A1.5,1.5 0 0,0 13.5,19.5C13.5,19.11 13.35,18.76 13.11,18.5C12.88,18.23 12.73,17.88 12.73,17.5A1.5,1.5 0 0,1 14.23,16H16A5,5 0 0,0 21,11C21,6.58 16.97,3 12,3Z' />";
      content += "</svg>";
      content += "</li>";

      content += "<li name='time'>";
      content += "<svg style='width:24px;height:24px' viewBox='0 0 24 24'>";
      content += "<path fill='#eee' d='M12,20A8,8 0 0,0 20,12A8,8 0 0,0 12,4A8,8 0 0,0 4,12A8,8 0 0,0 12,20M12,2A10,10 0 0,1 22,12A10,10 0 0,1 12,22C6.47,22 2,17.5 2,12A10,10 0 0,1 12,2M12.5,7V12.25L17,14.92L16.25,16.15L11,13V7H12.5Z' />";
      content += "</svg>";
      content += "</li>";

      content += "<li name='dnd'>";
      content += "<svg style='width:24px;height:24px' viewBox='0 0 24 24'>";
      content += "<path fill='#eee' d='M17.75,4.09L15.22,6.03L16.13,9.09L13.5,7.28L10.87,9.09L11.78,6.03L9.25,4.09L12.44,4L13.5,1L14.56,4L17.75,4.09M21.25,11L19.61,12.25L20.2,14.23L18.5,13.06L16.8,14.23L17.39,12.25L15.75,11L17.81,10.95L18.5,9L19.19,10.95L21.25,11M18.97,15.95C19.8,15.87 20.69,17.05 20.16,17.8C19.84,18.25 19.5,18.67 19.08,19.07C15.17,23 8.84,23 4.94,19.07C1.03,15.17 1.03,8.83 4.94,4.93C5.34,4.53 5.76,4.17 6.21,3.85C6.96,3.32 8.14,4.21 8.06,5.04C7.79,7.9 8.75,10.87 10.95,13.06C13.14,15.26 16.1,16.22 18.97,15.95M17.33,17.97C14.5,17.81 11.7,16.64 9.53,14.5C7.36,12.31 6.2,9.5 6.04,6.68C3.23,9.82 3.34,14.64 6.35,17.66C9.37,20.67 14.19,20.78 17.33,17.97Z' />";
      content += "</svg>";
      content += "</li>";
  }
  else if (part == 2) {
      content += "<li name='weather'>";
      content += "<svg style='width:24px;height:24px' viewBox='0 0 500 500'>";
      content += "<path fill='#eee' d='M406.081 265.984c-16-44.8-52.267-73.6-94.933-73.6h-2.133c-5.333-46.933-38.4-86.4-83.2-99.2-28.8-7.467-58.667-3.2-84.267 11.733-26.667 16-45.867 41.6-53.333 72.533-12.8 53.333 11.733 107.733 56.533 132.267-13.867 20.267-22.4 45.867-22.4 73.6 0 59.733 38.4 109.867 90.667 116.267 4.267 1.067 189.867 1.067 194.133 0 53.333-4.267 93.867-55.467 93.867-116.267-.001-61.866-41.601-113.066-94.934-117.333zm-277.334-78.933c5.333-20.267 17.067-37.333 34.133-46.933 16-9.6 34.133-11.733 52.267-7.467 29.867 8.533 52.267 37.333 53.333 71.467-23.467 12.8-41.6 35.2-51.2 61.867-13.867 1.067-26.667 5.333-38.4 12.8-38.399-10.667-59.733-51.201-50.133-91.734zm273.067 272H218.347c-29.867-3.2-54.4-36.267-54.4-74.667 0-41.6 26.667-75.733 59.733-75.733h4.267c11.733 2.133 21.333-5.333 23.467-16 5.333-26.667 22.4-48 43.733-54.4 5.333-1.067 9.6-2.133 14.933-2.133 27.733 0 52.267 23.467 58.667 56.533 2.133 10.667 12.8 17.067 23.467 16h6.4c33.067 0 59.733 34.133 59.733 75.733 0 39.467-24.533 72.533-56.533 74.667zM249.281 1.451c-11.733-3.2-22.4 3.2-25.6 14.933l-8.533 32c-3.2 11.733 3.2 22.4 14.933 25.6 2.133 1.067 3.2 1.067 5.333 1.067 9.6 0 18.133-6.4 20.267-16l8.533-32c3.2-11.734-3.2-22.4-14.933-25.6zm123.733 97.066c-6.4-9.6-19.2-12.8-28.8-6.4l-27.733 17.067c-9.6 6.4-12.8 19.2-6.4 28.8 4.267 6.4 10.667 9.6 18.133 9.6 3.2 0 7.467-1.067 10.667-3.2l27.733-17.067c9.6-6.4 12.8-19.199 6.4-28.8zm-288 177.067c-6.4-9.6-19.2-12.8-28.8-6.4L28.48 286.251c-9.6 6.4-12.8 19.2-6.4 28.8 4.267 6.4 10.667 9.6 18.133 9.6 4.267 0 7.467 0 10.667-3.2l27.733-17.067c9.601-6.4 12.801-19.199 6.401-28.8zm54.4-215.466l-16-28.8c-6.4-9.6-19.2-12.8-28.8-7.467-10.667 5.333-13.867 18.133-8.533 28.8l17.066 28.8c4.267 6.4 10.667 10.667 18.133 10.667 3.2 0 6.4-1.067 10.667-3.2 9.601-6.4 12.801-19.2 7.467-28.8zm-82.133 87.466l-30.933-8.533c-10.667-3.2-22.4 4.267-25.6 14.933s4.267 22.4 14.933 25.6l30.933 7.467c2.133 1.066 3.2 1.066 5.333 1.066 9.6 0 17.067-6.4 20.267-14.933 3.2-10.666-4.267-22.4-14.933-25.6z' />";
      content += "</svg>";
      content += "</li>";

      content += "<li name='wifi'>";
      content += "<svg style='width:24px;height:24px' viewBox='0 0 24 24'>";
      content += "<path fill='#eee' d='M12,21L15.6,16.2C14.6,15.45 13.35,15 12,15C10.65,15 9.4,15.45 8.4,16.2L12,21M12,3C7.95,3 4.21,4.34 1.2,6.6L3,9C5.5,7.12 8.62,6 12,6C15.38,6 18.5,7.12 21,9L22.8,6.6C19.79,4.34 16.05,3 12,3M12,9C9.3,9 6.81,9.89 4.8,11.4L6.6,13.8C8.1,12.67 9.97,12 12,12C14.03,12 15.9,12.67 17.4,13.8L19.2,11.4C17.19,9.89 14.7,9 12,9Z' />";
      content += "</svg>";
      content += "</li>";
      content += "</nav>";
  }
  return content;
}

String Gui::createScript() {
    String content = "";
    content += "window.onload=function(){var e=function(e){return document.getElementById(e)},t=document.querySelector('nav'),n=t.querySelectorAll('li'),a=document.querySelectorAll('main section'),i=e('save'),l=e('reset_wifi'),d=e('reset_wifi_message'),o='color',u={fg:e('fg'),bg:e('bg'),m1:e('m1'),m2:e('m2'),m3:e('m3'),m4:e('m4'),mx:e('mx'),mlength:e('mlength'),atime:e('atime'),power_supply:e('power_supply'),brightness:e('brightness'),animation_bell:e('animation_bell'),animation_type:e('animation_type'),tzAuto:e('tz_auto'),tz:e('tz'),ntp:e('ntp'),DNS_name:e('DNS_name'),viertel:e('viertel'),dndActive:e('dnd_active'),dndSH:e('dnd_s_h'),dndSM:e('dnd_s_m'),dndEH:e('dnd_e_h'),dndEM:e('dnd_e_m'),ticker:e('ticker'),tc:e('tc'),tspeed:e('tspeed'),tu:e('tu'),WX:e('WX'),apikey:e('apikey'),lon:e('lon'),lat:e('lat'),WXcommand:e('WXcommand')};n.forEach(function(t){t.onclick=function(t){if('li'==t.currentTarget.tagName.toLowerCase()&&t.currentTarget.hasAttribute('name')){o=t.currentTarget.getAttribute('name');for(var l=0;l<n.length;l++)n[l].classList.remove('active');for(t.currentTarget.classList.add('active'),l=0;l<a.length;l++)a[l].classList.remove('active');i.style.display='wifi'==o?'none':'inline-block',e(o).classList.add('active')}}}),i.onclick=function(e){var selectedArray = new Array();var i;var count = 0;var t={},n='/api/'+o;if('color'==o){t.fg=u.fg.value,t.bg=u.bg.value,t.m1=u.m1.value,t.m2=u.m2.value,t.m3=u.m3.value,t.m4=u.m4.value,t.mx=u.mx.value,t.mlength=u.mlength.value,t.atime=u.atime.value,t.power_supply=u.power_supply.value,t.brightness=u.brightness.value,t.animation_bell=u.animation_bell.value; for(i=0;i<u.animation_type.options.length; i++) {if (u.animation_type.options[i].selected) {selectedArray[count] = u.animation_type.options[i].value ;count++;}} t.animation_type = JSON.stringify(selectedArray);}else if('time'==o)t.tz_auto=u.tzAuto.value,t.tz=u.tz.value,t.ntp=u.ntp.value,t.DNS_name=u.DNS_name.value,t.viertel=u.viertel.value;else if('dnd'==o){t.dnd_active=u.dndActive.value,t.dnd_start_hour=u.dndSH.value,t.dnd_start_minute=u.dndSM.value,t.dnd_end_hour=u.dndEH.value,t.dnd_end_minute=u.dndEM.value,t.ticker=u.ticker.value,t.tc=u.tc.value,t.tspeed=u.tspeed.value,t.tu=u.tu.value}else if('weather' == o) {t.WX = u.WX.value,t.apikey = u.apikey.value,t.lon = u.lon.value,t.lat = u.lat.value,t.WXcommand = u.WXcommand.value}else return; fetch(n,{method:'PUT',headers:{'Content-Type':'application/json'},body:JSON.stringify(t)}).then(function(e){})},l.onclick=function(e){fetch('/api/wifi',{method:'DELETE',headers:{'Content-Type':'application/json'},body:''}).then(function(e){}),t.classList.add('disabled'),l.style.display='none',d.style.display='block'};};";
    return content;
}


String Gui::createColor() {
    String content = "";
    
    content += "<section id='color' class='active'>";
    
    content += "<div><table><td colspan=2><label>Vordergrundfarbe</label></td><td colspan=2><label>Hintergrundfarbe</label></td><tr>";
    content += "<td colspan=2><input id='fg' value='#" + Color::rgbToHex(Config::color_fg) + "' type='color'></td>";
    content += "<td colspan=2><input id='bg' value='#" + Color::rgbToHex(Config::color_bg) + "' type='color'></td></tr><tr>";

    content += "<td><label>Minute 1</label></td><td><label>2</label></td><td><label>3</label></td><td><label>4</label></td></tr><tr>";
    content += "<td><input id='m1' value='#" + Color::rgbToHex(Config::color_m1) + "' type='color'></td>";
    content += "<td><input id='m2' value='#" + Color::rgbToHex(Config::color_m2) + "' type='color'></td>";
    content += "<td><input id='m3' value='#" + Color::rgbToHex(Config::color_m3) + "' type='color'></td>";
    content += "<td><input id='m4' value='#" + Color::rgbToHex(Config::color_m4) + "' type='color'></td></tr><tr>";

    content += "<td colspan=2><label>Matrixfarbe</label></td><td><label>Matrixlänge</label></td><td><label>AnimationTime</label></td></tr><tr>";
    content += "<td colspan=2><input id='mx' value='#" + Color::rgbToHex(Config::color_mx) + "' type='color'></td>";
    content += "<td ><select id='mlength'>";
    for (int length = 0; length < 11; length++) {
        String label = String((int)length);
        content += Gui::htmlOption(label, String(length), String(Config::mlength));
    }
    content += "</select></td>";
    
    content += "<td><select id='atime'>";
    for (int atime = 10; atime <= 100; atime += 10) {
        String label = String((int)atime);
        content += Gui::htmlOption(label, String(atime), String(Config::atime));
    }
    
    content += "</select></td></tr></table></div>";

    content += "<div><label>Stromversorgung in mA</label><input id='power_supply' type='number' min=0 step='100' value='" + String(Config::power_supply) + "'>";
    content += "<label>Helligkeit</label>";
    content += "<select id='brightness'>";

    for (double brightness_percnt = 0.0; brightness_percnt < Led::getMaxBrightnessPercnt(); brightness_percnt += 0.1) {
        String label = String((int)(brightness_percnt * 100 + 0.5)) + "&percnt;"; // adding 0.5 for rounding.
        content += Gui::htmlOption(label, String(brightness_percnt), String(Config::brightness));
    }
    if (Led::getMaxBrightnessPercnt() < 1.0) { // show maximum brightness percentage between the 10% steps.
        String label = String((int)(Led::getMaxBrightnessPercnt() * 100 + 0.5)) + "&percnt;";
        content += Gui::htmlOption(label, String(Led::getMaxBrightnessPercnt()), String(Config::brightness));
    }

    content += "</select>";

    //Bell Animation selection
    content += "<label>Schlag Animation</label><select id='animation_bell'>";
    content += Gui::htmlOption("Inaktiv", String(0), String(Config::animation_bell));
    content += Gui::htmlOption("Stunden", String(1), String(Config::animation_bell));
    content += Gui::htmlOption("Stunden & Viertel", String(2), String(Config::animation_bell));
    content += "</select>";
    
    //Animation TYPE selection
    content += "<label>Animation Type</label><select multiple style='height:60px;' id='animation_type' >";
    int j = 0;
    int animation_type[15];
    for (int i = 1; i <= DEMO; i++) {
        if (Config::animation[j] == i) {
            animation_type[i] = i;
            j++;
        }else
            animation_type[i] = 0;
    }
  
    content += Gui::htmlOption("Matrix", String(1), String(animation_type[1]));
    content += Gui::htmlOption("Rainbow", String(2), String(animation_type[2]));
    content += Gui::htmlOption("Spirale OUT", String(3), String(animation_type[3]));
    content += Gui::htmlOption("Spirale IN", String(4), String(animation_type[4]));
    content += Gui::htmlOption("Regenschauer", String(5), String(animation_type[5]));
    content += Gui::htmlOption("TypeWriter", String(6), String(animation_type[6]));
    content += Gui::htmlOption("DropEffekt", String(7), String(animation_type[7]));
    content += Gui::htmlOption("FadeEffekt", String(8), String(animation_type[8]));
    content += Gui::htmlOption("BuchstabenLotto", String(9), String(animation_type[9]));
    content += Gui::htmlOption("MovingRainbow", String(10), String(animation_type[10]));
    content += Gui::htmlOption("Snake", String(11), String(animation_type[11]));
    content += Gui::htmlOption("Demo", String(12), String(animation_type[12]));
    content += "</select></div>";
    content += "</section>";
    return content;
}

String Gui::createTime() {
    String content = "";

    content += "<section id='time'>";
    content += "<div><label>Automatische Zeitzone</label><select id='tz_auto'>";
    content += Gui::htmlOption("Inaktiv", String(0), String(Config::automatic_timezone));
    content += Gui::htmlOption("Aktiv", String(1), String(Config::automatic_timezone));
    content += "</select></div>";
    content += "<div><label>Zeitzone</label>";
    if (Config::automatic_timezone) {
        content += "<select id='tz' disabled>";
    }
    else {
        content += "<select id='tz'>";
    }

    for (int i = -12; i < 13; i++) {
        String label = String(i);
        if (i > 0) {
            label = "+" + label;
        }
        content += Gui::htmlOption(label, String(i * 3600), String(Config::timezone));
    }
    content += "</select></div>";
    content += "<div><label>NTP-Server</label><input id='ntp' type='text' value='" + Config::ntp + "'></div>";
    content += "<div style='margin-top:40px'><label>mDNS-Name</label><input id='DNS_name' type='text' value='" + String(Config::DNS_name) + "'></div>";  
    content += "<div><label>Uhr Anzeige</label><select id='viertel'>";
    content += Gui::htmlOption("Viertel", String(1), String(Config::viertel));
    content += Gui::htmlOption("Dreiviertel", String(0), String(Config::viertel));
    content += "</select></div>";
    content += "</section>";
    return content;
}

String Gui::createDND() {
    String content = "";

    content += "<section id='dnd'>"; 
    content += "<div><label>Nachtmodus</label><select id='dnd_active'>";
    content += Gui::htmlOption("Inaktiv", String(0), String(Config::dnd_active));
    content += Gui::htmlOption("Aktiv", String(1), String(Config::dnd_active));
    content += "</select></div>";
    content += "<div><label>Start</label><div class='time'>";
    content += "<select id='dnd_s_h'>";
    for (int i = 0; i < 24; i++) { 
        content += Gui::htmlOption(Gui::pad(i), String(i), String(Config::dnd_start.hour));
    }
    content += "</select><span>:</span><select id='dnd_s_m'>";
    for (int i = 0; i < 60; i = i + 5) { //60
       content += Gui::htmlOption(Gui::pad(i), String(i), String(Config::dnd_start.minute));   
    }
    content += "</select></div></div>";
   
    content += "<div><label>Ende</label><div class='time'><select id='dnd_e_h'>";
    for (int i = 0; i < 24; i++) {
        content += Gui::htmlOption(Gui::pad(i), String(i), String(Config::dnd_end.hour));
    }
    content += "</select><span>:</span><select id='dnd_e_m'>";
    for (int i = 0; i < 60; i = i + 5) {
        content += Gui::htmlOption(Gui::pad(i), String(i), String(Config::dnd_end.minute));
    }
    content += "</select></div></div>";

    content += "<hr><div><label>Ticker String: $D atum</label><input id='ticker' type='text' maxlength='50' value='" + String(Config::ticker) + "'>";
    content += "<table><td><label>Ticker Farbe</label></td><td><label>Ticker Geschwindigkeit</label></td><tr>";
    content += "<td><input id='tc' value='#" + Color::rgbToHex(Config::color_tc) + "' type='color'></td>";
    content += "<td><select id='tspeed'>";
    for (int i = 50; i <= 500; i = i + 50) {
        content += Gui::htmlOption(String(i), String(i), String(Config::tspeed));
    }
    content += "</select></table>";
    content += "<label>Ticker update Minuten</label><select id='tu'>";
    for (int i = 0; i < 60; i += 2) {
        content += Gui::htmlOption(Gui::pad(i), String(i), String(Config::tu));
    }
    content += "</select></div></section>";
    return content;
}
//new weather page
String Gui::createWeather() {
    String content = "";

    content += "<section id='weather'>";
    content += "<div><label>OpenWeather update Minuten</label><select id='WX'>";
    for (int i = 0; i < 60; i+=2) {
        content += Gui::htmlOption(Gui::pad(i), String(i), String(Config::WX));
    }

    content += "</select></div>";
    content += "<div><label>Open Weather API KEY</label><input id='apikey' type='text' value='" + String(Config::apikey) + "'></div>";
    content += "<div><label>Position</label><div class='time'><span>Lon:</span><input id='lon' type='text' value='" + String(Config::lon) + "'><span>Lat:</span><input id='lat' type='text' value='" + String(Config::lat) + "'></div>";
    content += "<div><span>Ort: " + String(weather::City) + "</span><span>\t / " + String(weather::Country) + "</span></div></div>";
    content += "<div><label>Wetter String: $T,H,P,W,D,R,S (Temp $T $H Untergang $S)</label><input id='WXcommand' type='text' value='" + String(Config::WXcommand) + "'>";
    content += "<span>Temperature,Humidity,Pessure<br>Wind,Direction<br>R/S-Sonnen-auf/untergang</span></div>";
    content += "</section>";
    return content;
}

String Gui::createWIFI() {
    String content = "";

    content += "<section id='wifi'>";
    content += "<button id='reset_wifi' type='submit'>WiFi Einstellungen zurücksetzen</button>";
    content += "<div id='reset_wifi_message'>Die WiFi Einstellungen wurden zurückgesetzt. Es besteht keine Verbindung mehr zu der WordClock. Um die WordClock weiterhin zu verwenden muss das WiFi erneut eingerichtet werden.</div>";
    content += "</section>";
   
    return content;
}


String Gui::createFooter() {
  String content = "";

  content += "<footer>";
  content += "<b style='color:#ccc;font-size:small;padding-right:1em'>IP:" + HttpServer::ip + "&nbsp &nbsp" + String(Config::DNS_name) +"&nbsp &nbsp"+ String(VERSION) + "</b>";
  content += "<button id='save' type='submit' class='button'>Speichern</button>";
  content += "</footer>";

  return content;
}


String Gui::index(int part) {
    String contentI = "";
    switch (part) {
        case 1:
            contentI += "<!DOCTYPE html><html><head>";
            contentI += "<meta charset='UTF-8'>";
            //contentI += "<meta charset='iso-8859-1'>";
            contentI += "<link rel='manifest' href='/manifest.json'>";
            //content += "<link rel='icon' type='image/svg+xml' href='logo.svg' sizes='any'>";
            contentI += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
            contentI += "<meta http-equiv='Cache-Control' content='no-cache, no-store, must-revalidate'>"; // add no cache
            contentI += "<meta http-equiv='Pragma' content='no-cache'>";
            contentI += "<meta http-equiv='Expires' content='0'>";

            contentI += "<style>";
            contentI += Gui::createStyleSheet();
            contentI += "</style>";
            //file.print(content);
            contentI += "<script>";
            contentI += Gui::createScript();
            contentI += "</script></head>";
            return contentI;
            break;
        case 2:
            contentI = "<body>";
            contentI += "<div id='c'>";
            contentI += Gui::createNav(1);//split the nav 
            return contentI;
            break;
        case 3:
            contentI += Gui::createNav(2);
            contentI += "<main>";
            return contentI;
            break;
        case 4:
            contentI = Gui::createColor();
            return contentI;
            break;
        case 5:
            contentI = Gui::createTime();
            return contentI;
            break;
        case 6:
            contentI = Gui::createDND();
            return contentI;
            break;
        case 7:
            contentI = Gui::createWeather();
            contentI += Gui::createWIFI();
            contentI += "</main>";
            contentI += Gui::createFooter();
            contentI += "</div>";
            contentI += "</body></html>";
            return contentI;
            break;
        default:
            return "";
    }


}
