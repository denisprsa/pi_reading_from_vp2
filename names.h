/****************************************************************************/
/*  vproweather 0.6                                                         */
/*  A program for acquiring data from a Davis Vantage Pro Weather Station.  */
/*                                                                          */
/*  Thanks to Paul Davis for his 'remserial' program, and Aaron Logue's     */
/*  'weastat' program.                                                      */
/*                                                                          */
/* (c)2004 Joe Jaworski    email: jj@joejaworski.com                        */
/* VPROWEATHER is covered under the GNU general license. See the attached   */
/* LICENSE file for details.                                                */
/*                                                                          */
/* This software is provided as-is, without any expressed or implied        */
/* warranties, including, without limitation, the implied warranties of     */
/* merchantibility and fitness for any particular purpose.                  */
/*                                                                          */
/*                                                                          */
/* names.h - definition of output file names                                */
/* tabs every 4 places                                                      */
/*                                                                          */
/****************************************************************************/

#ifndef __NAMES_H__
#define __NAMES_H__

#ifdef __cplusplus
#define extern "C" {        /* respect c++ callers */
#endif

/* real time data */
#define _BARO_TREND     "rtBaroTrend"
#define _BARO_TREND_IMG "rtBaroTrendImg"
#define _BARO_CURR      "rtBaroCurr"
#define _INSIDE_TEMP    "rtInsideTemp"
#define _INSIDE_HUM     "rtInsideHum"
#define _OUTSIDE_TEMP   "rtOutsideTemp"
#define _WIND_SPEED     "rtWindSpeed"
#define _WIND_AVG_SPEED "rtWindAvgSpeed"
#define _WIND_DIR       "rtWindDir"
#define _WIND_DIR_ROSE  "rtWindDirRose"
#define _OUTSIDE_HUM    "rtOutsideHum"
#define _RAIN_RATE      "rtRainRate"
#define _IS_RAINING     "rtIsRaining"
#define _UV_LEVEL       "rtUVLevel"
#define _SOLAR_RAD      "rtSolarRad"
#define _RAIN_STORM     "rtRainStorm"
#define _STORM_START_DATE "rtStormStartDate"
#define _DAY_RAIN       "rtDayRain"
#define _MONTH_RAIN     "rtMonthRain"
#define _YEAR_RAIN      "rtYearRain"
#define _DAY_ET         "rtDayET"
#define _MONTH_ET       "rtMonthET"
#define _XMIT_BATT      "rtXmitBatt"
#define _BATT_VOLTAGE   "rtBattVoltage"
#define _FORE_ICON      "rtForeIcon"
#define _FORE_RULE      "rtForeRule"
#define _FORE_STRING    "rtForecast"
#define _SUNRISE        "rtSunrise"
#define _SUNSET         "rtSunset"

/* highs/lows data */
#define _BARO_LO_DAY        "hlBaroLoDay"
#define _BARO_HI_DAY        "hlBaroHiDay"
#define _BARO_LO_MONTH      "hlBaroLoMonth"
#define _BARO_HI_MONTH      "hlBaroHiMonth"
#define _BARO_LO_YEAR       "hlBaroLoYear"
#define _BARO_HI_YEAR       "hlBaroHiYear"
#define _BARO_LO_TIME       "hlBaroLoTime"
#define _BARO_HI_TIME       "hlBaroHiTime"
#define _WIND_HI_DAY        "hlWindHiDay"
#define _WIND_HI_TIME       "hlWindHiTime"
#define _WIND_HI_MONTH      "hlWindHiMonth"
#define _WIND_HI_YEAR       "hlWindHiYear"
#define _IN_TEMP_BY_DAY     "hlInTempHiDay"
#define _IN_TEMP_HI_DAY     "hlInTempHiDay"
#define _IN_TEMP_LO_DAY     "hlInTempLoDay"
#define _IN_TEMP_HI_TIME    "hlInTempHiTime"
#define _IN_TEMP_LO_TIME    "hlInTempLoTime"
#define _IN_TEMP_LO_MONTH   "hlInTempLoMonth"
#define _IN_TEMP_HI_MONTH   "hlInTempHiMonth"
#define _IN_TEMP_LO_YEAR    "hlInTempLoYear"
#define _IN_TEMP_HI_YEAR    "hlInTempHiYear"
#define _IN_HUM_HI_DAY      "hlInHumHiDay"
#define _IN_HUM_LO_DAY      "hlInHumLoDay"
#define _IN_HUM_HI_TIME     "hlInHumHiTime"
#define _IN_HUM_LO_TIME     "hlInHumLoTime"
#define _IN_HUM_HI_MONTH    "hlInHumHiMonth"
#define _IN_HUM_LO_MONTH    "hlInHumLoMonth"
#define _IN_HUM_HI_YEAR     "hlInHumHiYear"
#define _IN_HUM_LO_YEAR     "hlInHumLoYear"
#define _TEMP_HI_DAY        "hlOutTempHiDay"
#define _TEMP_LO_DAY        "hlOutTempLoDay"
#define _TEMP_HI_TIME       "hlOutTempHiTime"
#define _TEMP_LO_TIME       "hlOutTempLoTime"
#define _TEMP_LO_MONTH      "hlOutTempLoMonth"
#define _TEMP_HI_MONTH      "hlOutTempHiMonth"
#define _TEMP_LO_YEAR       "hlOutTempLoYear"
#define _TEMP_HI_YEAR       "hlOutTempHiYear"
#define _DEW_LO_DAY         "hlDewLoDay"
#define _DEW_HI_DAY         "hlDewHiDay"
#define _DEW_LO_TIME        "hlDewLoTime"
#define _DEW_HI_TIME        "hlDewHiTime"
#define _DEW_HI_MONTH       "hlDewHiMonth"
#define _DEW_LO_MONTH       "hlDewLoMonth"
#define _DEW_HI_YEAR        "hlDewHiYear"
#define _DEW_LO_YEAR        "hlDewLoYear"
#define _CHILL_LO_DAY       "hlChillLoDay"
#define _CHILL_LO_TIME      "hlChillLoTime"
#define _CHILL_LO_MONTH     "hlChillLoMonth"
#define _CHILL_LO_YEAR      "hlChillLoYear"
#define _HEAT_HI_DAY        "hlHeatHiDay"
#define _HEAT_HI_TIME       "hlHeatHiTime"
#define _HEAT_HI_MONTH      "hlHeatHiMonth"
#define _HEAT_HI_YEAR       "hlHeatHiYear"
#define _SOLAR_HI_DAY       "hlSolarHiDay"
#define _SOLAR_HI_TIME      "hlSolarHiTime"
#define _SOLAR_HI_MONTH     "hlSolarHiMonth"
#define _SOLAR_HI_YEAR      "hlSolarHiYear"
#define _UV_HI_DAY          "hlUVHiDay"
#define _UV_HI_TIME         "hlUVHiTime"
#define _UV_HI_MONTH        "hlUVHiMonth"
#define _UV_HI_YEAR         "hlUVHiYear"
#define _RAIN_RATE_HI_DAY   "hlRainRateHiDay"
#define _RAIN_RATE_HI_TIME  "hlRainRateHiTime"
#define _RAIN_RATE_HI_HOUR  "hlRainRateHiHour"
#define _RAIN_RATE_HI_MONTH "hlRainRateHiMonth"
#define _RAIN_RATE_HI_YEAR  "hlRainRateHiYear"


/* graph data */
#define _TIME_REF_10MIN         "grTimeRef10Min"
#define _TIME_REF_15MIN         "grTimeRef15Min"
#define _TIME_REF_HOURS         "grTimeRefHours"
#define _TIME_REF_DAYS          "grTimeRefDays"
#define _TIME_REF_MONTHS        "grTimeRefMonths"
#define _TIME_REF_YEARS         "grTimeRefYears"
#define _IN_TEMP_BY_HOURS       "grInTempByHours"
#define _IN_TEMP_HI_BY_DAYS     "grInTempHiByDays"
#define _IN_TEMP_HI_BY_DAYS_TIME "grInTempHiByDaysTime"
#define _IN_TEMP_LO_BY_DAYS     "grInTempLoByDays"
#define _IN_TEMP_LO_BY_DAYS_TIME "grInTempLoByDaysTime"
#define _IN_TEMP_HI_BY_MONTHS   "grInTempHiByMonths"
#define _IN_TEMP_LO_BY_MONTHS   "grInTempLoByMonths"
#define _TEMP_BY_HOURS          "grOutTempByHours"
#define _TEMP_HI_BY_DAYS        "grOutTempHiByDays"
#define _TEMP_HI_BY_DAYS_TIME   "grOutTempHiByDaysTime"
#define _TEMP_LO_BY_DAYS        "grOutTempLoByDays"
#define _TEMP_LO_BY_DAYS_TIME   "grOutTempLoByDaysTime"
#define _TEMP_HI_BY_MONTHS      "grOutTempHiByMonths"
#define _TEMP_LO_BY_MONTHS      "grOutTempLoByMonths"
#define _DEW_BY_HOURS           "grDewByHours"
#define _DEW_HI_BY_DAYS         "grDewHiByDays"
#define _DEW_HI_BY_DAYS_TIME    "grDewHiByDaysTime"
#define _DEW_LO_BY_DAYS         "grDewLoByDays"
#define _DEW_LO_BY_DAYS_TIME    "grDewLoByDaysTime"
#define _DEW_HI_BY_MONTHS       "grDewHiByMonths"
#define _DEW_LO_BY_MONTHS       "grDewLoByMonths"
#define _CHILL_LO_BY_HOURS      "grChillLoByHours"
#define _CHILL_LO_BY_DAYS       "grChillLoByDays"
#define _CHILL_LO_BY_DAYS_TIME  "grChillLoByDaysTime"
#define _CHILL_LO_BY_MONTHS     "grChillLoByMonths"
#define _HEAT_HI_BY_HOURS       "grHeatHiByHours"
#define _HEAT_HI_BY_DAYS        "grHeatHiByDays"
#define _HEAT_HI_BY_DAYS_TIME   "grHeatHiByDaysTime"
#define _HEAT_HI_BY_MONTHS      "grHeatHiByMonths"
#define _IN_HUM_BY_HOURS        "grInHumByHours"
#define _IN_HUM_HI_BY_DAYS      "grInHumHiByDays"
#define _IN_HUM_HI_BY_DAYS_TIME "grInHumHiByDaysTime"
#define _IN_HUM_LO_BY_DAYS      "grInHumLoByDays"
#define _IN_HUM_LO_BY_DAYS_TIME "grInHumLoByDaysTime"
#define _IN_HUM_HI_BY_MONTHS    "grInHumHiByMonths"
#define _IN_HUM_LO_BY_MONTHS    "grInHumLoByMonths"
#define _HUM_BY_HOURS           "grOutHumByHours"
#define _HUM_HI_BY_DAYS         "grOutHumHiByDays"
#define _HUM_HI_BY_DAYS_TIME    "grOutHumHiByDaysTime"
#define _HUM_LO_BY_DAYS         "grOutHumLoByDays"
#define _HUM_LO_BY_DAYS_TIME    "grOutHumLoByDaysTime"
#define _HUM_HI_BY_MONTHS       "grOutHumHiByMonths"
#define _HUM_LO_BY_MONTHS       "grOutHumLoByMonths"
#define _BAR_BY_15MIN           "grBarBy15Min"
#define _BAR_BY_HOURS           "grBarByHours"
#define _BAR_HI_BY_DAYS         "grBarHiByDays"
#define _BAR_HI_BY_DAYS_TIME    "grBarHiByDaysTime"
#define _BAR_LO_BY_DAYS         "grBarLoByDays"
#define _BAR_LO_BY_DAYS_TIME    "grBarLoByDaysTime"
#define _BAR_HI_BY_MONTHS       "grBarHiByMonths"
#define _BAR_LO_BY_MONTHS       "grBarLoByMonths"
#define _WIND_AVG_BY_10MIN      "grWindAvgBy10Min"
#define _WIND_AVG_BY_HOURS      "grWindAvgByHours"
#define _WIND_HI_BY_DAYS        "grWindHiByDays"
#define _WIND_HI_BY_DAYS_TIME   "grWindHiByDaysTime"
#define _WIND_HI_BY_MONTHS      "grWindHiByMonths"
#define _WIND_HI_BY_YEARS       "grWindHiByYears"
#define _WIND_DIR_BY_HOURS      "grWindDirByHours"
#define _WIND_DIR_BY_DAYS       "grWindDirByDays"
#define _WIND_DIR_BY_MONTHS     "grWindDirByMonths"
#define _RAIN_RATE_BY_MINUTES   "grRainRateByMinutes"
#define _RAIN_RATE_BY_HOURS     "grRainRateByHours"
#define _RAIN_RATE_HI_BY_DAYS   "grRainRateHiByDays"
#define _RAIN_RATE_HI_BY_DAYS_TIME "grRainRateHiByDaysTime"
#define _RAIN_RATE_HI_BY_MONTHS "grRainRateHiByMonths"
#define _RAIN_RATE_HI_BY_YEARS  "grRainRateHiByYears"
#define _RAIN_BY_15MIN          "grRainBy15Min"
#define _RAIN_BY_HOUR           "grRainByHour"
#define _RAIN_STORMS            "grRainStorms"
#define _RAIN_STORMS_START_DATE "grRainStormsStartDate"
#define _RAIN_STORMS_END_DATE   "grRainStormsEndDate"
#define _RAIN_BY_DAY            "grRainByDay"
#define _RAIN_BY_MONTH          "grRainByMonth"
#define _RAIN_BY_YEAR           "grRainByYear"


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __NAMES_H__ */
