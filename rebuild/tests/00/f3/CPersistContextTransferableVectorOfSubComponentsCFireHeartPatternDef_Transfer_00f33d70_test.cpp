#include <cstdio>
// Behaviour test for 00f33d70: CPersistContext_TransferableVectorOfSubComponents<CFireHeartPatternDef>::Transfer on a model vector.
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
    w.Transfer(ctx, 2); w.Transfer(ctx, 2); w.Transfer(ctx, 0);
    if (defs[2].transferred != 2 || defs[0].transferred != 1 || defs[1].transferred != 0 || ctx.calls != 3) ok = 0;
    if (!ok) { std::printf("00f33d70_TEST FAIL\n"); return 1; }
    std::printf("00f33d70_TEST PASS\n");
    return 0;
}