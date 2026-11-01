// NScript::CV_BordelloScript::~CV_BordelloScript() -- implicit quest-script destructor: clear and free the csargs map, destroy seh_Whore, seh_Madam, seh_Guard and seh_Boss, destroy BordelloVillage, then ~CScriptBase
// Leaf: ??1CV_BordelloScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E46A50, 380 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510; sole caller the
//   scalar deleting dtor 0x00E46A30 (slot 0 of the RTTI-less quest vtable 0x012E38B0).
// Identity (proven): vtable 0x012E38B0 is stored only by the ctor 0x00E398F0, whose only caller is the Alloc 0x00E46A00
//   (`push 0C0h ; call new ; ... call 0x00E398F0`); CScriptInfoManager::RegisterScripts stores that Alloc at 0x00CD8535 right
//   after `push "S_VB"` (0x012F7744); refs/script_recovery/conversion_queue.tsv agrees (V_Bordello / S_VB / 0x00E46A00).
//   Ego_r.pdb CV_BordelloScript is 0xD8 = 0xC0 + 0x14 (CScriptBase) + 4 (Dinkumware map). The vtable has no RTTI COL and the
//   body walks a CScriptThing / seh / map layout, so the catalog label CSystemManager::~CSystemManager (an RTTI engine class)
//   is propagated and wrong. No vtable store in the body = compiler-generated dtor. The manifest ctor label
//   "CQ_HangingTreeGoodScript" on 0x00E398F0 is propagated too (the landed CQ_HangingTreeGoodScript ctor is 0x00D6EF20).
// Layout: Ego_r.pdb CV_BordelloScript at retail offsets (Ego_r - 0x14; the map is last): nine bools +0x48..+0x50,
//   ClientsAlive +0x54, ClientInUse[3] +0x58, BordelloVillage +0x5c, WearingWhoreWig +0x68, HadSex +0x69, BeersDrunk +0x6c,
//   BeerSetToDPad +0x70, seh_Boss +0x74, seh_Guard +0x84, seh_Madam +0x94, seh_Whore +0xa4, csargs +0xb4 (0xC0 = the Alloc size).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunk 665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt").
// Callees: std::map<CCharString,CCharString> clear inline with the out-of-line _Rb_tree::_M_erase 0x009AA830 and the header
//   node free 0x00BFEA14 (STLport text as the landed 0x00E68EB0), the seh_* implicit dtors inline (PImp release, operator
//   delete 0x00BFE9BC, vtable 0x0126008C, ~CBaseObject 0x0099A430), ~CScriptThing inline (vtable 0x01238C8C, PImp release,
//   ~CBaseClass 0x0099A2E0), ~CScriptBase 0x00CBD510.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00E398F0), which emits the vtable, ??_G and this implicit dtor.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};
class CCharStringData;
class CCharString { public:
    ~CCharString();                                                      // 0x0099EAE0
    bool operator<(const CCharString&) const;
    CCharStringData* PStringData;
};
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_construct.h, stl/_pair.h, stl/_function_base.h, stl/_tree.h (the members this row needs;
// the malloc-backed allocator of this build: deallocate = free 0x00BFEA14)
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
    void _M_erase(_Link_type __x);
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
class CV_BordelloScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_BordelloScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool                                       HeroPartying;             // +0x48
    bool                                       HeroTricking;             // +0x49
    bool                                       MagicianSleeping;         // +0x4a
    bool                                       CutscenePlaying;          // +0x4b
    bool                                       PlayerOwned;              // +0x4c
    bool                                       BecomeNunnery;            // +0x4d
    bool                                       PlayerHasLeftRegion;      // +0x4e
    bool                                       HeroFoundDeeds;           // +0x4f
    bool                                       HeroFoundDeedsLocation;   // +0x50
    long                                       ClientsAlive;             // +0x54
    bool                                       ClientInUse[3];           // +0x58
    CScriptThing                               BordelloVillage;          // +0x5c
    bool                                       WearingWhoreWig;          // +0x68
    bool                                       HadSex;                   // +0x69
    long                                       BeersDrunk;               // +0x6c
    bool                                       BeerSetToDPad;            // +0x70
    CScriptGameResourceObjectScriptedThingBase seh_Boss;                 // +0x74
    CScriptGameResourceObjectScriptedThingBase seh_Guard;                // +0x84
    CScriptGameResourceObjectScriptedThingBase seh_Madam;                // +0x94
    CScriptGameResourceObjectScriptedThingBase seh_Whore;                // +0xa4
    std::map<CCharString, CCharString>         csargs;                   // +0xb4
};
CV_BordelloScript::CV_BordelloScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00E398F0
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BordelloScript) == 0xC0);