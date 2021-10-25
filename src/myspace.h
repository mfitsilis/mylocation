#pragma once

#include "table.h"
#include "Statistics.h"

#define ST std::string
#define SN std::to_string
#define SP ST(" ")
#define SM ST(";")
#define SBT ST(";`")
#define SB(x) ST(";`"+x)
#define SC ST(")")
#define CR ST("\n")

namespace myspace {
    ImGuiWindowFlags window_flags = 0;

    const char* wdays[] = { "Sat","Sun","Mon","Tue","Wed","Thu","Fri" };
    const char* weekdays[] = { "Saturday","Sunday","Monday","Tuesday","Wednesday","Thursday","Friday" };

    ImColor color_black = ImColor(0, 0, 0);
    ImColor color_red = ImColor(255, 0, 0);
    ImColor color_green = ImColor(0, 255, 0);
    ImColor color_yellow = ImColor(255, 255, 0);
    ImColor color_white = ImColor(255, 255, 255);
    ImColor color_cyan = ImColor(0, 255, 255);
    ImColor color_blue = ImColor(0, 0, 255);
    ImColor color_magenta = ImColor(255, 0, 255);

    bool show_geodata = true;
    bool show_statistics = true;
    bool show_altitude_plot = true;
    bool show_settings = true;
    bool show_mylocation = true;
    bool show_geodata_window = true;
    bool show_statistics_window = true;
    bool show_altitude_plot_window = true;
    bool show_settings_window = true;
    bool show_mylocation_window = true;
    bool show_console = false;

    Long tmst;
    Long timestarted;

    int  kdbport = 5900;
    bool isSubscribed = false;
    bool finished_thread = true;
    bool pbflag = false;

    int khandle = 0;
    int tday = 0;
    const int wknd = 7;

    std::string timestartedstr;
    char kdbhost[40] = "              ";
    char kdbhostLocal[40] = "localhost";
    char kdbhostCloud[40] = "              ";
    std::string kdbtsstr = "";
    std::string datetimestr;
    std::string timestr; // HH:MM:SS
    std::string datestr; // YYYY.mm.dd
    std::string datestr_start;

    std::vector<std::string> GeoData;
    
    char deviceName[6] = "";
    int deviceSelected = 0;

    int maxListSize = 20;

    Stats stats;
    Table* table;
    std::thread thread1;
}
