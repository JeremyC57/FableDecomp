// NScript::CQ_TraderEscortScript::CEndTrader::OnPredicateFail() -- the end trader was killed after the ending could start: flag EndStarted in the parent
// Leaf: ?OnPredicateFail@CEndTrader@CQ_TraderEscortScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E04AE0, 68 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012DF748, which the entity Alloc 0x00E04A30 (`NScript::CQ_TraderEscortScript::CEndTrader::Alloc`) stores; ?OnPredicateFail@CEndTrader@CQ_TraderEscortScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00e04ae0" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::MsgIsKilledBy (slot 18, +0x48; CScriptThing vtable 0x01238C8C).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. CEndTrader: EndingCanStart +0x1c; parent EndStarted +0x58 (Ego_r.pdb +0x6c).
// Shape: two nested ifs, not `&&`: the "" temporary dies right after the first test (`&&` adds a conditional-destruction flag store and a merged bool).
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
class CGameScriptInterfaceBase;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderEscortScript {
public:
    class CEndTrader;
    unsigned char _pad000[0x58];
    bool EndStarted;                                    // +0x58 (retail = Ego_r.pdb - 0x14)
};
class CQ_TraderEscortScript::CEndTrader : public CEntityScriptBase {
public:
    CQ_TraderEscortScript*    ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    bool                      EndingCanStart;             // +0x1c
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_TraderEscortScript::CEndTrader::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        if (EndingCanStart)
            ParentClass->EndStarted = true;
    }
}