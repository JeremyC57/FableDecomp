// NScript::CV_TalentlessBardScript::~CV_TalentlessBardScript() -- implicit quest-script destructor: deregister RandomSpeechTimer, free SongThingAvailable, destroy QuestInfo[12], SongItems, BardWanderTime and stBard in reverse order, then ~CScriptBase
// Leaf: ??1CV_TalentlessBardScript@NScript@@UAE@XZ
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
