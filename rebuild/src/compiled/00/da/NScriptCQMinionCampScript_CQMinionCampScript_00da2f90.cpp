// NScript::CQ_MinionCampScript::~CQ_MinionCampScript() -- implicit quest-script destructor: destroy SpawnPointToSpawn, the RightSpawnPoints / LeftSpawnPoints lists, HeroAttackerTwo / HeroAttackerOne, deregister MinionSpawnTimer / ChantTimer, then ~CScriptBase
// Leaf: ??1CQ_MinionCampScript@NScript@@UAE@XZ
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA2F90, 283 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510; sole caller the
//   scalar deleting dtor 0x00DA2F70 (slot 0 of the RTTI-less quest vtable 0x012D6C60).
// Identity (proven): vtable 0x012D6C60 is stored only by the ctor 0x00D9E210, whose only caller is the Alloc 0x00DA2F40
//   (`push 0ACh ; call new ; ... call 0x00D9E210`); CScriptInfoManager::RegisterScripts stores that Alloc at 0x00CD6D23 right
//   after `push "S_QMC"` (0x012F78AC); refs/script_recovery/conversion_queue.tsv agrees (Q_MinionCamp / S_QMC / 0x00DA2F40);
//   the landed CQ_MinionCampScript ctor 0x00D9E210 and the landed list clear 0x00CD4DF0 (whose header already names this row)
//   agree. Ego_r.pdb / FableWin.pdb carry ??1CQ_MinionCampScript@NScript@@UAE@XZ. The catalog label
//   CV_FisticuffsClubScript::~CV_FisticuffsClubScript is propagated and wrong: chunk 673 landed that dtor at 0x00E8B750
//   (vtable 0x012EAF14, Alloc 0x00E8B700, "S_VFCC"). No vtable store in the body = compiler-generated dtor.
// Layout: Ego_r.pdb CQ_MinionCampScript (0xD0) at retail offsets (Ego_r - 0x14, then - 8 after each 12-byte Dinkumware list,
//   STLport list = one sentinel pointer): six bools +0x48..+0x4d, NumberOfMinions +0x50, MaxNumberOfMinions +0x54,
//   MinMinionSpawnPeriod +0x58, MaxMinionSpawnPeriod +0x5c, ChantTimer +0x60, ChantTimerBar +0x64, ChantTimerPaused +0x68,
//   PausedTime +0x6c, MinionSpawnTimer +0x70, CurrentStage +0x74, HeroAttackerOne +0x78, HeroAttackerTwo +0x84,
//   LeftSpawnPoints +0x90, RightSpawnPoints +0x94, ShouldSpawn +0x98, SpawnPointToSpawn +0x9c, BriarState +0xa8
//   (0xAC = the Alloc size).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunk 665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). The two
//   enums are declared in the class (the PDB name sets carry no mangled use that fixes their scope: descriptive placement).
// Callees: ~CScriptThing inline (vtable 0x01238C8C store, engine/CCountedPointer.h PImp release: DeleteFunc slot, operator
//   delete 0x00BFE9BC), CBaseClass::~CBaseClass 0x0099A2E0, STLport ~_List_base inline: the out-of-line
//   _List_base<CScriptThing>::clear 0x00CD4DF0 (_list.c) then the sentinel free 0x00BFEA14, CTimer::~CTimer inline
//   (engine/CTimer.h: pDSTGame 0x0143E8F8 slot 88 DeregisterTimer), ~CScriptBase 0x00CBD510.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00D9E210), which emits the vtable, ??_G and this implicit dtor.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }
#include "engine/CScriptBase.h"
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_list.h (the members this row needs; the malloc-backed allocator of this build:
// deallocate = free 0x00BFEA14). _List_base::clear is the _list.c text; VC7.1 keeps it out of line on its own (0x00CD4DF0).
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
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    ~_List_base() {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();                                                        // 0x00CD4DF0 (_list.c)
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != (_List_node<_Tp>*)this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
};
}
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
class CQ_MinionCampScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum MissionStage { };                                               // Ego_r type of CurrentStage (enumerators not in the dump)
    enum BriarRoseState { };                                             // Ego_r type of BriarState
    CQ_MinionCampScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool                    QuestStartScreenShown;                       // +0x48
    bool                    IntroDone;                                   // +0x49
    bool                    BriarRoseAtAltar;                            // +0x4a
    bool                    MinionsSummoned;                             // +0x4b
    bool                    PortalSealed;                                // +0x4c
    bool                    MissionFailed;                               // +0x4d
    long                    NumberOfMinions;                             // +0x50
    long                    MaxNumberOfMinions;                          // +0x54
    float                   MinMinionSpawnPeriod;                        // +0x58
    float                   MaxMinionSpawnPeriod;                        // +0x5c
    CTimer                  ChantTimer;                                  // +0x60
    long                    ChantTimerBar;                               // +0x64
    bool                    ChantTimerPaused;                            // +0x68
    long                    PausedTime;                                  // +0x6c
    CTimer                  MinionSpawnTimer;                            // +0x70
    MissionStage            CurrentStage;                                // +0x74
    CScriptThing            HeroAttackerOne;                             // +0x78
    CScriptThing            HeroAttackerTwo;                             // +0x84
    std::list<CScriptThing> LeftSpawnPoints;                             // +0x90
    std::list<CScriptThing> RightSpawnPoints;                            // +0x94
    bool                    ShouldSpawn;                                 // +0x98
    CScriptThing            SpawnPointToSpawn;                           // +0x9c
    BriarRoseState          BriarState;                                  // +0xa8
};
CQ_MinionCampScript::CQ_MinionCampScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00D9E210
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_MinionCampScript) == 0xAC);