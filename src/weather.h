#pragma once

class weather {
public:
    static float temperature;
    static String City;
    static String Country;
    static String GetWeather();
    static void SetGrad(bool state);
    static String httpGETRequest(const char* serverName);
    static CRGB GetTempColor(float temperature);
};
