// NScript::CQ_MinionClifftopChaseScript::CMinionEscort::Init() -- a minion escort spawned while PrisonPath is loaded is special: count it in the parent; otherwise not special
// Leaf: ?Init@CMinionEscort@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA4540, 71 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012D7420, which the entity Alloc 0x00DA4490 (`NScript::CQ_MinionClifftopChaseScript::CMinionEscort::Alloc`) stores; ?Init@CMinionEscort@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00da4540" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slot 12 IsRegionLoaded(const CCharString&).
// Data: "PrisonPath" @ 0x01238FB8.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. CMinionEscort: SpecialMinion +0x1c; parent MinionCount +0x68.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_MinionClifftopChaseScript {
public:
    class CMinionEscort;
    unsigned char _pad000[0x68];
    long MinionCount;                                   // +0x68 (retail = Ego_r.pdb - 0x14)
};
class CQ_MinionClifftopChaseScript::CMinionEscort : public CEntityScriptBase {
public:
    CQ_MinionClifftopChaseScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    bool                      SpecialMinion;              // +0x1c
private:
    virtual void Init();
};
}

void NScript::CQ_MinionClifftopChaseScript::CMinionEscort::Init()
{
    if (Game->IsRegionLoaded("PrisonPath")) {
        ++ParentClass->MinionCount;
        SpecialMinion = true;
    } else {
        SpecialMinion = false;
    }
}