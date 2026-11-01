// NScript::CQ_FireHeartScript::~CQ_FireHeartScript() -- implicit quest-script destructor: VillagerEscapeMarker, ProphetComment, the three scripted-thing handles, seh_Prophet / Pattern / FollowerHandles / Followers / Villagers / Prophets, the six vector<bool>, the two FX strings, Effect, csargs and CutsceneFlags in reverse order, then ~CScriptBase
// Leaf: ??1CQ_FireHeartScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D3B010, 883 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00D3AFF3 is the scalar deleting destructor 0x00D3AFF0, slot 0 of the RTTI-less vtable
//   0x012CCB90, which only the landed ctor 0x00D34470 stores (NScript::CQ_FireHeartScript::CQ_FireHeartScript; its factory
//   0x00D3AFC2 allocates 0x1A4). The member layout destroyed here (two maps at +0x4C/+0x58, CScriptThing +0xA8, two strings,
//   six vector<bool>, three vector<CScriptThing>, a vector of 0x10-byte scripted-thing handles, a vector of 0x34-byte
//   CFireHeartPatternDef, a second handle vector, three handles, a string and a CScriptThing at +0x18C) is Ego_r.pdb
//   CQ_FireHeartScript at retail offsets and no other script class; the CFH_* entity-script rows sit beside it (landed
//   CFH_MemorialCutsceneMarker::Main 0x00D3AF50). Both PDBs spell ??1CQ_FireHeartScript@NScript@@UAE@XZ. The body stores no
//   vtable of its own: compiler-generated. The catalog label "CParticleEmitter_Dtor_4" and the manifest's
//   "CQ_HangingTreeGoodScript" (0x00D34470) are wrong.
// Layout: Ego_r.pdb CQ_FireHeartScript at retail offsets (CScriptBase 0x14 smaller, each Dinkumware vector/map 4 bytes
//   larger than STLport's, vector<bool> 8 larger): CutsceneFlags +0x4C, csargs +0x58, Effect +0xA8, PadSunFX +0xB4, PadMoonFX
//   +0xB8, StartingPattern.. CommentGivenThisRound +0xBC..+0xF8, Prophets +0x104, Villagers +0x110, Followers +0x11C,
//   FollowerHandles +0x128, Pattern +0x134, seh_Prophet +0x148, seh_BriarRose +0x154, seh_Scythe +0x164, seh_GuildMaster
//   +0x174, ProphetComment +0x188, VillagerEscapeMarker +0x18C; size 0x1A4.
// Callees: ~CScriptThing inline (vtable 0x01238C8C store, engine/CCountedPointer.h PImp release: DeleteFunc, operator delete
//   0x00BFE9BC, CBaseClass::~CBaseClass 0x0099A2E0), ~CScriptGameResourceObjectScriptedThingBase inline (PImp release, the
//   user ~CScriptGameResourceObjectBase: vtable 0x0126008C store, CBaseObject::~CBaseObject 0x0099A430), ~CCharString
//   0x0099EAE0, free 0x00BFEA14, _Rb_tree<CCharString, pair<const CCharString,CCharString>>::_M_erase 0x009AA830 and
//   _Rb_tree<CCharString, pair<const CCharString,bool>>::_M_erase 0x007ACC50 (the inline map clear()), the vector elements
//   through the virtual `push 0 ; call [vt]`, ~CScriptBase 0x00CBD510.
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunks 664/665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"); the master
//   data type T is unproven for this script (CQ_SunnyvaleMasterData is the only instantiation either PDB names). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// TU: the PDB ctor is defined with an empty body (retail's TU defines it), which emits the vtable, the scalar deleting dtor
//   and this implicit dtor; only the dtor is graded. Shape: the landed ~CQ_HangingTreeGoodScript 0x00D782E0 recipe plus the
//   STLport 4.6 _Rb_tree clear()/~_Rb_tree_base and the CBaseObject-family handle.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }
#include "engine/CScriptBase.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

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
    void _M_erase(_Link_type __x);                          // 0x007ACC50 (-> bool) / 0x009AA830 (-> CCharString)
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

class CFireHeartPatternDef { public:                        // 0x34 bytes, polymorphic (the element loop calls slot 0 with 0)
    virtual ~CFireHeartPatternDef();
    unsigned char _body[0x30];
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_FireHeartScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> { public:
    CQ_FireHeartScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool  QuestStartScreenShown;                            // +0x48
    bool  MissionSucceeded;                                 // +0x49
    bool  MissionFailed;                                    // +0x4A
    std::map<CCharString, bool>        CutsceneFlags;       // +0x4C
    std::map<CCharString, CCharString> csargs;              // +0x58
    long  FreedProphets;                                    // +0x64
    long  KilledProphets;                                   // +0x68
    long  StartNumberOfSuns;                                // +0x6C
    long  NumberOfRounds;                                   // +0x70
    long  NumberOfPatterns;                                 // +0x74
    long  NumberOfProphets;                                 // +0x78
    long  NumberOfPads;                                     // +0x7C
    long  NumberOfSuns;                                     // +0x80
    long  NumberOfVillagers;                                // +0x84
    long  PuzzleRound;                                      // +0x88
    bool  ResetBlocks;                                      // +0x8C
    long  NumBlocksReset;                                   // +0x90
    bool  RoundEnded;                                       // +0x94
    bool  RoundBeginning;                                   // +0x95
    bool  PreventRoundEnd;                                  // +0x96
    bool  HasHeroHitVillagersGroupOne;                      // +0x97
    bool  HasHeroHitVillagersGroupTwo;                      // +0x98
    bool  InfoDisplayed;                                    // +0x99
    long  EvilRounds;                                       // +0x9C
    long  GoodRounds;                                       // +0xA0
    float PadRadius;                                        // +0xA4
    CScriptThing Effect;                                    // +0xA8
    CCharString  PadSunFX;                                  // +0xB4
    CCharString  PadMoonFX;                                 // +0xB8
    std::vector<bool> StartingPattern;                      // +0xBC (retail vector<bool> is the plain 12-byte vector)
    std::vector<bool> ProphetsKilled;                       // +0xC8
    std::vector<bool> ProphetsReleased;                     // +0xD4
    std::vector<bool> ShouldProphetSpeak;                   // +0xE0
    std::vector<bool> ShouldVillagerSpeak;                  // +0xEC
    std::vector<bool> CommentGivenThisRound;                // +0xF8
    std::vector<CScriptThing> Prophets;                     // +0x104
    std::vector<CScriptThing> Villagers;                    // +0x110
    std::vector<CScriptThing> Followers;                    // +0x11C
    std::vector<CScriptGameResourceObjectScriptedThingBase> FollowerHandles;   // +0x128
    std::vector<CFireHeartPatternDef> Pattern;              // +0x134
    long  Guest;                                            // +0x140
    long  ProphetsScripted;                                 // +0x144
    std::vector<CScriptGameResourceObjectScriptedThingBase> seh_Prophet;       // +0x148
    CScriptGameResourceObjectScriptedThingBase seh_BriarRose;                  // +0x154
    CScriptGameResourceObjectScriptedThingBase seh_Scythe;                     // +0x164
    CScriptGameResourceObjectScriptedThingBase seh_GuildMaster;                // +0x174
    long  MissionHistory;                                   // +0x184 EMissionHistory
    CCharString  ProphetComment;                            // +0x188
    CScriptThing VillagerEscapeMarker;                      // +0x18C
    float MoralityChangeOnKilled;                           // +0x198
    float MoralityChangeOnFreed;                            // +0x19C
    bool  HasDemonDoorBeenEnabled;                          // +0x1A0
    bool  HasScytheCutscenePlayed;                          // +0x1A1
    bool  KilledAllPrisoners;                               // +0x1A2
    bool  FreedAllPrisoners;                                // +0x1A3
};
CQ_FireHeartScript::CQ_FireHeartScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00D34470
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_FireHeartScript) == 0x1A4);