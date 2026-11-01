// NScript::CQR_EscortTraderScript::~CQR_EscortTraderScript() -- implicit repeatable-quest destructor: destroy CurrentLoadedRegion / PreviousLoadedRegion, the RouteStatic[3] / RouteDynamic[3] vector<CCharString> arrays, RandomSpeechWait[5] / RandomSpeechFollow[10], deregister the four trader timers, destroy BonusObject, then ~CScriptBase
// Leaf: ??1CQR_EscortTraderScript@NScript@@UAE@XZ
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E24C80, 321 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510; sole caller the
//   scalar deleting dtor 0x00E24C60 (slot 0 of the RTTI-less quest vtable 0x012E12E0).
// Identity (proven): vtable 0x012E12E0 is stored only by the ctor 0x00E1DB00, whose only caller is the Alloc 0x00E24C30
//   (`push 120h ; call new ; ... call 0x00E1DB00`); CScriptInfoManager::RegisterScripts stores that Alloc at 0x00CD7B7B right
//   after `push "S_QRET"` (0x012F77EC); refs/script_recovery/conversion_queue.tsv agrees (QR_EscortTrader / S_QRET /
//   0x00E24C30). Ego_r.pdb / FableWin.pdb carry ??1CQR_EscortTraderScript@NScript@@UAE@XZ and the ctor
//   ??0CQR_EscortTraderScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z. The catalog label
//   CParticleEmitter::~CParticleEmitter_0x24c80 is a placeholder. No vtable store in the body = compiler-generated dtor.
// Layout: Ego_r.pdb CQR_EscortTraderScript (0x14C) at retail offsets (Ego_r - 0x14, then - 4 after each 16-byte Dinkumware
//   vector): ten bools +0x48..+0x51, BanditSpawnCount +0x54, BanditKillCount +0x58, eight bools +0x5c..+0x63,
//   TraderHitCount +0x64, TraderHealthBarID +0x68, TraderInitialHealth +0x6c, three bools +0x70..+0x72, BonusObject +0x74,
//   TraderComplainTimer +0x78, TraderExpressionDetectTimer +0x7c, TraderHitDetectTimer +0x80, TraderRandomSpeechTimer +0x84,
//   RandomSpeechFollow[10] +0x88, RandomSpeechWait[5] +0xb0, TradeRoute +0xc4, RouteDynamic[3] +0xc8, RouteStatic[3] +0xec,
//   PreviousLoadedRegion +0x110, CurrentLoadedRegion +0x114, RegionChangeDetected +0x118, QuestStage +0x11c (0x120 = the
//   Alloc size).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunk 665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use. EscortTraderQuestStage
//   is nested in the class (PDB Transfer<W4EscortTraderQuestStage@CQR_EscortTraderScript@NScript@@>); ETradeRoutes' scope is
//   not fixed by any PDB mangling (declared in the class, descriptive placement).
// Callees: ~CCharString 0x0099EAE0, STLport ~vector<CCharString> inline (element loop + free 0x00BFEA14, the arrays as inline
//   countdown loops, no EH vector iterator), CTimer::~CTimer inline (engine/CTimer.h: pDSTGame 0x0143E8F8 slot 88
//   DeregisterTimer), ~CScriptBase 0x00CBD510.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00E1DB00), which emits the vtable, ??_G and this implicit dtor.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_construct.h, stl/_vector.h (the members this row needs; the malloc-backed allocator of
// this build: deallocate = free 0x00BFEA14)
struct __false_type { __false_type() { } };
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
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    typedef _Alloc allocator_type;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#include "engine/CScriptBase.h"
class CCharStringData;
class CCharString { public:
    ~CCharString();                                                      // 0x0099EAE0
    CCharStringData* PStringData;
};
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
class CQR_EscortTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum ETradeRoutes { };                                               // Ego_r type of TradeRoute (enumerators not in the dump)
    enum EscortTraderQuestStage { };                                     // Ego_r nested enum (Transfer<EscortTraderQuestStage>)
    CQR_EscortTraderScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool                     MissionSucceeded;                           // +0x48
    bool                     MissionFailed;                              // +0x49
    bool                     HiredByTrader;                              // +0x4a
    bool                     TraderAtDestination;                        // +0x4b
    bool                     TraderDepartedLost;                         // +0x4c
    bool                     GreetedHero;                                // +0x4d
    bool                     TraderKilled;                               // +0x4e
    bool                     TraderHit;                                  // +0x4f
    bool                     TraderFollowing;                            // +0x50
    bool                     TraderDeparted;                             // +0x51
    long                     BanditSpawnCount;                           // +0x54
    long                     BanditKillCount;                            // +0x58
    bool                     TraderIntroDone;                            // +0x5c
    bool                     DoneExpressions;                            // +0x5d
    bool                     TraderCreated;                              // +0x5e
    bool                     WrongTurnDetected;                          // +0x5f
    bool                     TheBlackGateIsClosed;                       // +0x60
    bool                     AddedQuestInfo;                             // +0x61
    bool                     GoneWrongWay;                               // +0x62
    bool                     TotallyLost;                                // +0x63
    long                     TraderHitCount;                             // +0x64
    long                     TraderHealthBarID;                          // +0x68
    float                    TraderInitialHealth;                        // +0x6c
    bool                     ExpressionDetectedFollow;                   // +0x70
    bool                     ExpressionDetectedWait;                     // +0x71
    bool                     AddedBonusObject;                           // +0x72
    CCharString              BonusObject;                                // +0x74
    CTimer                   TraderComplainTimer;                        // +0x78
    CTimer                   TraderExpressionDetectTimer;                // +0x7c
    CTimer                   TraderHitDetectTimer;                       // +0x80
    CTimer                   TraderRandomSpeechTimer;                    // +0x84
    CCharString              RandomSpeechFollow[10];                     // +0x88
    CCharString              RandomSpeechWait[5];                        // +0xb0
    ETradeRoutes             TradeRoute;                                 // +0xc4
    std::vector<CCharString> RouteDynamic[3];                            // +0xc8
    std::vector<CCharString> RouteStatic[3];                             // +0xec
    CCharString              PreviousLoadedRegion;                       // +0x110
    CCharString              CurrentLoadedRegion;                        // +0x114
    bool                     RegionChangeDetected;                       // +0x118
    EscortTraderQuestStage   QuestStage;                                 // +0x11c
};
CQR_EscortTraderScript::CQR_EscortTraderScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00E1DB00
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQR_EscortTraderScript) == 0x120);