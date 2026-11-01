// NScript::CQ_OrchardFarmRaidScript::~CQ_OrchardFarmRaidScript() -- implicit quest-script destructor: deregister the timers, destroy the Teams / FailReasons arrays, CrateList vector members in reverse order, then ~CScriptBase
// Leaf: ??1CQ_OrchardFarmRaidScript@NScript@@UAE@XZ
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD1F40, tail jump to ~CScriptBase 0x00CBD510; sole caller the scalar deleting dtor
//   (vtable slot 0) of the RTTI-less quest vtable.
// Identity (proven): the vtable whose slot 0 calls this body is stored only by the ctor 0x00DCC040 (manifest label CQ_OrchardFarmRaidScript::CQ_OrchardFarmRaidScript);
//   that ctor's only caller is the Alloc that `new`s 0x10C bytes, registered by the script-registration routine with the
//   quest name "Q_OrchardFarmRaid" (string 0x012C32DC) -- CQ_OrchardFarmRaidScript is the Q_<name> + "Script" class of that registration. Ego_r.pdb and
//   FableWin.pdb both carry ??1CQ_OrchardFarmRaidScript@NScript@@UAE@XZ (virtual, public: the implicit dtor of a CScriptBase-derived class).
//   The body stores no vtable: the dtor is compiler-generated (no user ~CQ_OrchardFarmRaidScript). The manifest label (CParticleEmitter_Dtor_N)
//   is propagated and wrong.
// Layout: Ego_r.pdb CQ_OrchardFarmRaidScript members with retail offsets (CScriptBase 0x40 in retail vs 0x54 in Ego_r: quest-script members sit
//   0x14 lower; CLevelScriptBase<CQ_SunnyvaleMasterData> Game +0x40 / MasterScriptData +0x44), retail STLport containers.
//   Members destroyed: Teams CCrateTeamManager[2] +0x8c (out-of-line ??1CCrateTeamManager@CQ_OrchardFarmRaidScript@NScript@@QAE@XZ
//   0x00CDB3C0, Ego_r.pdb 0x40), TextSystemScriptCode +0x84, RemindHeroOfObjectivesTimer +0x80, FailReasons CWideString[4] +0x68
//   (0x0099B510), CrateList std::vector<CScriptThing> +0x54, CommentTimer +0x4c. MissionFailed (+0x64) is Ego_r.pdb
//   `<unnamed-type-MissionFailed>` (4 bytes, modelled as an unnamed enum; its real shape is unproven and does not reach the bytes).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (the
//   CLevelScriptBase<T> scalar deleting dtors 0x00CDD250 / 0x00CDD270 call ~CScriptBase 0x00CBD510 directly, no vtable
//   store; an inline user `~CLevelScriptBase() {}` adds `mov [esi],vftable` before the tail jump -- probe 1 of 0x00D2F640),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt").
// Callees: CTimer::~CTimer inline (engine/CTimer.h: interface slot 88 DeregisterTimer through pDSTGame 0x0143E8F8),
//   ~CScriptBase 0x00CBD510.
// TU: the ctor is defined (as retail's TU defines it), which emits the vtable, the scalar deleting dtor and this implicit dtor.
//   Built without /EHsc (no EH frame; the member-array dtors are inline countdown loops, no `eh vector destructor iterator`).
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
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
    ~CCharString();                                                      // 0x0099EAE0
    CCharStringData* PStringData;
};
class CWideStringData;
class CWideString { public:
    ~CWideString();                                                      // 0x0099B510
    CWideStringData* PStringData;
};
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_construct.h, stl/_pair.h, stl/_function_base.h, stl/_tree.h (the members this row needs;
// the malloc-backed allocator of this build: deallocate = free 0x00BFEA14)
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
}
namespace std {
// STLport 4.6 stl/_construct.h, stl/_vector.h (the members this row needs)
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
protected:
    _Tp* _M_start;                                               // +0x00
    _Tp* _M_finish;                                              // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OrchardFarmRaidScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CCrateTeamManager {                                            // Ego_r.pdb, 0x40
    public:
        CCrateTeamManager();
        ~CCrateTeamManager();                                            // 0x00CDB3C0
        long               StateCounter[6];                              // +0x00
        long               MemberCount;                                  // +0x18
        CTimer             TeamReinforcementsTimer;                      // +0x1c
        long               MaxTeamMembers;                               // +0x20
        CScriptThing       CrateDropPos;                                 // +0x24
        CScriptThing       TeamCrateCarrier;                             // +0x30
        CCrateTeamManager* EnemyTeam;                                    // +0x3c
    };
    CQ_OrchardFarmRaidScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    long CrateCount;                                                     // +0x48
    CTimer CommentTimer;                                                 // +0x4c
    bool DoneIntroduction;                                               // +0x50
    bool HeroAtWrongEntrance;                                            // +0x51
    bool WhisperSpawned;                                                 // +0x52
    std::vector<CScriptThing> CrateList;                                 // +0x54
    bool MissionSucceeded;                                               // +0x60
    enum { } MissionFailed;                                              // +0x64 (Ego_r.pdb <unnamed-type-MissionFailed>, 4 bytes)
    CWideString FailReasons[4];                                          // +0x68
    long HeroTeam;                                                       // +0x78
    long BanditWavesSpawned;                                             // +0x7c
    CTimer RemindHeroOfObjectivesTimer;                                  // +0x80
    CCharString TextSystemScriptCode;                                    // +0x84
    bool HeroMetWhisperBeforeFarm;                                       // +0x88
    bool ShownCombatMultiplierTutorial;                                  // +0x89
    bool ShownCombatFlourishTutorial;                                    // +0x8a
    bool WhisperInCutscene;                                              // +0x8b
    CCrateTeamManager Teams[2];                                          // +0x8c
};
CQ_OrchardFarmRaidScript::CQ_OrchardFarmRaidScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00DCC040
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}