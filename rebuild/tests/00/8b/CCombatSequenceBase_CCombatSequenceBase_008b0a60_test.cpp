// Behaviour test for 008b0a60 (CCombatSequenceBase::~CCombatSequenceBase): the CombatActions vector destroys its element range and then frees its block when allocated.
#include <cstdio>

struct CCombatAction { long m_words[2]; };

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };

static CCombatAction* g_destroyFirst = 0;
static CCombatAction* g_destroyLast = 0;
static int   g_destroyCalls = 0;
static void* g_freed = 0;

void DestroyRange(CCombatAction* first, CCombatAction* last, const SNonscalarPtrIteratorTag&)
{
    g_destroyFirst = first; g_destroyLast = last; ++g_destroyCalls;
}

template <class T> struct CRetailVector {
    T* first;
    T* last;
    T* end_of_storage;
    ~CRetailVector()
    {
        DestroyRange(first, last, SNonscalarPtrIteratorTag());
        if (first != 0)
            g_freed = first;
    }
};

struct CCombatSequenceBase {
    bool                          Running;
    long                          NoAvailableActions;
    long                          CurrentActionIndex;
    CRetailVector<CCombatAction>  CombatActions;
    virtual ~CCombatSequenceBase() { }
};

int main()
{
    static CCombatAction actions[3];
    {
        CCombatSequenceBase s;
        s.CombatActions.first = actions; s.CombatActions.last = actions + 2; s.CombatActions.end_of_storage = actions + 3;
    }
    if (g_destroyCalls != 1 || g_destroyFirst != actions || g_destroyLast != actions + 2 || g_freed != (void*)actions) { std::printf("008b0a60_TEST FAIL\n"); return 1; }
    g_freed = 0;
    {
        CCombatSequenceBase s;
        s.CombatActions.first = 0; s.CombatActions.last = 0; s.CombatActions.end_of_storage = 0;
    }
    if (g_destroyCalls != 2 || g_freed != 0) { std::printf("008b0a60_TEST FAIL\n"); return 1; }
    std::printf("008b0a60_TEST PASS\n");
    return 0;
}