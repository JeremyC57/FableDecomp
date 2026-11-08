#ifndef FABLE_ENGINE_CTCTARGETINGBASE_H
#define FABLE_ENGINE_CTCTARGETINGBASE_H
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
protected:bool ScriptOverrideIgnoreEverything;unsigned char PNearestInterestingThing[8],PScriptOverrideInterestingThing[8],PScriptOverrideRangedTarget[8],PTargetedThing[8];CDefPointer<const CTargetingDef> PTargetingDef;bool LookAtCamera;
};
FABLE_STATIC_ASSERT(sizeof(CTCTargetingBase)==56);
#endif
