#include <cstdio>
struct CDef { long* first; long* last; };
struct CSeq {
    const CDef* Def_Object;
    bool IsSequenceWithActions() { const CDef* def = Def_Object; for (const long* a = def->first; a != def->last; ++a) if (*a != 0) return true; return false; }
};
int main()
{
    long acts[3] = { 0, 0, 5 }; CDef d = { acts, acts + 3 }; CSeq s; s.Def_Object = &d;
    if (!s.IsSequenceWithActions()) { std::printf("008b06b0_TEST FAIL\n"); return 1; }
    d.last = acts + 2; if (s.IsSequenceWithActions()) { std::printf("008b06b0_TEST FAIL\n"); return 1; }
    d.last = acts; if (s.IsSequenceWithActions()) { std::printf("008b06b0_TEST FAIL\n"); return 1; }
    std::printf("008b06b0_TEST PASS\n");
    return 0;
}