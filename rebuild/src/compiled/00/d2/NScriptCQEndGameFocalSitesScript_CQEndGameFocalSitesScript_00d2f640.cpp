// NScript::CQ_EndGameFocalSitesScript::~CQ_EndGameFocalSitesScript() -- implicit quest-script destructor: deregister the timers, destroy the RegionDialogueNames / TeleportHSPs CCharString[5] members in reverse order, then ~CScriptBase
// Leaf: ??1CQ_EndGameFocalSitesScript@NScript@@UAE@XZ
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D2F640, tail jump to ~CScriptBase 0x00CBD510; sole caller the scalar deleting dtor
//   (vtable slot 0) of the RTTI-less quest vtable.
// Identity (proven): the vtable whose slot 0 calls this body is stored only by the ctor 0x00D2A820 (manifest label CQ_EndGameFocalSitesScript::CQ_EndGameFocalSitesScript);
//   that ctor's only caller is the Alloc that `new`s 0x94 bytes, registered by the script-registration routine with the
//   quest name "Q_EndGameFocalSites" (string 0x012C4AE0) -- CQ_EndGameFocalSitesScript is the Q_<name> + "Script" class of that registration. Ego_r.pdb and
//   FableWin.pdb both carry ??1CQ_EndGameFocalSitesScript@NScript@@UAE@XZ (virtual, public: the implicit dtor of a CScriptBase-derived class).
//   The body stores no vtable: the dtor is compiler-generated (no user ~CQ_EndGameFocalSitesScript). The manifest label (CParticleEmitter_Dtor_N)
//   is propagated and wrong.
// Layout: Ego_r.pdb CQ_EndGameFocalSitesScript members with retail offsets (CScriptBase 0x40 in retail vs 0x54 in Ego_r: quest-script members sit
//   0x14 lower; CLevelScriptBase<CQ_SunnyvaleMasterData> Game +0x40 / MasterScriptData +0x44), retail STLport containers.
//   Members destroyed: WorldTimer +0x8c, JackTimer +0x88, RegionDialogueNames[5] +0x60, TeleportHSPs[5] +0x4c (CCharString 0x0099EAE0).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (the
//   CLevelScriptBase<T> scalar deleting dtors 0x00CDD250 / 0x00CDD270 call ~CScriptBase 0x00CBD510 directly, no vtable
//   store; an inline user `~CLevelScriptBase() {}` adds `mov [esi],vftable` before the tail jump -- probe 1 of 0x00D2F640),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt").
// Callees: CTimer::~CTimer inline (engine/CTimer.h: interface slot 88 DeregisterTimer through pDSTGame 0x0143E8F8),
//   ~CScriptBase 0x00CBD510.
// TU: the ctor is defined (as retail's TU defines it), which emits the vtable, the scalar deleting dtor and this implicit dtor.
//   Built without /EHsc (no EH frame; the member-array dtors are inline countdown loops, no `eh vector destructor iterator`).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};

class CCharStringData;
class CCharString { public:
    CCharString();
    ~CCharString();                                                      // 0x0099EAE0
    CCharStringData* PStringData;
};
class C3DVector { public: float X; float Y; float Z; };

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameFocalSitesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_EndGameFocalSitesScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    long        CurrentFocalSiteRegion;                                  // +0x48
    CCharString TeleportHSPs[5];                                         // +0x4c
    CCharString RegionDialogueNames[5];                                  // +0x60
    C3DVector   TeleportPosition;                                        // +0x74
    bool        MissionFailed;                                           // +0x80
    bool        JackLeaves;                                              // +0x81
    bool        EndCutScenePlayed;                                       // +0x82
    bool        QuestStartScreenShown;                                   // +0x83
    long        counter;                                                 // +0x84
    CTimer      JackTimer;                                               // +0x88
    CTimer      WorldTimer;                                              // +0x8c
    bool        JackReached;                                             // +0x90
};
CQ_EndGameFocalSitesScript::CQ_EndGameFocalSitesScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}