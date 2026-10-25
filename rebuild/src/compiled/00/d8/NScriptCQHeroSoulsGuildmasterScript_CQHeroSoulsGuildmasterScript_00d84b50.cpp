// NScript::CQ_HeroSoulsGuildmasterScript::CQ_HeroSoulsGuildmasterScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- constructor of the Q_HeroSoulsGuildmaster quest script: level-script base, then the empty cs_flags map
// Leaf: ??0CQ_HeroSoulsGuildmasterScript@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D84B50, 86 bytes, `__fastcall`, two stack arguments (`ret 8`), returns this.
//   Its only caller is the quest's Alloc @ 0x00D87A80 (`new` 0x64 bytes, then this ctor out of line), the pAllocFunc of
//   the "Q_HeroSoulsGuildmaster" CScriptInfo record (section "S_QHSG", stored at 0x00CD6A2A).
// Label correction: the catalog "CHeroMorphDef_Create_3" is propagated from the CScriptBase ctor's own wrong label.
// Callees: CScriptBase::CScriptBase() @ 0x00CB8110, malloc @ 0x00BFEA0E (the map's 0x18-byte header node). Vtable 0x012D4804.
// Layout: Ego_r.pdb NScript::CQ_HeroSoulsGuildmasterScript (124 bytes) with the retail 0x40-byte script base (PDB offsets minus
//   0x14) and a 12-byte STLport map (PDB 16): cs_flags map<CCharString, bool> +0x58 (0x18-byte node), sizeof 0x64.
// Shape: speed TU (no pragma); the 0x00CFAD30 CQ_AwakeningTheOracleScript model with one map. CLevelScriptBase<T> is a
//   novtable intermediate base (Game +0x40, data +0x44 stored before the vtable); the map is the STLport
//   `_Rb_tree(_Compare(), allocator_type())` with imm32 zero stores (too few zeros for a shared register). The
//   plain-data members stay uninitialised.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

class CGameScriptInterfaceBase;
class CScriptDataBase;
class CCharString {                                         // 4 bytes
public:
    void* PStringData;
};

namespace std {
template <class T> class allocator {
public:
    allocator() { }
    template <class U> allocator(const allocator<U>&) { }
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
};
template <class T> class less {
public:
    bool operator()(const T& a, const T& b) const;
};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const A& a) : _M_header(allocator<_Node>(a), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
};
template <class K, class V, class KeyOfValue, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    unsigned int _M_node_count;
    Compare      _M_key_compare;
    _Rb_tree(const Compare& comp, const A& a) : _Rb_tree_base<V, A>(a), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
};
template <class T1, class T2> class pair {
public:
    T1 first;
    T2 second;
};
template <class P> struct _Select1st { };
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    _Rep_type _M_t;
    map() : _M_t(C(), A()) { }
};
} // namespace std

struct CScriptBase {                                        // 0x40 bytes, the quest-script root
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                              // +0x04..+0x3f
    CScriptBase();                                          // 0x00CB8110
};
template <class TData>
struct __declspec(novtable) CLevelScriptBase : CScriptBase {
    CGameScriptInterfaceBase* Game;                         // +0x40
    TData*                    MasterScriptData;             // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CScriptBase(), Game(&game), MasterScriptData((TData*)data) { }
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsGuildmasterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool PlayedBriarCS;                                     // +0x48
    bool PlayedGuildMasterCS;
    bool PlayedGuildCS;
    bool OutsideWoodGuardsAttack;
    bool GiveWarningCutscene;                               // +0x4c
    long GuardInWoodsCount;                                 // +0x50
    long DistanceReached;                                   // +0x54
    std::map<CCharString, bool> cs_flags;                  // +0x58
    CQ_HeroSoulsGuildmasterScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_HeroSoulsGuildmasterScript();
};
CQ_HeroSoulsGuildmasterScript::CQ_HeroSoulsGuildmasterScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}