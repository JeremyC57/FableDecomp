// Behaviour test for 0x0072DA60 CTCSpecialAbilities::~CTCSpecialAbilities (implicit): AbilityAvailability's block is freed,
// then the UndeadFollowers list is cleared (clear()) and its sentinel freed, then ~CTCBase runs.
#include <cstdio>
#include <cstdlib>
static const void* g_log[16]; static int g_n = 0;
static void Free(void* p) { if (p) { g_log[g_n++] = p; std::free(p); } }
static int g_clearAt = -1, g_baseAt = -1;
struct Node { Node* next; Node* prev; };
struct List {                                   // STLport list: the sentinel pointer
    Node* node;
    void clear() { g_clearAt = g_n; Node* c = node->next; while (c != node) { Node* t = c; c = c->next; Free(t); } node->next = node->prev = node; }
    ~List() { clear(); Free(node); }
};
struct BoolVector { bool* start; bool* finish; bool* eos; ~BoolVector() { if (start != 0) Free(start); } };
class CTCBase { public: virtual ~CTCBase() { g_baseAt = g_n; } void* ParentThing; bool ToBeRemoved, IsInGlobalUpdateTCs; };
class CTCSpecialAbilities : public CTCBase { public:
    bool AbilityMutexLock, UsingBed; int CurrentAbility;
    List UndeadFollowers; void* Creature; BoolVector AbilityAvailability;
};
int main()
{
    CTCSpecialAbilities* a = new CTCSpecialAbilities;
    Node* s = (Node*)std::malloc(sizeof(Node)); Node* f = (Node*)std::malloc(sizeof(Node));
    s->next = s->prev = f; f->next = f->prev = s;
    a->UndeadFollowers.node = s;
    a->AbilityAvailability.start = (bool*)std::malloc(20); a->AbilityAvailability.finish = a->AbilityAvailability.start + 20;
    a->AbilityAvailability.eos = a->AbilityAvailability.finish;
    void* block = a->AbilityAvailability.start;
    delete a;
    if (g_n != 3 || g_log[0] != block || g_clearAt != 1 || g_log[1] != f || g_log[2] != s || g_baseAt != 3) {
        std::printf("0072da60_TEST FAIL\n"); return 1;
    }
    std::printf("0072da60_TEST PASS\n");
    return 0;
}