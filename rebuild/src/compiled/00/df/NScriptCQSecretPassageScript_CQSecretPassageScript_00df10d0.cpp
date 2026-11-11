// NScript::CQ_SecretPassageScript::~CQ_SecretPassageScript() -- implicit quest-script destructor: destroy seh_Mother, the eleven minions handles (countdown), FlagMap and GlobalMymap, then ~CScriptBase
// Leaf: ??1CQ_SecretPassageScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF10D0, 285 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00DF10B3 is the scalar deleting destructor 0x00DF10B0, slot 0 of the RTTI-less vtable
//   0x012DDC30; that vtable is stored only by the ctor 0x00DEC7F0, whose only caller is the Alloc 0x00DF1080 (`push 12Ch ;
//   call new ; ... call 0x00DEC7F0`), stored at 0x00CD74E8 by the script-registration routine right after the level name
//   "Q_SecretPassage" (0x012DDC44) and the save tag "S_QSP" (0x012F7844); refs/script_recovery/conversion_queue.tsv maps
//   S_QSP -> Q_SecretPassage -> Alloc 0x00DF1080. Ego_r.pdb ??1CQ_SecretPassageScript@NScript@@UAE@XZ. The body stores no
//   vtable of its own: the destructor is compiler-generated. The catalog label "CParticleEmitter::~CParticleEmitter" is a
//   propagated placeholder and wrong.
// Layout: Ego_r.pdb CQ_SecretPassageScript (0x148) members, 0x14 lower in retail and 4 lower after each Dinkumware map:
//   GlobalMymap +0x54 (std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>), FlagMap +0x60
//   (std::map<CCharString, bool>), minions[11] +0x6C, seh_Mother +0x11C; retail size 0x12C (Alloc `push 12Ch`).
// Callees: _Rb_tree<..., pair<const CCharString, bool>, ...>::_M_erase 0x007ACC50 and _Rb_tree<..., pair<const CCharString,
//   CScriptGameResourceObjectScriptedThingBase>, ...>::_M_erase 0x00CD344A (out of line, private: the bound STLport spellings),
//   free 0x00BFEA14 (header nodes), CScriptGameResourceObjectScriptedThingBase implicit dtor inline (PImp release,
//   operator delete 0x00BFE9BC, then ~CScriptGameResourceObjectBase: vtable 0x0126008C + ~CBaseObject 0x0099A430),
//   ~CScriptBase 0x00CBD510.
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunks 664/665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// TU: the PDB ctor ??0CQ_SecretPassageScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z is defined with an
//   empty body (retail's TU defines it), which emits the vtable, the scalar deleting dtor and this implicit dtor; only the dtor
//   is graded.
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
class CQ_SecretPassageScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> { public:
    CQ_SecretPassageScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool MissionSucceeded;                                                       // +0x48
    bool MissionFailed;                                                          // +0x49
    bool MissionOver;                                                            // +0x4A
    bool MotherFound;                                                            // +0x4B
    bool DoorsBlocked;                                                           // +0x4C
    bool DoorsBeingDone;                                                         // +0x4D
    bool OkayToSpawn;                                                            // +0x4E
    bool QuestStartScreenShown;                                                  // +0x4F
    long GhostSightings;                                                         // +0x50
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> GlobalMymap;   // +0x54
    std::map<CCharString, bool> FlagMap;                                         // +0x60
    CScriptGameResourceObjectScriptedThingBase minions[11];                      // +0x6C
    CScriptGameResourceObjectScriptedThingBase seh_Mother;                       // +0x11C
};
CQ_SecretPassageScript::CQ_SecretPassageScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00DEC7F0
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_SecretPassageScript) == 0x12C);