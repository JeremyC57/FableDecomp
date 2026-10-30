// NScript::CV_RockTrollFirstEncounterScript::Init() -- switch off the Witchwood1 creature generators and reset the rock troll mission state
// Leaf: ?Init@CV_RockTrollFirstEncounterScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC3C50, 77 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012F15C4, which the landed NScript::CV_RockTrollFirstEncounterScript::CV_RockTrollFirstEncounterScript 0x00EC3C20 (and the landed Alloc 0x00EC5360) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_RockTrollFirstEncounterScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_RockTrollFirstEncounterScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("Witchwood1" 0x012CC268); interface slot 558 SetCreatureGeneratorsEnabled.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_RockTrollFirstEncounterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool MissionOver;                                   // +0x4a
    bool TrollAlive;                                    // +0x4b
    bool TrollAwake;                                    // +0x4c
    bool Activated;                                     // +0x4d
    bool RockTrollTriggered;                            // +0x4e
    long RockTrollHealthBarID;                          // +0x50
    bool PlayedExhumeCutScene;                          // +0x54
    bool AddedItemsToRockTroll;                         // +0x55
    virtual void Init();
};

void CV_RockTrollFirstEncounterScript::Init()
{
    Game->SetCreatureGeneratorsEnabled("Witchwood1", false);
    MissionSucceeded = false;
    MissionFailed = false;
    MissionOver = false;
    Activated = false;
    RockTrollTriggered = false;
    PlayedExhumeCutScene = false;
    AddedItemsToRockTroll = false;
    RockTrollHealthBarID = 0;
}
}