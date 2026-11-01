// NScript::CQ_HeroSoulsBriarScript::~CQ_HeroSoulsBriarScript() -- implicit quest-script destructor: deregister the timers, destroy the csargs map and seh_BriarRose members in reverse order, then ~CScriptBase
// Leaf: ??1CQ_HeroSoulsBriarScript@NScript@@UAE@XZ
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D83350, tail jump to ~CScriptBase 0x00CBD510; sole caller the scalar deleting dtor
//   (vtable slot 0) of the RTTI-less quest vtable.
// Identity (proven): the vtable whose slot 0 calls this body is stored only by the ctor 0x00D80C30 (manifest label CQ_HeroSoulsBriarScript::CQ_HeroSoulsBriarScript);
//   that ctor's only caller is the Alloc that `new`s 0x74 bytes, registered by the script-registration routine with the
//   quest name "Q_HeroSoulsBriar" (string 0x012D31AC) -- CQ_HeroSoulsBriarScript is the Q_<name> + "Script" class of that registration. Ego_r.pdb and
//   FableWin.pdb both carry ??1CQ_HeroSoulsBriarScript@NScript@@UAE@XZ (virtual, public: the implicit dtor of a CScriptBase-derived class).
//   The body stores no vtable: the dtor is compiler-generated (no user ~CQ_HeroSoulsBriarScript). The manifest label (CParticleEmitter_Dtor_N)
//   is propagated and wrong.
// Layout: Ego_r.pdb CQ_HeroSoulsBriarScript members with retail offsets (CScriptBase 0x40 in retail vs 0x54 in Ego_r: quest-script members sit
//   0x14 lower; CLevelScriptBase<CQ_SunnyvaleMasterData> Game +0x40 / MasterScriptData +0x44), retail STLport containers.
//   Members destroyed: BriarRoseImposterHereTimer +0x6c, BriarRoseImposterTauntTimer +0x68, csargs std::map<CCharString,
//   CCharString> +0x5c (_M_erase 0x009AA830), seh_BriarRose CScriptGameResourceObjectScriptedThingBase +0x4c (engine header:
//   implicit dtor = PImp release, then ~CScriptGameResourceObjectBase inline: vtable 0x0126008C + ~CBaseObject 0x0099A430).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (the
//   CLevelScriptBase<T> scalar deleting dtors 0x00CDD250 / 0x00CDD270 call ~CScriptBase 0x00CBD510 directly, no vtable
//   store; an inline user `~CLevelScriptBase() {}` adds `mov [esi],vftable` before the tail jump -- probe 1 of 0x00D2F640),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt").
// Callees: CTimer::~CTimer inline (engine/CTimer.h: interface slot 88 DeregisterTimer through pDSTGame 0x0143E8F8),
//   ~CScriptBase 0x00CBD510.
// TU: the ctor is defined (as retail's TU defines it), which emits the vtable, the scalar deleting dtor and this implicit dtor.
//   Built without /EHsc (no EH frame; the member-array dtors are inline countdown loops, no `eh vector destructor iterator`).
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
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
class CQ_HeroSoulsBriarScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_HeroSoulsBriarScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool PlayedBriarCS;                                                  // +0x48
    CScriptGameResourceObjectScriptedThingBase seh_BriarRose;            // +0x4c
    std::map<CCharString, CCharString> csargs;                           // +0x5c
    CTimer BriarRoseImposterTauntTimer;                                  // +0x68
    CTimer BriarRoseImposterHereTimer;                                   // +0x6c
    bool KickOffQuestStartScreen;                                        // +0x70
};
CQ_HeroSoulsBriarScript::CQ_HeroSoulsBriarScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00D80C30
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}