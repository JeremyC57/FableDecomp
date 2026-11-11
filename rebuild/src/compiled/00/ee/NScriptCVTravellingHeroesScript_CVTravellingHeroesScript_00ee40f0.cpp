// NScript::CV_TravellingHeroesScript::~CV_TravellingHeroesScript() -- implicit quest-script destructor: clear and free the csargs map, destroy seh_Boy, seh_Girl and seh_Rival in reverse order, then ~CScriptBase
// Leaf: ??1CV_TravellingHeroesScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE40F0, 247 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00EE40D3 is the scalar deleting destructor 0x00EE40D0, slot 0 of the RTTI-less vtable
//   0x012F407C; that vtable is stored only by the ctor 0x00EDEBA0, whose only caller is the Alloc 0x00EE40A0 (`push 8Ch ;
//   call new ; ... call 0x00EDEBA0`), stored at 0x00CD98EE by the script-registration routine right after the level name
//   "V_TravellingHeroes" (0x012C5C3C) and the save tag "S_VTH" (0x012F761C): NScript::CV_TravellingHeroesScript (Ego_r.pdb /
//   FableWin.pdb ??1CV_TravellingHeroesScript@NScript@@UAE@XZ). The body stores no vtable of its own: the destructor is
//   compiler-generated. The catalog label "CTCTavernGame::~CTCTavernGame" is propagated and wrong (CTCTavernGame is an RTTI
//   class; this vtable has no COL), and so is the manifest's ctor label 0x00EDEBA0 "CQ_WizardBattleScript".
// Layout: Ego_r.pdb CV_TravellingHeroesScript members, 0x14 lower in retail: seh_Rival +0x48, seh_Girl +0x58, seh_Boy +0x68
//   (CScriptGameResourceObjectScriptedThingBase, 16 bytes, engine header), six bools +0x78..+0x7D, csargs +0x80
//   (std::map<CCharString, CCharString>, STLport 12 bytes); retail size 0x8C (Alloc). Model copied from the landed
//   ~CQ_HeroSoulsBriarScript 0x00D83350 (same csargs / seh_ member types).
// Callees: _Rb_tree<CCharString, pair<const CCharString, CCharString>, ...>::_M_erase 0x009AA830 (out of line, private: the
//   bound STLport spelling), free 0x00BFEA14 (header node), CScriptGameResourceObjectScriptedThingBase implicit dtor inline
//   (PImp release, operator delete 0x00BFE9BC, then ~CScriptGameResourceObjectBase: vtable 0x0126008C + ~CBaseObject
//   0x0099A430), ~CScriptBase 0x00CBD510.
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunks 664/665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// TU: the PDB ctor ??0CV_TravellingHeroesScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z is defined with an empty body
//   (retail's TU defines it), which emits the vtable, the scalar deleting dtor and this implicit dtor; only the dtor is graded.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
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
class CV_TravellingHeroesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> { public:
    CV_TravellingHeroesScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    CScriptGameResourceObjectScriptedThingBase seh_Rival;   // +0x48
    CScriptGameResourceObjectScriptedThingBase seh_Girl;    // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_Boy;     // +0x68
    bool ThunderKidsTalking;                                // +0x78
    bool ThunderSceneStarted;                               // +0x79
    bool ThunderSceneDone;                                  // +0x7A
    bool MazeCutsceneDone;                                  // +0x7B
    bool WhisperCutscenePlayed;                             // +0x7C
    bool SnowTrollSceneDone;                                // +0x7D
    std::map<CCharString, CCharString> csargs;              // +0x80
};
CV_TravellingHeroesScript::CV_TravellingHeroesScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00EDEBA0
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_TravellingHeroesScript) == 0x8C);