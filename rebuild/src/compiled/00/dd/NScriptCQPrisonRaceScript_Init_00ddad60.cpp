// NScript::CQ_PrisonRaceScript::Init() -- reset the race: flags false, speeds and indices 0, nearest-marker distance 1000, conversation index -1
// Leaf: ?Init@CQ_PrisonRaceScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DDAD60, 35 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the quest vtable 0x012DC88C (slot 1 = `NScript::CQ_PrisonRaceScript::RegisterMain`), stored by the quest ctor 0x00DDAD30; ?Init@CQ_PrisonRaceScript@NScript@@UAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00ddad60" is a gap-scan placeholder.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 and the master data +0x44; the quest's own members sit 0x14 below their Ego_r.pdb offsets until the first STL container. HeroWonRace +0x48, RaceStarted +0x49, RaceEnded +0x4a, RacerSpeed +0x4c, HeroWaypointIndex +0x50, NearestRaceMarkerDistanceToHero +0x54, NearestRaceMarker +0x58, ConversationIndex +0x5c.
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
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonRaceScript : public CScriptBase {
public:
    virtual void Init();
    CGameScriptInterfaceBase* Game;                     // +0x40 (the ctor's CGameScriptInterfaceBase&; descriptive name)
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44 (the ctor's CScriptDataBase*; descriptive name)
    bool HeroWonRace;                                   // +0x48
    bool RaceStarted;                                   // +0x49
    bool RaceEnded;                                     // +0x4a
    unsigned char _pad04b[0x1];
    long RacerSpeed;                                    // +0x4c
    long HeroWaypointIndex;                             // +0x50
    float NearestRaceMarkerDistanceToHero;              // +0x54
    long NearestRaceMarker;                             // +0x58
    long ConversationIndex;                             // +0x5c
};
}

void NScript::CQ_PrisonRaceScript::Init()
{
    HeroWonRace = false;
    RaceStarted = false;
    RaceEnded = false;
    RacerSpeed = 0;
    HeroWaypointIndex = 0;
    NearestRaceMarkerDistanceToHero = 1000.0f;
    NearestRaceMarker = 0;
    ConversationIndex = -1;
}