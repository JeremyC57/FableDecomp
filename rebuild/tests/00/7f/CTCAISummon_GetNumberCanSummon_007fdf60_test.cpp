#include <cstdio>
// Behaviour test: CTCAISummon::GetNumberCanSummon -- 0 unless it is time to summon; else NumberToSummon capped by (MySummonLimit - own live summons) when that limit is positive and by the global budget minus all summoned; 0 if below MinimumSummon.
#define CHECK(c) if (!(c)) { std::printf("007fdf60_TEST FAIL\n"); return 1; }
// Test model of the STLport list: a circular doubly-linked list with a head node; size() walks it.
struct SNode { SNode* next; SNode* prev; int data; };
struct CList {
    SNode head;
    CList() { head.next = &head; head.prev = &head; }
    unsigned int size() const { unsigned int n = 0; for (const SNode* p = head.next; p != &head; p = p->next) ++n; return n; }
    void push_back(SNode* n) { n->next = &head; n->prev = head.prev; head.prev->next = n; head.prev = n; }
    void erase(SNode* n) { n->prev->next = n->next; n->next->prev = n->prev; }
};

template <class T> const T& Min(const T& a, const T& b) { return b < a ? b : a; }
class CSummonDef { public: long NumberToSummon, MySummonLimit, MinimumSummon; };
static long g_MaxTotalSummonedCreatures = 10;
static CList g_all;
class CTCAISummon { public:
    const CSummonDef* PSummonDef; CList MySummonedCreatures; bool TimeToSummon;
    long GetNumberCanSummon() const
    {
        if (TimeToSummon) {
            long globalRemaining = g_MaxTotalSummonedCreatures - g_all.size();
            long number = PSummonDef->NumberToSummon;
            if (PSummonDef->MySummonLimit > 0)
                number = Min(number, PSummonDef->MySummonLimit - (long)MySummonedCreatures.size());
            number = Min(number, globalRemaining);
            if (number >= PSummonDef->MinimumSummon)
                return number;
        }
        return 0;
    }
};
int main()
{
    CSummonDef def; def.NumberToSummon = 4; def.MySummonLimit = 0; def.MinimumSummon = 1;
    CTCAISummon s; s.PSummonDef = &def; s.TimeToSummon = false;
    CHECK(s.GetNumberCanSummon() == 0)
    s.TimeToSummon = true; CHECK(s.GetNumberCanSummon() == 4)
    def.MySummonLimit = 3; SNode m1; s.MySummonedCreatures.push_back(&m1); CHECK(s.GetNumberCanSummon() == 2)
    SNode g[9]; for (int i = 0; i < 9; ++i) g_all.push_back(&g[i]); CHECK(s.GetNumberCanSummon() == 1)
    def.MinimumSummon = 2; CHECK(s.GetNumberCanSummon() == 0)
    std::printf("007fdf60_TEST PASS\n");
    return 0;
}