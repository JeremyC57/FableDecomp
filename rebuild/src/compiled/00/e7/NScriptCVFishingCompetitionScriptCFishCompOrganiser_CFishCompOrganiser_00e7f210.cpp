// NScript::CV_FishingCompetitionScript::CFishCompOrganiser::~CFishCompOrganiser() -- implicit entity-script destructor: destroy the FishDef / RodDef vector<CCharString>, CompetitionEntry, clear and free the csargs map, destroy seh_Organiser and seh_Hero, then ~CEntityScriptBase
// Leaf: ??1CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7F210, 281 bytes, thiscall, tail jump to ~CEntityScriptBase 0x00F35B40; sole caller the
//   scalar deleting dtor 0x00E7F1F0 (slot 0 of the RTTI-less entity vtable 0x012EA620).
// Identity (proven): vtable 0x012EA620 is stored only by the ctor 0x00E7EF70, whose only caller is the factory 0x00E7EEF0
//   (`push 78h ; call new ; ... call 0x00E7EF70`); CV_FishingCompetitionScript::Main 0x00E7E370 (slot 2 of the quest vtable,
//   0x012EA480 holds it) stores that factory at 0x00E7E513 in the CEntityScriptBinding record (vtable 0x012EA57C) named
//   "FishCompOrganiser" (string 0x012EA554; the record before it, factory 0x00E7EC90, is "FishCompCompetitor" 0x012EA568);
//   the quest is V_FishingCompetition (S_VFC, Alloc 0x00E81EB0 in refs/script_recovery/conversion_queue.tsv). Ego_r.pdb
//   carries ??1CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@UAE@XZ, the ctor
//   ??0CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z and an Ego_r
//   size 0x84 = 0x78 + 4 (Dinkumware map) + 2 x 4 (Dinkumware vectors). The catalog label
//   CParticleEmitter::~CParticleEmitter_ScreenFree is a placeholder. No vtable store in the body = compiler-generated dtor.
//   (The manifest ctor label "CGameScriptThing" on 0x00E7EF70 is propagated too.)
// Layout: Ego_r.pdb CV_FishingCompetitionScript::CFishCompOrganiser (CEntityScriptBase 0x14 as in retail; - 4 after the map,
//   - 4 more after RodDef): ParentClass +0x14, PMasterData +0x18, seh_Hero +0x1c, seh_Organiser +0x2c, csargs +0x3c,
//   CommentHitByHero +0x48, Conversation +0x4c, FishingRodsInGame +0x50, FishTypesInGame +0x54, FishingPrizes +0x58,
//   CompetitionEntry +0x5c, RodDef +0x60, FishDef +0x6c (0x78 = the factory's `new` size).
// Callees: STLport ~vector<CCharString> inline (~CCharString 0x0099EAE0 per element, free 0x00BFEA14), ~CCharString, the
//   map<CCharString,CCharString> clear inline with the out-of-line _Rb_tree::_M_erase 0x009AA830 and the header node free
//   (STLport text as the landed 0x00E68EB0), the seh_* implicit dtors inline (PImp release, operator delete 0x00BFE9BC,
//   vtable 0x0126008C, ~CBaseObject 0x0099A430), ~CEntityScriptBase 0x00F35B40.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00E7EF70), which emits the vtable, ??_G and this implicit dtor.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
class CCharStringData;
class CCharString { public:
    ~CCharString();                                                      // 0x0099EAE0
    bool operator<(const CCharString&) const;
    CCharStringData* PStringData;
};
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_construct.h, stl/_pair.h, stl/_function_base.h, stl/_vector.h, stl/_tree.h (the members
// this row needs; the malloc-backed allocator of this build: deallocate = free 0x00BFEA14)
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
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                                       // 0x009AA830 (_tree.c)
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
};
}
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FishingCompetitionScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CFishCompOrganiser : public CEntityScriptBase {
    public:
        CFishCompOrganiser(CV_FishingCompetitionScript& parent, CScriptDataBase* data, const CScriptThing& me);
        CV_FishingCompetitionScript*               ParentClass;          // +0x14
        CQ_SunnyvaleMasterData*                    PMasterData;          // +0x18
        CScriptGameResourceObjectScriptedThingBase seh_Hero;             // +0x1c
        CScriptGameResourceObjectScriptedThingBase seh_Organiser;        // +0x2c
        std::map<CCharString, CCharString>         csargs;               // +0x3c
        bool                                       CommentHitByHero;     // +0x48
        long                                       Conversation;         // +0x4c
        long                                       FishingRodsInGame;    // +0x50
        long                                       FishTypesInGame;      // +0x54
        long                                       FishingPrizes;        // +0x58
        CCharString                                CompetitionEntry;     // +0x5c
        std::vector<CCharString>                   RodDef;               // +0x60
        std::vector<CCharString>                   FishDef;              // +0x6c
    private:
        virtual void Main();
        virtual void OnPredicateFail();
    public:
        virtual CScriptBase* GetParentScript() const;
    };
};
CV_FishingCompetitionScript::CFishCompOrganiser::CFishCompOrganiser(CV_FishingCompetitionScript& parent, CScriptDataBase* data,
                                                                    const CScriptThing& me)   // 0x00E7EF70
    : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_FishingCompetitionScript::CFishCompOrganiser) == 0x78);