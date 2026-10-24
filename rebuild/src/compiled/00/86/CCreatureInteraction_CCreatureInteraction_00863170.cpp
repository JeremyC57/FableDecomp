// CCreatureInteraction::CCreatureInteraction() -- an empty interaction: the participant list gets its sentinel node, the type is 0
// Leaf: ??0CCreatureInteraction@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00863170, length 37 bytes (+11 int3). `__fastcall`, returns `this`.
// Mangled (Ego_r.pdb): ??0CCreatureInteraction@@QAE@XZ.
// Name: the catalog row says "LinkedList_Init" (descriptive). CCreatureInteraction (Ego_r.pdb) is {list<CThingCreatureBase*>
//   Participants; ECreatureInteractionType Type}; with the 4-byte retail STLport list the type sits at +4, the word this
//   constructor zeroes last; AddParticipant 0x008631D0 (this chunk) inserts into the same list.
// Callees: malloc 0x00BFEA0E (the STLport node allocator: 12-byte node, no null test).
// Shape: STLport `_List_base` constructor: the allocator proxy zeroes the node pointer first, the sentinel is built in a local
//   (next/prev point at itself) and stored into the list last; `Type` follows as an init-list zero.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (MSVCR71)
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* where) throw() { return where; }
namespace std {
template<class _Tp> struct _List_node {                 // STLport list node: next, prev, data (0x0C bytes for a pointer)
    _List_node* _M_next;
    _List_node* _M_prev;
    _Tp         _M_data;
};
template<class _Tp> class list {                        // retail STLport list: one word, the sentinel node pointer
public:
    typedef _List_node<_Tp> _Node;
    _Node* _M_node;
    list() : _M_node(0)
    {
        _Node* __tmp = (_Node*)malloc(sizeof(_Node));
        __tmp->_M_next = __tmp;
        __tmp->_M_prev = __tmp;
        _M_node = __tmp;
    }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __tmp = (_Node*)malloc(sizeof(_Node));
        new (&__tmp->_M_data) _Tp(__x);
        return __tmp;
    }
    _Node* insert(_Node* __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _Node* __p = __position->_M_prev;
        __tmp->_M_next = __position;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __position->_M_prev = __tmp;
        return __tmp;
    }
    _Node* end() { return _M_node; }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}
class CThingCreatureBase;
enum ECreatureInteractionType { CREATURE_INTERACTION_NONE };
class CCreatureInteraction {
public:
    std::list<CThingCreatureBase*> Participants;        // +0x00 (retail STLport list: 4 bytes; Ego_r.pdb's Dinkumware list is 12)
    ECreatureInteractionType       Type;                // +0x04 (Ego_r.pdb +0x0C)
    CCreatureInteraction();
    void AddParticipant(CThingCreatureBase& creature);
};

CCreatureInteraction::CCreatureInteraction()
    : Type(CREATURE_INTERACTION_NONE)
{
}