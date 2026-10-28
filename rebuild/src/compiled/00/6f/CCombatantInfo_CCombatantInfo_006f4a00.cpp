// CCombatantInfo::CCombatantInfo() -- construct a combatant's info record: the attacker half's empty combat-sequence vector and the defender half (empty attacker map, empty combat wheel, null last attacker, null attack pattern)
// Leaf: ??0CCombatantInfo@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F4A00, 89 bytes, thiscall, `ret`, returns this.
// Identity (proven from layout + callers + PDB): the catalog label "C3DMeshMaterial" is propagated (C3DMeshMaterial is
//   0x30 bytes; this body writes +0x64). Callers 0x006ED2AE (CCombatManager::RegisterAttackedCombatant) and 0x006EDB0A
//   (CCombatManager::RegisterCombatant) build the map<unsigned __int64, CCombatantInfo> value. Ego_r.pdb CCombatantInfo
//   (a struct: ??0CCombatantInfo@@QAE@XZ): Parent +0x00 (left unset), AttackerInfo (CAttackingCombatantInfo) +0x04 whose
//   CombatSequences vector sits at +0x28, DefenderInfo (CDefendingCombatantInfo, the out-of-line copy is 0x006F4850)
//   at +0x34 with STLport member offsets (map +0x34, CombatWheel vector +0x40, PLastAttacker +0x5C, PAttackPattern +0x64).
// Callees: malloc 0x00BFEA0E (the map header node), CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10.
// Shape: both member ctors inline in declaration order; the zero lives in ebx.
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
    CDefendingCombatantInfo() { }
};

class CCombatSequenceBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };
class CAttackingCombatantInfo {
public:
    CThingCreatureBase* Attacked;                               // +0x00
    long  AttackerNumber;                                       // +0x04
    long  CombatWheelRingIndex;                                 // +0x08
    bool  PreferredAttackDollySet;                              // +0x0C
    long  PreferredAttackDollyTimer;                            // +0x10
    float PreferredAttackDollyVector[2];                        // +0x14 (C2DVector)
    bool  UsingCombatWheel;                                     // +0x1C
    long  CurrentSequence;                                      // +0x20
    std::vector<CCountedPointer<CCombatSequenceBase> > CombatSequences;   // +0x24
    CAttackingCombatantInfo() { }
};

struct CCombatantInfo {
    CThingCreatureBase*     Parent;                             // +0x00
    CAttackingCombatantInfo AttackerInfo;                       // +0x04
    CDefendingCombatantInfo DefenderInfo;                       // +0x34
    CCombatantInfo();
};

CCombatantInfo::CCombatantInfo()
{
}