// CNOVI_Bully::Init() -- reset the bully (4 health, no hits, intro not done, speech loop 10), clear the quest's bully flag, make him an unkillable non-informant ally of the hero who cannot be pushed
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DAED30, 172 bytes (+ int3 padding). __fastcall, `ret`.
// Callees (CGameScriptInterface virtuals, ghidra_out/cgsi_slots_aligned.tsv): EntitySetAsDamageable 516 (+0x810) 0x0088EAD0,
//   EntitySetAsKillable 517 (+0x814) 0x0088EB30, EntitySetAsToAddToComboMultiplierWhenHit 526 (+0x838) 0x0088EBC0,
//   SetThingHasInformation 360 (+0x5a0) 0x0088E440, GetHero 70 (+0x118) 0x00891CA0, EntitySetThingAsAllyOfThing 599 (+0x95c)
//   0x008A7EC0, SetIsPushableByHero 844 (+0xd30) 0x008A6DD0 (CScriptThing BY VALUE: inline CBaseClass vtable 0x01231710 then
//   CScriptThing vtable 0x01238C8C stored into the argument area, PImp copied with add-ref).
// Layout: Ego_r.pdb CNOVI_Bully (44 bytes): ParentClass +0x14, InitialHealth +0x1c, HitsTaken +0x20, DoneIntro +0x24,
//   SaidPieceAboutAttackingVictim +0x25, SpokenOnFirstProximity +0x26, IntimidateSpeechLoop +0x28. The parent's byte at
//   retail +0x70 is the quest's bully flag (local model: the retail CQ_NewOakValeIntroScript layout is not the PDB's).
// Shape: one `xor ebx,ebx` feeds every false argument and store; the nested GetHero call keeps the interface vtable in ebp.
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CScriptThingImp;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_NewOakValeIntroScript {                // retail offsets
    unsigned char _pad0[0x70];
    bool          BullySubdued;                  // +0x70 (PDB +0x80)
};
struct CQ_SunnyvaleMasterData;
struct CNOVI_Bully {
    virtual void v00();
    CGameScriptInterface*     GSI;               // +0x04
    CScriptThing              Me;                // +0x08
    CQ_NewOakValeIntroScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;       // +0x18
    long                      InitialHealth;     // +0x1c
    long                      HitsTaken;         // +0x20
    bool                      DoneIntro;         // +0x24
    bool                      SaidPieceAboutAttackingVictim;   // +0x25
    bool                      SpokenOnFirstProximity;          // +0x26
    long                      IntimidateSpeechLoop;            // +0x28
    void Init();
};

void CNOVI_Bully::Init()
{
    DoneIntro = false;
    HitsTaken = 0;
    InitialHealth = 4;
    SpokenOnFirstProximity = false;
    ParentClass->BullySubdued = false;
    SaidPieceAboutAttackingVictim = false;
    IntimidateSpeechLoop = 10;
    GSI->EntitySetAsDamageable(Me, false);
    GSI->EntitySetAsKillable(Me, false, false);
    GSI->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    GSI->SetThingHasInformation(Me, false, false, false);
    GSI->EntitySetThingAsAllyOfThing(Me, GSI->GetHero());
    GSI->SetIsPushableByHero(Me, false);
}