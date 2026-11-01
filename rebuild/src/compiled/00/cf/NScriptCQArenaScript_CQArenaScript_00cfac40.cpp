// NScript::CQ_ArenaScript::~CQ_ArenaScript() -- implicit quest-script destructor: destroy Whisper, deregister GlobalCrowdTimer, destroy CellsVillage, the Rounds vector of CArenaRoundDef and CrowdLoopTags[4][5] in reverse order, then ~CScriptBase
// Leaf: ??1CQ_ArenaScript@NScript@@UAE@XZ
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CFAC40, 232 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00CFAC23 is the scalar deleting destructor 0x00CFAC20, slot 0 of the RTTI-less vtable
//   0x012C7798; that vtable is stored only by the ctor 0x00CFA660, whose only caller is the Alloc 0x00CFABF0 (`push 11Ch ;
//   call new ; ... call 0x00CFA660`), stored at 0x00CD58D1 by the script-registration routine right after the quest name
//   "Q_Arena" (0x012C50A4) and the save tag "S_QA" (0x012F79BC): NScript::CQ_ArenaScript (Ego_r.pdb / FableWin.pdb
//   ??1CQ_ArenaScript@NScript@@UAE@XZ, ??_GCQ_ArenaScript@NScript@@UAEPAXI@Z). The body stores no vtable of its own: the
//   destructor is compiler-generated. Catalog label right.
// Layout: Ego_r.pdb CQ_ArenaScript members at retail offsets (CScriptBase 0x14 smaller, Rounds 12 vs 16 bytes, so members after
//   it sit 0x18 lower): CrowdLoopTags[4][5] +0x48, Rounds +0x98 (std::vector<CArenaRoundDef>), ArenaState +0xA4 .. NeedBertForSpeech
//   +0xBD, CellsVillage +0xC0, ..., GlobalCrowdTimer +0x100, FinalBattleCS +0x104, Whisper +0x108, ..., StopTimeIndex +0x118;
//   retail size 0x11C (Alloc). CArenaRoundDef / CArenaWaveDef / CArenaCreatureDef are the landed CDefClassBase models
//   (0x00F34D70, 0x00F1A8F0: no-RTTI def vtables, implicit inline dtors); the element dtor is the virtual `push 0 ; call [vt]`
//   that STLport's `__pointer->~_Tp()` gives on a polymorphic element (stride 0x38).
// Callees: ~CScriptThing inline (vtable 0x01238C8C store, engine/CCountedPointer.h PImp release: DeleteFunc, operator delete
//   0x00BFE9BC), CBaseClass::~CBaseClass 0x0099A2E0, CTimer::~CTimer inline (engine/CTimer.h: pDSTGame 0x0143E8F8
//   slot 88 DeregisterTimer), free 0x00BFEA14 (STLport allocator<T>::deallocate), ~CCharString 0x0099EAE0, ~CScriptBase 0x00CBD510.
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunks 664/665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// TU: the PDB ctor ??0CQ_ArenaScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z is defined with an empty body
//   (retail's TU defines it), which emits the vtable, the scalar deleting dtor and this implicit dtor; only the dtor is graded.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
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

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CArenaCreatureDef : public CDefClassBase { public:    // def vtable 0x012FB358 (no RTTI), 0x38 bytes
    CArenaCreatureDef();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    CCharString CreatureType;                               // +0x28
    long        NumCreatures;                               // +0x2C
    CCharString HUDType;                                    // +0x30
    long        DeathScore;                                 // +0x34
};
class CArenaWaveDef : public CDefClassBase { public:        // def vtable 0x012FB2F0 (no RTTI), 0x3C bytes
    CArenaWaveDef();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    long                           NumWaveCreatures;        // +0x28
    std::vector<CArenaCreatureDef> Creatures;               // +0x2C
    bool                           ShortWave;               // +0x38
};
class CArenaRoundDef : public CDefClassBase { public:       // def vtable 0x012FB288 (no RTTI), 0x38 bytes
    CArenaRoundDef();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    long                       NumWaves;                    // +0x28
    std::vector<CArenaWaveDef> Waves;                       // +0x2C
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_ArenaScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> { public:
    CQ_ArenaScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    CCharString                 CrowdLoopTags[4][5];        // +0x48
    std::vector<CArenaRoundDef> Rounds;                     // +0x98
    long         ArenaState;                                // +0xA4
    long         ArenaRound;                                // +0xA8
    long         ArenaRoundWave;                            // +0xAC
    long         GoldMultiplier;                            // +0xB0
    bool         ChamLeaving;                               // +0xB4
    long         GoldTotal;                                 // +0xB8
    bool         InHitCutsceneAlready;                      // +0xBC
    bool         NeedBertForSpeech;                         // +0xBD
    CScriptThing CellsVillage;                              // +0xC0
    bool         PlayerLeaving;                             // +0xCC
    bool         PlayerWon;                                 // +0xCD
    long         TotalCreatures[3];                         // +0xD0
    long         ExtraCreatures;                            // +0xDC
    bool         MissionSucceeded;                          // +0xE0
    bool         MissionFailed;                             // +0xE1
    bool         ArenaSpawnNeeded[16];                      // +0xE2
    long         NewCrowdPoints;                            // +0xF4
    long         NewCrowdHappiness;                         // +0xF8
    long         NewCrowdBaseLevel;                         // +0xFC
    CTimer       GlobalCrowdTimer;                          // +0x100
    bool         FinalBattleCS;                             // +0x104
    CScriptThing Whisper;                                   // +0x108
    bool         PauseCrowdChecker;                         // +0x114
    bool         WhisperNeededForCutscene;                  // +0x115
    long         StopTimeIndex;                             // +0x118
};
CQ_ArenaScript::CQ_ArenaScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00CFA660
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(CArenaRoundDef) == 0x38);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_ArenaScript) == 0x11C);