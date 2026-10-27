// behaviour test for 0x004343f0 CEngineDef::Alloc(): new-expression with the inlined def-class default constructor chain
// (resource base, PDefManager null, flag bits clear, InstantiationName table position -1, empty sub-def map);
// a failed allocation returns null without constructing
#include <cstdio>
#include <cstdlib>
#include <new>
static bool gFail = false; static int gResourceCtors = 0;
static void* Alloc(unsigned int n) { return gFail ? 0 : std::malloc(n); }
struct Resource { unsigned long IVRefCount; Resource() : IVRefCount(0) { ++gResourceCtors; } virtual ~Resource() { } };
struct DefClassBase : Resource {
    void* PDefManager; unsigned long GlobalIndex; unsigned char Setup : 1; unsigned char Template : 1; unsigned char DefaultValsApplied : 1;
    DefClassBase() : PDefManager(0) { Setup = false; Template = false; DefaultValsApplied = false; }
};
struct ParentDefClassBase : DefClassBase {
    long InstantiationName; void* MapStart; void* MapFinish; void* MapEnd; bool Dirty;
    ParentDefClassBase() : InstantiationName(-1), MapStart(0), MapFinish(0), MapEnd(0), Dirty(false) { }
};
struct Def : ParentDefClassBase { float Members[16]; };
static DefClassBase* DoAlloc() { void* p = Alloc(sizeof(Def)); return p ? new (p) Def() : 0; }
int main()
{
    int fails = 0;
    DefClassBase* b = DoAlloc();
    Def* d = static_cast<Def*>(b);
    if (!d || gResourceCtors != 1) fails |= 1;
    if (d->PDefManager != 0 || d->Setup || d->Template || d->DefaultValsApplied) fails |= 2;
    if (d->InstantiationName != -1 || d->MapStart || d->MapFinish || d->MapEnd || d->Dirty) fails |= 4;
    d->~Def(); std::free(d);
    gFail = true;
    if (DoAlloc() != 0 || gResourceCtors != 1) fails |= 16;
    if (fails) { std::printf("004343f0_TEST FAIL %d\n", fails); return 1; }
    std::printf("004343f0_TEST PASS\n");
    return 0;
}