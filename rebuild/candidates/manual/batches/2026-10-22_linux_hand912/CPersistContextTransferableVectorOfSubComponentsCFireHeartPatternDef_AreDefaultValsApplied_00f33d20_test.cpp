#include <cstdio>
// Behaviour test for 00f33d20: CPersistContext_TransferableVectorOfSubComponents<CFireHeartPatternDef>::AreDefaultValsApplied on a model vector.
struct CPersistContext { int calls; };
struct Def {
    int transferred;
    unsigned char Setup : 1;
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
    virtual void Transfer(CPersistContext& context) { ++transferred; ++context.calls; }
};
struct Vec {
    Def* _M_start; Def* _M_finish; Def* _M_end_of_storage;
    long size() const { return (long)(_M_finish - _M_start); }
    Def& operator[](long index) { return _M_start[index]; }
};
struct Wrapper {
    Vec* TransVector;
    long Size() { return TransVector->size(); }
    bool AreDefaultValsApplied(long index) { return (*TransVector)[index].DefaultValsApplied; }
    void SetDefaultValsApplied(long index, bool applied) { Def& def = (*TransVector)[index]; def.DefaultValsApplied = applied; }
    void Transfer(CPersistContext& context, long index) { (*TransVector)[index].Transfer(context); }
};
int main()
{
    Def defs[4];
    for (int i = 0; i < 4; ++i) { defs[i].transferred = 0; defs[i].Setup = 1; defs[i].Template = 0; defs[i].DefaultValsApplied = (i & 1); }
    Vec v; v._M_start = defs; v._M_finish = defs + 3; v._M_end_of_storage = defs + 4;
    Wrapper w; w.TransVector = &v;
    CPersistContext ctx; ctx.calls = 0;
    int ok = 1;
    if (w.AreDefaultValsApplied(0) != false) ok = 0;
    if (w.AreDefaultValsApplied(1) != true) ok = 0;
    if (!ok) { std::printf("00f33d20_TEST FAIL\n"); return 1; }
    std::printf("00f33d20_TEST PASS\n");
    return 0;
}
