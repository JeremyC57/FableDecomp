// NScript::CQ_AmbushTradersScript::~CQ_AmbushTradersScript() -- implicit quest-script destructor: destroy TeleportInfo (TeleportData records), clear and free CSFlagMap, the ten CCharString[5] speech tables, the six script-thing vectors, deregister the two timers, then ~CScriptBase
// Leaf: ??1CQ_AmbushTradersScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CFA340, 792 bytes, thiscall, `push ecx` frame (the __false_type tag byte), ends in a
//   call to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00CFA323 is the scalar deleting destructor 0x00CFA320, slot 0 of the RTTI-less vtable
//   0x012C6CAC, which only the landed ctor 0x00CF9350 stores (??0CQ_AmbushTradersScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z,
//   whose only caller is the Alloc 0x00CFA2F0, registered beside the S_QAT tag -> Q_AmbushTraders). The member layout
//   (six vector<CScriptThing>, ten CCharString[5], a map<CCharString,bool>, a vector<TeleportData>) is Ego_r.pdb
//   CQ_AmbushTradersScript at retail offsets; both PDBs spell ??1CQ_AmbushTradersScript@NScript@@UAE@XZ. The body stores no
//   vtable: compiler-generated. The catalog label "CParticleEmitter_Dtor" is wrong.
// Layout (the landed ctor's class, retail = Ego_r - 0x14, -4 per STLport vector/map): SpeechBanditTimer +0x9C, SpeechSpyTimer
//   +0xA0, AllBandits.. SentriesVector +0xA8..+0xEC, BanditSpeechNormalThere.. SpySpeechSentryRandom +0xF0..+0x1B7,
//   CSFlagMap +0x1B8, TeleportInfo +0x1C4; size 0x1D0.
// Callees: std::__destroy_aux<TeleportData*>(first, last, const __false_type&) 0x00CDC000 (the out-of-line COMDAT: the
//   TeleportData destructor is four ~CCharString calls, too big to inline), free 0x00BFEA14, _Rb_tree<CCharString,
//   pair<const CCharString,bool>,..>::_M_erase 0x007ACC50 (the inline clear() of map::~map), ~CCharString 0x0099EAE0,
//   the vector<CScriptThing> elements through the virtual `push 0 ; call [vt]`, CTimer::~CTimer inline (interface slot 88
//   DeregisterTimer through pDSTGame 0x0143E8F8, engine/CTimer.h), ~CScriptBase 0x00CBD510.
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunks 664/665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// TU: the PDB ctor is defined with an empty body (retail's TU defines it), which emits the vtable, the scalar deleting dtor
//   and this implicit dtor; only the dtor is graded. Shape: the landed ~CQ_HangingTreeGoodScript 0x00D782E0 recipe plus the
//   STLport 4.6 _Rb_tree clear()/~_Rb_tree_base and the CTimer destructor.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS)
#include "engine/CTimer.h"
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }   // retail expands 0x00CD4470 here
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }
#include "engine/CScriptBase.h"

extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E (the ctor's map headers; not graded)
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _T1, class _T2> struct pair { typedef _T1 first_type; typedef _T2 second_type; _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base {
    typedef _Rb_tree_Color_type _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                   // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    typedef allocator<_Node> _M_node_allocator_type;
    _Rb_tree_base(const allocator_type&) : _M_header(_M_node_allocator_type(), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, _M_node_allocator_type> _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
    typedef _Rb_tree_base<_Value, _Alloc> _Base;
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
protected:
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    size_type _M_node_count;
    _Compare _M_key_compare;
public:
    _Rb_tree(const _Compare& __comp, const allocator_type& __a)
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
        { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
    void clear() {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
private:
    void _M_empty_initialize() {
        this->_M_header._M_data->_M_color = false;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    void _M_erase(_Link_type __x);                          // 0x007ACC50 (CCharString -> bool instantiation)
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Compare key_compare;
    typedef _Alloc allocator_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    map() : _M_t(_Compare(), allocator_type()) {}
private:
    _Rep_type _M_t;
};
} // namespace std

class CCharStringData;
class CCharString { public:
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};
bool operator<(const CCharString& a, const CCharString& b);

class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase { public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                         // +0x40
    T*                        MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AmbushTradersScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> { public:
    struct TeleportData {
        CCharString   defName;                              // +0x00
        unsigned long appearanceSeed;                       // +0x04
        float         health;                               // +0x08
        CCharString   scriptName;                           // +0x0C
        CCharString   teleportTo;                           // +0x10
        CCharString   teleportMkr;                          // +0x14
    };
    CQ_AmbushTradersScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool QuestFailed;                                       // +0x48
    bool QuestFinished;                                     // +0x49
    bool QuestSucceeded;                                    // +0x4A
    bool QuestOver;                                         // +0x4B
    bool RegionInvalid;                                     // +0x4C
    bool BanditIntroDone;                                   // +0x4D
    bool TraderEscaped;                                     // +0x4E
    bool NoEnemiesLeft;                                     // +0x4F
    bool PhaseComplete;                                     // +0x50
    bool ConvoyTrigger2;                                    // +0x51
    bool AllBanditsDead;                                    // +0x52
    bool AllTradersDead;                                    // +0x53
    bool AllGuardsDead;                                     // +0x54
    bool ContactDead;                                       // +0x55
    bool ContactAttackedByHero;                             // +0x56
    bool HeroDead;                                          // +0x57
    bool SpyDead;                                           // +0x58
    bool SentryAttackedByHero;                              // +0x59
    bool SpySeenBySentry;                                   // +0x5A
    bool SpyFollowing;                                      // +0x5B
    bool SpyFollowingHero;                                  // +0x5C
    bool SpyHeroProximity;                                  // +0x5D
    bool SpyWanderReset;                                    // +0x5E
    bool SpyContactMade;                                    // +0x5F
    bool SpyEscaped;                                        // +0x60
    bool SpyExited;                                         // +0x61
    bool SpyReachedNoEntryMarker;                           // +0x62
    bool AnyTraderEscaped;                                  // +0x63
    bool RegionClear;                                       // +0x64
    bool QuestSuccessSpeechComplete;                        // +0x65
    bool ActivatedPhase3;                                   // +0x66
    bool BanditContactIntroComplete;                        // +0x67
    bool BanditContactKilledBeforeQuestEnd;                 // +0x68
    bool EnableSpeechSpy;                                   // +0x69
    bool EnableSpeechBandit;                                // +0x6A
    long TraderAliveCount;                                  // +0x6C
    long GuardAliveCount;                                   // +0x70
    long SpeechBanditCount;                                 // +0x74
    long SpeechSpyCount;                                    // +0x78
    long SpeechSpyProximityCount;                           // +0x7C
    long SpeechSpySentryCount;                              // +0x80
    long SpyHealthBarID;                                    // +0x84
    unsigned long ContactSeed;                              // +0x88
    long HiredBanditCount;                                  // +0x8C
    bool BanditAttackedByHero;                              // +0x90
    long BanditAttackByHeroCount;                           // +0x94
    bool BanditSwitchAllegienceComplete;                    // +0x98
    CTimer SpeechBanditTimer;                               // +0x9C
    CTimer SpeechSpyTimer;                                  // +0xA0
    long lastRandomSpySpeechIdx;                            // +0xA4
    std::vector<CScriptThing> AllBandits;                   // +0xA8
    std::vector<CScriptThing> AllTraders;                   // +0xB4
    std::vector<CScriptThing> AllGuards;                    // +0xC0
    std::vector<CScriptThing> BanditsVector;                // +0xCC
    std::vector<CScriptThing> ConvoyMembersVector;          // +0xD8
    std::vector<CScriptThing> SentriesVector;               // +0xE4
    CCharString BanditSpeechNormalThere[5];                 // +0xF0
    CCharString BanditSpeechRandomThere[5];                 // +0x104
    CCharString BanditSpeechNormalBack[5];                  // +0x118
    CCharString BanditSpeechRandomBack[5];                  // +0x12C
    CCharString SpySpeechNormal[5];                         // +0x140
    CCharString SpySpeechRandom[5];                         // +0x154
    CCharString SpySpeechProximityNormal[5];                // +0x168
    CCharString SpySpeechProximityRandom[5];                // +0x17C
    CCharString SpySpeechSentryNormal[5];                   // +0x190
    CCharString SpySpeechSentryRandom[5];                   // +0x1A4
    std::map<CCharString, bool> CSFlagMap;                  // +0x1B8
    std::vector<TeleportData> TeleportInfo;                 // +0x1C4
};
CQ_AmbushTradersScript::CQ_AmbushTradersScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00CF9350
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_AmbushTradersScript) == 0x1D0);