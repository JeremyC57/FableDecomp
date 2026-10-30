// CNOVI_BarrelMan::Init() -- restart the barrel man: reset his phase and flags, clear the quest's barrel timer, make him an unkillable informant, remember his home as the broken-barrel spot and give him a 10m sight radius
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB5260, 167 bytes (+ int3 padding). __fastcall, `ret`.
// Callees: the global script interface g_pGameScriptInterface (0x0143E8F8) SetTimer 89 (+0x164) 0x006E76E0; through the entity
//   script's own interface pointer: EntitySetAsDamageable 516 (+0x810) 0x0088EAD0, EntitySetAsKillable 517 (+0x814) 0x0088EB30,
//   EntitySetAsToAddToComboMultiplierWhenHit 526 (+0x838) 0x0088EBC0, SetThingHasInformation 360 (+0x5a0) 0x0088E440,
//   EntitySetSightRadius 640 (+0xa00) 0x0089D220 (ghidra_out/cgsi_slots_aligned.tsv); CScriptThing::GetHomePos slot 7 (+0x1c)
//   0x004AA9A0 (vtable 0x01238C8C), returned by value into a frame temporary.
// Layout: Ego_r.pdb CNOVI_BarrelMan (36 bytes): ParentClass +0x14, ComplainedAboutStock +0x1c, HeroLetMeDown +0x1d,
//   OverheardYet +0x1e, MyPhase +0x20. The parent CQ_NewOakValeIntroScript is byte-packed (the PDB has BarrelBrokenPos at the
//   unaligned +0x8a); in retail it sits at +0x85 and the quest's barrel timer handle at +0x108 (retail offsets, local model).
// Shape: one `xor ebx,ebx` zero feeds every false/0 argument and store; the home position is copied with the implicit
//   C3DVector assignment (one `add edx,0x85` destination, three dword moves).
class C3DVector { public: float X, Y, Z; };
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
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
#pragma pack(push, 1)
struct CQ_NewOakValeIntroScript {                // retail offsets (packed quest script)
    unsigned char _pad0[0x85];
    C3DVector     BarrelBrokenPos;               // +0x85
    unsigned char _pad91[0x108 - 0x91];
    long          BarrelTimer;                   // +0x108 (CTimer handle)
};
#pragma pack(pop)
struct CQ_SunnyvaleMasterData;
struct CNOVI_BarrelMan {
    virtual void v00();
    CGameScriptInterface*     GSI;               // +0x04
    CScriptThing              Me;                // +0x08
    CQ_NewOakValeIntroScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;       // +0x18
    bool                      ComplainedAboutStock;   // +0x1c
    bool                      HeroLetMeDown;     // +0x1d
    bool                      OverheardYet;      // +0x1e
    long                      MyPhase;           // +0x20 EMyPhase
    void Init();
};

void CNOVI_BarrelMan::Init()
{
    g_pGameScriptInterface->SetTimer(ParentClass->BarrelTimer, 0);
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