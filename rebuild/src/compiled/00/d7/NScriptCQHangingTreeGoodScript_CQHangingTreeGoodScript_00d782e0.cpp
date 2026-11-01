// NScript::CQ_HangingTreeGoodScript::~CQ_HangingTreeGoodScript() -- implicit quest-script destructor: free GuardsHealthVector, destroy AreasSpawnedIn / RegionsWhereSpawned (strings), CurrentGuardVector / CurrentEnemyVector (script things), ReasonForFailing, CurrentLevel and the four CScriptThing handles in reverse order, then ~CScriptBase
// Leaf: ??1CQ_HangingTreeGoodScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D782E0, 491 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00D782C3 is the scalar deleting destructor 0x00D782C0, slot 0 of the RTTI-less vtable
//   0x012D260C; that vtable is stored only by the ctor 0x00D6EF20 (landed), whose only caller is the Alloc 0x00D78290
//   (`push 0DCh ; call new ; ... call 0x00D6EF20`), stored at 0x00CD65F7 by the script-registration routine beside the save
//   tag "S_QHTG" (0x012F790C); refs/script_recovery/conversion_queue.tsv maps S_QHTG -> Q_HangingTreeGood -> Alloc 0x00D78290:
//   NScript::CQ_HangingTreeGoodScript (Ego_r.pdb / FableWin.pdb ??1CQ_HangingTreeGoodScript@NScript@@UAE@XZ). The body
//   stores no vtable of its own: compiler-generated. The catalog label "CParticleEmitter_Dtor_5" is wrong.
// Layout: Ego_r.pdb CQ_HangingTreeGoodScript at retail offsets (CScriptBase 0x14 smaller, each Dinkumware vector 4 bytes
//   larger than STLport's): Leader +0x68 .. ActiveWayPoint +0x8C, CurrentLevel +0x98, ReasonForFailing +0x9C,
//   CurrentEnemyVector +0xA0, CurrentGuardVector +0xAC, RegionsWhereSpawned +0xB8, AreasSpawnedIn +0xC4, GuardsHealthVector
//   +0xD0; retail size 0xDC (Alloc), as the landed ctor 0x00D6EF20 lays it out. Master data CQ_SunnyvaleMasterData (the
//   landed CHTG_* entity rows cast their data to it; HangingTreeBanditKilled / HangingTreeGuardKilled are its members).
// Callees: ~CScriptThing inline (vtable 0x01238C8C store, engine/CCountedPointer.h PImp release: DeleteFunc, operator delete
//   0x00BFE9BC), CBaseClass::~CBaseClass 0x0099A2E0, free 0x00BFEA14 (STLport allocator<T>::deallocate), ~CCharString
//   0x0099EAE0, ~CScriptBase 0x00CBD510; vector<CScriptThing> elements through the virtual `push 0 ; call [vt]`.
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunks 664/665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// TU: the PDB ctor ??0CQ_HangingTreeGoodScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z is defined with
//   an empty body (retail's TU defines it), which emits the vtable, the scalar deleting dtor and this implicit dtor; only the
//   dtor is graded. Shape: the chunk 673 quest-script recipe (CQ_ArenaScript 0x00CFAC40), first probe.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }
#include "engine/CScriptBase.h"

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
class CQ_HangingTreeGoodScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> { public:
    CQ_HangingTreeGoodScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool LeaderSpokenTo;                                    // +0x48
    bool PartySuccessfullySetUp;                            // +0x49
    bool MissionSucceeded;                                  // +0x4A
    bool MissionFailed;                                     // +0x4B
    bool SwitchingRegions;                                  // +0x4C
    bool ReassignLeaderNow;                                 // +0x4D
    bool PartyDefeated;                                     // +0x4E
    bool SpawnHere;                                         // +0x4F
    bool AtWayPoint;                                        // +0x50
    bool HeroKilledPrisoner;                                // +0x51
    bool QuestStartScreenShown;                             // +0x52
    bool PartyReadyToTransition;                            // +0x53
    bool BanditYelledOut;                                   // +0x54
    bool ExecutionerBeenAttacked;                           // +0x55
    bool ExecutionerBeenAttackedByHero;                     // +0x56
    bool BanditsFlee;                                       // +0x57
    bool PrisonerEscaping;                                  // +0x58
    bool ExitCutsceneStart;                                 // +0x59
    long HTG_State;                                         // +0x5C
    long CurrentWayPoint;                                   // +0x60
    long CurrentGuards;                                     // +0x64
    CScriptThing Leader;                                    // +0x68
    CScriptThing Prisoner;                                  // +0x74
    CScriptThing PartySpawnPoint;                           // +0x80
    CScriptThing ActiveWayPoint;                            // +0x8C
    CCharString  CurrentLevel;                              // +0x98
    CCharString  ReasonForFailing;                          // +0x9C
    std::vector<CScriptThing> CurrentEnemyVector;           // +0xA0
    std::vector<CScriptThing> CurrentGuardVector;           // +0xAC
    std::vector<CCharString>  RegionsWhereSpawned;          // +0xB8
    std::vector<CCharString>  AreasSpawnedIn;               // +0xC4
    std::vector<float>        GuardsHealthVector;           // +0xD0
};
CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00D6EF20
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_HangingTreeGoodScript) == 0xDC);