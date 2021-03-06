///==============================================================
/*!
  \file   anusplin.cpp
  \brief

  <long description>

  \author Chang Liao
  \date   2016-09-30
*/
///==============================================================
#include "anusplin.h"
anusplin::anusplin()
{
}
anusplin::~anusplin()
{
}
///==============================================================
/*!
  <long-description>

  \param iDay_end
  \param iDay_start
  \param iYear_end
  \param iYear_start
  \param ncolumn
  \param nrow
  \param unit_code
  \param dResolution
  \param double
  \param aBoundary_lapgrd
  \param double
  \param aBoundary_splina
  \param sFilename_dem
  \param sFilename_lapgrd
  \param sFilename_splina
  \param sVariable
  \param sWorkspace_in
  \param sWorkspace_out
  \return <ReturnValue>
*/
///==============================================================

int anusplin::prepare_anusplin_splina_and_lapgrd_cmd(
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
                                                     )
{
  int error_code = 1;
  ///====================================================
  int start_day1;
  int end_day1;
  ///==============================================================
  ///get the boundary from parameters
  ///=================================================================
  double lower_x_splina = aBoundary_splina[0];
  double upper_x_splina = aBoundary_splina[1];
  double lower_y_splina = aBoundary_splina[2];
  double upper_y_splina = aBoundary_splina[3];
  double lower_z_splina = -100; ///<elevation
  double upper_z_splina = 9000; ///<elevation
  ///===================================================================
  double lower_x_lapgrd = aBoundary_lapgrd[0];
  double upper_x_lapgrd = lower_x_lapgrd + dResolution * ncolumn;
  double lower_y_lapgrd = aBoundary_lapgrd[1];
  double upper_y_lapgrd = lower_y_lapgrd + dResolution * nrow;
  ///==============================================================
  ///temporay variables
  ///==============================================================
  std::string sYear_str;
  std::string sYear_out;
  std::string sDay_str;
  std::string sDay_out;
  std::string sFilename_txt;
  ///============================================================
  ///splina
  ///============================================================
  std::string sFilename_splina_configuration;///parameters
  std::string sFilename_splina_shell;
  std::string sFilename_splina_bat;///<parameters for Windows
  std::string sFilename_splina_log;///<parameters for Linux
  std::string sFilename_splina_copy;
  std::string sCommand_splina;
  ///============================================================
  ///lapgrd
  ///============================================================
  std::string sFilename_lapgrd_configuration;
  std::string sFilename_lapgrd_shell;
  std::string sFilename_lapgrd_bat;
  std::string sFilename_lapgrd_log;
  std::string sFilename_lapgrd_copy;
  std::string sCommand_lapgrd;
  std::string sCommand_copy;
  ///===============================================================
  std::ofstream ofs;
  ///================================================================
  ///start year loop
  std::cout << "Start to prepare the ini and bat/shell file" << endl;
  for (int iYear = iYear_start; iYear <= iYear_end; iYear++)
    {
      sYear_str = convert_integer_to_string(iYear, 4);
      sYear_out = sWorkspace_out + slash + sVariable + slash + sYear_str;
      ///================================================================
      ///check whether this folder exist or not

      ///================================================================
      /// calculat the start and end
      ///================================================================
      if (iYear == iYear_start)
        {
          start_day1 = iDay_start;
        }
      else
        {
          start_day1 = 1;
        }
      if (iYear == iYear_end)
        {
          end_day1 = iDay_end;
        }
      else
        {
          if (leap_year(iYear) == 1)
            {
              end_day1 = 366;
            }
          else
            {
              end_day1 = 365;
            }
        }
      ///==============================================================
      /// start the day loop output
      ///==============================================================
      for (int iDay = start_day1; iDay <= end_day1; iDay++)
        {
          sDay_str = convert_integer_to_string(iDay, 3);
          sDay_out = sYear_out + slash + sYear_str + sDay_str;
          ///================================================================
          ///check whether this folder exist or not
          ///================================================================
          sFilename_txt = sDay_out + slash + sYear_str + sDay_str + ".txt";
          if (file_test( sFilename_txt) != 1  )
            {
              continue;
            }
          else
            {
              ///the folder already exists
            }
          ///================================================================
          ///prepare the splina command file
          ///================================================================
          sFilename_splina_configuration = sDay_out + slash + "splina.ini";
          ofs.open(sFilename_splina_configuration.c_str(), ios::out);
          ofs << "surface" << endl; ///< the the title of this surface fit
          ofs << unit_code << endl; ///<user defined
          ofs << "2" << endl; ///< x and y location as the independent spline variable
          ofs << "1" << endl; ///< elevation as independdent spline covariates
          ofs << "0" << endl; ///< no surface independent spline variable
          ofs << "0" << endl; ///< no surface independent covariates
          ofs << convert_double_to_string(lower_x_splina, 16, 6)
            + convert_double_to_string(upper_x_splina, 16, 6) + " 0 1" << endl;
          ofs << convert_double_to_string(lower_y_splina, 16, 6)
            + convert_double_to_string(upper_y_splina, 16, 6) + " 0 1" << endl;
          ofs << convert_double_to_string(lower_z_splina, 16, 6)
            + convert_double_to_string(upper_z_splina, 16, 6) + " 1 1" << endl;
          ofs << "1000.0" << endl;
          ofs << "0" << endl;
          ofs << "3" << endl;
          ofs << "1" << endl;
          ofs << "0" << endl;
          ofs << "1" << endl;
          ofs << "1" << endl;
          ofs << sDay_out + slash + sYear_str + sDay_str + ".txt" << endl;
          ofs << "10000" << endl;
          ofs << "15" << endl; ///<site label length
          ofs << "(a15,2f16.6,f12.6,f16.6)" << endl;
          ofs << "10000" << endl;  ///<knot, select all data points as knot
          ofs << " " << endl; ///< no knot index file
          ofs << " " << endl; ///< no bad data flag file
          ofs << " " << endl; ///< no output bad data file
          ofs << " " << endl; ///< no residual file
          ofs << " " << endl; ///< no cross validataion file
          ofs << " " << endl; ///< no optimisaztion file
          ofs << sDay_out + slash + sYear_str + sDay_str + ".sur" << endl;  ///<surface coefficients file
          ofs << sDay_out + slash + sYear_str + sDay_str + ".cov" << endl; ///<error covaiance file
          ofs << " " << endl; ///< no output list file
          ofs << " " << endl; ///< no output cross validation file
          ofs << " " << endl; ///< no input test file
          for ( int l = 1; l <= 4; l++) ///< not test file is provided
            {
              ofs << " " << endl; ///<for empty line loop
            }
          ofs.close();
          ///=========================================================================
          ///prepare the lapgrd command file
          ///================================================================
          sFilename_lapgrd_configuration = sDay_out + slash + "lapgrd.ini";
          ofs.open(sFilename_lapgrd_configuration.c_str(), ios::out);
          ofs << sDay_out + slash + sYear_str + sDay_str + ".sur" << endl; ///the surface file from the splina file
          ofs << "1" << endl;
          ofs << "1" << endl; ///<calculate surface value
          ofs << sDay_out + slash + sYear_str + sDay_str + ".cov" << endl; ///error covariance file from splina
          ofs << "2" << endl; ///<prediction error
          ofs << " " << endl;
          ofs << "1" << endl; ///<center or left corner, 1 is center
          ofs << "1" << endl; ///<index of x
          ofs << convert_double_to_string(lower_x_lapgrd, 16, 6) + " "
            + convert_double_to_string(upper_x_lapgrd, 16, 6) + " " + convert_double_to_string(dResolution, 16, 6) << endl;
          ofs << "2" << endl; ///<index of y
          ofs << convert_double_to_string(lower_y_lapgrd, 16, 6) + " "
            + convert_double_to_string(upper_y_lapgrd, 16, 6) + " " + convert_double_to_string(dResolution, 16, 6) << endl;
          ofs << "0" << endl; ///< no mask grid
          ofs << "2" << endl; ///< used provided ArcGIS grid
          ofs << sFilename_dem << endl; ///<dem grid file
          ofs << "2" << endl; ///<output grid type
          ofs << "-9999.0" << endl; ///< no data in output
          ofs << sDay_out + slash + sYear_str + sDay_str + ".dat" << endl; ///<output grid file
          ofs << " " << endl; ///<unformatted binary
          ofs << "2" << endl;
          ofs << "-9999" << endl;
          ofs << sDay_out + slash + sYear_str + sDay_str + ".err" << endl;
          ofs << "(f8.2)";
          ofs.close();
          ///===============================================================
#if defined(__linux__)
          ///===============================================================
          ///write spline bash file
          ///===============================================================
          sFilename_splina_shell = sDay_out + slash + "splina.sh";
          ofs.open(sFilename_splina_shell.c_str(), ios::out);
          sFilename_splina_log = sDay_out + slash + "splina.log";
          sFilename_splina_copy =  sDay_out + "/splina";
          ///copy file
          sCommand_copy = "cp -f " + sFilename_splina + " " + sFilename_splina_copy;
          system( sCommand_copy.c_str());
          sCommand_splina = sFilename_splina_copy + " < " + sFilename_splina_configuration + " > " + sFilename_splina_log;
          ofs << sCommand_splina << endl;
          ofs.close();
          ///===============================================================
          ///change the permission on linux
          ///===============================================================
          chmod(sFilename_splina_shell.c_str(), S_IRWXU | S_IRGRP | S_IROTH);
          ///===============================================================
          ///write bash file
          ///===============================================================
          sFilename_lapgrd_shell = sDay_out + slash + "lapgrd.sh";
          ofs.open(sFilename_lapgrd_shell.c_str(), ios::out);
          sFilename_lapgrd_log = sDay_out + slash + "lapgrd.log";
          sFilename_lapgrd_copy =  sDay_out + "/lapgrd";
          ///copy file
          sCommand_copy = "cp -f " + sFilename_lapgrd + " " + sFilename_lapgrd_copy;
          system( sCommand_copy.c_str());
          sCommand_lapgrd = sFilename_lapgrd_copy + " < " + sFilename_lapgrd_configuration + " > " + sFilename_lapgrd_log;
          ofs << sCommand_lapgrd << endl;
          ofs.close();
          ///===============================================================
          ///change the permission on linux
          ///===============================================================
          chmod(sFilename_lapgrd_shell.c_str(), S_IRWXU | S_IRGRP | S_IROTH);
          ///===============================================================
#else
          ///===============================================================
          ///write the batch file to run this splina
          ///===============================================================
          sFilename_splina_bat = sDay_out + slash + "splina.bat";
          ofs.open(sFilename_splina_bat.c_str(), ios::out);
          sFilename_splina_log = sDay_out + slash + "splina.log";
          sCommand_splina = sFilename_splina + " < " + sFilename_splina_configuration + " > " + sFilename_splina_log;
          ofs << sCommand_splina << endl;
          ofs.close();
          ///===============================================================
          ///write the batch file to run this splina
          ///===============================================================
          sFilename_lapgrd_bat = sDay_out + slash + "lapgrd.bat";
          ofs.open(sFilename_lapgrd_bat.c_str(), ios::out);
          sFilename_lapgrd_log = sDay_out + slash + "lapgrd.log";
          sCommand_lapgrd = sFilename_lapgrd + " < " + sFilename_lapgrd_configuration + " > " + sFilename_lapgrd_log;
          ofs << sCommand_lapgrd << endl;
          ofs.close();
          ///===============================================================
#endif
          ///===============================================================
        } ///<end day loop
    } ///<end year loop
  std::cout << "Prepare splina and lapgrd cmd for is finished!" << endl;
  return error_code;
}
///==============================================================
/*!
  <long-description>

  \param iDay_end
  \param iDay_start
  \param iYear_end
  \param iYear_start
  \param dPause
  \param sVariable
  \param sWorkspace_in
  \return <ReturnValue>
*/
///==============================================================

int anusplin::run_splina(
                         int iDay_end,
                         int iDay_start,
                         int iYear_end,
                         int iYear_start,
                         double dPause,
                         std::string sVariable,
                         std::string sWorkspace_in
                         )
{
  int error_code = 1;
  int start_day1;
  int end_day1;
  std::string sYear_str;
  std::string sYear_in;
  std::string sDay_str;
  std::string sDay_in;
  std::string sFilename_txt;
  ///===============================================================
#if defined(__linux__)
  std::string sFilename_splina_shell;
  ///pid_t pid; ///<process id
  ///int rv; ///<return value from child process
#else
  std::string sFilename_splina_bat;
  std::wstring stemp;
  LPWSTR sw;
  DWORD flags = CREATE_NEW_CONSOLE;
#endif
  std::cout << "Start to run splina" << endl;
  ///===============================================================

  for (int iYear = iYear_start; iYear <= iYear_end; iYear++)
    {
      sYear_str = convert_integer_to_string(iYear, 4);
      sYear_in = sWorkspace_in + slash + sVariable + slash + sYear_str;
      ///==============================================================
      ///the path must exists
      ///==============================================================
      if( path_test(sYear_in) != 1 )
        {
          continue;
        }
      ///==============================================================
      /// calculat the start and end
      ///==============================================================
      if (iYear == iYear_start)
        {
          start_day1 = iDay_start;
        }
      else
        {
          start_day1 = 1;
        }
      ///==============================================
      if (iYear == iYear_end)
        {
          end_day1 = iDay_end;
        }
      else
        {
          if (leap_year(iYear) == 1)
            {
              end_day1 = 366;
            }
          else
            {
              end_day1 = 365;
            }
        }
      ///==============================================================
      ///==============================================================
#pragma omp parallel for private(sDay_str, sDay_in, sFilename_txt, sFilename_splina_shell )
      for (int iDay = start_day1; iDay <= end_day1; iDay++)
        {
          sDay_str = convert_integer_to_string(iDay, 3);
          sDay_in = sYear_in + slash + sYear_str + sDay_str;
          if( path_test(sDay_in) != 1 )
            {
              continue;
            }
          sFilename_txt = sDay_in + slash + sYear_str + sDay_str + ".txt";
          if( file_test(sFilename_txt) != 1 )
            {
              continue;
            }

#if defined(__linux__)

          sFilename_splina_shell = sDay_in + slash + "splina.sh";
          system( sFilename_splina_shell.c_str());
          std::cout << "Variable: " << sVariable << "; Process: splina; Date: " << sYear_str << " " << sDay_str << endl;

          ///================================
          ///wait
          ///================================
          usleep( dPause * 1000000.0); ///<that is 10 seconds waiting for the next scan
          ///===================================================================
#else
          sFilename_splina_bat = sDay_in + slash + "splina.bat";
          ///======================================================
          ///use thread to run multi processes
          ///======================================================
          STARTUPINFO si;
          PROCESS_INFORMATION pi;
          ZeroMemory(&si, sizeof(si));
          si.cb = sizeof(si);
          ZeroMemory(&pi, sizeof(pi));
          stemp = std::wstring(sFilename_splina_bat.begin(), sFilename_splina_bat.end());
          sw = (LPWSTR)(stemp.c_str());
          /// Start the child process.
          if (!CreateProcess(NULL,   // No module name (use command line)
                             sw,        // Command line
                             NULL,           // Process handle not inheritable
                             NULL,           // Thread handle not inheritable
                             FALSE,          // Set handle inheritance to FALSE
                             flags,              // No creation flags
                             NULL,           // Use parent's environment block
                             NULL,           // Use parent's starting directory
                             &si,            // Pointer to STARTUPINFO structure
                             &pi)           // Pointer to PROCESS_INFORMATION structure
              )
            {
              std::cout << "CreateProcess failed"<<endl;
              continue;

            }
          ///================================
          ///wait for 5 seconds
          ///================================
          Sleep(dPause * 1000000.0); ///<depreciated
          ///=================================================================================================
#endif

        }
    }
  std::cout << "Finishe to run splina!" << endl;
  return error_code;
}

///==============================================================
/*!
  <long-description>

  \param iDay_end
  \param iDay_start
  \param iYear_end
  \param iYear_start
  \param dPause
  \param sVariable
  \param sWorkspace_in
  \return <ReturnValue>
*/
///==============================================================

int anusplin::run_lapgrd(
                         int iDay_end,
                         int iDay_start,
                         int iYear_end,
                         int iYear_start,
                         double dPause,
                         std::string sVariable,
                         std::string sWorkspace_in
                         )
{
  int error_code = 1;
  int start_day1;
  int end_day1;
  std::string sYear_str;
  std::string sYear_in;
  std::string sDay_str;
  std::string sDay_in;
  std::string sFilename_txt;
#if defined(__linux__)
  std::string sFilename_lapgrd_shell;
  ///pid_t pid;
  ///int rv;
#else
  std::string sFilename_lapgrd_bat;
  std::wstring stemp;
  LPWSTR sw;
  DWORD flags = CREATE_NEW_CONSOLE;
#endif
  std::cout << "Start to run lapgrd" << endl;
  ///==================================================

  for (int iYear = iYear_start; iYear <= iYear_end; iYear++)
    {
      sYear_str = convert_integer_to_string(iYear, 4);
      sYear_in = sWorkspace_in + slash + sVariable + slash + sYear_str;
      ///==============================================================
      ///the path must exists
      ///==============================================================
      if( path_test(sYear_in) != 1 )
        {
          continue;
        }
      ///==============================================================
      /// calculat the start and end
      ///==============================================================
      if (iYear == iYear_start)
        {
          start_day1 = iDay_start;
        }
      else
        {
          start_day1 = 1;
        }
      if (iYear == iYear_end)
        {
          end_day1 = iDay_end;
        }
      else
        {
          if (leap_year(iYear) == 1)
            {
              end_day1 = 366;
            }
          else
            {
              end_day1 = 365;
            }
        }
      //!
      /*!
        We added the openmp to improve speed here
      */

#pragma omp parallel for private(sDay_str, sDay_in, sFilename_txt, sFilename_lapgrd_shell )
      for (int iDay = start_day1; iDay <= end_day1; iDay++)
        {
          sDay_str = convert_integer_to_string(iDay, 3);
          sDay_in = sYear_in + slash + sYear_str + sDay_str;
          if( path_test(sDay_in) != 1 )
            {
              error_code = 0;
              continue;
            }
          sFilename_txt = sDay_in + slash + sYear_str + sDay_str + ".txt";
          if( file_test(sFilename_txt) != 1 )
            {
              continue;
            }

#if defined(__linux__)

          sFilename_lapgrd_shell = sDay_in + slash + "lapgrd.sh";
          system( sFilename_lapgrd_shell.c_str());
          std::cout << "Variable: " << sVariable << "; Process: lapgrd; Date: " << sYear_str << " " << sDay_str << endl;


          ///======================================================
          ///wait
          ///======================================================
          usleep( dPause * 1000000.0); ///<that is 10 seconds waiting for the next scan

          ///======================================================
#else
          sFilename_lapgrd_bat = sDay_in + slash + "lapgrd.bat";
          ///======================================================
          ///use thread to run multi processes
          ///======================================================
          STARTUPINFO si;
          PROCESS_INFORMATION pi;
          ZeroMemory(&si, sizeof(si));
          si.cb = sizeof(si);
          ZeroMemory(&pi, sizeof(pi));
          stemp = std::wstring(sFilename_bat.begin(), sFilename_bat.end());
          sw = (LPWSTR)(stemp.c_str());
          // Start the child process.
          if (!CreateProcess(NULL,   // No module name (use command line)
                             sw,        // Command line
                             NULL,           // Process handle not inheritable
                             NULL,           // Thread handle not inheritable
                             FALSE,          // Set handle inheritance to FALSE
                             flags,              // No creation flags
                             NULL,           // Use parent's environment block
                             NULL,           // Use parent's starting directory
                             &si,            // Pointer to STARTUPINFO structure
                             &pi)           // Pointer to PROCESS_INFORMATION structure
              )
            {
              std::cout << "CreateProcess failed"<<endl;
              continue;

            }
          ///======================================================
          ///wait for seconds
          ///======================================================
          Sleep( dPause * 1000000.0);
          ///======================================================
#endif

        }
    }
  std::cout << "Finished to run lapgrd" << endl;
  return error_code;
}
