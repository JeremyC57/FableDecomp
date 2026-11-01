// NScript::CQ_DragonBossFightScript::~CQ_DragonBossFightScript() -- implicit quest-script destructor: deregister the timers, destroy the cs_flags map and spawn-point vector members in reverse order, then ~CScriptBase
// Leaf: ??1CQ_DragonBossFightScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D27290, tail jump to ~CScriptBase 0x00CBD510; sole caller the scalar deleting dtor
//   (vtable slot 0) of the RTTI-less quest vtable.
// Identity (proven): the vtable whose slot 0 calls this body is stored only by the ctor 0x00D25410 (manifest label CQ_DragonBossFightScript::CQ_DragonBossFightScript);
//   that ctor's only caller is the Alloc that `new`s 0x98 bytes, registered by the script-registration routine with the
//   quest name "Q_DragonBossFight" (string 0x012C45B4) -- CQ_DragonBossFightScript is the Q_<name> + "Script" class of that registration. Ego_r.pdb and
//   FableWin.pdb both carry ??1CQ_DragonBossFightScript@NScript@@UAE@XZ (virtual, public: the implicit dtor of a CScriptBase-derived class).
//   The body stores no vtable: the dtor is compiler-generated (no user ~CQ_DragonBossFightScript). The manifest label (CParticleEmitter_Dtor_N)
//   is propagated and wrong.
// Layout: Ego_r.pdb CQ_DragonBossFightScript members with retail offsets (CScriptBase 0x40 in retail vs 0x54 in Ego_r: quest-script members sit
//   0x14 lower; CLevelScriptBase<CQ_SunnyvaleMasterData> Game +0x40 / MasterScriptData +0x44), retail STLport containers.
//   Members destroyed: SummonerSpawnDelay +0x94, MinionSpawnDelay +0x90, cs_flags std::map<CCharString,bool> +0x84 (STLport
//   clear + _M_erase 0x007ACC50 + header free), SummonerSpawnPoints +0x58 / MinionSpawnPoints +0x4c std::vector<CScriptThing>
//   (virtual element dtor, free 0x00BFEA14).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (the
//   CLevelScriptBase<T> scalar deleting dtors 0x00CDD250 / 0x00CDD270 call ~CScriptBase 0x00CBD510 directly, no vtable
//   store; an inline user `~CLevelScriptBase() {}` adds `mov [esi],vftable` before the tail jump -- probe 1 of 0x00D2F640),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt").
// Callees: CTimer::~CTimer inline (engine/CTimer.h: interface slot 88 DeregisterTimer through g_pGameScriptInterface 0x0143E8F8),
//   ~CScriptBase 0x00CBD510.
// TU: the ctor is defined (as retail's TU defines it), which emits the vtable, the scalar deleting dtor and this implicit dtor.
//   Built without /EHsc (no EH frame; the member-array dtors are inline countdown loops, no `eh vector destructor iterator`).
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
#include "engine/CTimer.h"
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
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
namespace std {
// STLport 4.6 stl/_construct.h, stl/_vector.h (the members this row needs)
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
protected:
    _Tp* _M_start;                                               // +0x00
    _Tp* _M_finish;                                              // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
namespace NScript {
class CQ_SunnyvaleMasterData;
enum EDragonState { };
class CQ_DragonBossFightScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_DragonBossFightScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    EDragonState DragonState;                                            // +0x48
    std::vector<CScriptThing> MinionSpawnPoints;                         // +0x4c
    std::vector<CScriptThing> SummonerSpawnPoints;                       // +0x58
    long NextSummonerSpawnPoint;                                         // +0x64
    long TargetNumMinions;                                               // +0x68
    long TargetNumSummoners;                                             // +0x6c
    long NumMinions;                                                     // +0x70
    long NumSummoners;                                                   // +0x74
    long NumFlyBysBetweenSummonerSpawns;                                 // +0x78
    long NumFlyBysSinceLastSummonerSpawn;                                // +0x7c
    bool MinionSpawningEnabled;                                          // +0x80
    bool SummonerSpawningEnabled;                                        // +0x81
    std::map<CCharString, bool> cs_flags;                                // +0x84
    CTimer MinionSpawnDelay;                                             // +0x90
    CTimer SummonerSpawnDelay;                                           // +0x94
};
CQ_DragonBossFightScript::CQ_DragonBossFightScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00D25410
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}