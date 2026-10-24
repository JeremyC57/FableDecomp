// Behaviour test for 0x008631D0 CCreatureInteraction::AddParticipant (chunk 402): the creature's address is appended at
// the end of the participant list (a new node inserted before the sentinel), keeping the insertion order.
#include <cstdio>
#include <cstdlib>
#include <new>

namespace stl402 {
template<class _Tp> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    _Tp         _M_data;
};
template<class _Tp> class list {
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
    ~list()
    {
        _Node* n = _M_node->_M_next;
        while (n != _M_node) {
            _Node* next = n->_M_next;
            free(n);
            n = next;
        }
        free(_M_node);
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
class CThingCreatureBase {
public:
    int Id;
};
enum ECreatureInteractionType { CREATURE_INTERACTION_NONE };
class CCreatureInteraction {
public:
    stl402::list<CThingCreatureBase*> Participants;
    ECreatureInteractionType          Type;
    CCreatureInteraction() : Type(CREATURE_INTERACTION_NONE) {}
    void AddParticipant(CThingCreatureBase& creature);
};

void CCreatureInteraction::AddParticipant(CThingCreatureBase& creature)
{
    Participants.push_back(&creature);
}

int main()
{
    int failures = 0;
    CThingCreatureBase a, b, c;
    a.Id = 1; b.Id = 2; c.Id = 3;
    CCreatureInteraction interaction;
    interaction.AddParticipant(a);
    interaction.AddParticipant(b);
    interaction.AddParticipant(c);
    CThingCreatureBase* expected[3] = { &a, &b, &c };
    int count = 0;
    stl402::_List_node<CThingCreatureBase*>* sentinel = interaction.Participants._M_node;
    for (stl402::_List_node<CThingCreatureBase*>* n = sentinel->_M_next; n != sentinel; n = n->_M_next, ++count) {
        if (count >= 3 || n->_M_data != expected[count]) { printf("  wrong participant at %d\n", count); ++failures; break; }
        if (n->_M_next->_M_prev != n) { printf("  broken back link\n"); ++failures; }
    }
    if (count != 3) { printf("  %d participants\n", count); ++failures; }
    if (sentinel->_M_prev->_M_data != &c) { printf("  last is not the latest\n"); ++failures; }
    if (failures == 0) {
        printf("008631d0_TEST PASS\n");
        return 0;
    }
    printf("008631d0_TEST FAIL\n");
    return 1;
}