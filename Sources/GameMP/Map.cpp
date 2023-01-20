/* Copyright (c) 2002-2012 Croteam Ltd. 
This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

#include "StdAfx.h"
#include "LCDDrawing.h"

extern BOOL map_bIsFirstEncounter;

static CTextureObject atoIconsSE[13];
static CTextureObject atoIconsFE[55];

static CTextureObject _toPathDot;

static CTextureObject _toMapBcgLDFE;
static CTextureObject _toMapBcgLUFE;
static CTextureObject _toMapBcgRDFE;
static CTextureObject _toMapBcgRUFE;

static CTextureObject _toMapBcgLDSE;
static CTextureObject _toMapBcgLUSE;
static CTextureObject _toMapBcgRDSE;
static CTextureObject _toMapBcgRUSE;

PIX aIconCoordsSE[][2] =
{
  {0, 0},      // 00: Last Episode
  {168, 351},  // 01: Palenque 01 
  {42, 345},   // 02: Palenque 02 
  {41, 263},   // 03: Teotihuacan 01      
  {113, 300},  // 04: Teotihuacan 02      
  {334, 328},  // 05: Teotihuacan 03      
  {371, 187},  // 06: Ziggurat	 
  {265, 111},  // 07: Atrium		 
  {119, 172},  // 08: Gilgamesh	 
  {0, 145},    // 09: Babel		   
  {90, 30},    // 10: Citadel		 
  {171, 11},   // 11: Land of Damned		     
  {376, 0},    // 12: Cathedral	 
};

PIX aIconCoordsFE[][2] =
{
  {175,404},  // 00: Hatshepsut
  {60,381},   // 01: Sand Canyon
  {50,300},   // 02: Ramses
  {171,304},  // 03: Canyon
  {190,225},  // 04: Waterfall
  {303,305},  // 05: Oasis
  {361,296},  // 06: Dunes
  {362,222},  // 07: Suburbs
  {321,211},  // 08: Sewers
  {316,156},  // 09: Metropolis
  {194,157},  // 10: Sphynx
  {160,111},  // 11: Karnak
  {167,61},   // 12: Luxor
  {50,53},    // 13: Sacred
  {185,0},    // 14: Pyramid
  {0,-15},    // 15: CityEnter
  {0,-15},    // 16: CityMidlle
  {0,-15},    // 17: CityObelisk
  {0,-15},    // 18: EnterCave
  {0,-15},    // 19: KingValley
  {0,-15},    // 20: TombofMenes
  {0,-15},    // 21: TombofRamses
  {0,-15},    // 22: SecretTomb
  {0,-15},    // 23: MentalTomb
  {0,-15},    // 24: KingValleyExit
  {0,-15},    // 25: KarnakEnter
  {0,-15},    // 26: KarnakEnterComplex
  {0,-15},    // 27: KarnakTemple
  {0,-15},    // 28: KarnakLake&Alley
  {0,-15},    // 29: BabilonTemple
  {0,-15},    // 30: Hatshepsut Alpha
  {0,-15},    // 31: PyramidValley
  {0,-15},    // 32: WaterPlanetIntro
  {0,-15},    // 33: WaterPlanetValley
  {0,-15},    // 34: WaterPlanetTerra
  {0,-15},    // 35: WaterPlanetEnd
  {0,-15},    // 36: LavaPlanetValley
  {0,-15},    // 37: LavaPlanetCanyon
  {0,-15},    // 38: LavaPlanetVulcan
  {0,-15},    // 39: GreenPlanet
  {0,-15},    // 40: IcePlanet
  {0,-15},    // 41: IcePlanetEnd
  {0,-15},    // 42: TropicPlanet
  {0,-15},    // 43: TropicPlaneEnd
  {0,-15},    // 44: RockPlanet
  {0,-15},    // 45: SpaceCrusader (move)

  {0,-15},    // 46: LoonyMoony
  {0,-15},    // 47: Verticality
  {0,-15},    // 48: Gravitality
  {0,-15},    // 49: Labyrinth
  {0,-15},    // 50: Platformal
  {0,-15},    // 51: GrandFinale1
  {0,-15},    // 52: GrandFinale2
  {0,-15},    // 53: GrandFinale3
  {0,-15},    // 54: SpaceCrusader
};

#define LASTEPISODE_BIT 0ULL
#define PALENQUE01_BIT 1ULL
#define PALENQUE02_BIT  2ULL
#define TEOTIHUACAN01_BIT 3ULL
#define TEOTIHUACAN02_BIT 4ULL
#define TEOTIHUACAN03_BIT 5ULL
#define ZIGGURAT_BIT 6ULL
#define ATRIUM_BIT 7ULL
#define GILGAMESH_BIT 8ULL
#define BABEL_BIT 9ULL
#define CITADEL_BIT 10ULL
#define LOD_BIT 11ULL
#define CATHEDRAL_BIT 12ULL

#define HATSHEPSUT_BIT 0ULL
#define SAND_BIT 1ULL
#define RAMSES_BIT 2ULL
#define CANYON_BIT 3ULL
#define WATERFALL_BIT 4ULL
#define OASIS_BIT 5ULL
#define DUNES_BIT 6ULL
#define SUBURBS_BIT 7ULL
#define SEWERS_BIT 8ULL
#define METROPOLIS_BIT 9ULL
#define SPHYNX_BIT 10ULL
#define KARNAK_BIT 11ULL
#define LUXOR_BIT 12ULL
#define SACRED_BIT 13ULL
#define PYRAMID_BIT 14ULL
#define CITYENTER_BIT 15ULL
#define CITYMIDLLE_BIT 16ULL
#define CITYOBELISK_BIT 17ULL
#define ENTERCAVE_BIT 18ULL
#define KINGVALLEY_BIT 19ULL
#define TOMBOFMENES_BIT 20ULL
#define TOMBOFRAMSESII_BIT 21ULL
#define SECRETTOMB_BIT 22ULL
#define MENTALTOMB_BIT 23ULL
#define KINGVALLEYEXIT_BIT 24ULL
#define KARNAKENTER_BIT 25ULL
#define KARNAKENTERCOMPLEX_BIT 26ULL
#define KARNAKTEMPLE_BIT 27ULL
#define KARNAKLAKEANDALLEY_BIT 28ULL
#define BABILONTEMPLE_BIT 29ULL
#define HATSHEPSUTALPHA_BIT 30ULL
#define PYRANIDVALLEY_BIT 31ULL
#define WATERPLANETINTRO_BIT 32ULL
#define WATERPLANETVALLEY_BIT 33ULL
#define WATERPLANETTERRA_BIT 34ULL
#define WATERPLANETEND_BIT 35ULL
#define LAVAPLANETVALLEY_BIT 36ULL
#define LAVAPLANETCANYON_BIT 37ULL
#define LAVAPLANETVULCAN_BIT 38ULL
#define GREENPLANET_BIT 39ULL
#define ICEPLANET_BIT 40ULL
#define ICEPLANETEND_BIT 41ULL
#define TROPICPLANET_BIT 42ULL
#define TROPICPLANETEND_BIT 43ULL
#define ROCKPLANET_BIT 44ULL
#define SIRIUS01_BIT 45ULL
#define SIRIUS02_BIT 46ULL
#define SIRIUS03_BIT 47ULL
#define SIRIUS04_BIT 48ULL
#define SIRIUS05_BIT 49ULL
#define SIRIUS06_BIT 50ULL
#define SIRIUS07_BIT 51ULL
#define SIRIUS08_BIT 52ULL
#define SPACECRUSADER_BIT 53ULL

INDEX  aPathPrevNextLevelsSE[][2] = 
{
  {LASTEPISODE_BIT, PALENQUE01_BIT},      // 00
  {PALENQUE01_BIT, PALENQUE02_BIT},       // 01
  {PALENQUE02_BIT, TEOTIHUACAN01_BIT },   // 02
  {TEOTIHUACAN01_BIT, TEOTIHUACAN02_BIT}, // 03
  {TEOTIHUACAN02_BIT, TEOTIHUACAN03_BIT}, // 04
  {TEOTIHUACAN03_BIT, ZIGGURAT_BIT},      // 05
  {ZIGGURAT_BIT, ATRIUM_BIT},             // 06
  {ATRIUM_BIT, GILGAMESH_BIT},            // 07
  {GILGAMESH_BIT, BABEL_BIT},             // 08
  {BABEL_BIT, CITADEL_BIT},               // 09
  {CITADEL_BIT, LOD_BIT},                 // 10
  {LOD_BIT, CATHEDRAL_BIT},               // 11
};

INDEX  aPathPrevNextLevelsFE[][2] = 
{
  {HATSHEPSUT_BIT, SAND_BIT},     // 00
  {SAND_BIT, RAMSES_BIT},         // 01
  {RAMSES_BIT, CANYON_BIT},       // 02
  {CANYON_BIT, WATERFALL_BIT},    // 03
  {CANYON_BIT, OASIS_BIT},        // 04
  {WATERFALL_BIT, OASIS_BIT},     // 05
  {OASIS_BIT, DUNES_BIT},         // 06
  {DUNES_BIT, SUBURBS_BIT},       // 07
  {SUBURBS_BIT, SEWERS_BIT},      // 08
  {SEWERS_BIT, METROPOLIS_BIT},   // 09
  {METROPOLIS_BIT, SPHYNX_BIT},   // 10
  {SPHYNX_BIT, KARNAK_BIT},       // 11
  {KARNAK_BIT, LUXOR_BIT},        // 12
  {LUXOR_BIT, SACRED_BIT},        // 13
  {SACRED_BIT, PYRAMID_BIT},      // 14
  {LUXOR_BIT, PYRAMID_BIT},       // 15
  {CITYENTER_BIT, CITYMIDLLE_BIT},	       // 16
  {CITYMIDLLE_BIT, CITYOBELISK_BIT},	   // 17
  {CITYOBELISK_BIT, ENTERCAVE_BIT},	       // 18
  {ENTERCAVE_BIT, KINGVALLEY_BIT},	       // 19
  {KINGVALLEY_BIT, TOMBOFMENES_BIT},	   // 20
  {KINGVALLEY_BIT, TOMBOFRAMSESII_BIT},	   // 20
  {KINGVALLEY_BIT, SECRETTOMB_BIT},	   // 20
  {KINGVALLEY_BIT, MENTALTOMB_BIT},	   // 20
  {TOMBOFMENES_BIT, KINGVALLEY_BIT},   // 21
  {TOMBOFRAMSESII_BIT, KINGVALLEY_BIT},	   // 22
  {SECRETTOMB_BIT, KINGVALLEY_BIT},	   // 23
  {MENTALTOMB_BIT, SPACECRUSADER_BIT},	   // 45
  {KINGVALLEYEXIT_BIT, KARNAKENTER_BIT},   // 24
  {KARNAKENTER_BIT, KARNAKENTERCOMPLEX_BIT},	// 25
  {KARNAKENTERCOMPLEX_BIT, KARNAKTEMPLE_BIT},	// 26
  {KARNAKTEMPLE_BIT, KARNAKLAKEANDALLEY_BIT},	// 27
  {KARNAKLAKEANDALLEY_BIT, BABILONTEMPLE_BIT},	// 28
  {BABILONTEMPLE_BIT, HATSHEPSUTALPHA_BIT},	// 29
  {HATSHEPSUTALPHA_BIT, PYRANIDVALLEY_BIT},
  {PYRANIDVALLEY_BIT, WATERPLANETINTRO_BIT},	// 30
  {WATERPLANETINTRO_BIT, WATERPLANETVALLEY_BIT},	// 31
  {WATERPLANETVALLEY_BIT, WATERPLANETTERRA_BIT},	// 32
  {WATERPLANETTERRA_BIT, WATERPLANETEND_BIT},	// 33
  {WATERPLANETEND_BIT, LAVAPLANETVALLEY_BIT},	// 34
  {LAVAPLANETVALLEY_BIT, LAVAPLANETCANYON_BIT},	// 35
  {LAVAPLANETCANYON_BIT, LAVAPLANETVULCAN_BIT},	// 36
  {LAVAPLANETVULCAN_BIT, GREENPLANET_BIT},// 37
  {GREENPLANET_BIT, ICEPLANET_BIT},	// 38
  {ICEPLANET_BIT, ICEPLANETEND_BIT},	// 39
  {ICEPLANETEND_BIT, TROPICPLANET_BIT},	// 40
  {ICEPLANETEND_BIT, TROPICPLANET_BIT},	// 41
  {ICEPLANETEND_BIT, TROPICPLANET_BIT},	// 42
  {TROPICPLANET_BIT, TROPICPLANETEND_BIT},	// 43
  {TROPICPLANETEND_BIT, ROCKPLANET_BIT},	// 44
  {ROCKPLANET_BIT, SPACECRUSADER_BIT},	// 45
  {ROCKPLANET_BIT, SPACECRUSADER_BIT},	// 46

  {SIRIUS01_BIT, SIRIUS01_BIT},	// 38
  {SIRIUS01_BIT, SIRIUS02_BIT},	// 39
  {SIRIUS02_BIT, SIRIUS03_BIT},	// 40
  {SIRIUS03_BIT, SIRIUS04_BIT},	// 41
  {SIRIUS04_BIT, SIRIUS05_BIT},	// 42
  {SIRIUS05_BIT, SIRIUS06_BIT},	// 43
  {SIRIUS06_BIT, SIRIUS07_BIT},	// 44
  {SIRIUS07_BIT, SIRIUS08_BIT},	// 45
  {SIRIUS08_BIT, SPACECRUSADER_BIT},
};

PIX aPathDotsSE[][10][2] = 
{
  // 00: Palenque01 - Palenque02
  {
    {-1,-1},
  },

  // 01: Palenque01 - Palenque02
  {
    {211,440},
    {193,447},
    {175,444},
    {163,434},
    {152,423},
    {139,418},
    {-1,-1},
  },
  
  // 02: Palenque02 - Teotihuacan01
  {
    {100,372},
    {102,363},
    {108,354},
    {113,345},
    {106,338},
    {-1,-1},
  },

  // 03: Teotihuacan01 - Teotihuacan02
  {
    {153,337},
    {166,341},
    {180,346},
    {194,342},
    {207,337},
    {-1,-1},
  },

  // 04: Teotihuacan02 - Teotihuacan03
  {
    {279,339},
    {287,347},
    {296,352},
    {307,365},
    {321,367},
    {335,362},
    {-1,-1},
  },

  // 05: Teotihuacan03 - Ziggurat
  {
    {-1,-1},
  },

  // 06: Ziggurat - Atrium
  {
    {412,285},
    {396,282},
    {383,273},
    {368,266},
    {354,264},
    {-1,-1},
  },

  // 07: Atrium - Gilgamesh
  {
    {276,255},
    {262,258},
    {248,253},
    {235,245},
    {222,240},
    {-1,-1},
  },

  // 08: Gilgamesh - Babel
  {
    {152,245},
    {136,248},
    {118,253},
    {100,251},
    {85,246},
    {69,243},
    {-1,-1},
  },

  // 09: Babel - Citadel
  {
    {-1,-1},
  },

  // 10: Citadel - Lod
  {
    {190,130},
    {204,126},
    {215,119},
    {232,116},
    {241,107},
    {-1,-1},
  },

  // 11: Lod - Cathedral
  {
    {330,108},
    {341,117},
    {353,126},
    {364,136},
    {377,146},
    {395,147},
    {-1,-1},
  },
};

PIX aPathDotsFE[][10][2] = 
{
  // 00: Hatshepsut - Sand
  {
    {207,435},
    {196,440},
    {184,444},
    {172,443},
    {162,439},
    {156,432},
    {-1,-1},
  },
  
  // 01: Sand - Ramses
  {
    {115,388},
    {121,382},
    {128,377},
    {136,371},
    {-1,-1},
  },

  // 02: Ramses - Canyon
  {
    {148,368},
    {159,370},
    {169,374},
    {177,379},
    {187,381},
    {200,380},
    {211,376},
    {-1,-1},
  },

  // 03: Canyon - Waterfall
  {
    {273,339},
    {276,331},
    {278,322},
    {280,313},
    {279,305},
    {273,298},
    {266,293},
    {260,288},
    {-1,-1},
  },

  // 04: Canyon - Oasis
  {
    {288,360},
    {295,355},
    {302,360},
    {310,364},
    {319,367},
    {328,368},
    {-1,-1},
  },

  // 05: Waterfall - Oasis
  {
    {294,279},
    {302,282},
    {310,287},
    {316,294},
    {320,302},
    {323,310},
    {327,318},
    {332,326},
    {337,333},
    {-1,-1},
  },

  // 06: Oasis - Dunes
  {
    {384,360},
    {394,358},
    {405,353},
    {414,347},
    {421,339},
    {426,329},
    {-1,-1},
  },

  // 07: Dunes - Suburbs
  {
    {439,305},
    {434,300},
    {429,293},
    {-1,-1},
  },

  // 08: Suburbs - Sewers
  {
    {403,250},
    {402,244},
    {401,238},
    {398,232},
    {-1,-1},
  },

  // 09: Sewers - Metropolis
  {
    {372,266},
    {371,221},
    {370,216},
    {-1,-1},
  },

  // 10: Metropolis - Alley
  {
    {317,211},
    {310,215},
    {302,219},
    {293,222},
    {283,222},
    {273,221},
    {265,218},
    {-1,-1},
  },

  // 11: Alley - Karnak
  {
    {260,189},
    {259,181},
    {255,174},
    {249,168},
    {241,165},
    {233,164},
    {-1,-1},
  },

  // 12: Karnak - Luxor
  {
    {228,143},
    {228,136},
    {226,129},
    {221,123},
    {-1,-1},
  },

  // 13: Luxor - Sacred
  {
    {175,101},
    {169,106},
    {162,111},
    {154,113},
    {145,113},
    {136,112},
    {-1,-1},
  },

  // 14: Sacred - Pyramid
  {
    {126,59},
    {134,55},
    {142,52},
    {151,49},
    {160,47},
    {170,47},
    {179,48},
    {188,51},
    {-1,-1},
  },

  // 15: Luxor - Pyramid
  {
    {212,71},
    {217,66},
    {225,63},
    {234,63},
    {244,63},
    {253,62},
    {261,59},
    {-1,-1},
  },

    // 16: CityEnter - CityMidlle
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 17: CityMidlle - CityObelisk
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 18: CityObelisk - EnterCave
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 19: EnterCave - KingValley
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 20: KingValley - TombofMenes
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 21: TombofMenes - TombofRamsesII
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 22: TombofRamsesII - SecretTomb
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 23: SecretTomb - KingValleyExit
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 24: KingValleyExit - KarnakEnter
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 25:KarnakEnter - KarnakEnterComplex
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 26: KarnakEnterComplex - KarnakTemple
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 27: KarnakTemple -KarnakLakeandAlley
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 28: KarnakLakeandAlley - BabilonTemple
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 29: BabilonTemple - Hatshepsut Alpha
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 30: Hatshepsut Alpha - WaterPlanetIntro
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 31: WaterPlanetIntro - WaterPlanetValley
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 32: WaterPlanetValley - WaterPlanetTerra
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 33: WaterPlanetTerra - WaterPlanetEnd
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 34: WaterPlanetEnd - LavaPlanetValley
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 35: LavaPlanetValley - LavaPlanetCanyon
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 36: LavaPlanetCanyon - LavaPlanetVulcan
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 37: LavaPlanetVulcan - GreenPlanet
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 38: GreenPlanet - IcePlanet
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 39: IcePlanet - IcePlanetEnd
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 40: IcePlanetEnd - RockPlanet
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 41: RockPlanet - SpaceCrusader
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 42: GreenPlanet - IcePlanet
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 43: IcePlanet - IcePlanetEnd
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 44: IcePlanetEnd - RockPlanet
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 44: RockPlanet - SpaceCrusader
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 46: GreenPlanet - IcePlanet
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 47: IcePlanet - IcePlanetEnd
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 48: IcePlanetEnd - RockPlanet
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 49: RockPlanet - SpaceCrusader
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 50: RockPlanet - SpaceCrusader
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 51: GreenPlanet - IcePlanet
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 52: IcePlanet - IcePlanetEnd
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 53: IcePlanetEnd - RockPlanet
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },

     // 54: RockPlanet - SpaceCrusader
  {
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-1,-1},
  },
};

BOOL ObtainMapData(void)
{
  try {
    // the second encounter
    atoIconsSE[ 0].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Book.tex"));
    atoIconsSE[ 1].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Level00.tex"));
    atoIconsSE[ 2].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Level01.tex"));
    atoIconsSE[ 3].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Level02.tex"));
    atoIconsSE[ 4].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Level03.tex"));
    atoIconsSE[ 5].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Level04.tex"));
    atoIconsSE[ 6].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Level05.tex"));
    atoIconsSE[ 7].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Level06.tex"));
    atoIconsSE[ 8].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Level07.tex"));
    atoIconsSE[ 9].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Level08.tex"));
    atoIconsSE[10].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Level09.tex"));
    atoIconsSE[11].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Level10.tex"));
    atoIconsSE[12].SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\Level11.tex"));
    _toPathDot    .SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\PathDot.tex"));
    _toMapBcgLDSE .SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\MapBcgLD.tex"));
    _toMapBcgLUSE .SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\MapBcgLU.tex"));
    _toMapBcgRDSE .SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\MapBcgRD.tex"));
    _toMapBcgRUSE .SetData_t(CTFILENAME("TexturesMP\\Computer\\Map\\MapBcgRU.tex"));
    // force constant textures
    ((CTextureData*)atoIconsSE[ 0].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsSE[ 1].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsSE[ 2].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsSE[ 3].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsSE[ 4].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsSE[ 5].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsSE[ 6].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsSE[ 7].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsSE[ 8].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsSE[ 9].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsSE[10].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsSE[11].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsSE[12].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)_toPathDot    .GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)_toMapBcgLDSE .GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)_toMapBcgLUSE .GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)_toMapBcgRDSE .GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)_toMapBcgRUSE .GetData())->Force(TEX_CONSTANT);

    // the first encounter
    atoIconsFE[ 0].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level00.tex"));
    atoIconsFE[ 1].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level01.tex"));
    atoIconsFE[ 2].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level02.tex"));
    atoIconsFE[ 3].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level03.tex"));
    atoIconsFE[ 4].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level04.tex"));
    atoIconsFE[ 5].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level05.tex"));
    atoIconsFE[ 6].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level06.tex"));
    atoIconsFE[ 7].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level07.tex"));
    atoIconsFE[ 8].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level08.tex"));
    atoIconsFE[ 9].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level09.tex"));
    atoIconsFE[10].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level10.tex"));
    atoIconsFE[11].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level11.tex"));
    atoIconsFE[12].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level12.tex"));
    atoIconsFE[13].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level13.tex"));
    atoIconsFE[14].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level14.tex"));
	atoIconsFE[15].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level15.tex"));
	atoIconsFE[16].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level16.tex"));
	atoIconsFE[17].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level17.tex"));
	atoIconsFE[18].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level18.tex"));
	atoIconsFE[19].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level19.tex"));
	atoIconsFE[20].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level20.tex"));
	atoIconsFE[21].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level21.tex"));
	atoIconsFE[22].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level22.tex"));
	atoIconsFE[23].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level23.tex"));
	atoIconsFE[24].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level24.tex"));
	atoIconsFE[25].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level25.tex"));
	atoIconsFE[26].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level26.tex"));
	atoIconsFE[27].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level27.tex"));
	atoIconsFE[28].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level28.tex"));
	atoIconsFE[29].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level29.tex"));
	atoIconsFE[30].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level30.tex"));
	atoIconsFE[31].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level31.tex"));
	atoIconsFE[32].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level32.tex"));
	atoIconsFE[33].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level33.tex"));
	atoIconsFE[34].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level34.tex"));
	atoIconsFE[35].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level35.tex"));
	atoIconsFE[36].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level36.tex"));
	atoIconsFE[37].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level37.tex"));
	atoIconsFE[38].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level38.tex"));
	atoIconsFE[39].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level39.tex"));
	atoIconsFE[40].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level40.tex"));
	atoIconsFE[41].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level41.tex"));
	atoIconsFE[42].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level42.tex"));
	atoIconsFE[43].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level43.tex"));
	atoIconsFE[44].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level44.tex"));
	atoIconsFE[45].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level45.tex"));

	atoIconsFE[46].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level46.tex"));
	atoIconsFE[47].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level47.tex"));
	atoIconsFE[48].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level48.tex"));
	atoIconsFE[49].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level49.tex"));
	atoIconsFE[50].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level50.tex"));
	atoIconsFE[51].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level51.tex"));
	atoIconsFE[52].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level52.tex"));
	atoIconsFE[53].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level53.tex"));
	atoIconsFE[54].SetData_t(CTFILENAME("Textures\\Computer\\Map\\Level54.tex"));
    _toMapBcgLDFE .SetData_t(CTFILENAME("Textures\\Computer\\Map\\MapBcgLD.tex"));
    _toMapBcgLUFE .SetData_t(CTFILENAME("Textures\\Computer\\Map\\MapBcgLU.tex"));
    _toMapBcgRDFE .SetData_t(CTFILENAME("Textures\\Computer\\Map\\MapBcgRD.tex"));
    _toMapBcgRUFE .SetData_t(CTFILENAME("Textures\\Computer\\Map\\MapBcgRU.tex"));
    // force constant textures
    ((CTextureData*)atoIconsFE[ 0].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[ 1].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[ 2].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[ 3].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[ 4].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[ 5].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[ 6].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[ 7].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[ 8].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[ 9].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[10].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[11].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[12].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[13].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[14].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[15].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)atoIconsFE[16].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[17].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[18].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[19].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[20].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[21].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[22].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[23].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[24].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[25].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[26].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[27].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[28].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[29].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[30].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[31].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[32].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[33].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[34].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[35].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[36].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[37].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[38].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[39].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[40].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[41].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[42].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[43].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[44].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[45].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[46].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[47].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[48].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[49].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[50].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[51].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[52].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[53].GetData())->Force(TEX_CONSTANT);
	((CTextureData*)atoIconsFE[54].GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)_toMapBcgLDFE .GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)_toMapBcgLUFE .GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)_toMapBcgRDFE .GetData())->Force(TEX_CONSTANT);
    ((CTextureData*)_toMapBcgRUFE .GetData())->Force(TEX_CONSTANT);
  }
  catch (const char *strError) {
    CPrintF("%s\n", (const char *)strError);
    return FALSE;
  }
  return TRUE;
}

void ReleaseMapData(void)
{
  atoIconsSE[0].SetData(NULL);
  atoIconsSE[1].SetData(NULL);
  atoIconsSE[2].SetData(NULL);
  atoIconsSE[3].SetData(NULL);
  atoIconsSE[4].SetData(NULL);
  atoIconsSE[5].SetData(NULL);
  atoIconsSE[6].SetData(NULL);
  atoIconsSE[7].SetData(NULL);
  atoIconsSE[8].SetData(NULL);
  atoIconsSE[9].SetData(NULL);
  atoIconsSE[10].SetData(NULL);
  atoIconsSE[11].SetData(NULL);
  atoIconsSE[12].SetData(NULL);
  atoIconsFE[0].SetData(NULL);
  atoIconsFE[1].SetData(NULL);
  atoIconsFE[2].SetData(NULL);
  atoIconsFE[3].SetData(NULL);
  atoIconsFE[4].SetData(NULL);
  atoIconsFE[5].SetData(NULL);
  atoIconsFE[6].SetData(NULL);
  atoIconsFE[7].SetData(NULL);
  atoIconsFE[8].SetData(NULL);
  atoIconsFE[9].SetData(NULL);
  atoIconsFE[10].SetData(NULL);
  atoIconsFE[11].SetData(NULL);
  atoIconsFE[12].SetData(NULL);
  atoIconsFE[13].SetData(NULL);
  atoIconsFE[14].SetData(NULL);
  atoIconsFE[15].SetData(NULL);
  atoIconsFE[16].SetData(NULL);
  atoIconsFE[17].SetData(NULL);
  atoIconsFE[18].SetData(NULL);
  atoIconsFE[19].SetData(NULL);
  atoIconsFE[20].SetData(NULL);
  atoIconsFE[21].SetData(NULL);
  atoIconsFE[22].SetData(NULL);
  atoIconsFE[23].SetData(NULL);
  atoIconsFE[24].SetData(NULL);
  atoIconsFE[25].SetData(NULL);
  atoIconsFE[26].SetData(NULL);
  atoIconsFE[27].SetData(NULL);
  atoIconsFE[28].SetData(NULL);
  atoIconsFE[29].SetData(NULL);
  atoIconsFE[30].SetData(NULL);
  atoIconsFE[31].SetData(NULL);
  atoIconsFE[32].SetData(NULL);
  atoIconsFE[33].SetData(NULL);
  atoIconsFE[34].SetData(NULL);
  atoIconsFE[35].SetData(NULL);
  atoIconsFE[36].SetData(NULL);
  atoIconsFE[37].SetData(NULL);
  atoIconsFE[38].SetData(NULL);
  atoIconsFE[39].SetData(NULL);
  atoIconsFE[40].SetData(NULL);
  atoIconsFE[41].SetData(NULL);
  atoIconsFE[42].SetData(NULL);
  atoIconsFE[43].SetData(NULL);
  atoIconsFE[44].SetData(NULL);
  atoIconsFE[45].SetData(NULL);
  atoIconsFE[46].SetData(NULL);
  atoIconsFE[47].SetData(NULL);
  atoIconsFE[48].SetData(NULL);
  atoIconsFE[49].SetData(NULL);
  atoIconsFE[50].SetData(NULL);
  atoIconsFE[51].SetData(NULL);
  atoIconsFE[52].SetData(NULL);
  atoIconsFE[53].SetData(NULL);
  atoIconsFE[54].SetData(NULL);
  _toPathDot.SetData(NULL);
  _toMapBcgLDSE.SetData(NULL);
  _toMapBcgLUSE.SetData(NULL);
  _toMapBcgRDSE.SetData(NULL);
  _toMapBcgRUSE.SetData(NULL);
  _toMapBcgLDFE.SetData(NULL);
  _toMapBcgLUFE.SetData(NULL);
  _toMapBcgRDFE.SetData(NULL);
  _toMapBcgRUFE.SetData(NULL);
}

void RenderMap( CDrawPort *pdp, unsigned long long ulLevelMask, CProgressHookInfo *pphi, BOOL bAlphaLevels, BOOL bCompModels)
{
  if( !ObtainMapData())
  {
    ReleaseMapData();
    return;
  }

  PIX(*aIconCoords)[2]           = map_bIsFirstEncounter ? aIconCoordsFE         : aIconCoordsSE;
  CTextureObject* atoIcons       = map_bIsFirstEncounter ? atoIconsFE            : atoIconsSE;
  INDEX(*aPathPrevNextLevels)[2] = map_bIsFirstEncounter ? aPathPrevNextLevelsFE : aPathPrevNextLevelsSE;
  PIX(*aPathDots)[10][2]         = map_bIsFirstEncounter ? aPathDotsFE           : aPathDotsSE;

  unsigned long long ctLevels = map_bIsFirstEncounter ? ARRAYCOUNT(aIconCoordsFE) : ARRAYCOUNT(aIconCoordsSE);

  CTextureObject* _toMapBcgLD = &_toMapBcgLDSE;
  CTextureObject* _toMapBcgLU = &_toMapBcgLUSE;
  CTextureObject* _toMapBcgRD = &_toMapBcgRDSE;
  CTextureObject* _toMapBcgRU = &_toMapBcgRUSE;

  if(map_bIsFirstEncounter) {
    _toMapBcgLD = &_toMapBcgLDFE;
    _toMapBcgLU = &_toMapBcgLUFE;
    _toMapBcgRD = &_toMapBcgRDFE;
    _toMapBcgRU = &_toMapBcgRUFE;
  }

  PIX pixdpw = (PIX) pdp->GetWidth();
  PIX pixdph = (PIX) pdp->GetHeight();
  PIX imgw;
  PIX imgh;
  FLOAT fStretch;
  if (bAlphaLevels == TRUE) {
     imgw = 1024;
     imgh = 480;
     if (bCompModels == TRUE) {
        fStretch = 1.2f;
        //imgh = 480;
     } else {
        fStretch = 2.0f;
        //imgh = 480;
     }
  } else {
     imgw = 512;
     imgh = 480;
     fStretch = 0.5f; 
  }

  // determine max available picture stretch
  if (bAlphaLevels == FALSE) {
     if( pixdpw>=imgw*2 && pixdph>=imgh*2) {
       fStretch = 2.0f;
     } else if(pixdpw>=imgw && pixdph>=imgh) {
       fStretch = 1.0f;
     } else if(pixdpw>=imgw/2 && pixdph>=imgh/2) {
       fStretch = 0.5f;
     }
  }
  // calculate LU offset so picture would be centered in dp
  PIX pixSX = (PIX) ((pixdpw-imgw*fStretch)/2);
  PIX pixSY = Max( PIX((pixdph-imgh*fStretch)/2), PIX(0));

  PIX pixC1S = pixSX;                          // column 1 start pixel
  PIX pixR1S = pixSY;                          // raw 1 start pixel
  PIX pixC1E = (PIX) (pixSX+256*fStretch);     // column 1 end pixel
  PIX pixR1E = (PIX) (pixSY+256*fStretch);     // raw 1 end pixel
  PIX pixC2S = (PIX) (pixC1E-fStretch);        // column 2 start pixel
  PIX pixR2S = (PIX) (pixR1E-fStretch);        // raw 2 start pixel
  PIX pixC2E = (PIX) (pixC2S+256*fStretch);    // column 2 end pixel
  PIX pixR2E = (PIX) (pixR2S+256*fStretch);    // raw 2 end pixel

  if (ulLevelMask == 0x00000001ULL && !map_bIsFirstEncounter) {

    // render the book
    //PIX pixX = (PIX) (aIconCoords[0][0]*fStretch+pixC1S);
    //PIX pixY = (PIX) (aIconCoords[0][1]*fStretch+pixR1S);
    CTextureObject *pto = &atoIcons[0];
    // FIXME: DG: or was the line below supposed to use pixX and pixY?
    pdp->PutTexture( pto, PIXaabbox2D( PIX2D(pixC1S,pixR1S), PIX2D(pixC2E,pixR2E)), C_WHITE|255);

  } else {

    // render pale map bcg (background)
    pdp->PutTexture( _toMapBcgLU, PIXaabbox2D( PIX2D(pixC1S,pixR1S), PIX2D(pixC1E,pixR1E)), C_WHITE|255);
    pdp->PutTexture( _toMapBcgRU, PIXaabbox2D( PIX2D(pixC2S,pixR1S), PIX2D(pixC2E,pixR1E)), C_WHITE|255);
    pdp->PutTexture( _toMapBcgLD, PIXaabbox2D( PIX2D(pixC1S,pixR2S), PIX2D(pixC1E,pixR2E)), C_WHITE|255);
    pdp->PutTexture( _toMapBcgRD, PIXaabbox2D( PIX2D(pixC2S,pixR2S), PIX2D(pixC2E,pixR2E)), C_WHITE|255);

    // render icons (levels)
    for( unsigned long long iIcon=(!map_bIsFirstEncounter); iIcon<ctLevels; iIcon++)
    {
      // if level's icon should be rendered
      if( ulLevelMask & (1ULL<<iIcon))
      {
        PIX pixX = (PIX) (aIconCoords[iIcon][0]*fStretch+pixC1S);
        PIX pixY = (PIX) (aIconCoords[iIcon][1]*fStretch+pixR1S);
        CTextureObject *pto = &atoIcons[iIcon];
        PIX pixImgW = (PIX) (((CTextureData *)pto->GetData())->GetPixWidth()*fStretch);
        PIX pixImgH = (PIX) (((CTextureData *)pto->GetData())->GetPixHeight()*fStretch);
        pdp->PutTexture( pto, PIXaabbox2D( PIX2D(pixX, pixY), PIX2D(pixX+pixImgW, pixY+pixImgH)), C_WHITE|255);
      }
    }
  }


  if (bAlphaLevels == FALSE) { // dots
    // render paths
    for( INDEX iPath=0; iPath<ctLevels; iPath++)
    {
      INDEX iPrevLevelBit = aPathPrevNextLevels[iPath][0];
      INDEX iNextLevelBit = aPathPrevNextLevels[iPath][1];

      // if path dots should be rendered:
      // if path src and dst levels were discovered and secret level isn't inbetween or hasn't been discovered
      if( ulLevelMask&(1ULL<<iPrevLevelBit) &&
          ulLevelMask&(1ULL<<iNextLevelBit) &&
          ((iNextLevelBit-iPrevLevelBit)==1 || !(ulLevelMask&(1ULL<<(iNextLevelBit-1)))))
      {
        for( INDEX iDot=0; iDot<10; iDot++)
        {
          PIX pixDotX=(PIX) (pixC1S+aPathDots[iPath][iDot][0]*fStretch);
          PIX pixDotY=(PIX) (pixR1S+aPathDots[iPath][iDot][1]*fStretch);
          if(aPathDots[iPath][iDot][0]==-1) break;
          pdp->PutTexture( &_toPathDot, PIXaabbox2D( PIX2D(pixDotX, pixDotY), PIX2D(pixDotX+8*fStretch, pixDotY+8*fStretch)),
            (map_bIsFirstEncounter ? C_WHITE : C_BLACK)|255);
        }
      }
    } // end dots

}

  if( pphi != NULL)
  {
    // set font
    pdp->SetFont( _pfdDisplayFont);
    pdp->SetTextScaling( fStretch);
    pdp->SetTextAspect( 1.0f);
    
    INDEX iPosX, iPosY;
    COLOR colText = RGBToColor(200,128,56)|CT_OPAQUE;

    if(!map_bIsFirstEncounter) {
      // set coordinates and dot colors
      if (ulLevelMask == 0x00000001) {
        iPosX = 200;
        iPosY = 330;
        colText = 0x5c6a9aff;
      } else {
        iPosX = 395; 
        iPosY = 403;
        colText = 0xc87832ff;
      }
    }
    else // DG: make sure iPosX/Y is initialized
    {
      // no idea if 0 makes sense, but at least it's deterministic
      iPosX = 0;
      iPosY = 0;
    }

    PIX pixhtcx = (PIX) (pixC1S+iPosX*fStretch);
    PIX pixhtcy = (PIX) (pixR1S+iPosY*fStretch);

    if(map_bIsFirstEncounter) {
      pixhtcx = pixC1S+116*fStretch;
      pixhtcy = pixR1S+220*fStretch;
    }

    pdp->PutTextC( pphi->phi_strDescription, pixhtcx, pixhtcy, colText);
    for( INDEX iProgresDot=0; iProgresDot<16; iProgresDot+=1)
    {
      if(map_bIsFirstEncounter) {
        PIX pixDotX=(PIX) (pixC1S+(48+iProgresDot*8)*fStretch);
        PIX pixDotY=(PIX) (pixR1S+249*fStretch);

        COLOR colDot = C_WHITE|255;
        if(iProgresDot>pphi->phi_fCompleted*16) {
          colDot = C_WHITE|64;
        }
        pdp->PutTexture( &_toPathDot, PIXaabbox2D( PIX2D(pixDotX, pixDotY),
          PIX2D(pixDotX+8*fStretch, pixDotY+8*fStretch)), colDot);
      } else {
        PIX pixDotX=(PIX) (pixC1S+((iPosX-68)+iProgresDot*8)*fStretch);
        PIX pixDotY=(PIX) (pixR1S+(iPosY+19)*fStretch);

        COLOR colDot = colText|255;
        if(iProgresDot>pphi->phi_fCompleted*16) {
          colDot = C_BLACK|64;
        }
        pdp->PutTexture( &_toPathDot, PIXaabbox2D( PIX2D(pixDotX, pixDotY),
          PIX2D(pixDotX+2+8*fStretch, pixDotY+2+8*fStretch)), C_BLACK|255);
        pdp->PutTexture( &_toPathDot, PIXaabbox2D( PIX2D(pixDotX, pixDotY),
          PIX2D(pixDotX+8*fStretch, pixDotY+8*fStretch)), colDot);
      }
    }
  }

  // free textures used in map rendering
  ReleaseMapData();
}
