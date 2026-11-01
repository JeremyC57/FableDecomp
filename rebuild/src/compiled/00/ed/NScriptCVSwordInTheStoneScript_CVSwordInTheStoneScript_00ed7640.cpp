// NScript::CV_SwordInTheStoneScript::~CV_SwordInTheStoneScript() -- implicit quest-script destructor: deregister the crowd sword timer, destroy the two crowd-member CScriptThing members in reverse order, then ~CScriptBase
// Leaf: ??1CV_SwordInTheStoneScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED7640, 147 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00ED7623 is the scalar deleting destructor 0x00ED7620, slot 0 of the RTTI-less vtable
//   0x012F301C; that vtable is stored only by the ctor 0x00ED4AA0 (the landed CV_SwordInTheStoneScript ctor), whose only caller
//   is the Alloc 0x00ED75F0 (`push 80h ; call new ; ... call 0x00ED4AA0`), stored at 0x00CD968A by the script-registration
//   routine right after the level name "V_SwordInTheStone" (string 0x012C5B20) and the save tag "S_VSITS" (0x012F763C): the
//   class is NScript::CV_SwordInTheStoneScript (the V_<name> + "Script" class of that registration; Ego_r.pdb and FableWin.pdb
//   ??1CV_SwordInTheStoneScript@NScript@@UAE@XZ, ?Alloc@CV_SwordInTheStoneScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@
//   AAVCGameScriptInterfaceBase@@@Z). The body stores no vtable of its own: the dtor is compiler-generated. The catalog label
//   is right; the landed ??_G 0x00ED7620 is a CLandscapeBackgroundPatch stand-in.
// Layout: Ego_r.pdb CV_SwordInTheStoneScript members at retail offsets (CScriptBase 0x40 in retail vs 0x54 in Ego_r: members
//   sit 0x14 lower; CLevelScriptBase<CQ_SunnyvaleMasterData> Game +0x40 / MasterScriptData +0x44, as the ctor stores them):
//   SwordRemovalAttemptCount +0x48, the eight bools +0x4C..+0x53, HeroStrengthPhysique/Toughness/Health +0x54..+0x5C,
//   StatsInitialised +0x60, AttackedByHero +0x61, CrowdMemberPullingCurrent +0x64, CrowdMemberPullingNull +0x70 (CScriptThing,
//   vtable 0x01238C8C), CrowdSwordTimer +0x7C (CTimer); retail size 0x80 (the Alloc's `push 80h`).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunks 664/665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt").
// Callees: CTimer::~CTimer inline (engine/CTimer.h: g_pGameScriptInterface 0x0143E8F8 slot 88 DeregisterTimer), ~CScriptThing
//   inline (vtable store, engine/CCountedPointer.h PImp release: the info block's DeleteFunc, operator delete 0x00BFE9BC),
//   CBaseClass::~CBaseClass 0x0099A2E0, ~CScriptBase 0x00CBD510.
// TU: the PDB ctor ??0CV_SwordInTheStoneScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z is defined (as
//   retail's TU defines it), which emits the vtable, the scalar deleting dtor and this implicit dtor.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_SwordInTheStoneScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_SwordInTheStoneScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    long         SwordRemovalAttemptCount;                               // +0x48
    bool         SwordCutscene;                                          // +0x4C
    bool         CrowdAttemptingToPullSword;                             // +0x4D
    bool         CrowdSwordPullActive;                                   // +0x4E
    bool         HeroHasBeenToldSwordLegend;                             // +0x4F
    bool         HeroCongratulated;                                      // +0x50
    bool         CrowdDisperse;                                          // +0x51
    bool         HeroHasTakenSword;                                      // +0x52
    bool         SwordInTheStoneIntroComplete;                           // +0x53
    long         HeroStrengthPhysique;                                   // +0x54
    long         HeroStrengthToughness;                                  // +0x58
    long         HeroStrengthHealth;                                     // +0x5C
    bool         StatsInitialised;                                       // +0x60
    bool         AttackedByHero;                                         // +0x61
    CScriptThing CrowdMemberPullingCurrent;                              // +0x64
    CScriptThing CrowdMemberPullingNull;                                 // +0x70
    CTimer       CrowdSwordTimer;                                        // +0x7C
};
CV_SwordInTheStoneScript::CV_SwordInTheStoneScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00ED4AA0
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}