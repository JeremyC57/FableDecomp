// CCreatureInteraction::CCreatureInteraction(ECreatureInteractionType, CThingCreatureBase&) -- empty participant list, store the type, add the creature as the first participant
// Leaf: ??0CCreatureInteraction@@QAE@W4ECreatureInteractionType
// Retail 0x00863200, 0x4C bytes (+ int3 padding), thiscall `ret 8`. Manifest label "CreatureInteraction_InitializeList" is a
// descriptive auto label; the PDB (Ego_r) names ??0CCreatureInteraction@@QAE@W4ECreatureInteractionType@@AAVCThingCreatureBase@@@Z
// and the only caller, CThingCreatureBase::NewCreatureInteraction 0x00662970, does `new(8)` then `push this ; push type ; call`.
// Callee: malloc @ 0x00BFEA0E (STLport node allocator), twice.
// Layout (Ego_r CCreatureInteraction: Participants +0 list<CThingCreatureBase*>, Type after it): retail STLport list is one
// 4-byte sentinel pointer, so Type sits at +0x04 and the object is 8 bytes (matches the caller's new(8)).
// Shape: the inline STLport list constructor (zero, malloc 0x0C sentinel, self-link through a local, store) then the Type
// initialiser, then the body's AddParticipant -- an inlined push_back(&creature): _M_create_node (malloc + placement
// new null-test of &node->_M_data) and the four link stores before the sentinel (`mov ecx,[esi+4]` = old prev).
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CThingCreatureBase;

enum ECreatureInteractionType { ECREATURE_INTERACTION_TYPE_DUMMY = 0 };

struct _List_node_base
{
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base
{
    _Tp _M_data;
};

template <class _Tp> class CRetailList                // std::list<_Tp> (STLport): the sentinel pointer only
{
public:
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;

    CRetailList() : _M_node(0)
    {
        _List_node_base* node = (_List_node_base*)malloc(sizeof(_Node));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    void insert(_List_node_base* __n, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
    }
    void push_back(const _Tp& __x) { insert(_M_node, __x); }
};

class CCreatureInteraction
{
public:
    CRetailList<CThingCreatureBase*> Participants;   // +0x00
    ECreatureInteractionType         Type;           // +0x04

    CCreatureInteraction(ECreatureInteractionType type, CThingCreatureBase& creature);
    void AddParticipant(CThingCreatureBase& creature) { Participants.push_back(&creature); }
};

CCreatureInteraction::CCreatureInteraction(ECreatureInteractionType type, CThingCreatureBase& creature)
    : Type(type)
{
    AddParticipant(creature);
}