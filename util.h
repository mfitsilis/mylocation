#pragma once

#include <vector>

#include "mytypes.h"
#define KXVER 3
#include "k.h"

#define LOC_GMT (!pbflag?'l':'g')

int kdbtime2int(float f);

int indexOf(std::vector<std::string> vec, std::string item);

Long timestampNow();

int getdayofweek(char typ1);

std::string int2strtime(Long inttime, char typ1);

std::string int2strdatetime(Long inttime, char typ1);

std::string int2strdate(Long inttime, char typ1);

std::string strf(double x);

std::string strfx(float x, int y);

int connectToKdb(char* host, int kdbport);

int reconnectToKdb(char* host, int kdbport, int khandle);

