#ifndef FABLE_ENGINE_CTCTARGETINGBASE_H
#define FABLE_ENGINE_CTCTARGETINGBASE_H
#if defined(FABLE_RETAIL_UPDATE_NEAREST_INTERESTING) || defined(FABLE_RETAIL_FIRE_RANGED_WEAPON)
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
class CTargetingDef;class CThingCreatureBase;
#ifdef _MSC_VER
enum ETargetingType;
#else
enum ETargetingType {};
#endif
enum ECreatureInteractionType{CREATURE_INTERACTION_NULL=0,CREATURE_INTERACTION_CONVERSATION=1,CREATURE_INTERACTION_TAG=2,CREATURE_INTERACTION_MULTI_TAG=4,CREATURE_INTERACTION_PURCHASING=8};
class CTCTargetingBase:public CTCBase{
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_TARGETING;}
 virtual ETCInterfaceType GetInterfaceType() const;
 CTCTargetingBase(const CTCTargetingBase & p0);
 CTCTargetingBase(CThing & p0);
 virtual void OnInitialActivate();
 virtual void BulletTimeFrameUpdate();
 virtual CThing * GetPBestThingForMeleeAttack() const;
 virtual CThingCreatureBase * GetPBestEnemyCreatureForAttack() const;
 virtual CThing * GetPBestThingForProjectileTargetingLock() const;
 virtual CThingCreatureBase * GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType p0, float p1, float p2) const;
 virtual CThingCreatureBase * GetPBestCreatureForInteractionType(ECreatureInteractionType p0) const;
 CThing * GetPNearestInterestingThing() const;
 void SetScriptOverrideIgnoreEverything(bool p0);
 CThing * GetPScriptOverrideInterestingThing() const;
 void SetPScriptOverrideInterestingThing(CThing * p0);
 CThing * GetPScriptOverrideRangedTarget() const;
 void SetPScriptOverrideRangedTarget(CThing * p0);
 virtual CThing * GetPTargetedThing() const;
 virtual void SetPTargetedThing(CThing * p0);
 virtual CThing * GetPProjectileTargetedThing() const;
 virtual CThing * GetPTargetLockedThing() const;
 virtual bool GetTargetedPos(C3DVector & p0, float p1) const;
 bool IsTargetedThingOfType(ETargetingType p0) const;
 virtual bool IsTargetedThingInRange(unsigned long p0) const;
 void SetLookAtCamera(bool p0);
 bool IsLookingAtCamera() const;
protected:
 virtual void UpdateNearestInterestingThing();
 void SetPNearestInterestingThing(CThing * p0);
 const CTargetingDef & GetTargetingDef() const;
 bool ScriptOverrideIgnoreEverything;
 CIntelligentPointer<CThing> PNearestInterestingThing;
 CIntelligentPointer<CThing> PScriptOverrideInterestingThing;
 CIntelligentPointer<CThing> PScriptOverrideRangedTarget;
 CIntelligentPointer<CThing> PTargetedThing;
 CDefPointer<CTargetingDef const > PTargetingDef;
 bool LookAtCamera;
public:
 virtual ~CTCTargetingBase();
 CTCTargetingBase & operator=(const CTCTargetingBase & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCTargetingBase)==56);
#else
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CTargetingDef;class CThingCreatureBase;
#ifdef FABLE_RETAIL_CTCTARGETINGBASE_OFTYPE
enum ETargetingType;                                                     // MS forward enum (chunk 867: the row passes the value 1)
#endif
enum ECreatureInteractionType{CREATURE_INTERACTION_NULL=0,CREATURE_INTERACTION_CONVERSATION=1,CREATURE_INTERACTION_TAG=2,CREATURE_INTERACTION_MULTI_TAG=4,CREATURE_INTERACTION_PURCHASING=8};
// Layout: original TPI2251c, complete native size56; intelligent pointers
// are eight-byte original storage at unaligned offsets13,21,29,37.
class CTCTargetingBase:public CTCBase{
public: static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)8;}
public: virtual ETCInterfaceType GetInterfaceType() const;
public: virtual void OnInitialActivate();
public: virtual void BulletTimeFrameUpdate();
public: virtual CThing * GetPBestThingForMeleeAttack() const;
public: virtual CThingCreatureBase * GetPBestEnemyCreatureForAttack() const;
public: virtual CThing * GetPBestThingForProjectileTargetingLock() const;
public: virtual CThingCreatureBase * GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType,float,float) const;
public: virtual CThingCreatureBase * GetPBestCreatureForInteractionType(ECreatureInteractionType) const;
public: virtual CThing * GetPTargetedThing() const;
public: virtual void SetPTargetedThing(CThing *);
public: virtual CThing * GetPProjectileTargetedThing() const;
public: virtual CThing * GetPTargetLockedThing() const;
public: virtual bool GetTargetedPos(C3DVector &,float) const;
public: virtual bool IsTargetedThingInRange(unsigned long) const;
protected: virtual void UpdateNearestInterestingThing();
public: virtual ~CTCTargetingBase();
#ifdef FABLE_RETAIL_CTCTARGETINGBASE_LOOKATCAMERA
// Nested opt-in (chunk 768, row 0x00753380 CTCLook::FrameUpdateHeadInterested): Ego_r.pdb / FableWin.pdb
// ?GetPNearestInterestingThing@CTCTargetingBase@@QBEPAVCThing@@XZ (out of line, retail 0x007F5860) and FableWin.pdb
// ?IsLookingAtCamera@CTCTargetingBase@@QBE_NXZ (inline in retail: `mov al,[edi+34h]`, LookAtCamera). No layout change.
public: CThing* GetPNearestInterestingThing() const;
public: bool IsLookingAtCamera() const { return LookAtCamera; }
#endif
#ifdef FABLE_RETAIL_CTCTARGETINGBASE_OFTYPE
// Retail opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): FableWin.pdb
// ?IsTargetedThingOfType@CTCTargetingBase@@QBE_NW4ETargetingType@@@Z, out of line at 0x007F5A70 (`push 1 ; mov ecx,<targeting> ; call`).
// Declaration only; no layout change.
public: bool IsTargetedThingOfType(ETargetingType type) const;           // 0x007F5A70
#endif
#ifdef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
protected: const CTargetingDef& GetTargetingDef()const{return *PTargetingDef;}
#endif
protected:bool ScriptOverrideIgnoreEverything;unsigned char PNearestInterestingThing[8],PScriptOverrideInterestingThing[8],PScriptOverrideRangedTarget[8],PTargetedThing[8];CDefPointer<const CTargetingDef> PTargetingDef;bool LookAtCamera;
};
FABLE_STATIC_ASSERT(sizeof(CTCTargetingBase)==56);
#endif
#endif
