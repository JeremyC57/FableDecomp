// NScript::CalextestScript::Main() -- test script: wait (one frame at a time) for a region pre-unload message, then idle until the thread terminates
// Leaf: ?Main@CalextestScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EED0E0, 127 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the level-script vtable 0x012F57E4 (slot 1 = `NScript::CalextestScript::RegisterMain`), stored by 0x00EED0B0; ?Main@CalextestScript@NScript@@UAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00eed0e0" is a gap-scan placeholder.
// Callees: interface slots 24 MsgOnRegionPreunload(CCharString&), 7 NewScriptFrame(); CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 (and the master data +0x44 where the ctor takes one); the script's own members sit 0x14 below their Ego_r.pdb offsets: the interface +0x40 (no members of its own; Ego_r.pdb sizeof 0x5c).
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString();                                        // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CalextestScript : public CScriptBase {
public:
    virtual void Main();
    CGameScriptInterfaceBase* Game;                     // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
};
}

void NScript::CalextestScript::Main()
{
    CCharString region;
    while (!Game->MsgOnRegionPreunload(region)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}