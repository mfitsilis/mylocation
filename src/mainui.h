#pragma once

namespace mainspace {

  extern HWND hwnd;  //handle for main window
  extern HWND hwnd2; //handle for console window
  extern HANDLE hIcon;
}

void showUI();

/** subscribe to kdb and listen for live updates */
void taskLiveUpdates(std::string inpstr);

void showAltitudePlotWindow();

void showSettingsWindow();

void showMyLocationWindow();

void showStatisticsWindow();

void showGeoDataWindow();

void updateDateTime();

void checkListBounds();

void init(); // called only once