// NScript::CQ_DragonBossFightScript::CDragon::OnPredicateFail() -- the dragon's predicate failed: if it was killed, put the boss fight in its dead state (DragonState = 4)
// Grade: DIFFER(64v64) -- one register: retail loads ParentClass into the dead `this` register (mov esi,[esi+14h]; mov [esi+48h],4), ours into eax (mov eax,[esi+14h]; mov [eax+48h],4). 17 shape probes + /Og- + QFE (probe/00d259a0.log); only a guarded read-before-store (p15) reproduces esi. Next: find the real source of a plain store that VC7.1 allocates like a read-modify-write (all landed siblings with esi are `--Parent->X` / `+=`; every plain-store sibling uses eax).
// Leaf: ?OnPredicateFail@CDragon@CQ_DragonBossFightScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D259A0, 64 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012CB770 (slot 1 the landed CDragon::Main 0x00D25A00,
//   slot 2 the landed CDragon::Init 0x00D25880, stored by the landed CDragon::Alloc 0x00D257D0; the "Dragon" entity name sits
//   just before it at 0x012CB768); Ego_r.pdb and FableWin.pdb ?OnPredicateFail@CDragon@CQ_DragonBossFightScript@NScript@@EAEXXZ.
//   The catalog label "_gapscan::sub_00d259a0" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::MsgIsKilledBy (slot 18, +0x48).
// Data: "" @ 0x0122D70E.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} (engine header), CDragon ParentClass +0x14 / PMasterData +0x18 / the three
//   health thresholds +0x1C..+0x24 (as the landed CDragon::Init); parent DragonState +0x48 (Ego_r 92 - 0x14, as the landed
//   SetDragonState 0x00D258C0). EDragonState enumerators: neither PDB name list carries them; the first four are the landed
//   SetDragonState spellings, DRAGON_DEAD (= 4, the value stored here) is descriptive.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
class CGameScriptInterfaceBase;
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_DragonBossFightScript : public CScriptBase {
public:
    enum EDragonState {
        DRAGON_HIGH_HEALTH,
        DRAGON_MEDIUM_HEALTH,
        DRAGON_LOW_HEALTH,
        DRAGON_VERY_LOW_HEALTH,
        DRAGON_DEAD
    };
    class CDragon;
    void*         Game;                                 // +0x40
    void*         MasterScriptData;                     // +0x44
    EDragonState  DragonState;                          // +0x48
};
class CQ_DragonBossFightScript::CDragon : public CEntityScriptBase {
public:
    CQ_DragonBossFightScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    float MediumHealth;                                   // +0x1c
    float LowHealth;                                      // +0x20
    float VeryLowHealth;                                  // +0x24
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_DragonBossFightScript::CDragon::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
        ParentClass->DragonState = DRAGON_DEAD;
}
