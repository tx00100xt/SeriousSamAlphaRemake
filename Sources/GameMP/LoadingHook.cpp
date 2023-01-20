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
#include <locale.h>

#define USECUSTOMTEXT 0

extern CGame *_pGame;

#if USECUSTOMTEXT
  static CTString _strCustomText = "";
#endif
static CDrawPort *_pdpLoadingHook = NULL;  // drawport for loading hook
extern BOOL _bUserBreakEnabled;
extern BOOL map_bIsFirstEncounter;


#define REFRESHTIME (0.2f)

void RemapLevelNames(INDEX &iLevel)
{
  switch( iLevel) {
  case 10:  iLevel =  1;  break;
  case 11:  iLevel =  2;  break;
  case 12:  iLevel =  3;  break;
  case 13:  iLevel =  4;  break;
  case 14:  iLevel =  5;  break;
  case 15:  iLevel =  6;  break;
  case 21:  iLevel =  7;  break;
  case 22:  iLevel =  8;  break;
  case 23:  iLevel =  9;  break;
  case 24:  iLevel = 10;  break;
  case 31:  iLevel = 11;  break;
  case 32:  iLevel = 12;  break;
  case 33:  iLevel = 13;  break;
  default:  iLevel = -1;	break;
  }
}

void dumpbin(unsigned long long k)
{
	for (int i = 0; i < 64; i++) {
		CPrintF("%d", (k & 0x8000000000000000ULL) ? 1 : 0);
		k <<= 1;
	}
	CPrintF("\n");
}

static void LoadingHook_t(CProgressHookInfo *pphi)
{
  // if user presses escape
  ULONG ulCheckFlags = 0x8000;
  if (pphi->phi_fCompleted>0) {
    ulCheckFlags |= 0x0001;
  }
  if (_bUserBreakEnabled && (GetAsyncKeyState(VK_ESCAPE)&ulCheckFlags)) {
    // break loading
    throw TRANS("User break!");
  }

#if USECUSTOMTEXT
  // if no custom loading text
  if (_strCustomText=="") {
    // load it
    try {
      _strCustomText.Load_t(CTFILENAME("Data\\LoadingText.txt"));
    } catch (const char *strError) {
      _strCustomText = strError;
    }
  }
#endif

  // measure time since last call
  static CTimerValue tvLast((__int64) 0);
  CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();

  // if not first or final update, and not enough time passed
  if (pphi->phi_fCompleted!=0 && pphi->phi_fCompleted!=1 &&
     (tvNow-tvLast).GetSeconds() < REFRESHTIME) {
    // do nothing
    return;
  }
  tvLast = tvNow;

  // skip if cannot lock drawport
  CDrawPort *pdp = _pdpLoadingHook;                           
  ASSERT(pdp!=NULL);
  CDrawPort dpHook(pdp, TRUE);
  if( !dpHook.Lock()) return;

  // clear screen
  dpHook.Fill(C_BLACK|255);

  // get session properties currently loading
  CSessionProperties *psp = (CSessionProperties *)_pNetwork->GetSessionProperties();
  unsigned long long ulLevelMask = psp->sp_ulLevelsMask;
  BOOL bAlphaLevels = FALSE;
  //INDEX iLevel = -1;

  if (psp->sp_bCooperative) {
	  INDEX iLevel = -1;
	  INDEX iLevelNext = -1;
	  CTString strLevelName = _pNetwork->ga_fnmWorld.FileName();
	  CTString strNextLevelName = _pNetwork->ga_fnmNextLevel.FileName();
	  strLevelName.ScanF("%02d_", &iLevel);
	  strNextLevelName.ScanF("%02d_", &iLevelNext);
	  map_bIsFirstEncounter = TRUE;
	  unsigned long long ulLevel = (unsigned long long) iLevel;
	  unsigned long long ulLevelNext = (unsigned long long) iLevelNext;
	  if (iLevel == 55) {
	    iLevel = 46;
	  }

	  if (ulLevelMask != 0 && iLevel > 15 && _pGame->gm_bGameOn) {
		bAlphaLevels = TRUE;
		ulLevelMask = 1ULL << ulLevel;
	  } else {
		if (iLevel > 0 && iLevel < 16) {
		  ulLevelMask |= 1ULL << (ulLevel - 1);
        }
		if (iLevelNext > 0 && iLevel < 16) {
		  ulLevelMask |= 1ULL << (ulLevelNext - 1);
		}
		if (iLevel > 15) {
		  bAlphaLevels = TRUE;
		  ulLevelMask = 1ULL << (ulLevel - 1);
		}
	  }
	/*
	extern void dumpbin(unsigned long long k);
	CPrintF("[iLevel %d  ulLevelMask: ", iLevel, ulLevelMask); dumpbin(ulLevelMask);
	*/
  }

  if (ulLevelMask!=0 && !_pNetwork->IsPlayingDemo()) {
    // map hook
    extern void RenderMap( CDrawPort *pdp, unsigned long long ulLevelMask, CProgressHookInfo *pphi, BOOL bAlphaLevels, BOOL bCompModels);
    RenderMap(&dpHook, ulLevelMask, pphi, bAlphaLevels, FALSE);

    // finish rendering
    dpHook.Unlock();
    dpHook.dp_Raster->ra_pvpViewPort->SwapBuffers();

    // keep current time
    tvLast = _pTimer->GetHighPrecisionTimer();
    return;
  }

  // get sizes
  PIX pixSizeI = dpHook.GetWidth();
  PIX pixSizeJ = dpHook.GetHeight();
  CFontData *pfd = _pfdConsoleFont;						//dpHook.SetFont( _pfdDisplayFont);
  PIX pixCharSizeI = pfd->fd_pixCharWidth  + pfd->fd_pixCharSpacing;
  PIX pixCharSizeJ = pfd->fd_pixCharHeight + pfd->fd_pixLineSpacing;

  PIX pixBarSizeJ = 20;//*pixSizeJ/480;

  #ifdef FIRST_ENCOUNTER 
  dpHook.SetFont( _pfdConsoleFont);
  dpHook.SetTextScaling( 1.0f);
  dpHook.SetTextAspect( 1.0f);

  COLOR colBcg = LerpColor(C_BLACK, C_YELLOW, 0.30f)|0xff;
  COLOR colBar = LerpColor(C_BLACK, C_YELLOW, 0.45f)|0xff;
  COLOR colLines = C_BLACK|0xff;
  COLOR colText = LerpColor(C_YELLOW, C_YELLOW, 0.85f)|0xff;
  #else // Second Encounter
  COLOR colBcg = LerpColor(C_BLACK, SE_COL_BLUE_LIGHT, 0.30f)|0xff;
  COLOR colBar = LerpColor(C_BLACK, SE_COL_BLUE_LIGHT, 0.45f)|0xff;
  COLOR colLines = colBar; //C_vdGREEN|0xff;
  COLOR colText = LerpColor(C_BLACK, SE_COL_BLUE_LIGHT, 0.95f)|0xff;
  #endif
  COLOR colEsc = C_YELLOW|0xFF;

  dpHook.Fill(0, pixSizeJ-pixBarSizeJ, pixSizeI, pixBarSizeJ, colBcg);
  dpHook.Fill(0, pixSizeJ-pixBarSizeJ, pixSizeI*pphi->phi_fCompleted, pixBarSizeJ, colBar);
  dpHook.DrawBorder(0, pixSizeJ-pixBarSizeJ, pixSizeI, pixBarSizeJ, colLines);

  //dpHook.SetFont( _pfdConsoleFont);
  //dpHook.SetTextScaling( 1.0f);
  //dpHook.SetTextAspect( 1.0f);
  // print status text
  setlocale(LC_ALL, "");
  CTString strDesc(0, "%s", (const char *) pphi->phi_strDescription);
  strupr((char*)(const char*)strDesc);
  setlocale(LC_ALL, "C");
  CTString strPerc(0, "%3.0f%%", pphi->phi_fCompleted*100);
  //dpHook.PutText(strDesc, pixCharSizeI/2, pixSizeJ-pixBarSizeJ-2-pixCharSizeJ, C_GREEN|255);
  //dpHook.PutTextCXY(strPerc, pixSizeI/2, pixSizeJ-pixBarSizeJ/2+1, C_GREEN|255);
  //dpHook.PutText(strDesc, pixCharSizeI/2, pixSizeJ-pixBarSizeJ+pixCharSizeJ/2, colText);
  //dpHook.PutTextR(strPerc, pixSizeI-pixCharSizeI/2, pixSizeJ-pixBarSizeJ+pixCharSizeJ/2, colText);
  dpHook.PutText(strDesc, pixCharSizeI/2, pixSizeJ-pixBarSizeJ-2-pixCharSizeJ, C_YELLOW|255);

  //dpHook.PutText(strDesc, pixCharSizeI/2, pixSizeJ-pixBarSizeJ+pixCharSizeJ/2, colText);
    dpHook.SetFont( _pfdDisplayFont);
    dpHook.SetTextScaling( 2.0f);
    dpHook.SetTextAspect( 1.0f);
    dpHook.PutTextC( TRANS( "^iSerious Sam Alpha Remake"), pixSizeI/1.95, pixCharSizeJ/3, C_YELLOW|255);
	dpHook.SetFont( _pfdDisplayFont);
    dpHook.SetTextScaling( 1.0f);
    dpHook.SetTextAspect( 1.0f);
    dpHook.PutTextC( TRANS( "If you found any bugs or problems,send private message to facebook page."), pixSizeI/1.95, pixSizeJ/8, C_WHITE|255);
	dpHook.PutTextC( TRANS( "Facebook.com/SeriousSamAlphaRemake"), pixSizeI/1.95, pixSizeJ/6, C_WHITE|255);
	//dpHook.PutTextC( TRANS( "or official topic on seriouszone.com"), pixSizeI/1.95, pixSizeJ/5, C_WHITE|255);
    dpHook.PutTextC( TRANS( "SSA-RM 2015. Created by ^i^c0099ffZdzichu^cffffff.\n"), pixSizeI/1.3, pixSizeJ/1.1, C_WHITE|255);
	dpHook.SetFont( _pfdConsoleFont);
    dpHook.SetTextScaling( 1.0f);
    dpHook.SetTextAspect( 1.0f);
    dpHook.PutTextR(strPerc, pixSizeI-pixCharSizeI/2, pixSizeJ-pixBarSizeJ+pixCharSizeJ/2, colText);
  
  if (_bUserBreakEnabled && !_pGame->gm_bFirstLoading) {
    dpHook.PutTextC( TRANS( "PRESS ESC TO ABORT"), pixSizeI/2, pixSizeJ-pixBarSizeJ-2-pixCharSizeJ, colEsc);
  }
  
  //_pGame->LCDPrepare(1.0f);
  //_pGame->LCDSetDrawport(&dpHook);
  
  // fill the box with background dirt and grid
  //_pGame->LCDRenderClouds1();
  //_pGame->LCDRenderGrid();
/*
  // draw progress bar
  PIX pixBarCentI = pixBoxSizeI*1/2;
  PIX pixBarCentJ = pixBoxSizeJ*3/4;
  PIX pixBarSizeI = pixBoxSizeI*7/8;
  PIX pixBarSizeJ = pixBoxSizeJ*3/8;
  PIX pixBarMinI = pixBarCentI-pixBarSizeI/2;
  PIX pixBarMaxI = pixBarCentI+pixBarSizeI/2;
  PIX pixBarMinJ = pixBarCentJ-pixBarSizeJ/2;
  PIX pixBarMaxJ = pixBarCentJ+pixBarSizeJ/2;

  dpBox.Fill(pixBarMinI, pixBarMinJ, 
    pixBarMaxI-pixBarMinI, pixBarMaxJ-pixBarMinJ, C_BLACK|255);
  dpBox.Fill(pixBarMinI, pixBarMinJ, 
    (pixBarMaxI-pixBarMinI)*pphi->phi_fCompleted, pixBarMaxJ-pixBarMinJ, C_GREEN|255);

  // put more dirt
  _pGame->LCDRenderClouds2Light();

  // draw borders
  COLOR colBorders = LerpColor(C_GREEN, C_BLACK, 200);
  _pGame->LCDDrawBox(0,-1, PIXaabbox2D(
    PIX2D(pixBarMinI, pixBarMinJ), 
    PIX2D(pixBarMaxI, pixBarMaxJ)), 
    colBorders|255);
  _pGame->LCDDrawBox(0,-1, PIXaabbox2D(
    PIX2D(0,0), PIX2D(dpBox.GetWidth(), dpBox.GetHeight())), 
    colBorders|255);

  // print status text
  dpBox.SetFont( _pfdDisplayFont);
  dpBox.SetTextScaling( 1.0f);
  dpBox.SetTextAspect( 1.0f);
  // print status text
  CTString strRes;
  strRes.PrintF( "%s", (const char *) pphi->phi_strDescription);
  //strupr((char*)(const char*)strRes);
  dpBox.PutTextC( strRes, 160, 17, C_GREEN|255);
  strRes.PrintF( "%3.0f%%", pphi->phi_fCompleted*100);
  dpBox.PutTextCXY( strRes, pixBarCentI, pixBarCentJ, C_GREEN|255);
  dpBox.Unlock();

  if( Flesh.gm_bFirstLoading) {
#if USECUSTOMTEXT
    FLOAT fScaling = (FLOAT)slSizeI/640.0f;
    dpHook.Lock();
    dpHook.SetFont( _pfdDisplayFont);
    dpHook.SetTextScaling( fScaling);
    dpHook.SetTextAspect( 1.0f);
    //dpHook.Fill( 0, 0, slSizeI, pixCenterJ, C_vdGREEN|255, C_vdGREEN|255, C_vdGREEN|0, C_vdGREEN|0);
    dpHook.PutTextC( TRANS( "SERIOUS SAM - TEST VERSION"), pixCenterI, 5*fScaling, C_WHITE|255);
    dpHook.PutTextC( TRANS( "THIS IS NOT A DEMO VERSION, THIS IS A COMPATIBILITY TEST!"), pixCenterI, 25*fScaling, C_WHITE|255);
    dpHook.PutTextC( TRANS( "Serious Sam (c) 2000 Croteam LLC, All Rights Reserved.\n"), pixCenterI, 45*fScaling, C_WHITE|255);
    dpHook.PutText( _strCustomText, 1*fScaling, 85*fScaling, C_GREEN|255);
    dpHook.Unlock();
#endif
  } else if (_bUserBreakEnabled) {
    FLOAT fScaling = (FLOAT)slSizeI/640.0f;
    dpHook.Lock();
    dpHook.SetFont( _pfdDisplayFont);
    dpHook.SetTextScaling( fScaling);
    dpHook.SetTextAspect( 1.0f);
    //dpHook.Fill( 0, 0, slSizeI, pixCenterJ, C_vdGREEN|255, C_vdGREEN|255, C_vdGREEN|0, C_vdGREEN|0);
    dpHook.PutTextC( TRANS( "PRESS ESC TO ABORT"), pixCenterI, pixCenterJ+pixBoxSizeJ+5*fScaling, C_WHITE|255);
  }*/
  

  dpHook.Unlock();
  // finish rendering
  dpHook.dp_Raster->ra_pvpViewPort->SwapBuffers();

  // keep current time
  tvLast = _pTimer->GetHighPrecisionTimer();
}

// loading hook functions
void CGame::EnableLoadingHook(CDrawPort *pdpDrawport)
{
  _pdpLoadingHook = pdpDrawport;
  SetProgressHook(LoadingHook_t);
}

void CGame::DisableLoadingHook(void)
{
  SetProgressHook(NULL);
  _pdpLoadingHook = NULL;
}
