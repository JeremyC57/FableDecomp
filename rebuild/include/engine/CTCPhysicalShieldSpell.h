#ifdef FABLE_RETAIL_BRIAR_ROSE
#ifndef FABLE_ENGINE_CTCPHYSICALSHIELDSPELL_H
#define FABLE_ENGINE_CTCPHYSICALSHIELDSPELL_H
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CRGBColour.h"
#include "stlport/fable_map.h"
class CSpecialAbilitiesPhysicalShieldDef;class CThingCreatureBase;
// Layout: complete original 0x2f43c, both PDBs; native84.
class CTCPhysicalShieldSpell:public CTCBase{
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCPhysicalShieldSpell(const CTCPhysicalShieldSpell & p0);
 CTCPhysicalShieldSpell(CThing & p0);
 virtual ~CTCPhysicalShieldSpell();
 virtual bool HasFrameUpdate() const;
 virtual void FrameUpdate();
 virtual void OnKill();
 virtual void OnRemove();
 virtual void OnHitAtPos(const C3DVector & p0);
 void AllowReactionsOnHitFromAttacker(CThing * p0, bool p1);
 bool GetAllowReactionsOnHit(CThing * p0);
 bool IsPaused() const;
 void Pause();
 void UnPause();
 void TakeDamage(float p0);
 void CancelMove();
 void CancelForCutscene();
 void ResumeForCutscene();
 bool IsShieldActive();
 void SetTimeLimit(long p0){DurationTimer=p0;}
 void SetAICasting(long p0){SpellLevel=p0;}
 void SetShieldHealth(float p0);
protected:
 void FadeOutEffect(CThing * p0);
 void PlayShieldEffects(CThingCreatureBase * p0);
 void PlayEffect(CIntelligentPointer<CThing> * p0, const CCharString & p1, const CCharString & p2, CThingCreatureBase * p3, long p4);
 unsigned long PlaySoundW(const CCharString & p0, bool p1);
 void StopSound(unsigned long p0);
 void FinalCleanup();
 CDefPointer<CSpecialAbilitiesPhysicalShieldDef const > PDef;
 long SpellLevel;
 long ShieldFadeOutTimer;
 long CurShieldFadeOutTimer;
 long DurationTimer;
 float ShieldHealth;
 float CurWobbleTimer;
 float WobbleDelta;
 CRGBColour ShieldColor;
 CIntelligentPointer<CThing> ShieldEffect;
 CIntelligentPointer<CThing> SwirlyEffect;
 unsigned long LoopingSound;
 bool Pausable;
 bool Paused;
 bool CutSceneCancelled;
 std::set<CThing *,std::less<CThing *>,std::allocator<CThing *> > AllowReactionAttackers;
public:
 CTCPhysicalShieldSpell & operator=(const CTCPhysicalShieldSpell & p0);
#ifdef FABLE_RETAIL_CTCPHYSICALSHIELDSPELL_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x0079F500
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCPhysicalShieldSpell)==84);
#endif
#else
#ifndef FABLE_ENGINE_CTCPHYSICALSHIELDSPELL_H
#define FABLE_ENGINE_CTCPHYSICALSHIELDSPELL_H
// Retail opt-in (Codex pass 17): original PDB members, retail set12 instead of16; native size54.
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CRGBColour.h"
class CSpecialAbilitiesPhysicalShieldDef;
class CTCPhysicalShieldSpell:public CTCBase {public:
 void CancelMove();
 void SetAICasting(long level){SpellLevel=level;}
 void SetTimeLimit(long time){DurationTimer=time;}
 void SetShieldHealth(float health){ShieldHealth=health;}

protected:
 CDefPointer<const CSpecialAbilitiesPhysicalShieldDef> PDef;
 long SpellLevel,ShieldFadeOutTimer,CurShieldFadeOutTimer,DurationTimer;float ShieldHealth,CurWobbleTimer,WobbleDelta;
 CRGBColour ShieldColor;CIntelligentPointer<CThing> ShieldEffect,SwirlyEffect;unsigned long LoopingSound;bool Pausable,Paused,CutSceneCancelled;
 unsigned char AllowReactionAttackers[12];
#ifdef FABLE_RETAIL_CTCPHYSICALSHIELDSPELL_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x0079F500
public:
#endif
};
#endif

#endif
