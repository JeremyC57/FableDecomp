// CMurderTwistSheriff::Init() -- make the sheriff fight the hero (best enemy target), killable, damageable, pushable and moving in actions, then drop any old scripting handles (his own and the parent quest's)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB1EC0, 178 bytes (+ int3 padding). __fastcall, `ret`.
// Callees (CGameScriptInterface virtuals, ghidra_out/cgsi_slots_aligned.tsv): GetHero 70 (+0x118) 0x00891CA0,
//   GiveThingBestEnemyTarget 451 (+0x70c) 0x008A0260, EntitySetAsDamageable 516 (+0x810) 0x0088EAD0, EntitySetAsKillable 517
//   (+0x814) 0x0088EB30, EntitySetAsUseMovementInActions 529 (+0x844) 0x0088EC20, SetIsPushableByHero 844 (+0xd30) 0x008A6DD0
//   (CScriptThing BY VALUE: inline CBaseClass vtable 0x01231710 then CScriptThing vtable 0x01238C8C stored into the argument area,
//   PImp copied with add-ref); the handle's IsValid 0x00CD23B9 / Release 0x00CD2770 (manifest labels
//   "C3DMeshInfo::HasPhysicsMesh" / "CMemoryDataOutputStream::Clear" are propagated; see landed CFH_BriarRose::Main 0x00D37330).
// Class: the catalog label is propagated over four byte-similar Init bodies (no vtable in .rdata references this address);
//   Ego_r.pdb CMurderTwistSheriff (56 bytes) has MyEntityHandle at +0x1c, which is the handle tested first. The parent quest's
//   handle sits at retail ParentClass+0x6c.
// Shape: the nested GetHero call fetches the interface vtable once (ebx); the by-value CScriptThing copy ctor is inline;
//   the parent's handle is a named pointer
//   (esi), the last Release is a tail jump.
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0, implicit copy
struct CScriptThingImp;
class CScriptThing : public CBaseClass { public:                    // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImp> PImp;       // +0x04
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
    virtual ~CScriptThing();
};
class CScriptGameResourceObjectScriptedThingBase { public:
    unsigned char _body[0x10];
    bool IsValid() const;                          // 0x00CD23B9
    void Release();                              // 0x00CD2770
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CV_MurderTwistFinalScript {               // retail offsets
    unsigned char _pad0[0x6c];
    CScriptGameResourceObjectScriptedThingBase SheriffHandle;   // +0x6c
};
struct CQ_SunnyvaleMasterData;
struct CMurderTwistSheriff {
    void*                       __vftable;       // +0x00
    CGameScriptInterface*       Game;            // +0x04
    CScriptThing                Me;              // +0x08
    CV_MurderTwistFinalScript*  ParentClass;     // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;     // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    void Init();
};

void CMurderTwistSheriff::Init()
{
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    Game->EntitySetAsKillable(Me, true, true);
    Game->EntitySetAsDamageable(Me, true);
    Game->SetIsPushableByHero(Me, true);
    Game->EntitySetAsUseMovementInActions(Me, true);
    if (MyEntityHandle.IsValid())
        MyEntityHandle.Release();
    CScriptGameResourceObjectScriptedThingBase* handle = &ParentClass->SheriffHandle;
    if (handle->IsValid())
        handle->Release();
}