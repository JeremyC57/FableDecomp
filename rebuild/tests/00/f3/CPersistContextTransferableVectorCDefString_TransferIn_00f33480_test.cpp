#include <cstdio>
// Behaviour test for 00f33480: CPersistContext_TransferableVector<CDefString>::TransferIn on a model vector.
struct CPersistContext { int id; };
struct CStringParser { long next; };
struct CCharString { long written; };
struct CDefString { long Index; };
struct Traits {
    void TransferIn(CPersistContext& context, CStringParser& parser, CDefString* value) { value->Index = parser.next + context.id; }
    void TransferOut(CPersistContext& context, CCharString& out, CDefString* value) { out.written = value->Index + context.id; }
};
struct Vec {
    CDefString* _M_start; CDefString* _M_finish; CDefString* _M_end_of_storage;
    long size() const { return (long)(_M_finish - _M_start); }
    CDefString& operator[](long index) { return _M_start[index]; }
};
struct Wrapper {
    Vec* TransVector;
    long Size() { return TransVector->size(); }
    void TransferIn(CPersistContext& context, CStringParser& parser, long index) { Traits traits; traits.TransferIn(context, parser, &(*TransVector)[index]); }
    void TransferOut(CPersistContext& context, CCharString& out, long index) { Traits traits; traits.TransferOut(context, out, &(*TransVector)[index]); }
};
int main()
{
    CDefString defs[5];
    for (int i = 0; i < 5; ++i) defs[i].Index = 10 * i;
    Vec v; v._M_start = defs; v._M_finish = defs + 4; v._M_end_of_storage = defs + 5;
    Wrapper w; w.TransVector = &v;
    CPersistContext ctx; ctx.id = 1;
    CStringParser parser; parser.next = 41;
    CCharString out; out.written = 0;
    int ok = 1;
    w.TransferIn(ctx, parser, 2);
    if (defs[2].Index != 42 || defs[1].Index != 10 || defs[3].Index != 30) ok = 0;
    if (!ok) { std::printf("00f33480_TEST FAIL\n"); return 1; }
    std::printf("00f33480_TEST PASS\n");
    return 0;
}