301
%{
#include "Entities/StdH/StdH.h"
#include "Models/Enemies/Catman/CatMan.h"
%}

uses "Entities/EnemyBase";
uses "Entities/Projectile";

enum CatmanType {
  0 CMT_SOLDIER    "Soldier",
  1 CMT_GENERAL    "General",
  2 CMT_ROGUE      "Rogue" 
};


%{
// info structure
static EntityInfo eiCatman = {
  EIBT_FLESH, 140.0f,
  0.0f, 2.0f, 0.0f,
  0.0f, 1.5f, 0.0f,
};
%}


class CCatman : CEnemyBase {
name      "Catman";
thumbnail "Thumbnails\\Catman.tbn";

properties:
  1 enum CatmanType m_cmtType "Type" 'T' = CMT_SOLDIER,
  
components:
  0 class   CLASS_BASE        "Classes\\EnemyBase.ecl",
  1 model   MODEL_CATMAN      "Models\\Enemies\\Catman\\Catman.mdl",
  2 texture TEXTURE_SOLDIER   "Models\\Enemies\\Catman\\Catman03.tex",
 12 model   MODEL_CATMAN_P     "Models\\Enemies\\Catman\\Projectile\\Projectile.mdl",
 13 texture TEXTURE_CATMAN_P   "Models\\Enemies\\Catman\\Projectile\\Projectile.tex",
 
// ************** SOUNDS **************
 50 sound   SOUND_IDLE      "Models\\Enemies\\Catman\\Sounds\\Idle.wav",
 51 sound   SOUND_SIGHT     "Models\\Enemies\\Catman\\Sounds\\Sight.wav",
 52 sound   SOUND_WOUND     "Models\\Enemies\\Catman\\Sounds\\Wound.wav",
 53 sound   SOUND_FIRE      "Models\\Enemies\\Catman\\Sounds\\Fire.wav",
 54 sound   SOUND_KICK      "Models\\Enemies\\Catman\\Sounds\\Kick.wav",
 55 sound   SOUND_DEATH     "Models\\Enemies\\Catman\\Sounds\\Death.wav",

functions:
  // describe how this enemy killed player
  virtual CTString GetPlayerKillDescription(const CTString &strPlayerName, const EDeath &eDeath)
  {
    CTString str;
    str.PrintF(TRANS("%s was killed by a Catman"), (const char *)strPlayerName);
    return str;
  }

  /* Fill in entity statistics - for AI purposes only */
  BOOL FillEntityStatistics(EntityStats *pes)
  {
    CEnemyBase::FillEntityStatistics(pes);
    switch(m_cmtType) {
    case CMT_ROGUE:   { pes->es_strName+=" Rogue"; pes->es_iScore = 3000; } break;
    case CMT_GENERAL: { pes->es_strName+=" General"; pes->es_iScore = 2000; } break;
    case CMT_SOLDIER: { pes->es_strName+=" Soldier"; pes->es_iScore = 1000; } break;
    }
    return TRUE;
  }

  virtual const CTFileName &GetComputerMessageName(void) const {
    static DECLARE_CTFILENAME(fnmRogue, "Data\\Messages\\Enemies\\CatmanRogue.txt");
    static DECLARE_CTFILENAME(fnmGeneral, "Data\\Messages\\Enemies\\CatmanGeneral.txt");
    static DECLARE_CTFILENAME(fnmSoldier, "Data\\Messages\\Enemies\\CatmanSoldier.txt");
    switch(m_cmtType) {
    default: ASSERT(FALSE);
    case CMT_ROGUE: return fnmRogue;
    case CMT_GENERAL: return fnmGeneral;
    case CMT_SOLDIER: return fnmSoldier;
    }
  };

  void Precache(void) {
    CEnemyBase::Precache();
	
    PrecacheModel(MODEL_CATMAN_P);
	PrecacheTexture(TEXTURE_CATMAN_P);
    PrecacheModel(MODEL_CATMAN);
    PrecacheTexture(TEXTURE_SOLDIER);
    
	PrecacheSound(SOUND_IDLE);
    PrecacheSound(SOUND_SIGHT);
    PrecacheSound(SOUND_WOUND);
    PrecacheSound(SOUND_FIRE);
    PrecacheSound(SOUND_KICK);
    PrecacheSound(SOUND_DEATH);
  };
  /* Entity info */
  void *GetEntityInfo(void) {
    return &eiCatman;
  };

  /* Receive damage */
  void ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType,
    FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection) 
  {
    // catman can't harm catman
    if (!IsOfClass(penInflictor, "Catman")) {
      CEnemyBase::ReceiveDamage(penInflictor, dmtType, fDamageAmmount, vHitPoint, vDirection);
    }
  };


  // damage anim
  INDEX AnimForDamage(FLOAT fDamage) {
    INDEX iAnim;
    switch (IRnd()%3) {
      case 0: iAnim = CATMAN_ANIM_WOUND01; break;
      case 1: iAnim = CATMAN_ANIM_WOUND02; break;
      case 2: iAnim = CATMAN_ANIM_WOUND03; break;
      default: ASSERTALWAYS("Catman unknown damage");
    }
    StartModelAnim(iAnim, 0);
    return iAnim;
  };

  // death
  INDEX AnimForDeath(void) {
    INDEX iAnim;
    switch (IRnd()%2) {
      case 0: iAnim = CATMAN_ANIM_DEATH01; break;
      case 1: iAnim = CATMAN_ANIM_DEATH02; break;
      default: ASSERTALWAYS("Catman unknown death");
    }
    StartModelAnim(iAnim, 0);
    return iAnim;
  };

  void DeathNotify(void) {
    ChangeCollisionBoxIndexWhenPossible(CATMAN_COLLISION_BOX_DEATH);
    en_fDensity = 500.0f;
  };

  // virtual anim functions
  void StandingAnim(void) {
    StartModelAnim(CATMAN_ANIM_STAND, AOF_LOOPING|AOF_NORESTART);
  };
  void WalkingAnim(void) {
    StartModelAnim(CATMAN_ANIM_WALK, AOF_LOOPING|AOF_NORESTART);
  };
  void RunningAnim(void) {
    StartModelAnim(CATMAN_ANIM_RUN, AOF_LOOPING|AOF_NORESTART);
  };
  void RotatingAnim(void) {
    StartModelAnim(CATMAN_ANIM_WALK, AOF_LOOPING|AOF_NORESTART);
  };

  // virtual sound functions
  void IdleSound(void) {
    PlaySound(m_soSound, SOUND_IDLE, SOF_3D);
  };
  void SightSound(void) {
    PlaySound(m_soSound, SOUND_SIGHT, SOF_3D);
  };
  void WoundSound(void) {
    PlaySound(m_soSound, SOUND_WOUND, SOF_3D);
  };
  void DeathSound(void) {
    PlaySound(m_soSound, SOUND_DEATH, SOF_3D);
  };


procedures:
/************************************************************
 *                A T T A C K   E N E M Y                   *
 ************************************************************/
  Fire(EVoid) : CEnemyBase::Fire {
    // wait for a while
    StandingAnim();
    autowait(0.2f + FRnd()/4);

    // fire projectile
    StartModelAnim(CATMAN_ANIM_ATTACK02, 0);
    ShootProjectile(PRT_CATMAN_FIRE, FLOAT3D(0.0f, 1.5f, 0.5f), ANGLE3D(0, 0, 0));
    PlaySound(m_soSound, SOUND_FIRE, SOF_3D);
    autowait(FRnd()/3+0.6f);

    return EReturn();
  };



/************************************************************
 *                       M  A  I  N                         *
 ************************************************************/
  Main(EVoid) {
    // declare yourself as a model
    InitAsModel();
    SetPhysicsFlags(EPF_MODEL_WALKING|EPF_HASLUNGS);
    SetCollisionFlags(ECF_MODEL);
    SetFlags(GetFlags()|ENF_ALIVE);
    en_tmMaxHoldBreath = 5.0f;
    en_fDensity = 2000.0f;

    //m_iScore = 300;
    // set your appearance
    SetModel(MODEL_CATMAN);
    switch (m_cmtType) {
      case CMT_SOLDIER:
        SetHealth(100.0f);
        m_fMaxHealth = 100.0f;
        SetModelMainTexture(TEXTURE_SOLDIER);
        StandingAnim();
        // setup moving speed
        m_fWalkSpeed = FRnd() + 1.5f;
        m_aWalkRotateSpeed = AngleDeg(FRnd()*10.0f + 25.0f);
        m_fAttackRunSpeed = FRnd()*2.0f + 4.0f;
        m_aAttackRotateSpeed = AngleDeg(FRnd()*50 + 245.0f);
        m_fCloseRunSpeed = FRnd()*2.0f + 4.0f;
        m_aCloseRotateSpeed = AngleDeg(FRnd()*50 + 245.0f);
        // setup attack distances
        m_fAttackDistance = 40.0f;
        m_fCloseDistance = 0.0f;
        m_fStopDistance = 10.0f;
        m_fAttackFireTime = 3.0f;
        m_fCloseFireTime = 1.0f;
        m_fIgnoreRange = 200.0f;
        // damage/explode properties
        m_fBlowUpAmount = 100.0f;
        m_fBodyParts = 4;
        m_fDamageWounded = 20.0f;
        m_iScore = 1000;
	    break;

      case CMT_GENERAL:
        SetHealth(200.0f);
        m_fMaxHealth = 200.0f;
        SetModelMainTexture(TEXTURE_SOLDIER);
        StandingAnim();
        // setup moving speed
        m_fWalkSpeed = FRnd() + 1.5f;
        m_aWalkRotateSpeed = AngleDeg(FRnd()*10.0f + 25.0f);
        m_fAttackRunSpeed = FRnd()*2.0f + 4.0f;
        m_aAttackRotateSpeed = AngleDeg(FRnd()*50 + 245.0f);
        m_fCloseRunSpeed = FRnd()*2.0f + 4.0f;
        m_aCloseRotateSpeed = AngleDeg(FRnd()*50 + 245.0f);
        // setup attack distances
        m_fAttackDistance = 60.0f;
        m_fCloseDistance = 0.0f;
        m_fStopDistance = 10.0f;
        m_fAttackFireTime = 3.0f;
        m_fCloseFireTime = 1.0f;
        m_fIgnoreRange = 200.0f;
        // damage/explode properties
        m_fBlowUpAmount = 100.0f;
        m_fBodyParts = 4;
        m_fDamageWounded = 40.0f;
        m_iScore = 2000;
	    break;

      case CMT_ROGUE:
        SetModelMainTexture(TEXTURE_SOLDIER);
        StandingAnim();
        SetHealth(300.0f);
        m_fMaxHealth = 300.0f;
        // setup moving speed
        m_fWalkSpeed = FRnd() + 1.5f;
        m_aWalkRotateSpeed = AngleDeg(FRnd()*10.0f + 25.0f);
        m_fAttackRunSpeed = FRnd()*2.0f + 4.0f;
        m_aAttackRotateSpeed = AngleDeg(FRnd()*50 + 245.0f);
        m_fCloseRunSpeed = FRnd()*2.0f + 4.0f;
        m_aCloseRotateSpeed = AngleDeg(FRnd()*50 + 245.0f);
        // setup attack distances
        m_fAttackDistance = 70.0f;
        m_fCloseDistance = 0.0f;
        m_fStopDistance = 10.0f;
        m_fAttackFireTime = 3.0f;
        m_fCloseFireTime = 1.0f;
        m_fIgnoreRange = 200.0f;
        // damage/explode properties
        m_fBlowUpAmount = 100.0f;
        m_fBodyParts = 4;
        m_fDamageWounded = 60.0f;
        m_iScore = 3000;
        break;
    }
    // continue behavior in base class
    jump CEnemyBase::MainLoop();
  };
};
