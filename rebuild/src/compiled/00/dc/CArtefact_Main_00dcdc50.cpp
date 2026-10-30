// Levers: ~CScriptThing __forceinline; `#pragma inline_depth(0)` around the CIsThingAlive class definition keeps its member dtor a call inside the inlined filter dtor; for(;;) with the terminating return first; in-body inline_depth(0) region from the carried test to the end of the near branch makes those four exits call 0x004AA840 (tail-merged); a `parent` local for the raid script (one ParentClass load, count read/decrement and MasterData read before the store).
// CArtefact::Main() -- the orchard-farm-raid crate artefact: hold an alive filter over Me, find the bandit team's crate drop, then every frame while the hero is near it: if the hero is not carrying the artefact, count it off the parent raid, flag the master data and remove the artefact; track whether it was near on the previous frame
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCDC50, 454 bytes (464 with padding); entity-script vtable slot 1 (Main).
// Leaf: Main
// Callees: CScriptThing copy ctor 0x004ABE90 / ~CScriptThing 0x004AA840 (also inlined at three exits), ~CBaseClass
//   0x0099A2E0, operator delete 0x00BFE9BC (the counted pointer's info block), CEntityScriptBase::SetPredicate 0x00F35B10 /
//   IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsDistanceBetweenThingsUnder 0x00CBE2FF
//   (__fastcall); CScriptThing::IsBeingCarriedBy slot 63 (+0xfc) on Me; CGameScriptInterface NewScriptFrame 7,
//   GetThingWithScriptName 72 (by value), RemoveThing 108.
// Literals: "BanditTeamCrateDrop" 0x012DB214, "SCRIPT_NAME_HERO" 0x0125D1C8.
// Layout: Ego_r.pdb CArtefact (28 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_OrchardFarmRaidScript: master data +0x44 with a flag at +0x109, a remaining-artefact count at +0x48).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC

class CCPPointerInfo { public:                                // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class __declspec(novtable) CPredicateBase
{
public:
    virtual void v00();
};
}
#pragma inline_depth(0)
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
#pragma inline_depth()

struct SRaidMasterData { unsigned char _pad[0x109]; bool ArtefactTaken; };
struct CQ_OrchardFarmRaidScript {
    unsigned char    _pad[0x44];
    SRaidMasterData* MasterData;                       // +0x44
    long             ArtefactsRemaining;               // +0x48
};

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

struct CArtefact : CEntityScriptBase {
    CQ_OrchardFarmRaidScript* ParentClass;             // +0x14
    void* PMasterData;                                 // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};

void CArtefact::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    bool wasNear = false;
    CScriptThing crate = Game->GetThingWithScriptName("BanditTeamCrateDrop");
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Me, crate, 3.0f)) {
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth(0)
            bool carried = Me.IsBeingCarriedBy("SCRIPT_NAME_HERO");
            if (!carried) {
                if (IsActiveThreadTerminating())
                    return;
                CQ_OrchardFarmRaidScript* parent = ParentClass;
                --parent->ArtefactsRemaining;
                parent->MasterData->ArtefactTaken = true;
                Game->RemoveThing(Me, false, true);
                return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (!wasNear) {
                if (IsActiveThreadTerminating())
                    return;
                wasNear = true;
            }
#pragma inline_depth()
        } else {
            if (IsActiveThreadTerminating())
                return;
            wasNear = false;
        }
        Game->NewScriptFrame();
    }
}