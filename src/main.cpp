///============================================================== 
/*!
  \file   main.cpp
  \brief  

  <long description>

  \author Chang Liao
  \date   2016-10-03
*/
///============================================================== 
#include <fstream>
#include <iostream>
#include <string>
#include "anusplin.h"
using namespace std;
//===========================
int main(int argc, char *argv[])
{
  //initial program running status as success, the status variable changes to 0 if any step fails to proceed.
  int error_code = 1;
  ///=================================================================
  std::cout << "Start to run anusplin pro!" << endl;
  ///=================================================================
  ///here are some important variables to define a simulation run
  ///=================================================================
  std::string sFilename_configuration; ///<this is the argument user must provide, which is the configuration file
  ///=================================================================
  if (argc < 1)
    {
      cout << "No arguments are provided!" << endl;
      cout << "Please input the workspace:" << endl;
    }
  else
    {
      std::cout << "The following arguments are provided:" << endl;
      for (int i = 0; i < argc; i++)
        {
          cout << argv[i] << endl;
        }
      ///=================================================================
      ///the parameters must be prepared within the configuration file
      ///=================================================================

      std::string sWorkspace_in;
      std::string sWorkspace_out;
      std::string sFilename_splina;
      std::string sFilename_lapgrd;
      std::string sFilename_dem;
      std::string sFilename_log;
      int iFlag_preparation;
      int iFlag_splina;
      int iFlag_lapgrd;
      int iYear_start;
      int iYear_end;
      int iDay_start;
      int iDay_end;
      int ncolumn;
      int nrow;
      int unit_code;
      int nvariable;
      double dResolution;
      double dPause;
      std::string sVariable;
      std::vector<int> vUnit_code;
      std::vector<string> vVariable;
      std::array<double, 2 >aBoundary_lapgrd; ///< = { {231162.990809277, 1513567.631114728} };
      std::array<double, 4 >aBoundary_splina;
      ///=================================================================
      ///temporary variables for reading
      ///=================================================================
      std::string line = "";
      int iNumber;
      int iVariable;
      double dNumber;
      ///=================================================================
      ///we use the parameter file to input arguments
      ///=================================================================
      sFilename_configuration = argv[1];
      fstream ifs;
      ifs.open(sFilename_configuration.c_str(), ios::in);
      if (ifs.good())
        {
          std::cout << "Start to read the configuration file" << endl;
          ifs >> line;
          sWorkspace_in=line;
          ifs >> line;
          sWorkspace_out=line;
          ifs >> line;
          sFilename_splina=line;
          ifs >> line;
          sFilename_lapgrd=line;
          ifs >> line;
          sFilename_dem=line;
          ifs >> line;
          sFilename_log=line;
          ///==============================================
          ///the flag indicator whether splina and lapgrd need to run or not
          ///==============================================
          ifs >> iNumber;
          iFlag_preparation = iNumber;
          ifs >> iNumber;
          iFlag_splina= iNumber;
          ifs >> iNumber;
          iFlag_lapgrd= iNumber;
          ///==============================================
          ///other information is also needed
          ///==============================================
          ///time
          ///==============================================
          ifs >> iNumber;
          iYear_start= iNumber;
          ifs >> iNumber;
          iDay_start= iNumber;
          ifs >> iNumber;
          iYear_end= iNumber;
          ifs >> iNumber;
          iDay_end= iNumber;
          ///==============================================
          ///spatial
          ///==============================================
          ifs >> iNumber;
          ncolumn = iNumber;
          ifs >> iNumber;
          nrow= iNumber;
          ///==============================================
          ///resolution
          ///==============================================
          ifs >> dNumber;
          dResolution = dNumber;
          ///==============================================
          ///spatail location
          ///==============================================
          ifs >> dNumber;
          aBoundary_lapgrd[0] = dNumber;
          ifs >> dNumber;
          aBoundary_lapgrd[1] = dNumber;
          ///==============================================
          ///variables
          ///==============================================
          ifs >> iNumber;
          nvariable = iNumber;
          vVariable.clear();///this may be uncessary
          vUnit_code.clear();
          for ( iVariable = 1 ; iVariable<=nvariable; iVariable++)
            {
              ///=============================================
              ///variable name, should be the same with directory name
              ///=============================================
              ifs >> line;
              vVariable.push_back(line);
              ///==============================================
              ///unit
              ///==============================================
              ifs >> iNumber;
              vUnit_code.push_back(iNumber);

            }
          ifs >> dNumber;
          dPause = dNumber;
          std::cout << "Finished reading the configuration file" << endl;
          ifs.close();
        }
      else
        {
          std::cout << "Cannot open the configuration file" << endl;
          error_code = 0;
          return error_code;
        }

      ///=================================================================
      ///check the parameter
      ///=================================================================
      std::cout << "Start to check the parameters" << endl;
      if (file_test(sFilename_splina) != 1 )
        {
          error_code = 0;
          std::cout << "Splina is missing" << endl;
          return error_code;
        }
      if (file_test(sFilename_lapgrd) != 1 )
        {
          error_code = 0;
          std::cout << "Lapgrd is missing" << endl;
          return error_code;
        }
      if (file_test(sFilename_dem) != 1 )
        {
          error_code = 0;
          std::cout << "DEM is missing" << endl;
          return error_code;
        }
      std::cout << "Finished checking the parameters" << endl;
      ///=================================================================
      ///calculate the larger domain for splina
      ///=================================================================
      aBoundary_splina[0] = aBoundary_lapgrd[0] - 2 * ncolumn * dResolution;
      aBoundary_splina[1] = aBoundary_lapgrd[0] + ncolumn * dResolution * 3;
      aBoundary_splina[2] = aBoundary_lapgrd[1] - 2 * nrow * dResolution;
      aBoundary_splina[3] = aBoundary_lapgrd[1] + nrow * dResolution * 3;
      ///======================================
      ///start anusplin pro
      ///======================================
      anusplin anu;
      std::cout << "Start to run anusplin pro program" << endl;
      for ( iVariable = 1; iVariable <= nvariable; iVariable++)
        {
          sVariable = vVariable[iVariable-1];
          unit_code= vUnit_code[iVariable-1];
          std::cout << "Start run for variable: " << sVariable << endl;
          ///=============================================================
          ///prepare the files
          ///=============================================================
          if (iFlag_preparation == 1)
            {
              anu.prepare_anusplin_splina_and_lapgrd_cmd
                (
                 iDay_end,
                 iDay_start,
                 iYear_end,
                 iYear_start,
                 ncolumn,
                 nrow,
                 unit_code,
                 dResolution,
                 aBoundary_lapgrd,
                 aBoundary_splina,
                 sFilename_dem,
                 sFilename_lapgrd,
                 sFilename_splina,
                 sVariable,
                 sWorkspace_in,
                 sWorkspace_out
                 );
            }
          ///=============================================================
          ///run the splina program
          ///=============================================================

          if (iFlag_splina == 1)
            {
              anu.run_splina
                (
                 iDay_end,
                 iDay_start,
                 iYear_end,
                 iYear_start,
		 dPause,
                 sVariable,
                 sWorkspace_in
                 );
            }
          ///=============================================================
          ///run the lapgrd program
          ///=============================================================
          if (iFlag_lapgrd == 1)
            {
              anu.run_lapgrd
                (
                 iDay_end,
                 iDay_start,
                 iYear_end,
                 iYear_start,
		 dPause,
                 sVariable,
                 sWorkspace_in
                 );
            }
          std::cout << "Finished run for variable: " << sVariable << endl;
        }
      std::cout << "Finished all variables for anusplin pro program! ";
    }
  return error_code;
}
