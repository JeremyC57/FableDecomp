// Behaviour test for 0x006F3A30 std::_S_sort<const CThingCreatureBase*, allocator, CThingCompare_Nearest>: the shared STLport
// list merge sort (FABLE_RETAIL_STLP_LIST_SORT) run on lists of 0, 1, 2, 3, 64 and 1000 creature pointers with a STAND-IN
// nearest-to-a-point bool predicate (12-byte position by value, like CThingCompare_Nearest). It does NOT copy the retail
// comparator's result type: retail CThingCompare_Nearest::operator() (0x006EF290) returns the nearer T*, which _S_merge only
// null-tests, so the retail instance compares any two non-null creatures true and never orders by distance. This test checks
// the shared merge-sort body itself with a real strict weak ordering. Checks: ascending distance, every element kept exactly once, the
// node ring consistent in both directions, equal keys keep their original order (merge sort is stable), the short lists untouched.
#define FABLE_RETAIL_STLP_LIST_SORT
#include "stlport/fable_containers.h"
#include <cstdio>

struct CTestCreature { float X, Y, Z; int Order; int Seen; };
struct CTestCompare_Nearest {                       // stand-in: bool result (retail's returns T*); reference position by value
    float X, Y, Z;
    static float Dist(const CTestCompare_Nearest& c, const CTestCreature* p)
    { float dx = p->X - c.X, dy = p->Y - c.Y, dz = p->Z - c.Z; return dx * dx + dy * dy + dz * dz; }
    bool operator()(const CTestCreature* a, const CTestCreature* b) const { return Dist(*this, a) < Dist(*this, b); }
};
typedef std::list<const CTestCreature*> CTestList;

static CTestCreature g_pool[1000];
static unsigned long g_seed = 12345;
static int Rand() { g_seed = g_seed * 1103515245UL + 12345UL; return (int)((g_seed >> 16) & 0x7fff); }

static bool Check(int count, int keyRange)
{
    CTestCompare_Nearest comp; comp.X = 3.0f; comp.Y = -2.0f; comp.Z = 1.0f;
    CTestList l;
    int i;
    for (i = 0; i < count; ++i) {
        g_pool[i].X = (float)(Rand() % keyRange); g_pool[i].Y = -2.0f; g_pool[i].Z = 1.0f;
        g_pool[i].Order = i; g_pool[i].Seen = 0;
        l.push_back(&g_pool[i]);
    }
    const CTestCreature* firstBefore = count ? *l.begin() : 0;
    l.sort(comp);                                   // list::sort(comp) { _S_sort(*this, comp); }
    int n = 0;
    const CTestCreature* prev = 0;
    std::_List_node_base* head = l._M_node._M_data;
    std::_List_node_base* back = head;
    for (CTestList::iterator it = l.begin(); it != l.end(); ++it) {
        const CTestCreature* cur = *it;
        if (it._M_node->_M_prev != back) return false;              // ring consistent backwards
        back = it._M_node;
        if (cur < g_pool || cur >= g_pool + count) return false;
        if (((CTestCreature*)cur)->Seen++) return false;            // each element once
        if (prev) {
            float dp = CTestCompare_Nearest::Dist(comp, prev), dc = CTestCompare_Nearest::Dist(comp, cur);
            if (dc < dp) return false;                              // ascending distance
            if (dc == dp && cur->Order < prev->Order) return false; // stable
        }
        prev = cur;
        ++n;
    }
    if (head->_M_prev != back) return false;
    if (n != count) return false;
    if (count == 1 && *l.begin() != firstBefore) return false;
    return true;
}

int main()
{
    static const int counts[] = { 0, 1, 2, 3, 5, 64, 65, 257, 1000 };
    bool ok = true;
    for (int k = 0; k < (int)(sizeof(counts) / sizeof(counts[0])); ++k) {
        ok = ok && Check(counts[k], 1000);          // mostly distinct keys
        ok = ok && Check(counts[k], 4);             // many ties: stability
    }
    // a fixed case, checked value by value: distances from x=3 of 9,1,5,3,7 -> 3,1,5,7,9 order of |x-3|
    {
        CTestCompare_Nearest comp; comp.X = 3.0f; comp.Y = 0.0f; comp.Z = 0.0f;
        static const float xs[5] = { 9.0f, 1.0f, 5.0f, 3.0f, 7.0f };
        static const int want[5] = { 3, 1, 2, 4, 0 };   // 3 (d0), 1 (d4, first of the tie), 5 (d4), 7 (d16), 9 (d36)
        CTestList l;
        int i;
        for (i = 0; i < 5; ++i) { g_pool[i].X = xs[i]; g_pool[i].Y = 0.0f; g_pool[i].Z = 0.0f; g_pool[i].Order = i; l.push_back(&g_pool[i]); }
        std::_S_sort(l, comp);
        i = 0;
        for (CTestList::iterator it = l.begin(); it != l.end(); ++it, ++i)
            if (i >= 5 || (*it)->Order != want[i]) ok = false;
        if (i != 5) ok = false;
    }
    if (!ok) { std::printf("006f3a30_TEST FAIL\n"); return 1; }
    std::printf("006f3a30_TEST PASS\n");
    return 0;
}