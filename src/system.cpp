#include "system.h"
///===================================================================
///<it is used to convert integer to string since c++ 11 doesn't support well
///===================================================================
std::string convert_integer_to_string(int iNumber, int iWidth)
{
  std::string str = "";
  stringstream ss;
  ss << setfill('0') << setw(iWidth) << iNumber;
  str = ss.str();
  return str;
}
std::string convert_double_to_string(double dNumber, int iWidth, int iPrecision)
{
  std::string str = "";
  stringstream ss;
  ss<<std::fixed<<std::setw(iWidth) << std::setprecision(iPrecision) << dNumber;
  str = ss.str();
  return str;
}
//=============================================================================================
//file and path operation functions
//=============================================================================================
//test the existance of a file
//not the most efficient, but easy to understand for now
//=============================================================================================
int file_test(std::string sFilename)
{
  std::ifstream ifs(sFilename.c_str());
  if (ifs.good())
    {
      return 1;
    }
  else
    {
      return 0;
    }
}
//=============================================================================================
//recursive mkdir when it does not exist
//=============================================================================================
int make_directory(std::string sDirectory)
{
  //the maximum length for current setting
  int dir_err;
  char tmp[255];
  char *p = NULL;
  size_t len;
#if defined(__linux__)
  snprintf(tmp, sizeof(tmp), "%s", sDirectory.c_str());
#else
  strcpy(tmp, sDirectory.c_str());
#endif
  len = strlen(tmp);
  if (tmp[len - 1] == slash)
    {
      tmp[len - 1] = 0;
    }
  for (p = tmp + 1; *p; p++)
    {
      if (*p == slash)
        {
          *p = 0;
          std::string temp(tmp);
          if (path_test(temp) == false)
            {
#if defined(__linux__)
              mkdir(tmp, S_IRWXU);
#else
              dir_err = _mkdir(tmp);
#endif
            }
          *p = slash;
        }
    }
  //final step

#if defined(__linux__)
  dir_err = mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#else
  dir_err = _mkdir(tmp);
#endif
  if (-1 == dir_err)
    {
      cout << "Error creating directory!" << tmp << endl;
      return 0;
    }
  else
    {
      return 1;
    }
}
//=============================================================================================
//test the existance of a path
//use the stat header file
int path_test(std::string sPath)
{
#if defined(__linux__)
  struct stat sb;
  if (stat(sPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
      return 1;
    }
  else
    {
      return 0;
    }
#else
  std::wstring stemp = std::wstring(sPath.begin(), sPath.end());
  LPCWSTR sw = stemp.c_str();
  if (0xffffffff == GetFileAttributes(sw))
    {
      return 0;
    }
  else
    {
      return 1;
    }
#endif
}


std::string find_alternative_file(int iYear,
                                  int iDay,
                                  std::string sExtension,
                                  std::string sPrefix,
                                  std::string sWorkspace)
{
  std::string sFilename = "";
  std::string sDay;
  //use the file in the nearest time
  stringstream ss0;
  ss0 << setfill('0') << setw(3) << iDay;
  sDay = ss0.str();
  auto max_search = 10000;
  int   iFound = 0;
  for (auto iSearch = 1; iSearch < max_search; iSearch++)
    {
      if (iFound == 0)
        {
          stringstream ss1;
          auto search_year = iYear + pow(-1.0, iSearch) * iSearch;
          ss1 << setfill('0') << setw(4) << (search_year);
          string sYear = ss1.str();
          sFilename = sWorkspace + slash + sYear + slash + sPrefix + sYear + sDay + sExtension;
          if (file_test(sFilename) == 1)
            {
              iFound = 1;
              break;
            }
          else
            {
              continue;
            }
        }
      else
        {
          break;
        }
    }

  return sFilename;


}


int leap_year(int year)
{
  int error_code = 0;
  if (year % 4 == 0)
    {
      if (year % 100 == 0)
        {
          if (year % 400 == 0)
            {
              error_code = 1;
            }
          else
            {
              error_code = 0;
            }
        }
      else
        {
          error_code = 1;
        }
    }
  else
    {
      error_code = 0;
    }
  return error_code;
}
