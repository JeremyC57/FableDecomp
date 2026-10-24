// Behaviour test for 0x006CE530 vector<const CThingCreatureBase*>::push_back: with spare capacity the pointer is
// stored in place and the end advances; when full, _M_insert_overflow(end, x, tag, 1, true) is called.
#include <cstdio>
#include <cstdlib>

struct CThingCreatureBase { int m_id; };
struct TrueType {};

static int g_overflowCalls = 0;

struct PtrVector {
    const CThingCreatureBase** Start;
    const CThingCreatureBase** Finish;
    const CThingCreatureBase** End;
    void InsertOverflow(const CThingCreatureBase** pos, const CThingCreatureBase* const& x, const TrueType&, unsigned int n, bool atEnd)
    {
        ++g_overflowCalls;
        unsigned int size = (unsigned int)(Finish - Start);
        unsigned int cap = size + (size < n ? n : size);
        const CThingCreatureBase** mem = (const CThingCreatureBase**)std::malloc(cap * sizeof(*mem));
        unsigned int i = 0;
        for (const CThingCreatureBase** p = Start; p != pos; ++p) mem[i++] = *p;
        for (unsigned int k = 0; k < n; ++k) mem[i++] = x;
        if (!atEnd) for (const CThingCreatureBase** p = pos; p != Finish; ++p) mem[i++] = *p;
        std::free(Start);
        Start = mem; Finish = mem + i; End = mem + cap;
    }
    void push_back(const CThingCreatureBase* const& x)
    {
        if (Finish != End) { *Finish = x; ++Finish; }
        else InsertOverflow(Finish, x, TrueType(), 1, true);
    }
};

int main()
{
    CThingCreatureBase a, b, c;
    PtrVector v; v.Start = v.Finish = v.End = 0;
    const CThingCreatureBase* pa = &a; v.push_back(pa);           // empty: overflow
    if (g_overflowCalls != 1 || v.Finish - v.Start != 1 || v.Start[0] != &a) { std::printf("006ce530_TEST FAIL\n"); return 1; }
    const CThingCreatureBase* pb = &b; v.push_back(pb);           // full again (cap 1): overflow to 2
    const CThingCreatureBase* pc = &c; v.push_back(pc);           // cap 2 full: overflow to 4
    if (g_overflowCalls != 3 || v.Finish - v.Start != 3 || v.Start[2] != &c || v.End - v.Start != 4) { std::printf("006ce530_TEST FAIL\n"); return 1; }
    v.push_back(pa);                                              // spare slot: in place
    if (g_overflowCalls != 3 || v.Finish - v.Start != 4 || v.Start[3] != &a) { std::printf("006ce530_TEST FAIL\n"); return 1; }
    std::free(v.Start);
    std::printf("006ce530_TEST PASS\n");
    return 0;
}