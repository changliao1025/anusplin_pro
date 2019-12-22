#pragma once
//the global variables for all other modules
//c library
#include <cstring>
#include <cstdio>
#include <cmath>
//c++ library
#include <array>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
//cros-platform header
#ifdef __linux__
#include <sys/stat.h>
#include <sys/types.h>
#elif _WIN32
#include <direct.h>
#include <windows.h>
#else
#endif
#include "global.h"
using namespace std;
//=============================================

//=============================================================================================
std::string convert_integer_to_string(int iNumber, int iWidth);
std::string convert_double_to_string(double dNumber, int iWidth, int iPrecision);
int file_test(std::string sFilename);
int make_directory(std::string sDirectory);
int path_test(std::string sPath);

std::string find_alternative_file
(
 int iYear,
 int iDay,
 std::string sExtension,
 std::string sPrefix,
 std::string sWorkspace
 );
int leap_year(int year);
