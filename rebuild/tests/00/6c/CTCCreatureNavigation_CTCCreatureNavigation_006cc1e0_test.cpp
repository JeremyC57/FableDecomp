// Behaviour test for CTCCreatureNavigation::~CTCCreatureNavigation @ 0x006cc1e0: with an active navigator the agent list
// leaves the global lists, the navigator is forgotten and every possible navigator is deleted and nulled; without one, only
// the member dtors run.
#include <stdio.h>
#include <stdlib.h>
static char g_log[256];
static void logc(char c) { size_t n = 0; while (g_log[n]) ++n; g_log[n] = c; g_log[n + 1] = 0; }
static int g_deleted = 0;
static void* g_removedList = 0;
class CNavigatorBase { public: virtual ~CNavigatorBase() { g_deleted++; logc('d'); } };
template <class T> struct Vec { T* Start; T* Finish; T* End; unsigned int size() const { return (unsigned int)(Finish - Start); } };
class CNavigatorExternalInterface { public: static void RemoveAgentListFromLists(void* agents) { g_removedList = agents; logc('R'); } };
class CTCBase { public: virtual ~CTCBase() { logc('T'); } };
class CTCCreatureNavigation : public CTCBase { public:
    CNavigatorBase* PNavigator;
    Vec<CNavigatorBase*> PossibleNavigators;
    Vec<const void*> Agents;
    CNavigatorBase** seen;   // test hook: the vector block before it is freed
    virtual ~CTCCreatureNavigation()
    {
        if (PNavigator) {
            CNavigatorExternalInterface::RemoveAgentListFromLists(&Agents);
            PNavigator = 0;
            for (unsigned long i = 0; i < PossibleNavigators.size(); ++i) {
                delete PossibleNavigators.Start[i];
                PossibleNavigators.Start[i] = 0;
            }
        }
        logc('a'); free(Agents.Start);        // implicit member dtors, reverse order
        logc('p');                            // PThingToNotAvoid
        for (unsigned int i = 0; i < PossibleNavigators.size(); ++i) if (PossibleNavigators.Start[i]) logc('!');
        logc('v'); free(PossibleNavigators.Start);
    }
};
static CTCCreatureNavigation* make(bool active)
{
    CTCCreatureNavigation* n = new CTCCreatureNavigation;
    n->PossibleNavigators.Start = (CNavigatorBase**)malloc(3 * sizeof(CNavigatorBase*));
    n->PossibleNavigators.Start[0] = new CNavigatorBase; n->PossibleNavigators.Start[1] = 0; n->PossibleNavigators.Start[2] = new CNavigatorBase;
    n->PossibleNavigators.Finish = n->PossibleNavigators.End = n->PossibleNavigators.Start + 3;
    n->Agents.Start = (const void**)malloc(sizeof(void*)); n->Agents.Finish = n->Agents.End = n->Agents.Start;
    n->PNavigator = active ? n->PossibleNavigators.Start[0] : 0;
    return n;
}
int main()
{
    CTCCreatureNavigation* n = make(true);
    void* agents = &n->Agents;
    CTCBase* b = n; delete b;
    const char* want = "RddapvT";
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) { printf("006cc1e0_TEST FAIL\n"); return 1; }
    if (g_deleted != 2 || g_removedList != agents) { printf("006cc1e0_TEST FAIL\n"); return 1; }
    g_log[0] = 0; g_deleted = 0; g_removedList = 0;
    n = make(false);
    CNavigatorBase* keep0 = n->PossibleNavigators.Start[0]; CNavigatorBase* keep2 = n->PossibleNavigators.Start[2];
    b = n; delete b;
    if (g_deleted != 0 || g_removedList != 0 || g_log[0] != 'a' || g_log[2] != '!') { printf("006cc1e0_TEST FAIL\n"); return 1; }
    delete keep0; delete keep2;
    printf("006cc1e0_TEST PASS\n");
    return 0;
}