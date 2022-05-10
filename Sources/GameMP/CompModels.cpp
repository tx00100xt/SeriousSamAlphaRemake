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

#ifdef DECL_DLL
#undef DECL_DLL
#endif
#define DECL_DLL
#ifdef FIRST_ENCOUNTER
#include "Entities/Common/Particles.h"
#else
#include "EntitiesMP/Common/Particles.h"
#endif

#include "Models/Enemies/Headman/headman.h"
#include "Models/Enemies/Eyeman/Eyeman.h"
#include "Models/Enemies/Boneman/Boneman.h"
#include "Models/Enemies/WereBull/WereBull.h"
#include "Models/Enemies/SCORPMAN/scorpman.h"
#include "Models/Enemies/Walker/Walker.h"
#include "Models/Enemies/Woman/Woman.h"
#include "Models/Enemies/Gizmo/Gizmo.h"
#include "Models/Enemies/Fish/Fish.h"
#include "Models/Enemies/Beast/Beast.h"
#include "Models/Enemies/Devil/Devil.h"
#include "Models/Enemies/DevilAlpha/devil.h"
#include "Models/Enemies/ElementalLava/ElementalLava.h"
#include "ModelsMP/Enemies/Guffy/Guffy.h" 
#include "ModelsMP/Enemies/Grunt/Grunt.h"
#include "ModelsMP/Enemies/Demon/Demon.h"
#include "ModelsMP/Enemies/ChainSawFreak/Freak.h"
#include "ModelsMP/Enemies/CannonStatic/Turret.h"
#include "ModelsMP/Enemies/CannonRotating/Turret.h"
#include "ModelsMP/Enemies/CannonRotating/RotatingMechanism.h"
#include "ModelsMP/Enemies/Summoner/Summoner.h"
#include "ModelsMP/Enemies/ExotechLarva/Body.h"
#include "ModelsMP/Enemies/ExotechLarva/ExotechLarva.h"
#include "ModelsMP/Enemies/ExotechLarva/Arm.h"
#include "ModelsMP/Enemies/ExotechLarva/BackArms.h"
#include "ModelsMP/Enemies/AirElemental/Elemental.h"

#include "Models/Weapons/Knife/KnifeForPlayer.h"
////
#include "Models/Enemies/Catman/CatMan.h"
#include "Models/Enemies/Fishman/fishman.h"
#include "Models/Enemies/HuanMan/Huanman.h"
#include "Models/Enemies/Mamut/MAMUT.H"
#include "Models/Enemies/MAMUTMAN/Mamutman.h"
#include "Models/Enemies/MANTAMAN/mantaman.h"
#include "Models/Enemies/DragonMan/DragonMan.h"
//
#include "Models/Enemies/Cyborg/Cyborg.h"
#include "Models/Enemies/Cyborg/CyborgONEPART.h"
#include "Models/Enemies/Robots/FloatKamikaze/FloatKamikaze.h"
#include "Models/Enemies/Robots/FlyingFighter/FlyingFighter.h"
#include "Models/Enemies/Robots/DrivingSpider/DrivingSpider.h"
#include "Models/Enemies/Robots/DrivingWheel/Robot.h"
//
#include "Models/Enemies/Elementals/AirMan.h"
#include "Models/Enemies/Elementals/IceMan.h"
#include "Models/Enemies/Elementals/Stoneman.h"
#include "Models/Enemies/Elementals/WaterMan.h"
#include "Models/Enemies/Elementals/Maul.h"
#include "Models/Enemies/Elementals/Twister.h"
////
#include "Models/Weapons/Knife/KnifeItem.h"

#include "Models/Weapons/Colt/ColtItem.h"

#include "Models/Weapons/SingleShotgun/SingleShotgunItem.h"
#include "Models/Weapons/DoubleShotgun/DoubleShotgunItem.h"
#include "Models/Weapons/MiniGun/MiniGunItem.h"
#include "Models/Weapons/TommyGun/TommyGunItem.h"
#include "Models/Weapons/RocketLauncher/RocketLauncherItem.h"
#include "Models/Weapons/GrenadeLauncher/GrenadeLauncherItem.h"
#include "Models/Weapons/Laser/LaserItem.h"
#include "Models/Weapons/Cannon/Cannon.h"
//#include "Models/Weapons/GhostBuster/GhostBusterItem.h"
//#include "Models/Weapons/Pipebomb/StickItem.h"
#include "Models/Weapons/LRG/LRGItem.h"
#include "ModelsMP/Weapons/Sniper/SniperItem.h"
#include "ModelsMP/Weapons/ChainSaw/ChainsawItem.h"
#include "ModelsMP/Weapons/ChainSaw/BladeForPlayer.h"
#include "ModelsMP/Weapons/Flamer/FlamerItem.h"

#include "Models/Weapons/GhostBuster/GhostBusterItem.h"
#include "Models/Weapons/GhostBuster/Effect01.h"
//
#include "Models/Weapons/Pipebomb/StickItem.h"
#include "Models/Weapons/Pipebomb/HandWithBomb.h"
#include "Models/Weapons/Pipebomb/HandWithStick.h"
//

#define PARTICLES_NONE            (0L)
#define PARTICLES_AIR_ELEMENTAL   (1L<<1)
#define PARTICLES_LAVA_ELEMENTAL  (1L<<2)

// model's data
static CModelObject _moModel;
static CModelObject _moFloor;
static CPlacement3D _plModel;
static ANGLE3D _aRotation;
static BOOL _bHasFloor = FALSE;
static FLOAT _fFloorY = 0.0f;
static FLOAT _fFOV = 90.0f;
static FLOAT3D _vLightDir = FLOAT3D( -0.2f, -0.2f, -0.2f);
static COLOR _colLight = C_GRAY;
static COLOR _colAmbient = C_vdGRAY;
static COLOR _iParticleType = PARTICLES_NONE;


// model setting values
static CTString _strLastModel = "";
static BOOL _bModelOK = FALSE;

extern FLOAT _fMsgAppearFade;

static CModelObject *AddAttachment_t(CModelObject *pmoParent, INDEX iPosition,
   const CTFileName &fnmModel, INDEX iAnim,
   const CTFileName &fnmTexture,
   const CTFileName &fnmReflection=CTFILENAME(""),
   const CTFileName &fnmSpecular=CTFILENAME(""))
{
  CAttachmentModelObject *pamo = pmoParent->AddAttachmentModel(iPosition);
  ASSERT(pamo!=NULL);
  pamo->amo_moModelObject.SetData_t(fnmModel);
  pamo->amo_moModelObject.PlayAnim(iAnim, AOF_LOOPING);
  pamo->amo_moModelObject.mo_toTexture.SetData_t(fnmTexture);
  pamo->amo_moModelObject.mo_toReflection.SetData_t(fnmReflection);
  pamo->amo_moModelObject.mo_toSpecular.SetData_t(fnmSpecular);
  return &pamo->amo_moModelObject;
}

extern void SetupCompModel_t(const CTString &strName)
{
  CModelObject *pmo = &_moModel;
  _aRotation = ANGLE3D(0,0,0);
  _bHasFloor = FALSE;
  _fFloorY = 0.0f;
  _fFOV = 90.0f;
  _vLightDir = FLOAT3D( -0.2f, -0.2f, -0.2f);
  _colLight = C_GRAY;
  _colAmbient = C_vdGRAY;
  _iParticleType = PARTICLES_NONE;
#ifdef FIRST_ENCOUNTER
  _moFloor.SetData_t(CTFILENAME("Models\\Computer\\Floor.mdl"));
#else
  _moFloor.SetData_t(CTFILENAME("ModelsMP\\Computer\\Floor.mdl"));
#endif
  _moFloor.mo_toTexture.SetData_t(CTFILENAME("Models\\Computer\\Floor.tex"));
  pmo->mo_colBlendColor = 0xFFFFFFFF;
  if (strName=="Rocketman") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Headman\\Headman.mdl"));
    pmo->PlayAnim(HEADMAN_ANIM_IDLE01, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Headman\\Rocketman.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.0,-1.75), ANGLE3D(210,0,0));

    AddAttachment_t(pmo, HEADMAN_ATTACHMENT_LEFT_HAND, 
      CTFILENAME("Models\\Enemies\\Headman\\Head.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Headman\\Head.tex"));
	AddAttachment_t(pmo, HEADMAN_ATTACHMENT_RIGHT_HAND, 
      CTFILENAME("Models\\Enemies\\Headman\\RocketLauncher.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Headman\\RocketLauncher.tex"));
	AddAttachment_t(pmo, HEADMAN_ATTACHMENT_SAW, 
      CTFILENAME("Models\\Enemies\\Headman\\Saw.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Headman\\Saw.tex"));
    pmo->StretchModel(FLOAT3D(1.25f,1.25f,1.25f));
    _bHasFloor = TRUE;

  } else if (strName=="Firecracker") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Headman\\Headman.mdl"));
    pmo->PlayAnim(HEADMAN_ANIM_IDLE01
		, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Headman\\Firecracker.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.0,-1.75), ANGLE3D(210,0,0));

    AddAttachment_t(pmo, HEADMAN_ATTACHMENT_LEFT_HAND, 
      CTFILENAME("Models\\Enemies\\Headman\\Head.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Headman\\FirecrackerHead.tex"));
    AddAttachment_t(pmo, HEADMAN_ATTACHMENT_RIGHT_HAND, 
      CTFILENAME("Models\\Enemies\\Headman\\Chainsaw.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Headman\\Chainsaw.tex"));
	AddAttachment_t(pmo, HEADMAN_ATTACHMENT_SAW, 
      CTFILENAME("Models\\Enemies\\Headman\\Stick.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Headman\\Stick.tex"));
    pmo->StretchModel(FLOAT3D(1.25f,1.25f,1.25f));
    _bHasFloor = TRUE;

  } else if (strName=="Bomberman") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Headman\\Headman.mdl"));
    pmo->PlayAnim(HEADMAN_ANIM_IDLE01, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Headman\\Bomberman.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.0,-1.75), ANGLE3D(210,0,0));
  
    AddAttachment_t(pmo, HEADMAN_ATTACHMENT_SAW, 
      CTFILENAME("Models\\Enemies\\Headman\\HeadOnStick.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Headman\\BombermanHead.tex"));
	AddAttachment_t(pmo, HEADMAN_ATTACHMENT_LEFT_HAND, 
      CTFILENAME("Models\\Enemies\\Headman\\Bag.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Headman\\Bag.tex"));
    pmo->StretchModel(FLOAT3D(1.25f,1.25f,1.25f));
    _bHasFloor = TRUE;

  } else if (strName=="Kamikaze") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Headman\\Headman.mdl"));
    pmo->PlayAnim(HEADMAN_ANIM_IDLE01, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Headman\\Kamikaze.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.0,-1.75), ANGLE3D(210,0,0));
  
    AddAttachment_t(pmo, HEADMAN_ATTACHMENT_BOMB_RIGHT_HAND, 
      CTFILENAME("Models\\Enemies\\Headman\\Projectile\\Bomb.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Headman\\Projectile\\Bomb.tex"));
    AddAttachment_t(pmo, HEADMAN_ATTACHMENT_BOMB_LEFT_HAND, 
      CTFILENAME("Models\\Enemies\\Headman\\Projectile\\Bomb.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Headman\\Projectile\\Bomb.tex"));
    pmo->StretchModel(FLOAT3D(1.25f,1.25f,1.25f));
    _bHasFloor = TRUE;

  } else if (strName=="EyemanPurple") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Eyeman\\Eyeman.mdl"));
    pmo->PlayAnim(EYEMAN_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Eyeman\\Eyeman4.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.9f,-1.5), ANGLE3D(210,0,0));
    pmo->StretchModel(FLOAT3D(1,1,1));
    _bHasFloor = TRUE;

  } else if (strName=="EyemanGreen") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Eyeman\\Eyeman.mdl"));
    pmo->PlayAnim(EYEMAN_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Eyeman\\Eyeman5.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.2f,-2.0), ANGLE3D(210,0,0));
    pmo->StretchModel(FLOAT3D(1.3f, 1.3f, 1.3f));
    _bHasFloor = TRUE;

  } else if (strName=="robotfighter") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Robots\\FlyingFighter\\Ship.mdl"));
    pmo->PlayAnim(FLYINGFIGHTER_ANIM_DEFAULT_ANIMATION, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Robots\\FlyingFighter\\Ship.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.5f,-2.0), ANGLE3D(210,0,0));
    pmo->StretchModel(FLOAT3D(1.3f, 1.3f, 1.3f));
    _bHasFloor = TRUE;

  } else if (strName=="robotkamikaze") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Robots\\FloatKamikaze\\FloatKamikaze.mdl"));
    pmo->PlayAnim(FLOATKAMIKAZE_ANIM_DEFAULT_ANIMATION, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Robots\\SentryBall\\Ball.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.3f,-2.0), ANGLE3D(210,0,0));
    pmo->StretchModel(FLOAT3D(0.6f, 0.6f, 0.6f));
    _bHasFloor = TRUE;
  } else if (strName=="robotr2d2") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Robots\\DrivingWheel\\Robot.mdl"));
    pmo->PlayAnim(ROBOT_ANIM_DEFAULT_ANIMATION, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Robots\\DrivingWheel\\Robot.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.7f,-2.0), ANGLE3D(210,0,0));
    pmo->StretchModel(FLOAT3D(0.6f, 0.6f, 0.6f));
    _bHasFloor = TRUE;

  } else if (strName=="RobotSpider") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Robots\\DrivingSpider\\DrivingSpider.mdl"));
    pmo->PlayAnim(DRIVINGSPIDER_ANIM_DEFAULT_ANIMATION, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Robots\\SentryBall\\Ball.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.7f,-2.0), ANGLE3D(210,0,0));
    pmo->StretchModel(FLOAT3D(0.4f, 0.4f, 0.4f));
    _bHasFloor = TRUE;

  } else if (strName=="Boneman") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Boneman\\Boneman.mdl"));
    pmo->PlayAnim(BONEMAN_ANIM_WALKLOOP, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Boneman\\Boneman.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.0f,-3.0), ANGLE3D(210,0,0));
    pmo->StretchModel(FLOAT3D(1,1,1));
    _bHasFloor = TRUE;

  } else if (strName=="BullSummer") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Werebull\\Werebull.mdl"));
    pmo->PlayAnim(WEREBULL_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Werebull\\Werebull.tex"));
    _plModel = CPlacement3D(FLOAT3D(0.0f,-1.5f,-4.0), ANGLE3D(-110,0,0));
    pmo->StretchModel(FLOAT3D(1,1,1));
    _bHasFloor = TRUE;

  } else if (strName=="BullWinter") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Werebull\\Werebull.mdl"));
    pmo->PlayAnim(WEREBULL_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Werebull\\Werebull2.tex"));
    _plModel = CPlacement3D(FLOAT3D(0.0f,-1.5f,-4.0), ANGLE3D(-110,0,0));
    pmo->StretchModel(FLOAT3D(1,1,1));
    _bHasFloor = TRUE;

  } else if (strName=="MamutSummer") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Mamut\\Mamut.mdl"));
    pmo->PlayAnim(MAMUT_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Mamut\\Mamut.tex"));
    _plModel = CPlacement3D(FLOAT3D(0.0f,-3.5f,-6.0), ANGLE3D(-110,0,0));
	
	AddAttachment_t(pmo, MAMUT_ATTACHMENT_MAN_FRONT, 
      CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.tex"));
	AddAttachment_t(pmo, MAMUT_ATTACHMENT_MAN_MIDDLE, 
      CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.tex"));
	AddAttachment_t(pmo, MAMUT_ATTACHMENT_MAN_REAR, 
      CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.tex"));	  
	  
    pmo->StretchModel(FLOAT3D(1,1,1));
    _bHasFloor = TRUE;
	
  } else if (strName=="MamutWinter") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Mamut\\Mamut.mdl"));
    pmo->PlayAnim(MAMUT_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Mamut\\Mamut3.tex"));
    _plModel = CPlacement3D(FLOAT3D(0.0f,-3.5f,-6.0), ANGLE3D(-110,0,0));
	
	AddAttachment_t(pmo, MAMUT_ATTACHMENT_MAN_FRONT, 
      CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.tex"));
	AddAttachment_t(pmo, MAMUT_ATTACHMENT_MAN_MIDDLE, 
      CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.tex"));
	AddAttachment_t(pmo, MAMUT_ATTACHMENT_MAN_REAR, 
      CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.tex"));	  
	  
    pmo->StretchModel(FLOAT3D(1,1,1));
    _bHasFloor = TRUE;
	
  } else if (strName=="Cyborg") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.mdl"));
    pmo->PlayAnim(CYBORG_ANIM_WALK01, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.0f,-2.25f), ANGLE3D(210,0,0));
	
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_ASS, 
      CTFILENAME("Models\\Enemies\\Cyborg\\AssHole.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo,CYBORG_ATTACHMENT_TORSO, 
      CTFILENAME("Models\\Enemies\\Cyborg\\Torso.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_HEAD, 
      CTFILENAME("Models\\Enemies\\Cyborg\\Head.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_RIGHTUPPERARM, 
      CTFILENAME("Models\\Enemies\\Cyborg\\RightUpperArm.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo,CYBORG_ATTACHMENT_RIGHTLOWERARM, 
      CTFILENAME("Models\\Enemies\\Cyborg\\RightLowerArm.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_LEFTUPPERARM, 
      CTFILENAME("Models\\Enemies\\Cyborg\\LeftUpperArm.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_LEFTLOWERARM, 
      CTFILENAME("Models\\Enemies\\Cyborg\\LeftLowerArm.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	  
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_RIGHTUPPERLEG, 
      CTFILENAME("Models\\Enemies\\Cyborg\\RightUpperLeg.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex")); 
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_RIGHTLOWERLEG, 
      CTFILENAME("Models\\Enemies\\Cyborg\\RightLowerLeg.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo,CYBORG_ATTACHMENT_LEFTUPPERLEG, 
      CTFILENAME("Models\\Enemies\\Cyborg\\LeftUpperLeg.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo,  CYBORG_ATTACHMENT_LEFTLOWERLEG, 
      CTFILENAME("Models\\Enemies\\Cyborg\\LeftLowerLeg.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_FOOTRIGHT, 
      CTFILENAME("Models\\Enemies\\Cyborg\\Foot.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_FOOTLEFT, 
      CTFILENAME("Models\\Enemies\\Cyborg\\Foot.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	  
    pmo->StretchModel(FLOAT3D(1.0f,1.0f,1.0f));
    _bHasFloor = TRUE;
	
  } else if (strName=="Cyborg Bike") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.mdl"));
    pmo->PlayAnim(CYBORG_ANIM_BIKEREST, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-2.0,-2.75), ANGLE3D(210,0,0));
	
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_ASS, 
      CTFILENAME("Models\\Enemies\\Cyborg\\AssHole.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo,CYBORG_ATTACHMENT_TORSO, 
      CTFILENAME("Models\\Enemies\\Cyborg\\Torso.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_HEAD, 
      CTFILENAME("Models\\Enemies\\Cyborg\\Head.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_RIGHTUPPERARM, 
      CTFILENAME("Models\\Enemies\\Cyborg\\RightUpperArm.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo,CYBORG_ATTACHMENT_RIGHTLOWERARM, 
      CTFILENAME("Models\\Enemies\\Cyborg\\RightLowerArm.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_LEFTUPPERARM, 
      CTFILENAME("Models\\Enemies\\Cyborg\\LeftUpperArm.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_LEFTLOWERARM, 
      CTFILENAME("Models\\Enemies\\Cyborg\\LeftLowerArm.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	  
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_RIGHTUPPERLEG, 
      CTFILENAME("Models\\Enemies\\Cyborg\\RightUpperLeg.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex")); 
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_RIGHTLOWERLEG, 
      CTFILENAME("Models\\Enemies\\Cyborg\\RightLowerLeg.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo,CYBORG_ATTACHMENT_LEFTUPPERLEG, 
      CTFILENAME("Models\\Enemies\\Cyborg\\LeftUpperLeg.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo,  CYBORG_ATTACHMENT_LEFTLOWERLEG, 
      CTFILENAME("Models\\Enemies\\Cyborg\\LeftLowerLeg.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_FOOTRIGHT, 
      CTFILENAME("Models\\Enemies\\Cyborg\\Foot.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_FOOTLEFT, 
      CTFILENAME("Models\\Enemies\\Cyborg\\Foot.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Cyborg.tex"));
	  
	AddAttachment_t(pmo, CYBORG_ATTACHMENT_BIKE, 
	  CTFILENAME("Models\\Enemies\\Cyborg\\Bike.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Cyborg\\Bike.tex"));
	  
    pmo->StretchModel(FLOAT3D(1.0f,1.0f,1.0f));
    _bHasFloor = TRUE;
		
  } else if (strName=="Fishman") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Fishman\\Fishman.mdl"));
    pmo->PlayAnim(FISHMAN_ANIM_GROUNDWALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Fishman\\Fishman.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.0,-1.75), ANGLE3D(210,0,0));

    pmo->StretchModel(FLOAT3D(1.0f,1.0f,1.0f));
    _bHasFloor = TRUE;

  } else if (strName=="DragonmanSoldier") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Dragonman\\Dragonman.mdl"));
    pmo->PlayAnim(DRAGONMAN_ANIM_GROUNDWALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Dragonman\\DragonMan01.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.7,-2.75), ANGLE3D(210,0,0));

    pmo->StretchModel(FLOAT3D(1.0f,1.0f,1.0f));
    _bHasFloor = TRUE;

  } else if (strName=="DragonmanSergeant") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Dragonman\\Dragonman.mdl"));
    pmo->PlayAnim(DRAGONMAN_ANIM_GROUNDWALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Dragonman\\DragonMan02.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.7,-2.75), ANGLE3D(210,0,0));

    pmo->StretchModel(FLOAT3D(1.0f,1.0f,1.0f));
    _bHasFloor = TRUE;

  } else if (strName=="DragonmanMonster") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Dragonman\\Dragonman.mdl"));
    pmo->PlayAnim(DRAGONMAN_ANIM_GROUNDWALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Dragonman\\DragonMan03.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.7,-2.75), ANGLE3D(210,0,0));

    pmo->StretchModel(FLOAT3D(1.0f,1.0f,1.0f));
    _bHasFloor = TRUE;
			
  } else if (strName=="CatmanSoldier") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Catman\\Catman.mdl"));
    pmo->PlayAnim(CATMAN_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Catman\\Catman03.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.7,-2.75), ANGLE3D(210,0,0));

    pmo->StretchModel(FLOAT3D(1.15f,1.15f,1.15f));
    _bHasFloor = TRUE;

  } else if (strName=="CatmanGeneral") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Catman\\Catman.mdl"));
    pmo->PlayAnim(CATMAN_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Catman\\Catman03.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.7,-2.75), ANGLE3D(210,0,0));

    pmo->StretchModel(FLOAT3D(1.15f,1.15f,1.15f));
    _bHasFloor = TRUE;

  } else if (strName=="CatmanRogue") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Catman\\Catman.mdl"));
    pmo->PlayAnim(CATMAN_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Catman\\Catman03.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.7,-2.75), ANGLE3D(210,0,0));

    pmo->StretchModel(FLOAT3D(1.15f,1.15f,1.15f));
    _bHasFloor = TRUE;

  } else if (strName=="Mamutman") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.mdl"));
    pmo->PlayAnim(MAMUTMAN_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Mamutman\\Mamutman.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.0,-1.75), ANGLE3D(210,0,0));

    pmo->StretchModel(FLOAT3D(1.25f,1.25f,1.25f));
    _bHasFloor = TRUE;

  } else if (strName=="Huanman") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Huanman\\Huanman.mdl"));
    pmo->PlayAnim(HUANMAN_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Huanman\\Huanman.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-2.0f,-3.0), ANGLE3D(210,30,0));
    pmo->StretchModel(FLOAT3D(1,1,1));
    _bHasFloor = TRUE;
    _fFloorY = -1.5f;
 
  } else if (strName=="Mantaman") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Mantaman\\Mantaman.mdl"));
    pmo->PlayAnim(MANTAMAN_ANIM_SWIMFAST, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Mantaman\\Mantaman.tex"));

    _plModel = CPlacement3D(FLOAT3D(0,0.0f,-2.0), ANGLE3D(210,30,0));
    pmo->StretchModel(FLOAT3D( 0.75f, 0.75f, 0.75f));
    _bHasFloor = TRUE;
    _fFloorY = -1.5f;

  } else if (strName=="ScorpmanSoldier") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Scorpman\\Scorpman.mdl"));
    pmo->mo_toSpecular.SetData_t(CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->PlayAnim(SCORPMAN_ANIM_STAND, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Scorpman\\Soldier.tex"));

    _plModel = CPlacement3D(FLOAT3D(0+0.5f*3,-3.0f,-7.0)*2/3, ANGLE3D(135,0,0));
    _vLightDir = FLOAT3D( 0.2f, -0.2f, -0.2f);
    _colLight = C_lGRAY;
    _colAmbient = C_vdGRAY;

    AddAttachment_t(pmo, SCORPMAN_ATTACHMENT_MINIGUN, 
      CTFILENAME("Models\\Enemies\\Scorpman\\Gun.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Scorpman\\Gun.tex"));

    pmo->StretchModel(FLOAT3D(2.0f,2.0f,2.0f));
    _bHasFloor = TRUE;

  } else if (strName=="ScorpmanGeneral") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Scorpman\\Scorpman.mdl"));
    pmo->PlayAnim(SCORPMAN_ANIM_STAND, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Scorpman\\General.tex"));
    pmo->mo_toSpecular.SetData_t(CTFILENAME("Models\\SpecularTextures\\Medium.tex"));

    _plModel = CPlacement3D(FLOAT3D(0+0.5f*3,-3.0f,-7.0), ANGLE3D(135,0,0));
    _vLightDir = FLOAT3D( 0.2f, -0.2f, -0.2f);
    _colLight = C_lGRAY;
    _colAmbient = C_vdGRAY;

    AddAttachment_t(pmo, SCORPMAN_ATTACHMENT_MINIGUN, 
      CTFILENAME("Models\\Enemies\\Scorpman\\Gun.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Scorpman\\Gun.tex"));

    pmo->StretchModel(FLOAT3D(3.0f,3.0f,3.0f));
    _bHasFloor = TRUE;

  } else if (strName=="DevilAlphaEgyptian") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\DevilAlpha\\Yknot\\Devil2.mdl"));
    pmo->PlayAnim(DEVIL_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\DevilAlpha\\Devil2.tex"));
    pmo->mo_toSpecular.SetData_t(CTFILENAME("Models\\SpecularTextures\\Medium.tex"));

    _plModel = CPlacement3D(FLOAT3D(0,-2.0f,-5.0), ANGLE3D(210,0,0));
    _vLightDir = FLOAT3D( 0.2f, -0.2f, -0.2f);
    _colLight = C_lGRAY;
    _colAmbient = C_vdGRAY;

    AddAttachment_t(pmo, DEVIL_ATTACHMENT_MINIGUN, 
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Minigun.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Minigun\\Minigun.tex"));
    AddAttachment_t(pmo, DEVIL_ATTACHMENT_STICK, 
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Stick.mdl"), 0,
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Stick.tex"));
    AddAttachment_t(pmo, DEVIL_ATTACHMENT_SHIELD, 
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Shield.mdl"), 0,
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Shield.tex"));

    pmo->StretchModel(FLOAT3D(1.5,1.5,1.5));
    _bHasFloor = TRUE;

  } else if (strName=="DevilAlphaPlanetoidal") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\DevilAlpha\\Yknot\\Devil2.mdl"));
    pmo->mo_toSpecular.SetData_t(CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->PlayAnim(DEVIL_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\DevilAlpha\\Devil.tex"));

    _plModel = CPlacement3D(FLOAT3D(0,-2.0f,-5.0), ANGLE3D(210,0,0));
    _vLightDir = FLOAT3D( 0.2f, -0.2f, -0.2f);
    _colLight = C_lGRAY;
    _colAmbient = C_vdGRAY;

    AddAttachment_t(pmo, DEVIL_ATTACHMENT_MINIGUN, 
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Minigun.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Minigun\\Minigun.tex"));
    AddAttachment_t(pmo, DEVIL_ATTACHMENT_STICK, 
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Stick.mdl"), 0,
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Stick.tex"));
    AddAttachment_t(pmo, DEVIL_ATTACHMENT_SHIELD, 
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Shield.mdl"), 0,
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Shield.tex"));

    pmo->StretchModel(FLOAT3D(1.5,1.5,1.5));
    _bHasFloor = TRUE;

  } else if (strName=="DevilAlphaSirius") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\DevilAlpha\\Yknot\\Devil2.mdl"));
    pmo->PlayAnim(DEVIL_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\DevilAlpha\\Devil4.tex"));
    pmo->mo_toSpecular.SetData_t(CTFILENAME("Models\\SpecularTextures\\Medium.tex"));

    _plModel = CPlacement3D(FLOAT3D(0,-2.0f,-5.0), ANGLE3D(210,0,0));
    _vLightDir = FLOAT3D( 0.2f, -0.2f, -0.2f);
    _colLight = C_lGRAY;
    _colAmbient = C_vdGRAY;

    AddAttachment_t(pmo, DEVIL_ATTACHMENT_MINIGUN, 
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Minigun.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Minigun\\Minigun.tex"));
    AddAttachment_t(pmo, DEVIL_ATTACHMENT_STICK, 
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Stick.mdl"), 0,
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Stick.tex"));
    AddAttachment_t(pmo, DEVIL_ATTACHMENT_SHIELD, 
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Shield.mdl"), 0,
      CTFILENAME("Models\\Enemies\\DevilAlpha\\Shield.tex"));

    pmo->StretchModel(FLOAT3D(1.5,1.5,1.5));
    _bHasFloor = TRUE;

  } else if (strName=="ScorpmanMonster") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Scorpman\\Scorpman.mdl"));
    pmo->PlayAnim(SCORPMAN_ANIM_STAND, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Scorpman\\Monster.tex"));
    pmo->mo_toSpecular.SetData_t(CTFILENAME("Models\\SpecularTextures\\Medium.tex"));

    _plModel = CPlacement3D(FLOAT3D(0+0.5f*3,-3.0f,-7.0), ANGLE3D(135,0,0));
    _vLightDir = FLOAT3D( 0.2f, -0.2f, -0.2f);
    _colLight = C_lGRAY;
    _colAmbient = C_vdGRAY;

    AddAttachment_t(pmo, SCORPMAN_ATTACHMENT_MINIGUN, 
      CTFILENAME("Models\\Enemies\\Scorpman\\Gun.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Scorpman\\Gun.tex"));

    pmo->StretchModel(FLOAT3D(3.0f,3.0f,3.0f));
    _bHasFloor = TRUE;

  } else if (strName=="WalkerSmall") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Walker\\Walker.mdl"));
    pmo->PlayAnim(WALKER_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Walker\\Walker.tex"));
    /*AddAttachment_t(pmo, WALKER_ATTACHMENT_ROCKETLAUNCHER_LT, 
      CTFILENAME("Models\\Enemies\\Walker\\RocketLauncher.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Walker\\RocketLauncher.tex"),
      CTFILENAME(""),*/
      CTFILENAME("Models\\SpecularTextures\\Medium.tex");
    /*AddAttachment_t(pmo, WALKER_ATTACHMENT_ROCKETLAUNCHER_RT, 
      CTFILENAME("Models\\Enemies\\Walker\\RocketLauncher.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Walker\\RocketLauncher.tex"),
      CTFILENAME(""),*/
      CTFILENAME("Models\\SpecularTextures\\Medium.tex");
    _plModel = CPlacement3D(FLOAT3D(0,-2.0f,-5.0), ANGLE3D(210,0,0));
    pmo->StretchModel(FLOAT3D(0.5,0.5,0.5));
    _bHasFloor = TRUE;
    _colLight = C_lGRAY;
    _colAmbient = C_vdGRAY;

  } else if (strName=="WalkerBig") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Walker\\Walker.mdl"));
    pmo->PlayAnim(WALKER_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Walker\\Walker3.tex"));
    /*AddAttachment_t(pmo, WALKER_ATTACHMENT_ROCKETLAUNCHER_LT, 
      CTFILENAME("Models\\Enemies\\Walker\\RocketLauncher.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Walker\\RocketLauncher.tex"),
      CTFILENAME(""),*/
      CTFILENAME("Models\\SpecularTextures\\Medium.tex");
    /*AddAttachment_t(pmo, WALKER_ATTACHMENT_ROCKETLAUNCHER_RT, 
      CTFILENAME("Models\\Enemies\\Walker\\RocketLauncher.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Walker\\RocketLauncher.tex"),
      CTFILENAME(""),*/
      CTFILENAME("Models\\SpecularTextures\\Medium.tex");
    _plModel = CPlacement3D(FLOAT3D(0,-4.0f,-10.0), ANGLE3D(210,0,0));
    pmo->StretchModel(FLOAT3D(1,1,1));
    _bHasFloor = TRUE;

  } else if (strName=="Woman") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Woman\\Woman.mdl"));
    pmo->PlayAnim(WOMAN_ANIM_AIRSTAND, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Woman\\Woman.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,0.0f,-2.0), ANGLE3D(210,30,0));
    pmo->StretchModel(FLOAT3D(1,1,1));
    _bHasFloor = TRUE;
    _fFloorY = -1.5f;

  } else if (strName=="Gizmo") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Gizmo\\Gizmo.mdl"));
    pmo->PlayAnim(GIZMO_ANIM_IDLE, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Gizmo\\Gizmo.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.5f,-1.2f), ANGLE3D(150,0,0));
    pmo->StretchModel(FLOAT3D(1,1,1));
    _fFloorY = 0.0f;
    _bHasFloor = TRUE;

  } else if (strName=="Fish") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Fish\\Fish.mdl"));
    pmo->PlayAnim(FISH_ANIM_IDLE, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Fish\\Fish1.tex"));
    pmo->mo_toSpecular.SetData_t(CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    _plModel = CPlacement3D(FLOAT3D(-0.2f,-0.5f,-3.0), ANGLE3D(250,0,0));
    pmo->StretchModel(FLOAT3D(1,1,1));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

  } else if (strName=="BeastNormal") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Beast\\Beast.mdl"));
    pmo->PlayAnim(BEAST_ANIM_IDLECOMPUTER, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Beast\\Beast.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-2.0f,-6.0), ANGLE3D(170,0,0));
    pmo->StretchModel(FLOAT3D(2,2,2));
    _bHasFloor = TRUE;

  } else if (strName=="BeastBig") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Beast\\Beast.mdl"));
    pmo->PlayAnim(BEAST_ANIM_IDLECOMPUTER, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Beast\\BeastBig.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-6.5f,-18.0), ANGLE3D(170,0,0));

    pmo->StretchModel(FLOAT3D(6,6,6));
    _bHasFloor = TRUE;

  } else if (strName=="ElementalLava") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\ElementalLava\\ElementalLava.mdl"));
    pmo->PlayAnim(LAVAMAN_ANIM_RUN, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\ElementalLava\\Lava04Fx.tex"));
    pmo->mo_toBump.SetData_t(CTFILENAME("Models\\Enemies\\ElementalLava\\Detail.tex"));
    AddAttachment_t(pmo, ELEMENTALLAVA_ATTACHMENT_BODY_FLARE, 
      CTFILENAME("Models\\Enemies\\ElementalLava\\BodyFlare.mdl"), 0,
      CTFILENAME("Models\\Enemies\\ElementalLava\\Flare.tex"));
    AddAttachment_t(pmo, ELEMENTALLAVA_ATTACHMENT_RIGHT_HAND_FLARE, 
      CTFILENAME("Models\\Enemies\\ElementalLava\\HandFlare.mdl"), 0,
      CTFILENAME("Models\\Enemies\\ElementalLava\\Flare.tex"));
    AddAttachment_t(pmo, ELEMENTALLAVA_ATTACHMENT_LEFT_HAND_FLARE, 
      CTFILENAME("Models\\Enemies\\ElementalLava\\HandFlare.mdl"), 0,
      CTFILENAME("Models\\Enemies\\ElementalLava\\Flare.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-4.5f,-8.0), ANGLE3D(150,0,0));
    pmo->StretchModel(FLOAT3D(4,4,4));
    _bHasFloor = TRUE;

    _iParticleType = PARTICLES_LAVA_ELEMENTAL;

  } else if (strName=="ElementalWater") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Elementals\\WaterMan.mdl"));
    pmo->PlayAnim(WATERMAN_ANIM_RUN, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Elementals\\WaterManFX.tex"));
    AddAttachment_t(pmo, WATERMAN_ATTACHMENT_BODY_FLARE, 
      CTFILENAME("Models\\Enemies\\Elementals\\WaterManFX\\BodyFlare.mdl"), 0,
      CTFILENAME("Models\\Enemies\\Elementals\\WaterManFX\\BodyFlare.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-4.5f,-8.0), ANGLE3D(150,0,0));
    pmo->StretchModel(FLOAT3D(4,4,4));
    _bHasFloor = TRUE;

  } else if (strName=="ElementalIce") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Elementals\\IceMan.mdl"));
    pmo->PlayAnim(ICEMAN_ANIM_RUN, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Elementals\\IceMan01.tex"));
    //AddAttachment_t(pmo, WATERMAN_ATTACHMENT_BODY_FLARE, 
    //  CTFILENAME("Models\\Enemies\\Elementals\\WaterManFX\\BodyFlare.mdl"), 0,
    //  CTFILENAME("Models\\Enemies\\Elementals\\WaterManFX\\BodyFlare.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-4.5f,-8.0), ANGLE3D(150,0,0));
    pmo->StretchModel(FLOAT3D(4,4,4));
    _bHasFloor = TRUE;

    //_iParticleType = PARTICLES_LAVA_ELEMENTAL;

  } else if (strName=="ElementalAir") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Elementals\\AirMan.mdl"));
    pmo->PlayAnim(AIRMAN_ANIM_RUN, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Elementals\\AirMan01.tex"));
    //AddAttachment_t(pmo, WATERMAN_ATTACHMENT_BODY_FLARE, 
    //  CTFILENAME("Models\\Enemies\\Elementals\\WaterManFX\\BodyFlare.mdl"), 0,
    //  CTFILENAME("Models\\Enemies\\Elementals\\WaterManFX\\BodyFlare.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-4.5f,-8.0), ANGLE3D(150,0,0));
    pmo->StretchModel(FLOAT3D(4,4,4));
    _bHasFloor = TRUE;

  } else if (strName=="ElementalStone") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Elementals\\StoneMan.mdl"));
    pmo->PlayAnim(STONEMAN_ANIM_RUN, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Elementals\\StoneMan01.tex"));
    //AddAttachment_t(pmo, WATERMAN_ATTACHMENT_BODY_FLARE, 
    //  CTFILENAME("Models\\Enemies\\Elementals\\WaterManFX\\BodyFlare.mdl"), 0,
    //  CTFILENAME("Models\\Enemies\\Elementals\\WaterManFX\\BodyFlare.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-4.5f,-8.0), ANGLE3D(150,0,0));
    pmo->StretchModel(FLOAT3D(4,4,4));
    _bHasFloor = TRUE;

  } else if (strName=="Devil") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Devil\\Devil.mdl"));
    pmo->PlayAnim(DEVIL_ANIM_WALK, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Enemies\\Devil\\Devil.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-12.0f,-25.0), ANGLE3D(150,0,0));
    pmo->StretchModel(FLOAT3D(12,12,12));
    _bHasFloor = TRUE;

  } else if (strName=="Guffy") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Enemies\\Guffy\\Guffy.mdl"));
    pmo->PlayAnim(GUFFY_ANIM_IDLE, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Enemies\\Guffy\\Guffy.tex"));
    AddAttachment_t(pmo, GUFFY_ATTACHMENT_GUNRIGHT, 
      CTFILENAME("ModelsMP\\Enemies\\Guffy\\Gun.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\Guffy\\Gun.tex"));
    AddAttachment_t(pmo, GUFFY_ATTACHMENT_GUNLEFT, 
      CTFILENAME("ModelsMP\\Enemies\\Guffy\\Gun.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\Guffy\\Gun.tex"));
    CModelObject *pmoRight = &pmo->GetAttachmentModel(GUFFY_ATTACHMENT_GUNRIGHT)->amo_moModelObject;
    pmoRight->StretchModel(FLOAT3D(-1,1,1));
    _plModel = CPlacement3D(FLOAT3D(0.0f,-2.0f,-5.75f), ANGLE3D(210,0,0));
    _fFOV = 70.0f;
    
    _vLightDir = FLOAT3D( -0.1f, -0.1f, -0.175f);
    pmo->StretchModel(FLOAT3D(1.5f, 1.5f, 1.5f));
    _bHasFloor = TRUE;

  } else if (strName=="GruntSoldier") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Enemies\\Grunt\\Grunt.mdl"));
    pmo->PlayAnim(GRUNT_ANIM_IDLEPATROL, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Enemies\\Grunt\\Soldier.tex"));
    AddAttachment_t(pmo, GRUNT_ATTACHMENT_GUN_SMALL, 
      CTFILENAME("ModelsMP\\Enemies\\Grunt\\Gun.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\Grunt\\Gun.tex"));
    _plModel = CPlacement3D(FLOAT3D(0.0f,-1.5f,-3.7f), ANGLE3D(165.0f,0.0f,0.0f));
    _fFOV = 70.0f;
    
    _vLightDir = FLOAT3D( -0.1f, -0.2f, -0.2f);
    pmo->StretchModel(FLOAT3D(1.2f, 1.2f, 1.2f));
    _fFloorY = 0.0f;
    _bHasFloor = TRUE;

  } else if (strName=="GruntCommander") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Enemies\\Grunt\\Grunt.mdl"));
    pmo->PlayAnim(GRUNT_ANIM_IDLEPATROL, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Enemies\\Grunt\\Commander.tex"));
    AddAttachment_t(pmo, GRUNT_ATTACHMENT_GUN_COMMANDER, 
      CTFILENAME("ModelsMP\\Enemies\\Grunt\\Gun_Commander.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\Grunt\\Gun_Commander.tex"));
    _plModel = CPlacement3D(FLOAT3D(0.0f,-1.65f,-3.9f), ANGLE3D(165.0f,0.0f,0.0f));
    _fFOV = 70.0f;
    
    _vLightDir = FLOAT3D( -0.1f, -0.2f, -0.2f);
    
    pmo->StretchModel(FLOAT3D(1.4f, 1.4f, 1.4f));
    _fFloorY = 0.0f;
    _bHasFloor = TRUE;

  } else if (strName=="Demon") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Enemies\\Demon\\Demon.mdl"));
    pmo->PlayAnim(DEMON_ANIM_IDLE, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Enemies\\Demon\\Demon.tex"));
    _plModel = CPlacement3D(FLOAT3D(0.0f, -1.25f, -2.25f), ANGLE3D(160.0f, 0.0f, 0.0f));
    
    pmo->StretchModel(FLOAT3D(1.4f, 1.4f, 1.4f));
    _fFloorY = 0.0f;
    _bHasFloor = TRUE;

  } else if (strName=="Chainsaw Freak") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Enemies\\ChainsawFreak\\Freak.mdl"));
    pmo->PlayAnim(FREAK_ANIM_IDLE, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Enemies\\ChainsawFreak\\Freak.tex"));
    AddAttachment_t(pmo, FREAK_ATTACHMENT_CHAINSAW, 
      CTFILENAME("ModelsMP\\Enemies\\ChainsawFreak\\Saw.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\ChainsawFreak\\Saw.tex"));
    
    _plModel = CPlacement3D(FLOAT3D(-0.25f, -2.0f, -3.75f), ANGLE3D(200.0f, 0.0f, 0.0f));
    
    pmo->StretchModel(FLOAT3D(1.4f, 1.4f, 1.4f));
    _fFloorY = 0.0f;
    _bHasFloor = TRUE;

  } else if (strName=="Cannon Static") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Enemies\\CannonStatic\\Turret.mdl"));
    pmo->PlayAnim(TURRET_ANIM_DEFAULT, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Enemies\\CannonStatic\\Turret.tex"));
    AddAttachment_t(pmo, TURRET_ATTACHMENT_CANNON, 
      CTFILENAME("ModelsMP\\Enemies\\CannonStatic\\Cannon.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\CannonStatic\\Cannon.tex"));
    CAttachmentModelObject *pmoMuzzle = pmo->GetAttachmentModel(TURRET_ATTACHMENT_CANNON);
    pmoMuzzle->amo_plRelative.pl_OrientationAngle(2) = 10.0f;

    _plModel = CPlacement3D(FLOAT3D(0.4f, -1.0f, -2.75), ANGLE3D(125.0f, 0.0f, 0.0f));
    
    pmo->StretchModel(FLOAT3D(1.4f, 1.4f, 1.4f));
    _fFloorY = 0.0f;
    _bHasFloor = TRUE;

  } else if (strName=="Cannon Rotating") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Enemies\\CannonRotating\\Turret.mdl"));
    pmo->PlayAnim(TURRET_ANIM_DEFAULT, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Enemies\\CannonRotating\\Turret.tex"));
    AddAttachment_t(pmo, TURRET_ATTACHMENT_ROTATORHEADING, 
      CTFILENAME("ModelsMP\\Enemies\\CannonRotating\\RotatingMechanism.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\CannonRotating\\RotatingMechanism.tex"));
    CModelObject *pmoRotator = &pmo->GetAttachmentModel(TURRET_ATTACHMENT_ROTATORHEADING)->amo_moModelObject;
    AddAttachment_t(pmoRotator, ROTATINGMECHANISM_ATTACHMENT_CANNON, 
      CTFILENAME("ModelsMP\\Enemies\\CannonStatic\\Cannon.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\CannonStatic\\Cannon.tex"));
    CAttachmentModelObject *pmoMuzzle = pmoRotator->GetAttachmentModel(ROTATINGMECHANISM_ATTACHMENT_CANNON);
    pmoMuzzle->amo_plRelative.pl_OrientationAngle(2) = 10.0f;

    _plModel = CPlacement3D(FLOAT3D(0.4f, -1.0f, -2.75f), ANGLE3D(125.0f, 0.0f, 0.0f));
    
    pmo->StretchModel(FLOAT3D(1.4f, 1.4f, 1.4f));
    _fFloorY = 0.0f;
    _bHasFloor = TRUE;

  } else if (strName=="Summoner") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Enemies\\Summoner\\Summoner.mdl"));
    pmo->PlayAnim(SUMMONER_ANIM_IDLE, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Enemies\\Summoner\\Summoner.tex"));
    pmo->mo_toBump.SetData_t(CTFILENAME("TexturesMP\\Detail\\Crumples04.tex"));
    AddAttachment_t(pmo, SUMMONER_ATTACHMENT_STAFF, 
      CTFILENAME("ModelsMP\\Enemies\\Summoner\\Staff.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\Summoner\\Staff.tex"));
    
    _plModel = CPlacement3D(FLOAT3D(0.25f, -7.0f, -31.0f), ANGLE3D(160.0f, 0.0f, 0.0f));
    _fFOV = 50.0f;
    _vLightDir = FLOAT3D( 0.1f, -0.3f, -0.2f);

    pmo->StretchModel(FLOAT3D(7.0f, 7.0f, 7.0f));
    _fFloorY = 0.0f;
    _bHasFloor = TRUE;

  } else if (strName=="Exotech Larva") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\Body.mdl"));
    pmo->PlayAnim(BODY_ANIM_IDLECOMPUTER, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\Body.tex"));
    pmo->StretchModel(FLOAT3D(1.0f, 1.0f, 1.0f));
    CModelObject *pmoAtt;
    // left side
    AddAttachment_t(pmo, BODY_ATTACHMENT_ARM_LEFT, 
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\Arm.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\Arm.tex"));    
    pmoAtt = &pmo->GetAttachmentModel(BODY_ATTACHMENT_ARM_LEFT)->amo_moModelObject;
    AddAttachment_t(pmoAtt, ARM_ATTACHMENT_PLASMAGUN, 
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\Weapons\\PlasmaGun.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\Weapons\\PlasmaGun.tex"));    
    // right side
    AddAttachment_t(pmo, BODY_ATTACHMENT_ARM_RIGHT, 
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\Arm.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\Arm.tex"));    
    pmoAtt = &pmo->GetAttachmentModel(BODY_ATTACHMENT_ARM_RIGHT)->amo_moModelObject;
    pmoAtt->StretchModel(FLOAT3D(-1,1,1));
    AddAttachment_t(pmoAtt, ARM_ATTACHMENT_PLASMAGUN, 
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\Weapons\\PlasmaGun.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\Weapons\\PlasmaGun.tex"));         
    CModelObject *pmoAtt2;
    pmoAtt2 = &pmoAtt->GetAttachmentModel(ARM_ATTACHMENT_PLASMAGUN)->amo_moModelObject;
    pmoAtt2->StretchModel(FLOAT3D(-1,1,1));
    // blades
    AddAttachment_t(pmo, BODY_ATTACHMENT_BACKARMS, 
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\BackArms.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\Body.tex"));         
    CModelObject *pmoAtt3;
    pmoAtt3 = &pmo->GetAttachmentModel(BODY_ATTACHMENT_BACKARMS)->amo_moModelObject;
    pmoAtt3->PlayAnim(BACKARMS_ANIM_ACTIVE, AOF_LOOPING);
    // holder
    AddAttachment_t(pmo, BODY_ATTACHMENT_EXOTECHLARVA, 
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\ExotechLarva.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\ExotechLarva.tex"));
    pmoAtt = &pmo->GetAttachmentModel(BODY_ATTACHMENT_EXOTECHLARVA)->amo_moModelObject;
    AddAttachment_t(pmoAtt, EXOTECHLARVA_ATTACHMENT_BEAM, 
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\Beam.mdl"), 0,
      CTFILENAME("ModelsMP\\Effects\\Laser\\Laser.tex"));
    AddAttachment_t(pmoAtt, EXOTECHLARVA_ATTACHMENT_ENERGYBEAMS, 
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\EnergyBeams.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\EnergyBeams.tex"));
    AddAttachment_t(pmoAtt, EXOTECHLARVA_ATTACHMENT_FLARE, 
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\EffectFlare.mdl"), 0,
      CTFILENAME("ModelsMP\\Enemies\\ExotechLarva\\EffectFlare.tex"));
    
    _plModel = CPlacement3D(FLOAT3D(0.5f, -8.0f, -21.0f), ANGLE3D(165.0f, 0.0f, 0.0f));
    _fFOV = 70.0f;

    _aRotation = ANGLE3D( 10.0f, 0, 0 );
	  _vLightDir = FLOAT3D( -0.1f, -0.2f, -0.2f);
    
    pmo->StretchModelRelative(FLOAT3D(2.5f, 2.5f, 2.5f));
    //pmo->StretchModel(FLOAT3D(2.5f, 2.5f, 2.5f));
    _fFloorY = -2.0f;
    _bHasFloor = TRUE;

  } else if (strName=="Air Elemental") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Enemies\\AirElemental\\Elemental.mdl"));
    pmo->PlayAnim(ELEMENTAL_ANIM_IDLE, AOF_LOOPING);
    pmo->mo_colBlendColor = 0;
    
    _plModel = CPlacement3D(FLOAT3D(-1.0f, -13.0f, -52.0f), ANGLE3D(170.0f, 0.0f, 0.0f));
    _fFOV = 50.0f;

    pmo->StretchModel(FLOAT3D(14.0f, 14.0f, 14.0f));
    _fFloorY = -2.0f;
    _bHasFloor = TRUE;

    _iParticleType = PARTICLES_AIR_ELEMENTAL;

  } else if (strName=="BeastHuge") {
    pmo->SetData_t(CTFILENAME("Models\\Enemies\\Beast\\Beast.mdl"));
    pmo->PlayAnim(BEAST_ANIM_IDLECOMPUTER, AOF_LOOPING);
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Enemies\\Beast\\BeastBiggest.tex"));
    
    _plModel = CPlacement3D(FLOAT3D(-0.5f, -12.0f, -30.0f), ANGLE3D(170.0f, 0.0f, 0.0f));
    _vLightDir = FLOAT3D( -0.1f, -0.3f, -0.2f);

    pmo->StretchModel(FLOAT3D(11.0f, 11.0f, 11.0f));
    _fFloorY = 0.0f;
    _bHasFloor = TRUE;

  } else if (strName=="Knife") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\Knife\\KnifeForPlayer.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\Knife\\KnifeItem.tex"));
    //pmo->PlayAnim(KNIFEITEM_ANIM_COMPUTER, AOF_LOOPING);
    _plModel = CPlacement3D(FLOAT3D(0,-0.5f,-2.0), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    pmo->StretchModel(FLOAT3D(4,4,4));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

  } else if (strName=="Colt") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\Colt\\ColtItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\Colt\\ColtMain.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.5f,-2.0), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, COLTITEM_ATTACHMENT_BULLETS, 
      CTFILENAME("Models\\Weapons\\Colt\\ColtBullets.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Colt\\ColtBullets.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightBlueMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, COLTITEM_ATTACHMENT_COCK, 
      CTFILENAME("Models\\Weapons\\Colt\\ColtCock.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Colt\\ColtCock.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightBlueMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, COLTITEM_ATTACHMENT_BODY, 
      CTFILENAME("Models\\Weapons\\Colt\\ColtMain.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Colt\\ColtMain.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightBlueMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->StretchModel(FLOAT3D(4,4,4));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

  } else if (strName=="SingleShotgun") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\SingleShotgun\\SingleShotgunItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\SingleShotgun\\Handle.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.5f,-3.0), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, SINGLESHOTGUNITEM_ATTACHMENT_BARRELS, 
      CTFILENAME("Models\\Weapons\\SingleShotgun\\Barrels.mdl"), 0,
      CTFILENAME("Models\\Weapons\\SingleShotgun\\Barrels.tex"),
      CTFILENAME("Models\\ReflectionTextures\\DarkMetal.tex"),
      CTFILENAME("Models\\SpecularTextures\\Weak.tex"));
    AddAttachment_t(pmo, SINGLESHOTGUNITEM_ATTACHMENT_HANDLE, 
      CTFILENAME("Models\\Weapons\\SingleShotgun\\Handle.mdl"), 0,
      CTFILENAME("Models\\Weapons\\SingleShotgun\\Handle.tex"),
      CTFILENAME("Models\\ReflectionTextures\\DarkMetal.tex"),
      CTFILENAME("Models\\SpecularTextures\\Weak.tex"));
    AddAttachment_t(pmo, SINGLESHOTGUNITEM_ATTACHMENT_SLIDER, 
      CTFILENAME("Models\\Weapons\\SingleShotgun\\Slider.mdl"), 0,
      CTFILENAME("Models\\Weapons\\SingleShotgun\\Barrels.tex"),
      CTFILENAME("Models\\ReflectionTextures\\DarkMetal.tex"),
      CTFILENAME("Models\\SpecularTextures\\Weak.tex"));
    pmo->StretchModel(FLOAT3D(3.5,3.5,3.5));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

  } else if (strName=="DoubleShotgun") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\DoubleShotgun\\DoubleShotgunItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\DoubleShotgun\\Handle.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.5f,-4.0), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, DOUBLESHOTGUNITEM_ATTACHMENT_BARRELS, 
      CTFILENAME("Models\\Weapons\\DoubleShotgun\\DshotgunBarrels.mdl"), 0,
      CTFILENAME("Models\\Weapons\\DoubleShotgun\\Barrels.tex"),
      CTFILENAME("Models\\ReflectionTextures\\BWRiples01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, DOUBLESHOTGUNITEM_ATTACHMENT_HANDLE, 
      CTFILENAME("Models\\Weapons\\DoubleShotgun\\DshotgunHandle.mdl"), 0,
      CTFILENAME("Models\\Weapons\\DoubleShotgun\\Handle.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, DOUBLESHOTGUNITEM_ATTACHMENT_SWITCH, 
      CTFILENAME("Models\\Weapons\\DoubleShotgun\\Switch.mdl"), 0,
      CTFILENAME("Models\\Weapons\\DoubleShotgun\\Switch.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->StretchModel(FLOAT3D(3.0,3.0,3.0));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

  } else if (strName=="Tommygun") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\Tommygun\\TommygunItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\Tommygun\\Body.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.4f,-1.8f), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, TOMMYGUNITEM_ATTACHMENT_BODY, 
      CTFILENAME("Models\\Weapons\\Tommygun\\Body.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Tommygun\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, TOMMYGUNITEM_ATTACHMENT_SLIDER, 
      CTFILENAME("Models\\Weapons\\Tommygun\\Slider.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Tommygun\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->StretchModel(FLOAT3D(2.1,2.1,2.1));
    _bHasFloor = TRUE;
    _fFloorY = -0.5f;

  } else if (strName=="Sniper") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Weapons\\Sniper\\Sniper.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Weapons\\Sniper\\Body.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.4f,-4.0f), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);
    _fFOV = 50.0f;
    _vLightDir = FLOAT3D( -0.1f, -0.2f, -0.2f);  

    AddAttachment_t(pmo, SNIPERITEM_ATTACHMENT_BODY, 
      CTFILENAME("ModelsMP\\Weapons\\Sniper\\Body.mdl"), 0,
      CTFILENAME("ModelsMP\\Weapons\\Sniper\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->StretchModel(FLOAT3D(1.5,1.5,1.5));
    _bHasFloor = TRUE;
    _fFloorY = -0.5f;

  } else if (strName=="ChainSaw") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Weapons\\Chainsaw\\ChainsawItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Weapons\\Chainsaw\\Body.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.4f,-3.0f), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);
    _fFOV = 60.0f;
    _vLightDir = FLOAT3D( -0.1f, -0.2f, -0.2f);  

    AddAttachment_t(pmo, CHAINSAWITEM_ATTACHMENT_CHAINSAW, 
      CTFILENAME("ModelsMP\\Weapons\\Chainsaw\\BodyForPlayer.mdl"), 0,
      CTFILENAME("ModelsMP\\Weapons\\Chainsaw\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, CHAINSAWITEM_ATTACHMENT_BLADE, 
      CTFILENAME("ModelsMP\\Weapons\\Chainsaw\\Blade.mdl"), 0,
      CTFILENAME("ModelsMP\\Weapons\\Chainsaw\\Blade.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    //CAttachmentModelObject *amo = pmo->GetAttachmentModel(CHAINSAWITEM_ATTACHMENT_BLADE);
    AddAttachment_t(pmo, BLADEFORPLAYER_ATTACHMENT_TEETH, 
      CTFILENAME("ModelsMP\\Weapons\\Chainsaw\\Teeth.mdl"), 0,
      CTFILENAME("ModelsMP\\Weapons\\Chainsaw\\Teeth.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    
    pmo->StretchModel(FLOAT3D(1.0,1.0,1.0));
    _bHasFloor = TRUE;
    _fFloorY = -0.5f;

  }  else if (strName=="Flamer") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\Flamer\\FlamerItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\Flamer\\Body.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.4f,-2.2f), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);
    _fFOV = 70.0f;
    _vLightDir = FLOAT3D( -0.1f, -0.2f, -0.2f);  

    AddAttachment_t(pmo, FLAMERITEM_ATTACHMENT_BODY, 
      CTFILENAME("Models\\Weapons\\Flamer\\Body.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Flamer\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, FLAMERITEM_ATTACHMENT_FUEL, 
      CTFILENAME("Models\\Weapons\\Flamer\\FuelReservoir.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Flamer\\FuelReservoir.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->StretchModel(FLOAT3D(1.8,1.8,1.8));
    _bHasFloor = TRUE;
    _fFloorY = -0.5f;

  }  else if (strName=="serious bomb") {
    pmo->SetData_t(CTFILENAME("ModelsMP\\Items\\PowerUps\\SeriousBomb\\SeriousBomb.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("ModelsMP\\Items\\PowerUps\\SeriousBomb\\SeriousBomb.tex"));
    
    _plModel = CPlacement3D(FLOAT3D(0.0f, -1.0f, -6.0f), ANGLE3D(0,-10.0f,0));
    _aRotation = ANGLE3D(60,0,0);
    _fFOV = 40.0f;
    /*_plModel = CPlacement3D(FLOAT3D(tmp_af[5],tmp_af[6],tmp_af[7]), ANGLE3D(0,tmp_af[8],0));
    _fFOV = tmp_af[9];
    CPrintF("%f %f %f : %f : %f\n", tmp_af[5],tmp_af[6],tmp_af[7], tmp_af[8], tmp_af[9]);*/

    pmo->StretchModel(FLOAT3D(3.0f, 3.0f, 3.0f));
    _bHasFloor = TRUE;
    _fFloorY = 0.0f;

  } else if (strName=="Minigun") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\Minigun\\MinigunItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\Minigun\\Minigun.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.5f,-3.75f), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, MINIGUNITEM_ATTACHMENT_BARRELS, 
      CTFILENAME("Models\\Weapons\\Minigun\\Barrels.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Minigun\\Barrels.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, MINIGUNITEM_ATTACHMENT_BODY, 
      CTFILENAME("Models\\Weapons\\Minigun\\Body.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Minigun\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, MINIGUNITEM_ATTACHMENT_ENGINE, 
      CTFILENAME("Models\\Weapons\\Minigun\\Engine.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Minigun\\Barrels.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->StretchModel(FLOAT3D(2.0,2.0,2.0));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

  } else if (strName=="RocketLauncher") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\RocketLauncher\\RocketLauncherItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\RocketLauncher\\Body.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.0f,-3.0), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, ROCKETLAUNCHERITEM_ATTACHMENT_BODY, 
      CTFILENAME("Models\\Weapons\\RocketLauncher\\Body.mdl"), 0,
      CTFILENAME("Models\\Weapons\\RocketLauncher\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, ROCKETLAUNCHERITEM_ATTACHMENT_ROTATINGPART, 
      CTFILENAME("Models\\Weapons\\RocketLauncher\\RotatingPart.mdl"), 0,
      CTFILENAME("Models\\Weapons\\RocketLauncher\\RotatingPart.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, ROCKETLAUNCHERITEM_ATTACHMENT_ROCKET1, 
      CTFILENAME("Models\\Weapons\\RocketLauncher\\Projectile\\Rocket.mdl"), 0,
      CTFILENAME("Models\\Weapons\\RocketLauncher\\Projectile\\Rocket.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, ROCKETLAUNCHERITEM_ATTACHMENT_ROCKET2, 
      CTFILENAME("Models\\Weapons\\RocketLauncher\\Projectile\\Rocket.mdl"), 0,
      CTFILENAME("Models\\Weapons\\RocketLauncher\\Projectile\\Rocket.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, ROCKETLAUNCHERITEM_ATTACHMENT_ROCKET3, 
      CTFILENAME("Models\\Weapons\\RocketLauncher\\Projectile\\Rocket.mdl"), 0,
      CTFILENAME("Models\\Weapons\\RocketLauncher\\Projectile\\Rocket.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, ROCKETLAUNCHERITEM_ATTACHMENT_ROCKET4, 
      CTFILENAME("Models\\Weapons\\RocketLauncher\\Projectile\\Rocket.mdl"), 0,
      CTFILENAME("Models\\Weapons\\RocketLauncher\\Projectile\\Rocket.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
      pmo->StretchModel(FLOAT3D(1.8,1.8,1.8));

     //pmo->StretchModel(FLOAT3D(3.0,3.0,3.0));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

  } else if (strName=="GrenadeLauncher") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\GrenadeLauncher\\GrenadeLauncherItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\GrenadeLauncher\\Body.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.0f,-4.0), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, GRENADELAUNCHERITEM_ATTACHMENT_MAIN_PART, 
      CTFILENAME("Models\\Weapons\\GrenadeLauncher\\MainPart.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GrenadeLauncher\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, GRENADELAUNCHERITEM_ATTACHMENT_SECOND_PART, 
      CTFILENAME("Models\\Weapons\\GrenadeLauncher\\SecondPart.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GrenadeLauncher\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, GRENADELAUNCHERITEM_ATTACHMENT_THRID_PART, 
      CTFILENAME("Models\\Weapons\\GrenadeLauncher\\ThridPart.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GrenadeLauncher\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
	AddAttachment_t(pmo, GRENADELAUNCHERITEM_ATTACHMENT_LAST_PART, 
      CTFILENAME("Models\\Weapons\\GrenadeLauncher\\LastPart.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GrenadeLauncher\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->StretchModel(FLOAT3D(2.5,2.5,2.5));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

  } else if (strName=="Laser") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\Laser\\LaserItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\Laser\\Body.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.5f,-3.0), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, LASERITEM_ATTACHMENT_BODY, 
      CTFILENAME("Models\\Weapons\\Laser\\Body.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Laser\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, LASERITEM_ATTACHMENT_LEFTUP, 
      CTFILENAME("Models\\Weapons\\Laser\\Barrel.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Laser\\Barrel.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, LASERITEM_ATTACHMENT_LEFTDOWN, 
      CTFILENAME("Models\\Weapons\\Laser\\Barrel.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Laser\\Barrel.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, LASERITEM_ATTACHMENT_RIGHTUP, 
      CTFILENAME("Models\\Weapons\\Laser\\Barrel.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Laser\\Barrel.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, LASERITEM_ATTACHMENT_RIGHTDOWN, 
      CTFILENAME("Models\\Weapons\\Laser\\Barrel.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Laser\\Barrel.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->StretchModel(FLOAT3D(2.5,2.5,2.5));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

  } else if (strName=="Cannon") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\Cannon\\Cannon.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\Cannon\\Body.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.8f,-3.0), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, CANNON_ATTACHMENT_BODY, 
      CTFILENAME("Models\\Weapons\\Cannon\\Body.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Cannon\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
	AddAttachment_t(pmo, CANNON_ATTACHMENT_FUSE, 
      CTFILENAME("Models\\Weapons\\Cannon\\Fuse.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Cannon\\Fuse.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->StretchModel(FLOAT3D(2.5,2.5,2.5));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

	} else if (strName=="GhostBuster") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\GhostBuster\\GhostBusterItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\GhostBuster\\Body.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.5f,-3.0), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, GHOSTBUSTERITEM_ATTACHMENT_BODY, 
      CTFILENAME("Models\\Weapons\\GhostBuster\\Body.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GhostBuster\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, GHOSTBUSTERITEM_ATTACHMENT_ROTATOR, 
      CTFILENAME("Models\\Weapons\\GhostBuster\\Rotator.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GhostBuster\\Rotator.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, GHOSTBUSTERITEM_ATTACHMENT_EFFECT01, 
      CTFILENAME("Models\\Weapons\\GhostBuster\\Effect01.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GhostBuster\\Lightning.tex"));
    AddAttachment_t(pmo, GHOSTBUSTERITEM_ATTACHMENT_EFFECT02, 
      CTFILENAME("Models\\Weapons\\GhostBuster\\Effect01.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GhostBuster\\Lightning.tex"));
    AddAttachment_t(pmo, GHOSTBUSTERITEM_ATTACHMENT_EFFECT03, 
      CTFILENAME("Models\\Weapons\\GhostBuster\\Effect01.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GhostBuster\\Lightning.tex"));
    AddAttachment_t(pmo, GHOSTBUSTERITEM_ATTACHMENT_EFFECT04, 
      CTFILENAME("Models\\Weapons\\GhostBuster\\Effect01.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GhostBuster\\Lightning.tex"));
//	
	CModelObject *pmoAtt;
	pmoAtt = &(pmo->GetAttachmentModel(GHOSTBUSTERITEM_ATTACHMENT_EFFECT01)->amo_moModelObject);
    AddAttachment_t(pmoAtt, EFFECT01_ATTACHMENT_FLARE, 
      CTFILENAME("Models\\Weapons\\GhostBuster\\EffectFlare01.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GhostBuster\\EffectFlare.tex"));
	pmoAtt = &(pmo->GetAttachmentModel(GHOSTBUSTERITEM_ATTACHMENT_EFFECT02)->amo_moModelObject);
    AddAttachment_t(pmoAtt, EFFECT01_ATTACHMENT_FLARE,
      CTFILENAME("Models\\Weapons\\GhostBuster\\EffectFlare01.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GhostBuster\\EffectFlare.tex"));
	pmoAtt = &(pmo->GetAttachmentModel(GHOSTBUSTERITEM_ATTACHMENT_EFFECT03)->amo_moModelObject);
    AddAttachment_t(pmoAtt, EFFECT01_ATTACHMENT_FLARE,
      CTFILENAME("Models\\Weapons\\GhostBuster\\EffectFlare01.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GhostBuster\\EffectFlare.tex"));
	pmoAtt = &(pmo->GetAttachmentModel(GHOSTBUSTERITEM_ATTACHMENT_EFFECT04)->amo_moModelObject);
    AddAttachment_t(pmoAtt, EFFECT01_ATTACHMENT_FLARE,
      CTFILENAME("Models\\Weapons\\GhostBuster\\EffectFlare01.mdl"), 0,
      CTFILENAME("Models\\Weapons\\GhostBuster\\EffectFlare.tex"));
//	  
    pmo->StretchModel(FLOAT3D(2.5,2.5,2.5));
    //pmo->StretchModel(FLOAT3D(3.0,3.0,3.0));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

	} else if (strName=="PipeBomb") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\PipeBomb\\StickItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\PipeBomb\\Stick.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.8f,-3.0), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, STICKITEM_ATTACHMENT_STICK, 
      CTFILENAME("Models\\Weapons\\PipeBomb\\Stick.mdl"), 0,
      CTFILENAME("Models\\Weapons\\PipeBomb\\Stick.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, STICKITEM_ATTACHMENT_SHIELD, 
      CTFILENAME("Models\\Weapons\\PipeBomb\\Shield.mdl"), 0,
      CTFILENAME("Models\\Weapons\\PipeBomb\\Stick.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
	AddAttachment_t(pmo, STICKITEM_ATTACHMENT_BUTTON, 
      CTFILENAME("Models\\Weapons\\PipeBomb\\Button.mdl"), 0,
      CTFILENAME("Models\\Weapons\\PipeBomb\\Stick.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->StretchModel(FLOAT3D(4.5,4.5,4.5));
	_fFOV = 60;
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

} else if (strName=="Flamer") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\Flamer\\FlamerItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\Flamer\\Body.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-1.0f,-3.0), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, FLAMERITEM_ATTACHMENT_BODY, 
      CTFILENAME("Models\\Weapons\\Flamer\\Body.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Flamer\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, FLAMERITEM_ATTACHMENT_FUEL, 
      CTFILENAME("Models\\Weapons\\Flamer\\FuelReservoir.mdl"), 0,
      CTFILENAME("Models\\Weapons\\Flamer\\FuelReservoir.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    pmo->StretchModel(FLOAT3D(2.7,2.7,2.7));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

	} else if (strName=="LavaRocksGun") {
    pmo->SetData_t(CTFILENAME("Models\\Weapons\\LRG\\LRGItem.mdl"));
    pmo->mo_toTexture.SetData_t(CTFILENAME("Models\\Weapons\\LRG\\Body.tex"));
    _plModel = CPlacement3D(FLOAT3D(0,-0.3f,-3.0), ANGLE3D(0,10,0));
    _aRotation = ANGLE3D(100,0,0);

    AddAttachment_t(pmo, LRGITEM_ATTACHMENT_BODY, 
      CTFILENAME("Models\\Weapons\\LRG\\Body.mdl"), 0,
      CTFILENAME("Models\\Weapons\\LRG\\Body.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    AddAttachment_t(pmo, LRGITEM_ATTACHMENT_BARREL, 
      CTFILENAME("Models\\Weapons\\LRG\\Barrel.mdl"), 0,
      CTFILENAME("Models\\Weapons\\LRG\\Barrel.tex"),
      CTFILENAME("Models\\ReflectionTextures\\LightMetal01.tex"),
      CTFILENAME("Models\\SpecularTextures\\Medium.tex"));
    //pmo->StretchModel(FLOAT3D(1.9,1.9,1.9));
    pmo->StretchModel(FLOAT3D(2.5,2.5,2.5));
    _bHasFloor = TRUE;
    _fFloorY = -1.0f;

  } else {
    ThrowF_t(TRANS("Unknown model '%s'"), (const char *) strName);
  }
}

void RenderMessageModel(CDrawPort *pdp, const CTString &strModel)
{
  // if new model
  if (_strLastModel!=strModel) {
    _strLastModel=strModel;
    _bModelOK = FALSE;
    // try to
    try {
      // load model
      SetupCompModel_t(strModel);
      _bModelOK = TRUE;
    // if failed
    } catch (const char *strError) {
      // report error
      CPrintF("Cannot setup model '%s':\n%s\n", (const char *) strModel, (const char *)strError);
      // do nothing
      return;
    }
  }

  // if model is not loaded ok
  if (!_bModelOK) {
    // do nothing
    return;
  }

  // for each eye
  for (INDEX iEye=STEREO_LEFT; iEye<=(Stereo_IsEnabled()?STEREO_RIGHT:STEREO_LEFT); iEye++) {
    // prepare projection
    CRenderModel rm;
    CPerspectiveProjection3D pr;
    pr.FOVL() = AngleDeg(_fFOV);
    pr.ScreenBBoxL() = FLOATaabbox2D(
      FLOAT2D(0.0f, 0.0f),
      FLOAT2D((float)pdp->GetWidth(), (float)pdp->GetHeight())
    );
    pr.AspectRatioL() = 1.0f;
    pr.FrontClipDistanceL() = 0.3f;
    pr.ViewerPlacementL() = CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0));
  
    // setup stereo rendering
    Stereo_SetBuffer(iEye);
    Stereo_AdjustProjection(pr, iEye, 0.16f);

    pdp->FillZBuffer(1.0f);

    // initialize rendering
    CAnyProjection3D apr;
    apr = pr;
    BeginModelRenderingView(apr, pdp);
    rm.rm_vLightDirection = _vLightDir;
    const FLOAT fDistance = 1.0f+ 10.f*(1.0f/(_fMsgAppearFade+0.01f) - 1.0f/(1.0f+0.01f));

    // if model needs floor
    if( _bHasFloor) {
      // set floor's position
      CPlacement3D pl = _plModel;
      pl.pl_OrientationAngle = ANGLE3D(0,0,0);
      pl.pl_PositionVector   = _plModel.pl_PositionVector;
      pl.pl_PositionVector(2) += _fFloorY;
      pl.pl_PositionVector(3) *= fDistance;
      rm.SetObjectPlacement(pl);
      // render the floor
    rm.rm_colLight   = C_YELLOW;
    rm.rm_colAmbient = C_YELLOW;
      rm.rm_fDistanceFactor = -999;
      _moFloor.SetupModelRendering(rm);
      _moFloor.RenderModel(rm);
    }

    // set model's position
    CPlacement3D pl;
    pl.pl_OrientationAngle   = _plModel.pl_OrientationAngle + _aRotation*_pTimer->GetLerpedCurrentTick();
    pl.pl_PositionVector     = _plModel.pl_PositionVector;
    pl.pl_PositionVector(3) *= fDistance / pdp->dp_fWideAdjustment;
    rm.SetObjectPlacement(pl);

    // render the model
    rm.rm_colLight   = _colLight;
    rm.rm_colAmbient = _colAmbient;
    rm.rm_fDistanceFactor = -999; // force highest mip disregarding stretch factors
    _moModel.SetupModelRendering(rm);
    FLOATplane3D plFloorPlane = FLOATplane3D( FLOAT3D( 0.0f, 1.0f, 0.0f), _plModel.pl_PositionVector(2)+_fFloorY);
    CPlacement3D plLightPlacement = CPlacement3D( _plModel.pl_PositionVector
                                  + rm.rm_vLightDirection * _plModel.pl_PositionVector(3) *5, ANGLE3D(0,0,0));
    _moModel.RenderShadow( rm, plLightPlacement, 200.0f, 200.0f, 1.0f, plFloorPlane);
    _moModel.RenderModel(rm);

    // render particles
#ifndef FIRST_ENCOUNTER
    if (_iParticleType!=PARTICLES_NONE) {
      Particle_PrepareSystem(pdp, apr);
      Particle_PrepareEntity( 1, 0, 0, NULL);
      switch(_iParticleType) {
      case PARTICLES_AIR_ELEMENTAL:
        Particles_AirElemental_Comp(&_moModel, 1.0f, 1.0f, pl);
        break;
      case PARTICLES_LAVA_ELEMENTAL:
        Particles_Burning_Comp(&_moModel, 0.25f, pl);
        break;
      }
      Particle_EndSystem();
    }
#endif
    EndModelRenderingView();
  }
  Stereo_SetBuffer(STEREO_BOTH);
}
