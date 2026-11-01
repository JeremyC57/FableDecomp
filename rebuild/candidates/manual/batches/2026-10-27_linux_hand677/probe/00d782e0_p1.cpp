// NScript::CQ_HangingTreeGoodScript::~CQ_HangingTreeGoodScript() -- implicit quest-script destructor: free GuardsHealthVector, destroy AreasSpawnedIn / RegionsWhereSpawned (strings), CurrentGuardVector / CurrentEnemyVector (script things), ReasonForFailing, CurrentLevel and the four CScriptThing handles in reverse order, then ~CScriptBase
// Leaf: ??1CQ_HangingTreeGoodScript@NScript@@UAE@XZ
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
