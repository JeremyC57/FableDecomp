#include <cstdio>
struct SCreatureNode { SCreatureNode* Next; SCreatureNode* Prev; };
struct CRetailList_Creature {
    SCreatureNode* Node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SCreatureNode* first = Node->Next; first != Node; ) { first = first->Next; ++n; }
        return n;
    }
};
static long g_GeneratedCreatureLimit = -1;
static long g_GeneratedCreatureCount = 0;
class CTCCreatureGenerator { public:
    CRetailList_Creature CreatedCreatures; long ActiveCreatureLimit; long TotalGenerationLimit; long CreationCount;
    bool HasHitCreationLimit() const
    {
        if (ActiveCreatureLimit != -1 && CreatedCreatures.size() >= (unsigned long)ActiveCreatureLimit)
            return true;
        if (g_GeneratedCreatureLimit != -1 && g_GeneratedCreatureCount >= g_GeneratedCreatureLimit)
            return true;
        if (TotalGenerationLimit != -1 && CreationCount >= TotalGenerationLimit)
            return true;
        return false;
    }
};
int main()
{
    SCreatureNode head, a, b;
    head.Next = &a; a.Next = &b; b.Next = &head; head.Prev = &b; b.Prev = &a; a.Prev = &head;
    CTCCreatureGenerator g; g.CreatedCreatures.Node = &head; g.ActiveCreatureLimit = 3; g.TotalGenerationLimit = -1; g.CreationCount = 50;
    if (g.HasHitCreationLimit()) { std::printf("0074a190_TEST FAIL\n"); return 1; }
    g.ActiveCreatureLimit = 2;
    if (!g.HasHitCreationLimit()) { std::printf("0074a190_TEST FAIL\n"); return 1; }
    g.ActiveCreatureLimit = -1; g_GeneratedCreatureLimit = 5; g_GeneratedCreatureCount = 4;
    if (g.HasHitCreationLimit()) { std::printf("0074a190_TEST FAIL\n"); return 1; }
    g_GeneratedCreatureCount = 5;
    if (!g.HasHitCreationLimit()) { std::printf("0074a190_TEST FAIL\n"); return 1; }
    g_GeneratedCreatureLimit = -1; g.TotalGenerationLimit = 51;
    if (g.HasHitCreationLimit()) { std::printf("0074a190_TEST FAIL\n"); return 1; }
    g.TotalGenerationLimit = 50;
    if (!g.HasHitCreationLimit()) { std::printf("0074a190_TEST FAIL\n"); return 1; }
    head.Next = &head; g.TotalGenerationLimit = -1; g.ActiveCreatureLimit = 0;    /* empty list, limit 0 */
    if (!g.HasHitCreationLimit()) { std::printf("0074a190_TEST FAIL\n"); return 1; }
    std::printf("0074a190_TEST PASS\n");
    return 0;
}