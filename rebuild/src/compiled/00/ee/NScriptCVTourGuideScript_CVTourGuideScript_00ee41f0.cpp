// NScript::CV_TourGuideScript::CV_TourGuideScript(CScriptDataBase* data, CGameScriptInterface& game) -- the tour-guide village script's ctor: script base and interface/data pair, vtable 0x012F46B8, the 18 waypoint records (three strings each), the three five-string response tables and the NextTourWaypoint handle
// Leaf: ??0CV_TourGuideScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterface@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE41F0, 176 bytes, `ret 8`. Its only caller is the factory 0x00EE6C00
//   (`push 178h`, CV_TourGuideScript::Alloc) registered at 0x00CD97BC under "S_VTG" / "V_TourGuide". PDB:
//   ??0CV_TourGuideScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CCharString::CCharString() @ 0x0099E4B0 (3 x 18 + 3 x 5),
//   CBaseClass::CBaseClass @ 0x0099A2D0 (NextTourWaypoint's base; CScriptThing vtable 0x01238C8C).
// Layout: Ego_r.pdb CV_TourGuideScript (396 bytes) minus 0x14 (the retail script base): WaypointInfo[18] +0x48 (WaypointData
//   = locMarker/locTextOverheard/locTextRequested), seven bools +0x120..+0x126, RandomGuideResponse[5] +0x128,
//   RandomFollowerResponseM[5] +0x13c, RandomFollowerResponseF[5] +0x150, WaypointCounter +0x164, NextTourWaypoint +0x168,
//   two bools +0x174; retail sizeof 0x178 (`push 178h`).
// Shape: the landed quest-script ctors (CQ_HangingTreeEvilScript 0x00D68F00): the argument pair is stored by the inlined
//   `__declspec(novtable)` CLevelScriptBase<T> before the derived vtable store; arrays of classes with an inline constructor
//   and no destructor in the TU become inline counted loops (`mov ebx,12h` ... `dec ebx ; jne`); the scalars stay
//   uninitialised; CScriptThing() is CBaseClass() + its vtable + the two zeroed counted-pointer words.
class CGameScriptInterface;
class CScriptDataBase;
class CScriptBase { public:                                    // 0x40 bytes, the quest-script root
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04..+0x3f
    CScriptBase();                                      // 0x00CB8110
};
template <class TData>
struct __declspec(novtable) CLevelScriptBase : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterface& game) : CScriptBase(), Game(&game), MasterScriptData((TData*)data) { }
};
class CCharString { public: void* PStringData; CCharString(); };   // default ctor 0x0099E4B0
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
template<class T> class CCountedPointer { public: T* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    CScriptThing() : CBaseClass(), PImp() { }
    virtual ~CScriptThing();
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TourGuideScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {   // vtable 0x012F46B8
public:
    struct WaypointData {                               // 12 bytes
        CCharString locMarker;                          // +0x0
        CCharString locTextOverheard;                   // +0x4
        CCharString locTextRequested;                   // +0x8
        WaypointData() { }
    };
    WaypointData WaypointInfo[18];                      // +0x48
    bool TourGuideKilled;                               // +0x120
    bool AllFollowersDead;
    bool QuestFinished;
    bool TourFinished;
    bool QuestActivated;
    bool Initialise;
    bool SpawnedQuestFinishThread;                      // +0x126
    CCharString RandomGuideResponse[5];                 // +0x128
    CCharString RandomFollowerResponseM[5];             // +0x13c
    CCharString RandomFollowerResponseF[5];             // +0x150
    long WaypointCounter;                               // +0x164
    CScriptThing NextTourWaypoint;                      // +0x168
    bool GuideSpokenToHeroThisWaypoint;                 // +0x174
    bool OverheardTourGuideThisWaypoint;                // +0x175
    virtual ~CV_TourGuideScript();
    CV_TourGuideScript(CScriptDataBase* data, CGameScriptInterface& game);
};
CV_TourGuideScript::CV_TourGuideScript(CScriptDataBase* data, CGameScriptInterface& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}