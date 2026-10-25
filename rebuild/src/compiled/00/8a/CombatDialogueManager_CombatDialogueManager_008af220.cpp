// CombatDialogueManager::CombatDialogueManager() -- construct the combat dialogue manager: empty speech-group map, no dialogue def, no current talker, zeroed text-group index and speech frames
// Leaf: ??0CombatDialogueManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AF220, length 134 bytes (+ int3 padding), thiscall, `ret`, returns this.
// Identity: one caller, 0x006ED46D in the landed CCombatManager::CCombatManager 0x006ED3F0, which declares this body as
//   `CombatDialogueManager::CombatDialogueManager() @ 0x008AF220` for its DialogueManager member. The catalog label
//   "CombatSequenceManager_InitializeHeapList" is wrong.
// Callees: malloc @ 0x00BFEA0E (the 0x18-byte map header node), CBaseIntelligentPointer::CBaseIntelligentPointer()
//   @ 0x00A01B10, SetPItem @ 0x00A01B90, GetPItem() const @ 0x00A01B50, virtual ~CBaseIntelligentPointer @ 0x00A01C10.
// Layout (Ego_r.pdb CombatDialogueManager, 40 bytes there; retail 0x24 with the 12-byte STLport map):
//   SpeechGroupInfoMap +0x00 (map<unsigned long, TSpeechGroupInfo>, 8-byte values -> 0x18-byte nodes),
//   PDialogueDef +0x0C (CDefPointer<const CCombatDialogueDef>), PCurrentTalker +0x10 (CIntelligentPointer<
//   CThingCreatureBase>, vtable 0x0123E7D8), CurrentTalkerTextGroupIndex +0x18, LastSpeechFrame +0x1C,
//   LastBattleCryFrame +0x20.
// Shape: the STLport map constructor inline (header zeroed, allocate(1), node count, _M_empty_initialize), the member
//   initialisers in declaration order with the zero in ebx (the default-constructed talker pointer between them), then
//   the body: `PCurrentTalker = NULL` (a temporary CIntelligentPointer(NULL), copy-assigned through
//   SetPItem(GetPItem()), destroyed) and `CurrentTalkerTextGroupIndex = 0` last.
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

class CBaseObject;
class CThingCreatureBase;
class CDefPointeeBase;
class CCombatDialogueDef;

#pragma pack(push, 1)
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    void* PData;                                            // +0x04
};
#pragma pack(pop)

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>: vtable 0x0123E7D8
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};

template <class T>
class CDefPointer {                                         // 4 bytes
public:
    CDefPointeeBase* Object;                                // +0x00
    CDefPointer() : Object(0) { }
};

class CombatDialogueManager {
public:
    struct TSpeechGroupInfo {                               // 4 bytes in retail (0x18-byte map nodes)
        unsigned long Data;
    };
    std::map<unsigned long, TSpeechGroupInfo> SpeechGroupInfoMap;   // +0x00
    CDefPointer<const CCombatDialogueDef>       PDialogueDef;         // +0x0C
    CIntelligentPointer<CThingCreatureBase>      PCurrentTalker;      // +0x10
    unsigned long CurrentTalkerTextGroupIndex;                        // +0x18
    long          LastSpeechFrame;                                    // +0x1C
    long          LastBattleCryFrame;                                 // +0x20

    CombatDialogueManager();
};

CombatDialogueManager::CombatDialogueManager()
    : LastSpeechFrame(0), LastBattleCryFrame(0)
{
    PCurrentTalker = 0;
    CurrentTalkerTextGroupIndex = 0;
}