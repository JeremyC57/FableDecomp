// NScript::CQ_MinionCampScript::CMC_Minion::Init() -- a minion spawns: count it in the camp's NumberOfMinions and top Me up to full health (100 minus its current health)
// Leaf: ?Init@CMC_Minion@CQ_MinionCampScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D9F160, 56 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 of the vtable 0x012D6DAC that the landed CMC_Minion::Alloc 0x00D9F0B0 stores (slot 0
//   ??_G 0x00D9F250, slot 1 the landed Main 0x00D9F270); Ego_r.pdb ?Init@CMC_Minion@CQ_MinionCampScript@NScript@@EAEXXZ.
// Callees: CGameScriptInterface slots 264 GetHealth (`?GetHealth@CGameScriptInterface@@UBEMABVCScriptThing@@@Z`) and
//   265 ModifyThingHealth (`...@UBEXABVCScriptThing@@M_N@Z`), ghidra_out/cgsi_slots_aligned.tsv; the constant 100.0f
//   @ 0x01230A08.
// Layout: Ego_r.pdb CMC_Minion (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18;
//   CQ_MinionCampScript.NumberOfMinions is Ego_r +0x64, retail +0x50 (level/quest script members sit 0x14 lower).
// Shape: right-to-left arguments: ModifyThingHealth's `false` is pushed before GetHealth runs; `fsubr [100.0]`.
class CScriptThing {                                    // vtable 0x01238C8C, 12 bytes
public:
    virtual ~CScriptThing();
    void* PImp[2];                                      // +0x04 CCountedPointer<CScriptThingImpl>
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CPersistContext;
class CEntityScriptBase {
public:
    virtual ~CEntityScriptBase();
    virtual void Main();
    virtual void Init();
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_MinionCampScript {
public:
    unsigned char _pad00[0x50];
    long NumberOfMinions;                               // +0x50 (Ego_r +0x64)
    class CMC_Minion : public CEntityScriptBase {
    public:
        CQ_MinionCampScript* ParentClass;               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    private:
        virtual void Init();
    };
};

void CQ_MinionCampScript::CMC_Minion::Init()
{
    ParentClass->NumberOfMinions++;
    Game->ModifyThingHealth(Me, 100.0f - Game->GetHealth(Me), false);
}
}