// NScript::CQ_AwakeningTheOracleScript::~CQ_AwakeningTheOracleScript() -- the quest script's compiler-generated destructor: clear and free the cs_chars and cs_args maps, destroy the DiggingPoints CScriptThings (virtual dtor) and free the vector, then ~CScriptBase
// Leaf: ??1CQ_AwakeningTheOracleScript
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D005E0, 185 bytes. Sole caller: the ??_G 0x00D005C0, slot 0 of the script vtable
//   0x012C78B0 (no RTTI COL: the NScript TUs; slots 1..4 = RegisterMain 0x00CFAEB0, Main 0x00CFB080, Init 0x00CFADD0,
//   OnPersist 0x00CFAF70, the CQ_AwakeningTheOracleScript members per Ego_r.pdb), stored by the class's ctor 0x00CFAD30.
// Identity: the ??_G/??1 pair closes the Q_AwakeningTheOracle TU (0x00CFAD30 ctor .. 0x00D00440 CLostBayCutsceneTrigger::Main,
//   then this pair, then 0x00D006A0 CQ_BanditCampScript); Ego_r.pdb ??1CQ_AwakeningTheOracleScript@NScript@@UAE@XZ (0x00D02380).
//   No vtable store: the dtor is compiler-generated (the class declares none), and so is CLevelScriptBase<T>'s (retail's
//   out-of-line copies 0x00CDD100..0x00CDD150 are bare `jmp ~CScriptBase`), so the tail jump goes to ~CScriptBase 0x00CBD510.
// Callees: STLport _Rb_tree<..>::_M_erase for map<CCharString, CScriptGameResourceObjectScriptedThingBase> 0x00CD344A and
//   map<CCharString, CCharString> 0x009AA830 (bound PDB-format spellings); free 0x00BFEA14; CScriptThing's virtual dtor (slot 0,
//   `push 0 ; call [eax]`); CScriptBase::~CScriptBase 0x00CBD510.
// Layout (Ego_r.pdb less 0x14 for the retail script base, 12-byte STLport containers): bools/longs +0x48..+0x70,
//   DiggingPoints vector<CScriptThing> +0x74, cs_args map<CCharString,CCharString> +0x80 (node 0x18 in the ctor),
//   cs_chars map<CCharString,CScriptGameResourceObjectScriptedThingBase> +0x8C (node 0x24).
// Shape: the members in reverse order through the STLport 4.6 inline bodies (~_Rb_tree = clear() + ~_Rb_tree_base freeing the
//   header; ~vector = _Destroy range + ~_Vector_base). The class's ctor (retail 0x00CFAD30) is defined so the TU emits the
//   vtable, ??_G and this implicit ??1, as retail's TU does.
// Header debt: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` -- with it the derived implicit dtor tail-jumps
//   to ??1CLevelScriptBase<T> (0x00CDD1xx) and a `{}` body adds its vtable store (191 v 185); retail's is compiler-generated, so
//   CLevelScriptBase<T> is the header's model without that declaration. CCharString has no header (the landed one-member model).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
// CLevelScriptBase<T>: engine/CLevelScriptBase.h less its `virtual ~CLevelScriptBase();` declaration (retail's dtor is the
// compiler-generated one; see the header comment above).
template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CCharString {
public:
    void* PStringData;                                                   // +0x00
    CCharString();                                                       // 0x0099E4B0
    ~CCharString();                                                      // 0x0099EAE0
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
// ---- vector (stl/_vector.h)
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
// ---- tree (stl/_tree.h)
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
    _Rb_tree_base(const allocator_type& __a) : _M_header(allocator<_Node>(), (_Rb_tree_node_base*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
    typedef _Rb_tree_base<_Value, _Alloc> _Base;
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Alloc allocator_type;
protected:
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;   // _S_rb_tree_red
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
public:
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(allocator_type()), _M_node_count(0), _M_key_compare(_Compare())
    {
        _M_empty_initialize();
    }
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
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
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
    map() : _M_t() { }
};
} // namespace std

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AwakeningTheOracleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_AwakeningTheOracleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool MissionSucceeded;                                               // +0x48
    bool PlayedOpeningCS;                                                // +0x49
    bool PlayedLostBayCS;                                                // +0x4a
    bool PlayedNecroCS;                                                  // +0x4b
    long TabletsCollected;                                               // +0x4c
    bool SpawnedFinalSpotsStuff;                                         // +0x50
    long PositionTaken;                                                  // +0x54
    bool GivenDiggingSpotsRewards;                                       // +0x58
    long GraveNumbersGivenOut;                                           // +0x5c
    long FinalSummonersAlive;                                            // +0x60
    bool DisplayedBadTabletMessage;                                      // +0x64
    bool PlayedOpeningNecroCS;                                           // +0x65
    long TotalSpawnedCreaturesAlive;                                     // +0x68
    bool AutosavedInNW3;                                                 // +0x6c
    bool GotY;                                                           // +0x6d
    bool GotM;                                                           // +0x6e
    bool GotC;                                                           // +0x6f
    bool GotA;                                                           // +0x70
    std::vector<CScriptThing> DiggingPoints;                             // +0x74
    std::map<CCharString, CCharString> cs_args;                          // +0x80
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cs_chars;   // +0x8c
};
CQ_AwakeningTheOracleScript::CQ_AwakeningTheOracleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
} // namespace NScript

