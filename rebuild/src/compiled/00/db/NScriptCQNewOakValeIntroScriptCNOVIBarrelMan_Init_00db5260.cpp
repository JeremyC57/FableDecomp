// NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelMan::Init() -- restart the barrel man: reset his phase and flags, clear the quest's barrel timer, make him an unkillable informant, remember his home as the broken-barrel spot and give him a 10m sight radius
// Leaf: ?Init@CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB5260, 167 bytes (+ int3 padding). __fastcall, `ret`.
// Callees: the global script interface pDSTGame (0x0143E8F8) SetTimer 89 (+0x164) 0x006E76E0; through the entity
//   script's own interface pointer: EntitySetAsDamageable 516 (+0x810) 0x0088EAD0, EntitySetAsKillable 517 (+0x814) 0x0088EB30,
//   EntitySetAsToAddToComboMultiplierWhenHit 526 (+0x838) 0x0088EBC0, SetThingHasInformation 360 (+0x5a0) 0x0088E440,
//   EntitySetSightRadius 640 (+0xa00) 0x0089D220 (ghidra_out/cgsi_slots_aligned.tsv); CScriptThing::GetHomePos slot 7 (+0x1c)
//   0x004AA9A0 (vtable 0x01238C8C), returned by value into a frame temporary.
// Layout: Ego_r.pdb CNOVI_BarrelMan (36 bytes): ParentClass +0x14, ComplainedAboutStock +0x1c, HeroLetMeDown +0x1d,
//   OverheardYet +0x1e, MyPhase +0x20. The parent CQ_NewOakValeIntroScript is byte-packed (the PDB has BarrelBrokenPos at the
//   unaligned +0x8a); in retail it sits at +0x85 and the quest's barrel timer handle at +0x108 (retail offsets, local model).
// Shape: one `xor ebx,ebx` zero feeds every false/0 argument and store; the home position is copied with the implicit
//   C3DVector assignment (one `add edx,0x85` destination, three dword moves).
// Naming wave 35: identity reg 00dabf71 "NOVI_BarrelMan" in 00dabac0; Alloc 00db51b0; vtable 012d9014 slot 2; PDB ?Init@CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CNOVI_BarrelMan::Init (wave3/labels_alloc.tsv); PDB ?Init@CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CNOVI_BarrelMan@@QAEXXZ`; this source now defines the PDB member (outer stand-in CQ_NewOakValeIntroScript moved into NScript); same bytes.
#include "engine/C3DVector.h"
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImp;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#pragma pack(push, 1)
namespace NScript {
class CQ_NewOakValeIntroScript {
public:                // retail offsets (packed quest script)
    unsigned char _pad0[0x85];
    C3DVector     BarrelBrokenPos;               // +0x85
    unsigned char _pad91[0x108 - 0x91];
    long          BarrelTimer;                   // +0x108 (CTimer handle)
public:
    class CNOVI_BarrelMan;           // the entity class below (naming wave 35)
};
}
using NScript::CQ_NewOakValeIntroScript;
#pragma pack(pop)
struct CQ_SunnyvaleMasterData;
namespace NScript {
class CQ_NewOakValeIntroScript::CNOVI_BarrelMan {
public:
    virtual void v00();
    CGameScriptInterface*     GSI;               // +0x04
    CScriptThing              Me;                // +0x08
    CQ_NewOakValeIntroScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;       // +0x18
    bool                      ComplainedAboutStock;   // +0x1c
    bool                      HeroLetMeDown;     // +0x1d
    bool                      OverheardYet;      // +0x1e
    long                      MyPhase;           // +0x20 EMyPhase
private:
    virtual void Init();                          // PDB ?Init@CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
};
}

void NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelMan::Init()
{
    pDSTGame->SetTimer(ParentClass->BarrelTimer, 0);
    ComplainedAboutStock = false;
    MyPhase = 0;
    GSI->EntitySetAsDamageable(Me, false);
    GSI->EntitySetAsKillable(Me, false, false);
    GSI->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    GSI->SetThingHasInformation(Me, false, true, false);
    ParentClass->BarrelBrokenPos = Me.GetHomePos();
    HeroLetMeDown = false;
    OverheardYet = false;
    GSI->EntitySetSightRadius(Me, 10.0f);
}