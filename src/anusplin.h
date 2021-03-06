#pragma once

///================================
///c header
///================================

///================================
///c++ header
///================================
#include <array>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
///================================
///cros-platform header
///================================
#ifdef __linux__
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#elif _WIN32
#include <direct.h>
#include <windows.h>
#else
#endif
///=================================
///local header
///================================
#include "system.h"
///==========================================
using namespace std;
///==========================================
class anusplin
{
 public:
  anusplin();
  ~anusplin();
  ///==================================================
  //functions
  ///==================================================
  int prepare_anusplin_splina_and_lapgrd_cmd
    (
     int iDay_end,
     int iDay_start,
     int iYear_end,
     int iYear_start,
     int ncolumn,
     int nrow,
     int unit_code,
     double dResolution,
     std::array<double, 2> aBoundary_lapgrd,
     std::array<double, 4> aBoundary_splina,
     std::string sFilename_dem,
     std::string sFilename_lapgrd,
     std::string sFilename_splina,
     std::string sVariable,
     std::string sWorkspace_in,
     std::string sWorkspace_out
     );
  int run_splina
    (
     int iDay_end,
     int iDay_start,
     int iYear_end,
     int iYear_start,
     double dPause,
     std::string sVariable,
     std::string sWorkspace_in
     );
  int run_lapgrd
    (
     int iDay_end,
     int iDay_start,
     int iYear_end,
     int iYear_start,
     double dPause,
     std::string sVariable,
     std::string sWorkspace_in
     );
};
