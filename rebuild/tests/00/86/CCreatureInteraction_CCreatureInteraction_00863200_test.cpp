// Behaviour test for 0x00863200: CCreatureInteraction(type, creature) stores the type and holds exactly that creature.
#include <cstdio>
#include <cstdlib>
#include <new>

class CThingCreatureBase { public: int id; };

enum ECreatureInteractionType { ECREATURE_INTERACTION_TYPE_DUMMY = 0 };

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };

template <class _Tp> class CRetailList
{
public:
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;
    CRetailList() : _M_node(0)
    {
        _List_node_base* node = (_List_node_base*)std::malloc(sizeof(_Node));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)std::malloc(sizeof(_Node));
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
    CRetailList<CThingCreatureBase*> Participants;
    ECreatureInteractionType         Type;
    CCreatureInteraction(ECreatureInteractionType type, CThingCreatureBase& creature);
    void AddParticipant(CThingCreatureBase& creature) { Participants.push_back(&creature); }
};

CCreatureInteraction::CCreatureInteraction(ECreatureInteractionType type, CThingCreatureBase& creature)
    : Type(type)
{
    AddParticipant(creature);
}

int main()
{
    CThingCreatureBase hero;
    hero.id = 7;
    CCreatureInteraction ci((ECreatureInteractionType)3, hero);
    _List_node_base* s = ci.Participants._M_node;
    _List_node<CThingCreatureBase*>* first = (_List_node<CThingCreatureBase*>*)s->_M_next;
    bool ok = ci.Type == 3 && (_List_node_base*)first != s && first->_M_data == &hero
           && first->_M_next == s && first->_M_prev == s && s->_M_prev == first;
    if (!ok) { std::printf("00863200_TEST FAIL\n"); return 1; }
    std::free(first);
    std::free(s);
    std::printf("00863200_TEST PASS\n");
    return 0;
}