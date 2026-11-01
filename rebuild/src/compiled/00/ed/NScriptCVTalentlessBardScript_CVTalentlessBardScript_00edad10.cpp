// NScript::CV_TalentlessBardScript::~CV_TalentlessBardScript() -- implicit quest-script destructor: deregister RandomSpeechTimer, free SongThingAvailable, destroy QuestInfo[12], SongItems, BardWanderTime and stBard in reverse order, then ~CScriptBase
// Leaf: ??1CV_TalentlessBardScript@NScript@@UAE@XZ
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDAD10, 219 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00EDACF3 is the scalar deleting destructor 0x00EDACF0, slot 0 of the RTTI-less vtable
//   0x012F3428; that vtable is stored only by the ctor 0x00ED76E0, whose only caller is the Alloc 0x00EDACC0 (`push 0E4h ;
//   call new ; ... call 0x00ED76E0`), stored at 0x00CD9723 by the script-registration routine right after the level name
//   "V_TalentlessBard" (0x012C54A4) and the save tag "S_VTB" (0x012F7634): NScript::CV_TalentlessBardScript (Ego_r.pdb and
//   FableWin.pdb ??1CV_TalentlessBardScript@NScript@@UAE@XZ, ??0CV_TalentlessBardScript@NScript@@QAE@PAVCScriptDataBase@@
//   AAVCGameScriptInterfaceBase@@@Z). The body stores no vtable of its own: the destructor is compiler-generated. Catalog
//   label right; the landed ??_G 0x00EDACF0 is a CLandscapeBackgroundPatch `vector deleting` stand-in.
// Layout: Ego_r.pdb CV_TalentlessBardScript members at retail offsets (Ego_r is a Dinkumware build: CScriptBase 0x54 vs retail
//   0x40, vectors 16 vs 12 bytes, vector<bool> 20 vs 12): bHurtBard +0x48 .. bRewardGiven +0x50, stBard +0x54 (CScriptThing),
//   BardIntroComplete/AttackedByHero/BardFollowing +0x60..+0x62, BardWanderTime +0x64, SongItems +0x68 (vector<CCharString>),
//   QuestInfo[12] +0x74 (QuestData {CCharString name, tag}, 8 bytes), SongThingAvailable +0xD4 (12 bytes in retail, freed with
//   no element loop: one byte per element, the generic vector template, as retail's CArray<bool> in chunk 671),
//   RandomSpeechTimer +0xE0; retail size 0xE4 (the Alloc's `push 0E4h`).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunks 664/665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// Callees: CTimer::~CTimer inline (engine/CTimer.h: pDSTGame 0x0143E8F8 slot 88 DeregisterTimer), free 0x00BFEA14
//   (STLport allocator<T>::deallocate), ~CCharString 0x0099EAE0, ~CScriptThing inline (vtable 0x01238C8C store,
//   engine/CCountedPointer.h PImp release: DeleteFunc, operator delete 0x00BFE9BC), CBaseClass::~CBaseClass 0x0099A2E0,
//   ~CScriptBase 0x00CBD510.
// TU: the PDB ctor is defined with an empty body (retail's TU defines it), which emits the vtable, the scalar deleting dtor
//   and this implicit dtor; only the dtor is graded.
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

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TalentlessBardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> { public:
    struct QuestData {
        QuestData();
        CCharString name;                                   // +0x00
        CCharString tag;                                    // +0x04
    };
    CV_TalentlessBardScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool                     bHurtBard;                     // +0x48
    bool                     bBard_finished;                // +0x49
    unsigned long            lNumber_of_things_sung_about;  // +0x4C
    bool                     bRewardGiven;                  // +0x50
    CScriptThing             stBard;                        // +0x54
    bool                     BardIntroComplete;             // +0x60
    bool                     AttackedByHero;                // +0x61
    bool                     BardFollowing;                 // +0x62
    CTimer                   BardWanderTime;                // +0x64
    std::vector<CCharString> SongItems;                     // +0x68
    QuestData                QuestInfo[12];                 // +0x74
    std::vector<bool>        SongThingAvailable;            // +0xD4
    CTimer                   RandomSpeechTimer;             // +0xE0
};
CV_TalentlessBardScript::CV_TalentlessBardScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00ED76E0
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_TalentlessBardScript) == 0xE4);