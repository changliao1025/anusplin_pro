#pragma once
///========================================================
///the global constant file for all the modules
///some constants or variables are written cross-platfrom
///========================================================
const double near_zero = 0.00001;
const double pi = 3.141592654;
//time
const int nmonth = 12;
//gis
//const long ncol = 388;
//const long nrow = 310;
const long nsegment = 37;
const double dResolution = 500.0;//unit: meter
const double dArea = dResolution * dResolution;
//land cover
const int nlandcover = 18; //based on modis land cover product
const int nsoil = 12;
const int nsoillayer = 6;
const int nvegetation = 14;
///soil
const double dFrozen_temperature = 273.15; //unit: K
const double dDensity_water = 1000.0;//density of water, unit: 
const double tkwat = 0.6;			// thermal conductivity of water (w/m/kelvin)
const double tkice = 2.2;			// thermal conductivity of ice (w/m/kelvin)
const double cwat = 4.188e06;		// specific heat capacity of water (j/m**3/kelvin)
const double cice = 2.094e06;		// specific heat capacity of ice (j/m**3/kelvin)
//physical
const double calorie_2_joule = 4.1858;//convert from calorie  to langley
const double joule_2_calorie = 1.0 / calorie_2_joule;
const double inch_2_cm = 2.54;
const double joule_2_langley = 1.0 / 41840.0; //convert from joulies  to langley
const double langley_2_joule = 41840.0;
const double dLatent_heat_water = 333.55; //heat of fusion (latent heat) of water.units: J/g
const double dSpecific_heat_water = 4.179; //the specific heat of water, units: j / (g * celsius )
const double dSpecific_heat_ice = 2.03; //the specific heat of ice, units: j / (g * celsius )
// revolution speed of the Earth, radians per day , this is very close to pi/180, but it is different.
const double radian = 0.0172;
const double dEccentricy = 0.0167;
const double dSolar_constant = 1368.0 *  0.0001 ; // solar const units: joule per square center meter per second
const double dTemperature_all_rain = 2.0; //unit: celsius
const double dTemperature_all_snow = -2.0; //unit: celsius
const double dFraction_sublimation = 0.5; //snow, this one should be in snow class
//system
#if defined(__linux__)
const char slash = '/';
#else
const char slash = '\\';
#endif
const double missing_value = -9999.0;

