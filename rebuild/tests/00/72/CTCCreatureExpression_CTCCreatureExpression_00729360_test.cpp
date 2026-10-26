// Behaviour test for CTCCreatureExpression::CTCCreatureExpression @ 0x00729360: the expression-state list
// starts empty (one sentinel node linked to itself) and the creature is not blinking.
#include <cstdio>
#include <cstdlib>
#include <new>

class CThing { public: int unused; };

struct CTCBase_Calls {
    virtual ~CTCBase_Calls() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
};

struct CExpressionState { unsigned char _body[0x14]; };

template<class T>
struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList() : head(0)
    {
        node* sentinel = (node*)std::malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
    ~CRetailList() { std::free(head); }
};

class CTCCreatureExpression : public CTCBase_Calls { public:
    CRetailList<CExpressionState> ExpressionStates;
    bool                          Blinking;
    CTCCreatureExpression(CThing* owner) : CTCBase_Calls(owner), Blinking(false) {}
};

int main()
{
    CThing owner;
    unsigned char storage[sizeof(CTCCreatureExpression)];
    for (unsigned i = 0; i < sizeof(storage); ++i) storage[i] = 0xCD;
    CTCCreatureExpression* p = new (storage) CTCCreatureExpression(&owner);
    CRetailList<CExpressionState>::node* h = p->ExpressionStates.head;
    bool ok = p->ParentThing == &owner && h != 0 && h->next == h && h->prev == h && !p->Blinking;
    p->~CTCCreatureExpression();
    if (!ok) { std::printf("00729360_TEST FAIL\n"); return 1; }
    std::printf("00729360_TEST PASS\n");
    return 0;
}