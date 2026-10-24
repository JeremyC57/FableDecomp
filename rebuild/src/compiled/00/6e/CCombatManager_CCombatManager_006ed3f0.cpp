// CCombatManager::CCombatManager(CWorld& world) -- construct the combat manager: two empty STLport maps (registered sequence factories, combatants), the world and the tuning defaults, the dialogue manager, then register the combat sequence factories
// Leaf: ??0CCombatManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006ED3F0, 144 bytes, thiscall, `ret 4`, returns this.
// Identity: the vtable stored is 0x01261EB4 (RTTI `.?AVCCombatManager@@`, one slot); Ego_r.pdb name set
//   `??0CCombatManager@@QAE@AAVCWorld@@@Z`. Single caller 0x004A71BF. The catalog's "CombatSequenceManager_Ctor" is
//   descriptive.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0 (the base), malloc @ 0x00BFEA0E (the two
//   map header nodes, 0x18 and 0x88 bytes), CombatDialogueManager::CombatDialogueManager() @ 0x008AF220,
//   CCombatManager::RegisterCombatSequences() @ 0x008B0D10 (landed).
// Layout (retail; Ego_r.pdb names, 12-byte STLport maps): RegisteredCombatSequences +0x04 (map<CCharString, factory>),
//   Combatants +0x10 (map<unsigned __int64, CCombatantInfo>, 0x70-byte values), CombatStrafeTurningEnabled +0x1C,
//   World +0x20, DrawDebug +0x24, TraceDebug +0x25, ProximityForNavigation +0x28 (2.6f), EnableAINonAttackMode +0x2C,
//   AttackPatternIdleStandardInterval +0x30 and AttacksFromNearestCombatantOnly +0x34 (left unset), DialogueManager +0x38.
// Shape: the STLport `_Rb_tree_base(a)` / `_Rb_tree()` / `_M_empty_initialize` inline map constructors (as 0x006E6150),
//   the member initialisers in declaration order with the zero in ebx, the out-of-line dialogue-manager constructor,
//   then the body's RegisterCombatSequences() call.
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
        this->_M_header._M_data->_M_color = false;          // red
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
}

class CCharString {
public:
    void* PStringData;
};
class CThingCreatureBase;
class CCombatSequenceBase;
class CWorld;
typedef CCombatSequenceBase* (__fastcall* TCombatSequenceFactory)(CThingCreatureBase&, CThingCreatureBase&);

struct CCombatantInfo {                                     // 0x70 bytes in retail
    unsigned char _data[0x70];
};

class CombatDialogueManager {                               // 0x24 bytes in retail, 4-aligned
public:
    unsigned long _data[0x24 / 4];
    CombatDialogueManager();                                // 0x008AF220
};

class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                                // 0x0099A2F0 (stores vtable 0x0129A7C4)
public:
    virtual ~CBaseClassNonCopyable();
};

class CCombatManager : public CBaseClassNonCopyable {
public:
    std::map<CCharString, TCombatSequenceFactory> RegisteredCombatSequences;   // +0x04
    std::map<unsigned __int64, CCombatantInfo>      Combatants;              // +0x10
    bool  CombatStrafeTurningEnabled;                       // +0x1C
    CWorld* World;                                          // +0x20
    bool  DrawDebug;                                        // +0x24
    bool  TraceDebug;                                       // +0x25
    float ProximityForNavigation;                           // +0x28
    bool  EnableAINonAttackMode;                            // +0x2C
    float AttackPatternIdleStandardInterval;                // +0x30
    bool  AttacksFromNearestCombatantOnly;                  // +0x34
    CombatDialogueManager DialogueManager;                  // +0x38

    CCombatManager(CWorld& world);
    virtual ~CCombatManager();
private:
    void RegisterCombatSequences();                         // 0x008B0D10
};

CCombatManager::CCombatManager(CWorld& world)
    : CombatStrafeTurningEnabled(true), World(&world), DrawDebug(false), TraceDebug(false),
      ProximityForNavigation(2.6f), EnableAINonAttackMode(false)
{
    RegisterCombatSequences();
}