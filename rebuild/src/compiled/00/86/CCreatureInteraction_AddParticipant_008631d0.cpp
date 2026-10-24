// CCreatureInteraction::AddParticipant(CThingCreatureBase& creature) -- append the creature to the interaction's participant list
// Leaf: ?AddParticipant@CCreatureInteraction@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008631D0, length 43 bytes (+5 int3). `__fastcall`, `ret 4`.
// Mangled (Ego_r.pdb): ?AddParticipant@CCreatureInteraction@@QAEXAAVCThingCreatureBase@@@Z.
// Name: the catalog row says "CreatureInteraction_InsertAfter" (descriptive). Its only caller is
//   CThingCreatureBase::AddToCreatureInteraction 0x00662550 (`push esi ; mov ecx,[esi+12Ch] ; call`: the creature itself).
// Callees: malloc 0x00BFEA0E (the 12-byte STLport node).
// Shape: the inlined STLport `push_back` = `insert(end(), x)`: end() (the sentinel, esi) is read before the node is allocated;
//   `_Construct` is a placement new (null-tested); the reference argument's address is the stored pointer value; the links are
//   written next, prev, prev->next, sentinel->prev.
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

void CCreatureInteraction::AddParticipant(CThingCreatureBase& creature)
{
    Participants.push_back(&creature);
}