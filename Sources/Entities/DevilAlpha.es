306
%{
#include "Entities/StdH/StdH.h"
#include "Models/Enemies/DevilAlpha/devil.h"
#include "Models/Enemies/DevilAlpha/Minigun.h"
#include "Models/Enemies/DevilAlpha/Stick.h"
#include "Models/Enemies/DevilAlpha/Flare.h"
#include "Models/Enemies/DevilAlpha/MiniGunItem.h"
#include "Models/Enemies/DevilAlpha/shield.h"
%}

uses "Entities/EnemyBase";
uses "Entities/Bullet";
uses "Entities/Reminder";
uses "Entities/BasicEffects";

enum DevilAlphaChar {
  0 DAC_EGYPTIAN     "Egyptian",    //"Soldier",
  1 DAC_PLANETOIDAL  "Planetoidal", //"General",
  2 DAC_SIRIUS       "Sirius",      //"Monster",
};


%{
#define GUN_X  0.375f
#define GUN_Y  0.6f
#define GUN_Z -1.85f
#define STRETCH_EGYPTIAN 5
#define STRETCH_PLANETOIDAL 4
#define STRETCH_SIRIUS 3
FLOAT3D vDevilAplhaStickBallLaunchPos;

// info structure
static EntityInfo eiDevilAlphaEgyptian = {
  EIBT_FLESH, 2000.0f,
  0, 1.6f*STRETCH_EGYPTIAN, 0,     // source (eyes)
  0.0f, 1.0f*STRETCH_EGYPTIAN, 0.0f,     // target (body)
};

static EntityInfo eiDevilAlphaPlanetoidal = {
  EIBT_FLESH, 1500.0f,
  0, 1.6f*STRETCH_PLANETOIDAL, 0,     // source (eyes)
  0.0f, 1.0f*STRETCH_PLANETOIDAL, 0.0f,     // target (body)
};

static EntityInfo eiDevilAlphaSirius = {
  EIBT_FLESH, 1000.0f,
  0, 1.6f*STRETCH_SIRIUS, 0,     // source (eyes)
  0.0f, 1.0f*STRETCH_SIRIUS, 0.0f,     // target (body)
};
#define LIGHT_ANIM_FIRE 3
#define LIGHT_ANIM_NONE 5
%}


class CDevilAlpha : CEnemyBase {
name      "DevilAlpha";
thumbnail "Thumbnails\\DevilAlpha.tbn";

properties:
  1 enum DevilAlphaChar m_dacChar "Character" 'C' = DAC_SIRIUS,
  2 INDEX m_bFireBulletCount = 0,       // fire bullet binary divider
  3 INDEX m_iSpawnEffect = 0,           // counter for spawn effect every 'x' times
  4 FLOAT m_fFireTime = 0.0f,           // time to fire bullets
  5 CAnimObject m_aoLightAnimation,     // light animation object
  6 BOOL m_bSleeping "Sleeping" 'S' = FALSE,  // set to make DEVILALPHA sleep initally
  //3 BOOL m_bDABoss  "Boss" 'B' = FALSE,
  
{
  CEntity *penBullet;     // bullet
  CLightSource m_lsLightSource;
}

components:
  0 class   CLASS_BASE        "Classes\\EnemyBase.ecl",
  1 class   CLASS_BULLET      "Classes\\Bullet.ecl",
//###
  2 class   CLASS_PROJECTILE  "Classes\\Projectile.ecl",

  5 model   MODEL_DEVILALPHA  "Models\\Enemies\\DevilAlpha\\Devil.mdl",
  6 texture TEXTURE_EGYPTIAN  "Models\\Enemies\\DevilAlpha\\Devil2.tex",
  7 texture TEXTURE_PLANETOIDAL   "Models\\Enemies\\DevilAlpha\\Devil.tex",
  8 texture TEXTURE_SIRIUS    "Models\\Enemies\\DevilAlpha\\Devil4.tex",
 12 texture TEXTURE_SPECULAR  "Models\\SpecularTextures\\Medium.tex",
  9 model   MODEL_GUN         "Models\\Enemies\\DevilAlpha\\Minigun.mdl",
 10 model   MODEL_FLARE       "Models\\Effects\\Weapons\\Flare01\\Flare.mdl",
 11 texture TEXTURE_GUN       "Models\\Weapons\\Minigun\\Minigun.tex",
//###
 20 texture TEXTURE_FLARE     "Models\\Effects\\Weapons\\Flare01\\Flare.tex",
 21 texture TEXTURE_FLARE12   "Models\\Enemies\\DevilAlpha\\12.tex",
 
 25 model   MODEL_STICK       "Models\\Enemies\\DevilAlpha\\Stick.mdl",
 26 model   MODEL_SHIELD      "Models\\Enemies\\DevilAlpha\\Shield.mdl",
 27 texture TEXTURE_STICK     "Models\\Enemies\\DevilAlpha\\Stick.tex",
 28 texture TEXTURE_SHIELD    "Models\\Enemies\\DevilAlpha\\Shield.tex",

// ************** SOUNDS **************
 50 sound   SOUND_IDLE      "Models\\Enemies\\DevilAlpha\\Sounds\\Idle.wav",
 51 sound   SOUND_SIGHT     "Models\\Enemies\\DevilAlpha\\Sounds\\Sight.wav",
 52 sound   SOUND_WOUND     "Models\\Enemies\\DevilAlpha\\Sounds\\Wound.wav",
 53 sound   SOUND_FIRE      "Models\\Enemies\\DevilAlpha\\Sounds\\Fire.wav",
 54 sound   SOUND_KICK      "Models\\Enemies\\DevilAlpha\\Sounds\\Kick.wav",
 55 sound   SOUND_DEATH     "Models\\Enemies\\DevilAlpha\\Sounds\\Death.wav",
//###
 55 sound   SOUND_DEATH     "Models\\Enemies\\DevilAlpha\\Sounds\\Death.wav",

functions:
  // describe how this enemy killed player
  virtual CTString GetPlayerKillDescription(const CTString &strPlayerName, const EDeath &eDeath)
  {
    CTString str;
    if (eDeath.eLastDamage.dmtType==DMT_CLOSERANGE) {
      str.PrintF(TRANS("%s was stabbed by an Devil"), (const char*)strPlayerName);
    } else {
      str.PrintF(TRANS("An Devil poured lead into %s"), (const char*)strPlayerName);
    }
    return str;
  }
  void Precache(void) {
    CEnemyBase::Precache();
    PrecacheModel(MODEL_FLARE);
    PrecacheSound(SOUND_IDLE );
    PrecacheSound(SOUND_SIGHT);
    PrecacheSound(SOUND_WOUND);
    PrecacheSound(SOUND_FIRE );
    PrecacheSound(SOUND_KICK );
    PrecacheSound(SOUND_DEATH);
    PrecacheTexture(TEXTURE_SIRIUS);
    PrecacheTexture(TEXTURE_EGYPTIAN);
    PrecacheTexture(TEXTURE_PLANETOIDAL);
    PrecacheModel(MODEL_SHIELD);
    PrecacheTexture(TEXTURE_SHIELD);
    PrecacheClass(CLASS_PROJECTILE, PRT_DALPHA_PROJECTILE);
  };

  /* Read from stream. */
  void Read_t( CTStream *istr) { // throw char *
    CEnemyBase::Read_t(istr);

    // setup light source
    SetupLightSource();
  }

  /* Fill in entity statistics - for AI purposes only */
  BOOL FillEntityStatistics(EntityStats *pes)
  {
    CEnemyBase::FillEntityStatistics(pes);
    switch(m_dacChar) {
    case DAC_SIRIUS: { pes->es_strName+=" Egyptian"; } break;
    case DAC_PLANETOIDAL: { pes->es_strName+=" Sirius"; } break;
    case DAC_EGYPTIAN: { pes->es_strName+=" Planetoidal"; } break;
    }
    return TRUE;
  }

  virtual const CTFileName &GetComputerMessageName(void) const {
    static DECLARE_CTFILENAME(fnmDevilEgyptian, "Data\\Messages\\Enemies\\DevilEgyptian.txt");
    static DECLARE_CTFILENAME(fnmDevilSirius, "Data\\Messages\\Enemies\\DevilSirius.txt");
    static DECLARE_CTFILENAME(fnmDevilPlanetoidal, "Data\\Messages\\Enemies\\DevilPlanetoidal.txt");
    switch(m_dacChar) {
    default: ASSERT(FALSE);
    case DAC_SIRIUS: return fnmDevilSirius;
    case DAC_PLANETOIDAL: return fnmDevilPlanetoidal;
    case DAC_EGYPTIAN: return fnmDevilEgyptian;
    }
  };

  /* Get static light source information. */
  CLightSource *GetLightSource(void) {
    if (!IsPredictor()) {
      return &m_lsLightSource;
    } else {
      return NULL;
    }
  }

  BOOL ForcesCannonballToExplode(void)
  {
    if (m_dacChar!=DAC_EGYPTIAN) {
      return TRUE;
    }
    return CEnemyBase::ForcesCannonballToExplode();
  }

  // Setup light source
  void SetupLightSource(void) {
    // setup light source
    CLightSource lsNew;
    lsNew.ls_ulFlags = LSF_NONPERSISTENT|LSF_DYNAMIC;
    lsNew.ls_rHotSpot = 2.0f;
    lsNew.ls_rFallOff = 8.0f;
    lsNew.ls_colColor = RGBToColor(128, 128, 128);
    lsNew.ls_plftLensFlare = NULL;
    lsNew.ls_ubPolygonalMask = 0;
    lsNew.ls_paoLightAnimation = &m_aoLightAnimation;

    m_lsLightSource.ls_penEntity = this;
    m_lsLightSource.SetLightSource(lsNew);
  }
  // play light animation
  void PlayLightAnim(INDEX iAnim, ULONG ulFlags) {
    if (m_aoLightAnimation.GetData()!=NULL) {
      m_aoLightAnimation.PlayAnim(iAnim, ulFlags);
    }
  };

  // fire minigun on/off
  void MinigunOn(void)
  {
    PlayLightAnim(LIGHT_ANIM_FIRE, AOF_LOOPING);
    CModelObject *pmoGun = &GetModelObject()->GetAttachmentModel(DEVIL_ATTACHMENT_MINIGUN)->
      amo_moModelObject;
    //pmoGun->PlayAnim(GUN_ANIM_FIRE, AOF_LOOPING);
    AddAttachmentToModel(this, *pmoGun, GUN_ATTACHMENT_FLAME, MODEL_FLARE, TEXTURE_FLARE, 0, 0, 0);
    switch (m_dacChar) {
      case DAC_SIRIUS: pmoGun->StretchModel(FLOAT3D(2.0f, 2.0f, 2.0f)); break;
      case DAC_PLANETOIDAL: pmoGun->StretchModel(FLOAT3D(3.0f, 3.0f, 3.0f)); break;
      case DAC_EGYPTIAN: pmoGun->StretchModel(FLOAT3D(4.0f, 4.0f, 4.0f)); break;
    }
  }
  void MinigunOff(void)
  {
    PlayLightAnim(LIGHT_ANIM_NONE, 0);
    CModelObject *pmoGun = &GetModelObject()->GetAttachmentModel(DEVIL_ATTACHMENT_MINIGUN)->
      amo_moModelObject;
    //pmoGun->PlayAnim(GUN_ANIM_IDLE, AOF_LOOPING);
    pmoGun->RemoveAttachmentModel(GUN_ATTACHMENT_FLAME);
  }
  /* Entity info */
  void *GetEntityInfo(void) {
    if (m_dacChar == DAC_SIRIUS) {
      return &eiDevilAlphaSirius;
    } else if (m_dacChar == DAC_PLANETOIDAL) {
      return &eiDevilAlphaPlanetoidal;
    } else {
      return &eiDevilAlphaEgyptian;
    }
  };

  /* Receive damage */
  void ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType,
    FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection) 
  {
    // DevilAlpha can't harm DevilAlpha
    if (!IsOfClass(penInflictor, "DevilAlpha")) {
      CEnemyBase::ReceiveDamage(penInflictor, dmtType, fDamageAmmount, vHitPoint, vDirection);
    }
  };


  // damage anim
  INDEX AnimForDamage(FLOAT fDamage) {
    INDEX iAnim;
    switch (IRnd()%3) {
      case 0: iAnim = DEVIL_ANIM_WOUND01SLIGHTFRONT; break;
      case 1: iAnim = DEVIL_ANIM_WOUND02SLIGHTBACK; break;
      case 2: iAnim = DEVIL_ANIM_WOUND03CRITICALFRONT; break;
      default: ASSERTALWAYS("DevilAlpha unknown damage");
    }
    StartModelAnim(iAnim, 0);
    MinigunOff();
    return iAnim;
  };

  // death
  INDEX AnimForDeath(void) {
    StartModelAnim(DEVIL_ANIM_DEATH, 0);
    MinigunOff();
    return DEVIL_ANIM_DEATH;
  };

  FLOAT WaitForDust(FLOAT3D &vStretch) {
    if(GetModelObject()->GetAnim()==DEVIL_ANIM_DEATH)
    {
      vStretch=FLOAT3D(1,1,1)*1.5f;
      return 1.3f;
    }
    return -1.0f;
  };

  void DeathNotify(void) {
    ChangeCollisionBoxIndexWhenPossible(DEVIL_COLLISION_BOX_DEATH);
    SetCollisionFlags(ECF_MODEL);
  };

  // virtual anim functions
  void StandingAnim(void) {
    StartModelAnim(DEVIL_ANIM_STANDLOOP, AOF_LOOPING|AOF_NORESTART);
  };
  void WalkingAnim(void) {
    StartModelAnim(DEVIL_ANIM_WALK, AOF_LOOPING|AOF_NORESTART);
  };
  void RunningAnim(void) {
    StartModelAnim(DEVIL_ANIM_RUN, AOF_LOOPING|AOF_NORESTART);
  };
  void RotatingAnim(void) {
    StartModelAnim(DEVIL_ANIM_WALK /*SCORPMAN_ANIM_WALK1*/, AOF_LOOPING|AOF_NORESTART);
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


/************************************************************
 *                   FIRE BULLET / RAIL                     *
 ************************************************************/
  BOOL CanFireAtPlayer(void)
  {
    // get ray source and target
    FLOAT3D vSource, vTarget;
    GetPositionCastRay(this, m_penEnemy, vSource, vTarget);

    // bullet start position
    CPlacement3D plBullet;
    plBullet.pl_OrientationAngle = ANGLE3D(0,0,0);
    plBullet.pl_PositionVector = FLOAT3D(GUN_X, GUN_Y, 0);
    // offset are changed according to stretch factor
    if (m_dacChar == DAC_SIRIUS) {
      plBullet.pl_PositionVector*=STRETCH_SIRIUS;
    } else if (m_dacChar == DAC_PLANETOIDAL) {
      plBullet.pl_PositionVector*=STRETCH_PLANETOIDAL;
    } else {
      plBullet.pl_PositionVector*=STRETCH_EGYPTIAN;
    }
    plBullet.RelativeToAbsolute(GetPlacement());
    vSource = plBullet.pl_PositionVector;

    // cast the ray
    CCastRay crRay(this, vSource, vTarget);
    crRay.cr_ttHitModels = CCastRay::TT_NONE;     // check for brushes only
    crRay.cr_bHitTranslucentPortals = FALSE;
    en_pwoWorld->CastRay(crRay);

    // if hit nothing (no brush) the entity can be seen
    return (crRay.cr_penHit==NULL);     
  }

  void PrepareBullet(FLOAT fDamage) {
    // bullet start position
    CPlacement3D plBullet;
    plBullet.pl_OrientationAngle = ANGLE3D(0,0,0);
    plBullet.pl_PositionVector = FLOAT3D(GUN_X, GUN_Y, 0);
    // offset are changed according to stretch factor
    if (m_dacChar == DAC_SIRIUS) {
      plBullet.pl_PositionVector*=STRETCH_SIRIUS;
    } else if (m_dacChar == DAC_PLANETOIDAL) {
      plBullet.pl_PositionVector*=STRETCH_PLANETOIDAL;
    } else {
      plBullet.pl_PositionVector*=STRETCH_EGYPTIAN;
    }
    plBullet.RelativeToAbsolute(GetPlacement());
    // create bullet
    penBullet = CreateEntity(plBullet, CLASS_BULLET);
    // init bullet
    EBulletInit eInit;
    eInit.penOwner = this;
    eInit.fDamage = fDamage;
    penBullet->Initialize(eInit);
  };

  // fire bullet
  void FireBullet(void) {
    // binary divide counter
    m_bFireBulletCount++;
    if (m_bFireBulletCount>1) { m_bFireBulletCount = 0; }
    if (m_bFireBulletCount==1) { return; }
    // bullet
    PrepareBullet(3.0f);
    ((CBullet&)*penBullet).CalcTarget(m_penEnemy, 250);
    ((CBullet&)*penBullet).CalcJitterTarget(10);
    ((CBullet&)*penBullet).LaunchBullet( TRUE, TRUE, TRUE);
    ((CBullet&)*penBullet).DestroyBullet();
  };


  // adjust sound and watcher parameters here if needed
  void EnemyPostInit(void) 
  {
    // set sound default parameters
    m_soSound.Set3DParameters(160.0f, 50.0f, 1.0f, 1.0f);
  };

procedures:
/************************************************************
 *                A T T A C K   E N E M Y                   *
 ************************************************************/
  // shoot
  Fire(EVoid) : CEnemyBase::Fire{
    if (!CanFireAtPlayer()) {
      return EReturn();
    }

    // confused amount
    switch (m_dacChar) {
      case DAC_SIRIUS:
        m_fDamageConfused = 50;
        m_fFireTime = 4.0f;
        break;
      case DAC_PLANETOIDAL:
        m_fDamageConfused = 100;
        m_fFireTime = 4.0f;
        break;
      case DAC_EGYPTIAN:
        m_fDamageConfused = 200;
        m_fFireTime = 8.0f;
        break;
    }
    if (GetSP()->sp_gdGameDifficulty<=CSessionProperties::GD_EASY) {
      m_fFireTime *= 0.5f;
    }

//#######################################################################
    // to ball
    switch (m_dacChar) {
      case DAC_SIRIUS:
        vDevilAplhaStickBallLaunchPos = (FLOAT3D(-0.5f, 3.9f, 0.15f)*2.5f);
        m_fFireTime = 4.0f;
        break;
      case DAC_PLANETOIDAL:
        vDevilAplhaStickBallLaunchPos = (FLOAT3D(-1.0f, 5.1f, 0.15f)*2.5f);
        break;
      case DAC_EGYPTIAN:
        vDevilAplhaStickBallLaunchPos = (FLOAT3D(-1.5f, 6.5f, 0.15f)*2.5f);
        break;
    }
    PlayLightAnim(LIGHT_ANIM_FIRE, AOF_LOOPING);
    StartModelAnim(DEVIL_ANIM_ATTACK03, AOF_LOOPING|AOF_NORESTART);
    autocall CMovableModelEntity::WaitUntilScheduledAnimStarts() EReturn;    

    StartModelAnim(DEVIL_ANIM_ATTACK03, AOF_LOOPING|AOF_NORESTART);
    autocall CMovableModelEntity::WaitUntilScheduledAnimStarts() EReturn;    
    PlaySound(m_soSound, SOUND_FIRE, SOF_3D);
    autowait(0.51f);

    ShootProjectile(PRT_DALPHA_PROJECTILE, vDevilAplhaStickBallLaunchPos,
        ANGLE3D(AngleDeg((FRnd()-0.5)*30.0f), AngleDeg(FRnd()*10.0f), 0));
    autowait(0.3f);
    MaybeSwitchToAnotherPlayer();
    autowait(FRnd()/2 + _pTimer->TickQuantum);
//#######################################################################
    // to fire
    StartModelAnim(DEVIL_ANIM_FROMSTANDTOATTACK01POSITION, 0);
    m_fLockOnEnemyTime = GetModelObject()->GetAnimLength(DEVIL_ANIM_FROMSTANDTOATTACK01POSITION) + 0.5f + FRnd()/3;
    autocall CEnemyBase::LockOnEnemy() EReturn;

    // fire
    m_iSpawnEffect = 0;                         // effect every 'x' frames
    m_fFireTime += _pTimer->CurrentTick();
    m_bFireBulletCount = 0;
    PlaySound(m_soSound, SOUND_FIRE, SOF_3D|SOF_LOOP);
    MinigunOn();

    while (m_fFireTime > _pTimer->CurrentTick()) {
      m_fMoveFrequency = 0.1f;
      wait(m_fMoveFrequency) {
        on (EBegin) : {
          // make fuss
          AddToFuss();
          // fire bullet
          FireBullet();
          m_vDesiredPosition = m_penEnemy->GetPlacement().pl_PositionVector;
          // rotate to enemy
          if (!IsInPlaneFrustum(m_penEnemy, CosFast(5.0f))) {
            m_fMoveSpeed = 0.0f;
            m_aRotateSpeed = 4000.0f;
            StartModelAnim(DEVIL_ANIM_ATTACK01LOOPMINIGUN, AOF_LOOPING|AOF_NORESTART);
          // stand in place
          } else {
            m_fMoveSpeed = 0.0f;
            m_aRotateSpeed = 0.0f;
            StartModelAnim(DEVIL_ANIM_ATTACK01LOOPMINIGUN, AOF_LOOPING|AOF_NORESTART);
          }
          // adjust direction and speed
          SetDesiredMovement(); 
          resume;
        }
        on (ETimer) : { stop; }
      }
    }
    m_soSound.Stop();
    MinigunOff();
    // set next shoot time
    m_fShootTime = _pTimer->CurrentTick() + m_fAttackFireTime*(1.0f + FRnd()/3.0f);

    // from fire
    StartModelAnim(DEVIL_ANIM_FROMATTACK01TOSTANDPOSITION, 0);
    autowait(GetModelObject()->GetAnimLength(DEVIL_ANIM_FROMATTACK01TOSTANDPOSITION));

    MaybeSwitchToAnotherPlayer();

    // shoot completed
    return EReturn();
  };

  // hit enemy
  Hit(EVoid) : CEnemyBase::Hit {
    // close attack
    StartModelAnim(DEVIL_ANIM_ATTACK02LOOPCLAWS, 0);
    autowait(0.5f);
    PlaySound(m_soSound, SOUND_KICK, SOF_3D);
    if (CalcDist(m_penEnemy) < m_fCloseDistance) {
      FLOAT3D vDirection = m_penEnemy->GetPlacement().pl_PositionVector-GetPlacement().pl_PositionVector;
      vDirection.Normalize();
      if (m_dacChar == DAC_EGYPTIAN) {
        InflictDirectDamage(m_penEnemy, this, DMT_CLOSERANGE, 80.0f, FLOAT3D(0, 0, 0), vDirection);
      } else if (m_dacChar == DAC_PLANETOIDAL) {
        InflictDirectDamage(m_penEnemy, this, DMT_CLOSERANGE, 40.0f, FLOAT3D(0, 0, 0), vDirection);
      } else {
        InflictDirectDamage(m_penEnemy, this, DMT_CLOSERANGE, 20.0f, FLOAT3D(0, 0, 0), vDirection);
      }
      // hit kick 
      FLOAT3D vSpeed;
      GetHeadingDirection(0.0f, vSpeed);
      vSpeed = vSpeed * 50.0f;
      KickEntity(m_penEnemy, vSpeed);
    }
    autowait(0.3f);
    MaybeSwitchToAnotherPlayer();
    return EReturn();
  };

  Sleep(EVoid)
  {
    // start sleeping anim
    StartModelAnim(DEVIL_ANIM_DEATHREST, AOF_LOOPING);
    // repeat
    wait() {
      // if triggered
      on(ETrigger eTrigger) : {
        // remember enemy
        SetTargetSoft(eTrigger.penCaused);
        // wake up
        jump WakeUp();
      }
      // if damaged
      on(EDamage eDamage) : {
        // wake up
        jump WakeUp();
      }
      otherwise() : {
        resume;
      }
    }
  }

  WakeUp(EVoid)
  {
    // wakeup anim
    SightSound();
    StartModelAnim(DEVIL_ANIM_WOUND01SLIGHTFRONT, 0);
    autowait(GetModelObject()->GetCurrentAnimLength());

    // trigger your target
    SendToTarget(m_penDeathTarget, m_eetDeathType);
    // proceed with normal functioning
    return EReturn();
  }

  // overridable called before main enemy loop actually begins
  PreMainLoop(EVoid) : CEnemyBase::PreMainLoop
  {
    // if sleeping
    if (m_bSleeping) {
      m_bSleeping = FALSE;
      // go to sleep until waken up
      wait() {
        on (EBegin) : {
          call Sleep();
        }
        on (EReturn) : {
          stop;
        };
        // if dead
        on(EDeath eDeath) : {
          // die
          jump CEnemyBase::Die(eDeath);
        }
      }
    }
    return EReturn();
  }

/************************************************************
 *                       M  A  I  N                         *
 ************************************************************/
  Main(EVoid) {
    //if (m_dacChar==DAC_SIRIUS) {
    //  m_dacChar=DAC_SIRIUS;
    //}

    // declare yourself as a model
    InitAsModel();
    SetPhysicsFlags(EPF_MODEL_WALKING|EPF_HASLUNGS);
    SetCollisionFlags(ECF_MODEL);
    SetFlags(GetFlags()|ENF_ALIVE);
    en_tmMaxHoldBreath = 25.0f;
    en_fDensity = 3000.0f;
    //m_bBoss = m_bDABoss;

    // set your appearance
    SetModel(MODEL_DEVILALPHA);
    switch (m_dacChar) {
      case DAC_EGYPTIAN:
        // set your texture
        SetModelMainTexture(TEXTURE_EGYPTIAN);
        SetModelSpecularTexture(TEXTURE_SPECULAR);
        SetHealth(10000.0f);
        m_fMaxHealth = 10000.0f;
        // damage/explode properties
        m_fDamageWounded = 800.0f;
        m_fBlowUpAmount = 1E10f;
        m_fBodyParts = 60;
        // setup attack distances
        m_fAttackDistance = 250.0f;
        m_fCloseDistance = 11.0f;
        m_fStopDistance = 9.0f;
        m_fAttackFireTime = 2.0f;
        m_fCloseFireTime = 1.0f;
        m_fIgnoreRange = 500.0f;
        m_iScore = 10000;
        break;

      case DAC_PLANETOIDAL:
        // set your texture
        SetModelMainTexture(TEXTURE_PLANETOIDAL);
        SetModelSpecularTexture(TEXTURE_SPECULAR);
        SetHealth(3000.0f);
        m_fMaxHealth = 3000.0f;
        // damage/explode properties
        m_fDamageWounded = 400.0f;
        m_fBlowUpAmount = 1E10f;
        m_fBodyParts = 30;
        // setup attack distances
        m_fAttackDistance = 200.0f;
        m_fCloseDistance = 5.0f;
        m_fStopDistance = 4.5f;
        m_fAttackFireTime = 2.0f;
        m_fCloseFireTime = 1.0f;
        m_fIgnoreRange = 350.0f;
        m_iScore = 5000;
        break;

      case DAC_SIRIUS:
        // set your texture
        SetModelMainTexture(TEXTURE_SIRIUS);
        SetModelSpecularTexture(TEXTURE_SPECULAR);
        SetHealth(1500.0f);
        m_fMaxHealth = 1500.0f;
        // damage/explode properties
        m_fDamageWounded = 200.0f;
        m_fBlowUpAmount = 1E10f;
        m_fBodyParts = 30;
        // setup attack distances
        m_fAttackDistance = 200.0f;
        m_fCloseDistance = 5.0f;
        m_fStopDistance = 4.5f;
        m_fAttackFireTime = 0.5f;
        m_fCloseFireTime = 1.0f;
        m_fIgnoreRange = 350.0f;
        m_iScore = 1000;
        break;
    }
    
    AddAttachment(DEVIL_ATTACHMENT_MINIGUN, MODEL_GUN, TEXTURE_GUN);
    AddAttachment(DEVIL_ATTACHMENT_STICK, MODEL_STICK, TEXTURE_STICK);
    AddAttachment(DEVIL_ATTACHMENT_SHIELD, MODEL_SHIELD, TEXTURE_SHIELD);

    // set stretch factors for height and width - MUST BE DONE BEFORE SETTING MODEL!
    switch (m_dacChar) {
      case DAC_EGYPTIAN: GetModelObject()->StretchModel(FLOAT3D(1.0f, 1.0f, 1.0f)*STRETCH_EGYPTIAN); break;
      case DAC_PLANETOIDAL: GetModelObject()->StretchModel(FLOAT3D(1.0f, 1.0f, 1.0f)*STRETCH_PLANETOIDAL); break;
      case DAC_SIRIUS: GetModelObject()->StretchModel(FLOAT3D(1.0f, 1.0f, 1.0f)*STRETCH_SIRIUS); break;
    }

    ModelChangeNotify();

    // setup moving speed
    m_fWalkSpeed = FRnd() + 1.5f;
    m_aWalkRotateSpeed = AngleDeg(FRnd()*20.0f + 550.0f);
    m_fAttackRunSpeed = FRnd()*1.5f + 4.5f;
    m_aAttackRotateSpeed = AngleDeg(FRnd()*50.0f + 275.0f);
    m_fCloseRunSpeed = FRnd()*1.5f + 4.5f;
    m_aCloseRotateSpeed = AngleDeg(FRnd()*50.0f + 275.0f);

    // setup light source
    SetupLightSource();
    // set light animation if available
    try {
      m_aoLightAnimation.SetData_t(CTFILENAME("Animations\\BasicEffects.ani"));
    } catch (const char *strError) {
      WarningMessage(TRANS("Cannot load Animations\\BasicEffects.ani: %s"), strError);
    }
    MinigunOff();

    // continue behavior in base class
    jump CEnemyBase::MainLoop();

  };
};
