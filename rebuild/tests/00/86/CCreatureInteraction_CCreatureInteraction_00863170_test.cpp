// Behaviour test for 0x00863170 CCreatureInteraction::CCreatureInteraction (chunk 402): the participant list starts with
// one allocated sentinel node linked to itself and the interaction type is NONE.
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
};
}
class CThingCreatureBase;
enum ECreatureInteractionType { CREATURE_INTERACTION_NONE, CREATURE_INTERACTION_OTHER };
class CCreatureInteraction {
public:
    stl402::list<CThingCreatureBase*> Participants;
    ECreatureInteractionType          Type;
    CCreatureInteraction();
};

CCreatureInteraction::CCreatureInteraction()
    : Type(CREATURE_INTERACTION_NONE)
{
}

int main()
{
    int failures = 0;
    union { double align; unsigned char bytes[sizeof(CCreatureInteraction)]; } raw;
    for (unsigned int i = 0; i < sizeof(raw.bytes); ++i)
        raw.bytes[i] = 0xCD;                                // garbage before construction
    CCreatureInteraction* interaction = new (raw.bytes) CCreatureInteraction;
    if (interaction->Type != CREATURE_INTERACTION_NONE) { printf("  type not NONE\n"); ++failures; }
    if (interaction->Participants._M_node == 0) { printf("  no sentinel\n"); ++failures; }
    else if (interaction->Participants._M_node->_M_next != interaction->Participants._M_node
             || interaction->Participants._M_node->_M_prev != interaction->Participants._M_node) {
        printf("  sentinel not self-linked\n"); ++failures;
    }
    interaction->~CCreatureInteraction();
    if (failures == 0) {
        printf("00863170_TEST PASS\n");
        return 0;
    }
    printf("00863170_TEST FAIL\n");
    return 1;
}