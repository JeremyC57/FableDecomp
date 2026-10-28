// CDefendingCombatantInfo::CDefendingCombatantInfo() -- construct a defender's combat info: empty attacker map, empty combat wheel, a null last-attacker pointer and a null attack-pattern def
// Leaf: ??0CDefendingCombatantInfo@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F4850, 75 bytes, thiscall, `ret`, returns this.
// Identity (proven from layout + PDB): the catalog label "C3DAnimFileSequenceChunk" is propagated (that class is 0x58 bytes
//   of Ego_r padding with no map). The body sits among the CCombatManager / CDefendingCombatantInfo / CCombatantInfo
//   functions (0x006F46D0 CDefendingCombatantInfo copy, 0x006F4990 CCombatantInfo) and builds exactly the Ego_r.pdb
//   CDefendingCombatantInfo members at their STLport offsets: Attackers map<unsigned __int64, CThingCreatureBase*> +0x00
//   (0x20-byte nodes = 0x10 base + 8-byte key + pointer, padded), CombatWheel (a vector<CCombatRing>) +0x0C,
//   LastVictimWheelUpdatePos +0x18 (C3DVector, no ctor), LastWheelUpdateWorldFrame +0x24, PLastAttacker
//   (CIntelligentPointer<CThing>, vtable 0x01238C6C) +0x28, PAttackPattern (CDefPointer<const CAttackPatternDef>) +0x30.
//   The same member group is inlined at +0x34 by 0x006F4A00 (CCombatantInfo::CCombatantInfo, DefenderInfo +0x34).
//   Ego_r.pdb ??0CDefendingCombatantInfo@@QAE@XZ.
// Callees: malloc 0x00BFEA0E (the map header node), CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10.
// Shape: the STLport 4.6 map ctor inline (proxy zeroed, allocate(1), node count, _M_empty_initialize), the vector
//   ctor's three zero stores, the intelligent pointer's base ctor then its vptr, the def pointer zero.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

namespace std {
template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
};
template <class T> struct less { };
template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;
    _Rb_tree_base(const A& a) : _M_header(allocator<_Node>(), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
};
template <class K, class V, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    unsigned int _M_node_count;
    Compare      _M_key_compare;
    _Rb_tree() : _Rb_tree_base<V, A>(A()), _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class K, class T, class Compare = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    _Rb_tree<K, pair<const K, T>, Compare, A> _M_t;
};
template <class V, class T, class A> struct _STLP_alloc_proxy3 : public A {
    V _M_data;
    _STLP_alloc_proxy3(const A& a, V p) : A(a), _M_data(p) { }
};
template <class T, class A> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy3<T*, T, A> _M_end_of_storage;
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    vector() : _Vector_base<T, A>(A()) { }
};
}

class CBaseObject;
class CThing;
class CThingCreatureBase;
class CDefPointeeBase;
class CAttackPatternDef;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void* PData;                                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>: vtable 0x01238C6C
public:
    CIntelligentPointer() { }
};
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
};
class C3DVector { public: float X, Y, Z; };

class CDefendingCombatantInfo {
public:
    class CCombatRing { public: unsigned char _body[0x1C]; };
    class CCombatWheel {
    public:
        std::vector<CCombatRing> Rings;                     // +0x00
    };
    std::map<unsigned __int64, CThingCreatureBase*> Attackers;   // +0x00
    CCombatWheel                       CombatWheel;              // +0x0C
    C3DVector                          LastVictimWheelUpdatePos; // +0x18
    long                               LastWheelUpdateWorldFrame;// +0x24
    CIntelligentPointer<CThing>        PLastAttacker;            // +0x28
    CDefPointer<const CAttackPatternDef> PAttackPattern;         // +0x30
    long                               AttackPatternIndex;       // +0x34
    long                               FrameForNextInstruction;  // +0x38
    CDefendingCombatantInfo();
};

CDefendingCombatantInfo::CDefendingCombatantInfo()
{
}