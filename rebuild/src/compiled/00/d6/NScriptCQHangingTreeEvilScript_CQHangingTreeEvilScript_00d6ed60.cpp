// NScript::CQ_HangingTreeEvilScript::~CQ_HangingTreeEvilScript() -- implicit quest-script destructor: destroy AreasSpawnedIn (strings), CurrentBanditVector / CurrentEnemyVector (script things), CurrentLevel, deregister MissionTimer, the four CScriptThing handles and ReasonForFailing in reverse order, then ~CScriptBase
// Leaf: ??1CQ_HangingTreeEvilScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D6ED60, 446 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00D6ED43 is the scalar deleting destructor 0x00D6ED40, slot 0 of the RTTI-less vtable
//   0x012D1F98 (slots: dtor, RegisterMain 0x00D69390, Main 0x00D69480, Init 0x00D68FF0, OnPersist 0x00D69450 -- all landed as
//   NScript::CQ_HangingTreeEvilScript members); that vtable is stored only by the landed ctor 0x00D68F00, whose caller is the Alloc
//   0x00D6ED10 (`push 0D0h`) registered under "S_QHTE" / Q_HangingTreeEvil; refs/script_recovery/conversion_queue.tsv maps
//   S_QHTE -> Q_HangingTreeEvil -> 0x00D6ED10. Both PDBs spell the nested classes ...@CQ_HangingTreeEvilScript@NScript@@ (the
//   outer class's own members are inlined in the donors): ??1CQ_HangingTreeEvilScript@NScript@@UAE@XZ by the sibling
//   ??1CQ_HangingTreeGoodScript@NScript@@UAE@XZ (landed 0x00D782E0). The body stores no vtable of its own: compiler-generated.
//   The catalog label is right (CQ_HangingTreeEvilScript::~CQ_HangingTreeEvilScript, without the namespace).
// Layout: the landed ctor 0x00D68F00 (Ego_r.pdb names at retail offsets: CScriptBase 0x14 smaller, STLport vectors 12): bools
//   +0x48..+0x58, ReasonForFailing +0x5C, HTE_State .. CarryOverGuards +0x60..+0x70, Executioner +0x74, Prisoner +0x80,
//   PartySpawnPoint +0x8C, BanditExitPoint +0x98, MissionTimer +0xA4, CurrentLevel +0xA8, CurrentEnemyVector +0xAC,
//   CurrentBanditVector +0xB8, AreasSpawnedIn +0xC4; size 0xD0 (Alloc). Master data CQ_SunnyvaleMasterData (the landed CHTE_*
//   entity rows and the sibling Good script use CLevelScriptBase<CQ_SunnyvaleMasterData>).
// Callees: ~CCharString 0x0099EAE0, vector<CScriptThing> elements through the virtual `push 0 ; call [vt]`, free 0x00BFEA14
//   (STLport allocator<T>::deallocate), g_pGameScriptInterface->DeregisterTimer (slot 88, +0x160) from the inline ~CTimer,
//   ~CScriptThing inline (vtable 0x01238C8C store, engine/CCountedPointer.h PImp release: DeleteFunc, operator delete 0x00BFE9BC),
//   CBaseClass::~CBaseClass 0x0099A2E0, ~CScriptBase 0x00CBD510.
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunks 664/665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). engine/CTimer.h
//   declares ~CTimer out of line (0x00CD4470); this TU expands it inline as retail does here (defined after the include). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// TU: the PDB-pattern ctor ??0CQ_HangingTreeEvilScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z is defined
//   with an empty body (retail's TU defines it, landed 0x00D68F00), which emits the vtable, the scalar deleting dtor and this implicit
//   dtor; only the dtor is graded. Shape: the landed ~CQ_HangingTreeGoodScript 0x00D782E0 recipe plus the CTimer member.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }
#include "engine/CScriptBase.h"
#include "engine/CTimer.h"
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }

extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
namespace std {
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

class CCharStringData;
class CCharString { public:
    CCharString();
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};

class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase { public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                         // +0x40
    T*                        MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> { public:
    CQ_HangingTreeEvilScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool IntroCutscenePlayed;                               // +0x48
    bool BanditsStarted;                                    // +0x49
    bool BanditKilled;                                      // +0x4A
    bool PartySuccessfullySetUp;                            // +0x4B
    bool MissionSucceeded;                                  // +0x4C
    bool MissionFailed;                                     // +0x4D
    bool BanditsLeftRegion;                                 // +0x4E
    bool SpawnHere;                                         // +0x4F
    bool AtWayPoint;                                        // +0x50
    bool SwitchingRegions;                                  // +0x51
    bool RemoveBandits;                                     // +0x52
    bool AllEnemiesDead;                                    // +0x53
    bool QuestStartScreenShown;                             // +0x54
    bool BanditYelledRecently;                              // +0x55
    bool ExecutionerInterrupted;                            // +0x56
    bool AudienceLeggit;                                    // +0x57
    bool ShouldFollowHero;                                  // +0x58
    CCharString ReasonForFailing;                           // +0x5C
    long HTE_State;                                         // +0x60
    long CurrentBandits;                                    // +0x64
    long BanditsAtNextRegion;                               // +0x68
    long TimerGUIIndex;                                     // +0x6C
    long CarryOverGuards;                                   // +0x70
    CScriptThing Executioner;                               // +0x74
    CScriptThing Prisoner;                                  // +0x80
    CScriptThing PartySpawnPoint;                           // +0x8C
    CScriptThing BanditExitPoint;                           // +0x98
    CTimer MissionTimer;                                    // +0xA4
    CCharString CurrentLevel;                               // +0xA8
    std::vector<CScriptThing> CurrentEnemyVector;           // +0xAC
    std::vector<CScriptThing> CurrentBanditVector;          // +0xB8
    std::vector<CCharString>  AreasSpawnedIn;               // +0xC4
};
CQ_HangingTreeEvilScript::CQ_HangingTreeEvilScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00D68F00 (landed)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HangingTreeEvilScript) == 0xD0);