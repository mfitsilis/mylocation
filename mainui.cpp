
#include <iostream>
#include <thread>

#include "windows.h"  // defines ShowWindow 
#include "util.h"
#include "myspace.h"
#include "mainui.h"

// forward declarations
void showStatisticsWindow();
void showGeoDataWindow();
void showAltitudePlotWindow();
void showSettingsWindow();
void showMyLocationWindow();
void updateDateTime();
void checkListBounds();

using namespace myspace;

using namespace mainspace;

void showUI() {

    updateDateTime();

    if (show_geodata_window) {
        showGeoDataWindow();
    }

    if (show_statistics_window) {
        showStatisticsWindow();
    }

    if (show_altitude_plot_window) {
        showAltitudePlotWindow();
    }

    if (show_settings_window) {
        showSettingsWindow();
    }

    if (show_mylocation_window) {
        showMyLocationWindow();
    }
}

/** subscribe to kdb and listen for live updates */
void taskLiveUpdates(std::string inpstr = "") {
    K kresultUpd=nullptr;
    if (false == isSubscribed) {
        kresultUpd = k(khandle, S(".u.sub[`location;`]"), (K)0);
        isSubscribed = true;
    }
    while (1) {
        if (isSubscribed == false) {
            break;
        }
        else
        if ((0 < khandle) && (true == isSubscribed)) {
            table->getDataUpd(kresultUpd);
        }
    }
}

void showAltitudePlotWindow() {
    ImGui::Begin("Plot Altitude", &show_altitude_plot_window, window_flags);
    if (ImGui::Button("show altitude chart")) {
        show_altitude_plot = !show_altitude_plot;
    };
    ImGui::SameLine();
    ImGui::Text("num of points: %d", table != nullptr ? table->getNRows() : 0);
    if (show_altitude_plot) {
        ImGui::InputText("device", deviceName, IM_ARRAYSIZE(deviceName));
        if ((table != nullptr) && (deviceSelected >= 0)) {
            ImGui::PushItemWidth(400);
            ImGui::PlotLines("altitude", table->getAltitude()->getHeights().data(), table->getAltitude()->getSize(), 0, NULL, table->getAltitude()->getMinHeight(), table->getAltitude()->getMaxHeight(), ImVec2(0, 100));
            ImGui::PopItemWidth();
        }
    }
    ImGui::End();
}

void showSettingsWindow() {
    ImGui::Begin("Settings", &show_settings_window, window_flags);
    ImGui::Checkbox("enable console", &show_console);
    if (show_console) ShowWindow(hwnd2, SW_SHOW);
    else ShowWindow(hwnd2, SW_HIDE);

    ImGui::PushItemWidth(250);
    
    char* hosts[] = { "cloud", "localhost" };
    static int selectedHost = 0;
    ImGui::Combo("combo", &selectedHost, hosts, IM_ARRAYSIZE(hosts));
    if (selectedHost==0)
        strcpy_s(kdbhost, kdbhostCloud);
    else 
        strcpy_s(kdbhost, kdbhostLocal);
    ImGui::InputInt("kdb port", &kdbport, false);
    ImGui::Text("connection handle: %d", khandle);
    ImGui::SameLine();
    if (ImGui::Button("Reconnect")) {
        khandle = reconnectToKdb(kdbhost, kdbport, khandle);
        isSubscribed = false;
    }

    ImGui::Text("subscription: %s", isSubscribed==true ?"on":"off");
    ImGui::NewLine();

    ImGui::Text("started on: %s", timestartedstr.c_str());
    ImGui::Text("date: %s time : %s day: %s", datestr.c_str(), timestr.c_str(), weekdays[tday]);

    if (ImGui::Button("fetch geoData from kdb") && (khandle>0) ) {
        if (table != nullptr) {
            delete table;
            khandle = reconnectToKdb(kdbhost, kdbport, khandle);
            isSubscribed = false;
        }
        table = new Table(khandle, std::string("select from location"));
        std::cout << "fetching kdb data \n";

        if (false == isSubscribed) {
            std::thread thread1(&taskLiveUpdates, "");
            thread1.detach();
        }
    }

    ImGui::PopItemWidth();

    ImGui::End();
}

void showMyLocationWindow() {
    ImGui::Begin("MyLocation", &show_mylocation_window);
    ImGui::TextColored(color_blue, "Points per Device");
    ImGui::PushItemWidth(250);
    ImGui::PopItemWidth();

    if (table != nullptr)
        ImGui::PlotHistogram("num of Points", table->getPointsPerCity().data(), table->getPointsPerCity().size(), 0, NULL, 0.0f, (float)table->getMaxPointsPerCity(), ImVec2(0, 100));

    ImGui::PushItemWidth(500);
   
    ImGui::PopItemWidth();
    ImGui::End();

}

void showStatisticsWindow() {
    ImGui::Begin("Statistics", &show_statistics_window, window_flags);
    if (ImGui::Button("show device statistics")) {
        show_statistics = !show_statistics;
    }
    if (show_statistics) {
        
        if (ImGui::Button("Update Statistics")) {
            K minAlt = table->kdbquery(kdbhost,kdbport, "exec min alt from location");
            stats.minAlt = minAlt->f;
            K maxAlt = table->kdbquery(kdbhost,kdbport, "exec max alt from location");
            stats.maxAlt = maxAlt->f;
            K avgAlt = table->kdbquery(kdbhost,kdbport, "exec avg alt from location");
            stats.avgAlt = avgAlt->f;
            K numOfCities = table->kdbquery(kdbhost, kdbport, "\"f\"$count select distinct city from location");
            stats.numOfCities = numOfCities->f;
        }
                
        ImGui::Text("min altitude: %f", stats.minAlt);
        ImGui::Text("max altitude: %f", stats.maxAlt);
        ImGui::Text("avg altitude: %f", stats.avgAlt);
        ImGui::Text("number of cities: %.0f", stats.numOfCities);
    }
    ImGui::End();
}

void showGeoDataWindow() {
    ImGui::Begin("GeoData", &show_geodata_window, window_flags);
    if (ImGui::Button("show geoData")) {
        show_geodata = !show_geodata;
    }; ImGui::SameLine(); ImGui::InputInt("list size", &maxListSize);
    checkListBounds();
    if (show_geodata) {
        ImGui::InputText("device", deviceName, IM_ARRAYSIZE(deviceName));
        {
            ImGui::PushItemWidth(400);

            ImGui::Text("    #      DateTime        GeoX     GeoY       Altitude  Speed       City    Country");

            if ((table != nullptr) && (table->getNRows() > 0)) {
                for (Long i = table->getNRows() - 1; i >= table->getNRows() - maxListSize - 1; i--) {
                    table->printRow(i);
                }
            }
            ImGui::PopItemWidth();
        }
    }
    ImGui::End();

}

void updateDateTime() {
    tmst = timestampNow();
    datestr = int2strdate(tmst / 1000 + 6 * 3600, 'l'); // tz offset 6 hours
    timestr = int2strtime(tmst / 1000 + 6 * 3600, 'l');
    tday = getdayofweek('l');
}

void checkListBounds() {
    if (table != nullptr) {
        if (maxListSize < 0) maxListSize = 0;
        else if (maxListSize > (int)table->getNRows() - 1) maxListSize = (int)table->getNRows() - 1;
    }
}

void init() {

    bool no_titlebar = false;
    bool no_scrollbar = false;
    bool no_menu = false;
    bool no_move = false;
    bool no_resize = false;
    bool no_collapse = false;
    bool no_nav = false;

    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)       window_flags |= ImGuiWindowFlags_NoNav;

    timestarted = timestampNow();
    timestartedstr = int2strdatetime(timestarted / 1000 + 6 * 3600, 'l');

    khandle = connectToKdb(kdbhost, kdbport);

}