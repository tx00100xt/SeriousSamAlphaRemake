329
%{
#include "Entities/StdH/StdH.h"
#include "Models/Enemies/MANTAMAN/mantaman.h"
%}

uses "Entities/EnemyDive";

%{
static EntityInfo eiMantamanGround = {
  EIBT_FLESH, 100.0f,
  0.0f, 1.4f, 0.0f,
  0.0f, 1.0f, 0.0f,
};

static EntityInfo eiMantamanLiquid = {
  EIBT_FLESH, 150.0f,
  0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f,

#define FIRE_WATER    FLOAT3D(0.0f, 0.5f, -1.25f)
#define FIRE_GROUND   FLOAT3D(0.0f, 0.8f, -1.25f)
#define SPEAR_HIT   1.75f
};
%}


class CMantaman : CEnemyDive {
name      "Mantaman";
thumbnail "Thumbnails\\Mantaman.tbn";

properties:
  //1 BOOL m_FixedState         "Fixed state" 'X' = FALSE,      // fixed state on beginning

components:
  0 class   CLASS_BASE        "Classes\\EnemyDive.ecl",
  1 model   MODEL_MANTAMAN    "Models\\Enemies\\Mantaman\\Mantaman.mdl",
  2 texture TEXTURE_MANTAMAN  "Models\\Enemies\\Mantaman\\Mantaman.tex",
 11 texture TEXTURE_MANTAMAN_W   "Models\\Enemies\\Mantaman\\Projectile\\Water.tex",
 12 model   MODEL_MANTAMAN_P     "Models\\Enemies\\Mantaman\\Projectile\\Projectile.mdl",

// ************** SOUNDS **************
 50 sound   SOUND_IDLE      "Models\\Enemies\\Mantaman\\Sounds\\Idle.wav",
 51 sound   SOUND_SIGHT     "Models\\Enemies\\Mantaman\\Sounds\\Sight.wav",
 52 sound   SOUND_WOUND     "Models\\Enemies\\Mantaman\\Sounds\\Wound.wav",
 53 sound   SOUND_FIRE      "Models\\Enemies\\Mantaman\\Sounds\\Fire.wav",
 54 sound   SOUND_KICK      "Models\\Enemies\\Mantaman\\Sounds\\Kick.wav",
 55 sound   SOUND_DEATH     "Models\\Enemies\\Mantaman\\Sounds\\Death.wav",
 56 sound   SOUND_IDLE_GROUND   "Models\\Enemies\\Fishman\\Sounds\\IdleGround.wav",
 57 sound   SOUND_SIGHT_GROUND  "Models\\Enemies\\Fishman\\Sounds\\SightGround.wav",
 58 sound   SOUND_WOUND_GROUND  "Models\\Enemies\\Fishman\\Sounds\\WoundGround.wav",
 59 sound   SOUND_DEATH_GROUND  "Models\\Enemies\\Fishman\\Sounds\\DeathGround.wav",
 
functions:
  // describe how this enemy killed player
  virtual CTString GetPlayerKillDescription(const CTString &strPlayerName, const EDeath &eDeath)
  {
    CTString str;
    str.PrintF(TRANS("%s was killed by a Mantaman"), strPlayerName);
    return str;
  }
  virtual const CTFileName &GetComputerMessageName(void) const {
    static DECLARE_CTFILENAME(fnm, "Data\\Messages\\Enemies\\Mantaman.txt");
    return fnm;
  };
  void Precache(void) {
    CEnemyBase::Precache();
	
    PrecacheModel(MODEL_MANTAMAN_P);
    PrecacheTexture(TEXTURE_MANTAMAN_W);
	
    PrecacheSound(SOUND_IDLE);
    PrecacheSound(SOUND_SIGHT);
    PrecacheSound(SOUND_WOUND);
    PrecacheSound(SOUND_FIRE);
    PrecacheSound(SOUND_KICK);
    PrecacheSound(SOUND_DEATH);
	
    PrecacheSound(SOUND_IDLE_GROUND);
    PrecacheSound(SOUND_SIGHT_GROUND);
    PrecacheSound(SOUND_WOUND_GROUND);	
	PrecacheSound(SOUND_DEATH_GROUND);
  };
  /* Entity info */
  void *GetEntityInfo(void) {
    //return &eiMantamanLiquid;
    if (m_bInLiquid) {
      return &eiMantamanLiquid;
    } else {
      return &eiMantamanGround;
    }	
  };

  /* Receive damage */
  void ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType,
    FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection) 
  {
    // mantaman can't harm mantaman
    if (!IsOfClass(penInflictor, "Mantaman")) {
      CEnemyDive::ReceiveDamage(penInflictor, dmtType, fDamageAmmount, vHitPoint, vDirection);
    }
  };


  // damage anim
  INDEX AnimForDamage(FLOAT fDamage) {
    /*INDEX iAnim;
    switch (IRnd()%2) {
      case 0: iAnim = MANTAMAN_ANIM_WOUND01; break;
      case 1: iAnim = MANTAMAN_ANIM_WOUND02; break;
      default: ASSERTALWAYS("Mantaman unknown liquid damage");
    }*/
	INDEX iAnim;
    if (m_bInLiquid) {
      switch (IRnd()%2) {
        case 0: iAnim = MANTAMAN_ANIM_WOUND01; break;
        case 1: iAnim = MANTAMAN_ANIM_WOUND01; break;
        default: ASSERTALWAYS("Mantaman unknown liquid damage");
      }
    } else {
      switch (IRnd()%2) {
        case 0: iAnim = MANTAMAN_ANIM_WOUND01; break;
        case 1: iAnim = MANTAMAN_ANIM_WOUND01; break;
        default: ASSERTALWAYS("Mantaman unknown ground damage");
      }
    }
    StartModelAnim(iAnim, 0);
    return iAnim;
  };

  // death
  INDEX AnimForDeath(void) {
    //StartModelAnim(MANTAMAN_ANIM_DEATH, 0);
	INDEX iAnim;
    if (m_bInLiquid) {
      iAnim = MANTAMAN_ANIM_DEATH;
    } else {
      iAnim = MANTAMAN_ANIM_DEATH;
    }
    StartModelAnim(iAnim, 0);
    return iAnim;
    //return MANTAMAN_ANIM_DEATH;
  };

  void DeathNotify(void) {
    //ChangeCollisionBoxIndexWhenPossible(MANTAMAN_COLLISION_BOX_DEATH);
	if (m_bInLiquid) {
      ChangeCollisionBoxIndexWhenPossible(MANTAMAN_COLLISION_BOX_DEATH);
    } else {
      ChangeCollisionBoxIndexWhenPossible(MANTAMAN_COLLISION_BOX_DEATH);
    }
    en_fDensity = 500.0f;
  };

//##
  // virtual anim functions
  /*void StandingAnim(void) {
    if (m_FixedState) {
      StartModelAnim(MANTAMAN_ANIM_DEFAULT_ANIMATION02, AOF_LOOPING|AOF_NORESTART);
    } else {
      StartModelAnim(MANTAMAN_ANIM_STANDORANDSWIMSLOW, AOF_LOOPING|AOF_NORESTART);
    }
  };*/
    void StandingAnim(void) {
    if (m_bInLiquid) {
      StartModelAnim(MANTAMAN_ANIM_DEFAULT_ANIMATION02, AOF_LOOPING|AOF_NORESTART);
    } else {
      StartModelAnim(MANTAMAN_ANIM_DEFAULT_ANIMATION02, AOF_LOOPING|AOF_NORESTART);
    }
  };
  
  /*void WalkingAnim(void) {
    StartModelAnim(MANTAMAN_ANIM_STANDORANDSWIMSLOW, AOF_LOOPING|AOF_NORESTART);
  };*/ 
  void WalkingAnim(void) {
    if (m_bInLiquid) {
      StartModelAnim(MANTAMAN_ANIM_STANDORANDSWIMSLOW, AOF_LOOPING|AOF_NORESTART);
    } else {
      StartModelAnim(MANTAMAN_ANIM_STANDORANDSWIMSLOW, AOF_LOOPING|AOF_NORESTART);
    }
  };
  
  /*void RunningAnim(void) {
    StartModelAnim(MANTAMAN_ANIM_SWIMFAST, AOF_LOOPING|AOF_NORESTART);
  };*/
  void RunningAnim(void) {
    if (m_bInLiquid) {
      StartModelAnim(MANTAMAN_ANIM_SWIMFAST, AOF_LOOPING|AOF_NORESTART);
    } else {
      StartModelAnim(MANTAMAN_ANIM_SWIMFAST, AOF_LOOPING|AOF_NORESTART);
    }
  };
  
  /*void RotatingAnim(void) {
    StartModelAnim(MANTAMAN_ANIM_STANDORANDSWIMSLOW, AOF_LOOPING|AOF_NORESTART);
  };*/
  void RotatingAnim(void) {
    if (m_bInLiquid) {
      StartModelAnim(MANTAMAN_ANIM_STANDORANDSWIMSLOW, AOF_LOOPING|AOF_NORESTART);
    } else {
      StartModelAnim(MANTAMAN_ANIM_STANDORANDSWIMSLOW, AOF_LOOPING|AOF_NORESTART);
    }
  };
//##
  
  void ChangeCollisionToLiquid() {
    ChangeCollisionBoxIndexWhenPossible(MANTAMAN_COLLISION_BOX_DEAFULT);
  };
  void ChangeCollisionToGround() {
    ChangeCollisionBoxIndexWhenPossible(MANTAMAN_COLLISION_BOX_DEAFULT);
  };

  // virtual sound functions
  void IdleSound(void) {
    if (m_bInLiquid) {
      PlaySound(m_soSound, SOUND_IDLE , SOF_3D);
    } else {
      PlaySound(m_soSound, SOUND_IDLE_GROUND, SOF_3D);
    }
  };
  void SightSound(void) {
    if (m_bInLiquid) {
      PlaySound(m_soSound, SOUND_SIGHT , SOF_3D);
    } else {                                 
      PlaySound(m_soSound, SOUND_SIGHT_GROUND, SOF_3D);
    }
  };
  void WoundSound(void) {
    if (m_bInLiquid) {
      PlaySound(m_soSound, SOUND_WOUND , SOF_3D);
    } else {                                        
      PlaySound(m_soSound, SOUND_WOUND_GROUND, SOF_3D);
    }
  };
  void DeathSound(void) {
    if (m_bInLiquid) {
      PlaySound(m_soSound, SOUND_DEATH , SOF_3D);
    } else {                                        
      PlaySound(m_soSound, SOUND_DEATH_GROUND, SOF_3D);
    }
  };



procedures:
/************************************************************
 *                A T T A C K   E N E M Y                   *
 ************************************************************/
  /*AttackEnemy(EVoid) : CEnemyBase::AttackEnemy {
    if (m_FixedState) {
      m_FixedState = FALSE;
      StartModelAnim(MANTAMAN_ANIM_MORPH, 0);
      wait(GetModelObject()->GetAnimLength(MANTAMAN_ANIM_MORPH)) {
        on (EBegin) : { resume; }
        on (ETimer) : { stop; }
        on (EWatch) : { resume; }
        on (EDamage) : { resume; }
      }
    }
    jump CEnemyBase::AttackEnemy();
  };
  */


  DiveFire(EVoid) : CEnemyDive::DiveFire {
    // fire projectile
    //StartModelAnim(MANTAMAN_ANIM_ATTACK01, 0);
	StartModelAnim(MANTAMAN_ANIM_ATTACK02, 0);
    autowait(0.3f);
    ShootProjectile(PRT_MANTAMAN_FIRE, FIRE_WATER, ANGLE3D(0, 0, 0));
    PlaySound(m_soSound, SOUND_FIRE, SOF_3D);
    autowait(0.8f);
    StandingAnim();
    autowait(FRnd()/2 + _pTimer->TickQuantum);

    return EReturn();
  };


  DiveHit(EVoid) : CEnemyDive::DiveHit {
    if (CalcDist(m_penEnemy) > SPEAR_HIT) {
      // run to enemy
      m_fShootTime = _pTimer->CurrentTick() + 0.25f;
      return EReturn();
    }

    // attack with spear
    //StartModelAnim(FISHMAN_ANIM_WATERATTACK01, 0);
	StartModelAnim(MANTAMAN_ANIM_ATTACK01, 0);
	
    // to left hit
    autowait(0.5f);
    PlaySound(m_soSound, SOUND_KICK, SOF_3D);
    if (CalcDist(m_penEnemy)<SPEAR_HIT) {
      // damage enemy
      FLOAT3D vDirection = m_penEnemy->GetPlacement().pl_PositionVector-GetPlacement().pl_PositionVector;
      vDirection.Normalize();
      InflictDirectDamage(m_penEnemy, this, DMT_CLOSERANGE, 5.0f, FLOAT3D(0, 0, 0), vDirection);
      // push target away
      FLOAT3D vSpeed;
      GetHeadingDirection(0.0f, vSpeed);
      vSpeed = vSpeed * 5.0f;
      KickEntity(m_penEnemy, vSpeed);
    }
    autowait(0.5f);

    StandingAnim();
    autowait(0.2f + FRnd()/3);
    return EReturn();
  };

  Fire(EVoid) : CEnemyBase::Fire {
    // wait anim end
    if (!GetModelObject()->IsAnimFinished()) {
      autowait(GetModelObject()->GetCurrentAnimLength() - GetModelObject()->GetPassedTime());
    }

    // fire projectile
    //StartModelAnim(FISHMAN_ANIM_GROUNDATTACK02, 0);
	StartModelAnim(MANTAMAN_ANIM_ATTACK02, 0);
	
    autowait(0.3f);
    ShootProjectile(PRT_MANTAMAN_FIRE, FIRE_GROUND, ANGLE3D(0, 0, 0));
    PlaySound(m_soSound, SOUND_FIRE, SOF_3D);
    autowait(0.4f);
    StandingAnim();
    autowait(FRnd()/3 + _pTimer->TickQuantum);

    return EReturn();
  };

  Hit(EVoid) : CEnemyBase::Hit {
    if (CalcDist(m_penEnemy) > SPEAR_HIT) {
      // run to enemy
      m_fShootTime = _pTimer->CurrentTick() + 0.25f;
      return EReturn();
    }

    // wait anim end
    if (!GetModelObject()->IsAnimFinished()) {
      autowait(GetModelObject()->GetCurrentAnimLength() - GetModelObject()->GetPassedTime());
    }

    // attack with spear
    //StartModelAnim(FISHMAN_ANIM_GROUNDATTACKLOOP, 0);

    // to left hit
    //autowait(0.5f);
    PlaySound(m_soSound, SOUND_KICK, SOF_3D);
    if (CalcDist(m_penEnemy)<SPEAR_HIT) {
      // damage enemy
      FLOAT3D vDirection = m_penEnemy->GetPlacement().pl_PositionVector-GetPlacement().pl_PositionVector;
      vDirection.Normalize();
      InflictDirectDamage(m_penEnemy, this, DMT_CLOSERANGE, 2.5f, FLOAT3D(0, 0, 0), vDirection);
      // push target left
      FLOAT3D vSpeed;
      GetHeadingDirection(90.0f, vSpeed);
      vSpeed = vSpeed * 5.0f;
      KickEntity(m_penEnemy, vSpeed);
    }
    autowait(0.5f);
    PlaySound(m_soSound, SOUND_KICK, SOF_3D);
    if (CalcDist(m_penEnemy)<SPEAR_HIT) {
      // damage enemy
      FLOAT3D vDirection = m_penEnemy->GetPlacement().pl_PositionVector-GetPlacement().pl_PositionVector;
      vDirection.Normalize();
      InflictDirectDamage(m_penEnemy, this, DMT_CLOSERANGE, 2.5f, FLOAT3D(0, 0, 0), vDirection);
      // push target right
      FLOAT3D vSpeed;
      GetHeadingDirection(-90.0f, vSpeed);
      vSpeed = vSpeed * 5.0f;
      KickEntity(m_penEnemy, vSpeed);
    }
    autowait(0.6f);

    StandingAnim();
    autowait(0.2f + FRnd()/3);
    return EReturn();
  };
  

/************************************************************
 *                       M  A  I  N                         *
 ************************************************************/
  Main(EVoid) {
    // declare yourself as a model
    InitAsModel();
    //SetPhysicsFlags(EPF_MODEL_WALKING|EPF_HASGILLS);
	SetPhysicsFlags(EPF_MODEL_WALKING|EPF_HASLUNGS|EPF_HASGILLS);
    SetCollisionFlags(ECF_MODEL);
    SetFlags(GetFlags()|ENF_ALIVE);
    SetHealth(50.0f);
    m_fMaxHealth = 50.0f;
    en_tmMaxHoldBreath = 5.0f;
    en_fDensity = 1000.0f;

    // set your appearance
    SetModel(MODEL_MANTAMAN);
    SetModelMainTexture(TEXTURE_MANTAMAN);
	
	/*m_fWalkSpeed = 15.0f;
    m_aWalkRotateSpeed = 900.0f;
    m_fAttackRunSpeed = 10.0f;*/
	
	// setup moving speed
    m_fWalkSpeed = FRnd() + 1.5f;
    m_aWalkRotateSpeed = FRnd()*10.0f + 500.0f;
    m_fAttackRunSpeed = FRnd()*0.5f + 5.0f;
    m_aAttackRotateSpeed = FRnd()*50 + 245.0f;
    m_fCloseRunSpeed = FRnd()*0.5f + 5.0f;
    m_aCloseRotateSpeed = FRnd()*50 + 245.0f;
	
    // dive moving properties
    m_fDiveWalkSpeed = FRnd() + 2.0f;
    m_aDiveWalkRotateSpeed = FRnd()*10.0f + 500.0f;
    m_fDiveAttackRunSpeed = FRnd()*4.0f + 14.0f;
    m_aDiveAttackRotateSpeed = FRnd()*25 + 250.0f;
    m_fDiveCloseRunSpeed = FRnd()*2.0f + 6.5f;
    m_aDiveCloseRotateSpeed = FRnd()*50 + 250.0f;
	
    // attack properties
    m_fDiveAttackDistance = 100.0f;
    m_fDiveCloseDistance = 0.0f;
    m_fDiveStopDistance = 5.0f;
    m_fDiveAttackFireTime = 3.0f;
    m_fDiveCloseFireTime = 2.0f;
    m_fDiveIgnoreRange = 200.0f;
    // damage/explode properties
    m_fBlowUpAmount = 140.0f;
    m_fBodyParts = 4;
    m_fDamageWounded = 30.0f;
    m_iScore = 2000;


    // allowed types
    //m_EedtType = EDT_DIVE_ONLY;
	m_EedtType = EDT_GROUND_DIVE;

    // continue behavior in base class
    jump CEnemyDive::MainLoop();
  };
};
