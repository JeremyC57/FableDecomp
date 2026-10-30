// NScript::CQ_HerosOldHouseScript::CExtraBooty::Main() -- wait (frame by frame) while the digging spot is still enabled, then mark the booty dug up and, unless the wife was attacked, complete the active quest
// Leaf: ?Main@CExtraBooty@CQ_HerosOldHouseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D8A8A0, 147 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main) of the RTTI-less vtable 0x012D50F8, which the entity Alloc 0x00D8A7F0 (`NScript::CQ_HerosOldHouseScript::CExtraBooty::Alloc`) stores; ?Main@CExtraBooty@CQ_HerosOldHouseScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d8a8a0" is a gap-scan placeholder.
// Callees: CEntityScriptBase::IsActiveThreadTerminating() const 0x00F35B30, ~CCharString 0x0099EAE0; interface slots 576 IsDiggingSpotEnabled, 7 NewScriptFrame, 655 GetActiveQuestName (by-value result), 288 SetQuestAsCompleted.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). Parent WifeAttacked +0x4b, BootyDugUp +0x4d (Ego_r.pdb +0x5f/+0x61).
// Shape: for Game->SetQuestAsCompleted(Game->GetActiveQuestName(), ...) VC7.1 loads the outer vtable before the inner call.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HerosOldHouseScript {
public:
    unsigned char _pad000[0x4b];
    bool WifeAttacked;                                  // +0x4b (Ego_r.pdb +0x5f)
    bool WifeAttackedAndLeft;                           // +0x4c
    bool BootyDugUp;                                    // +0x4d (Ego_r.pdb +0x61)
    class CExtraBooty;
};
class CQ_HerosOldHouseScript::CExtraBooty : public CEntityScriptBase {
public:
    CQ_HerosOldHouseScript*   ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void Main();
};
}

void NScript::CQ_HerosOldHouseScript::CExtraBooty::Main()
{
    while (Game->IsDiggingSpotEnabled(Me)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    ParentClass->BootyDugUp = true;
    if (ParentClass->WifeAttacked) return;
    if (IsActiveThreadTerminating()) return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
}