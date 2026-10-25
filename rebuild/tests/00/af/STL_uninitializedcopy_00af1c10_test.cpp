// behaviour test for 0x00AF1C10 std::__uninitialized_copy over NParticleEngine::CParticleEmitterInfo: counted copies of [first, last) into raw storage, return the end
#include <cstdio>
#include <new>
struct SPointee { void* VTable; unsigned int IVRefCount; };
struct CIVCountedPointer { SPointee* Object;
    CIVCountedPointer(SPointee* p) : Object(p) { }
    CIVCountedPointer(const CIVCountedPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; } };
struct CParticleEmitterInfo { CIVCountedPointer Data; void* Bank; unsigned int Index;
    CParticleEmitterInfo(SPointee* p, void* b, unsigned int i) : Data(p), Bank(b), Index(i) { } };
static CParticleEmitterInfo* UninitFillN(CParticleEmitterInfo* first, unsigned int n, const CParticleEmitterInfo& x)
{
    CParticleEmitterInfo* cur = first;
    for (; n > 0; --n, ++cur) new (cur) CParticleEmitterInfo(x);
    return cur;
}
static CParticleEmitterInfo* UninitCopy(CParticleEmitterInfo* first, CParticleEmitterInfo* last, CParticleEmitterInfo* result)
{
    CParticleEmitterInfo* cur = result;
    for (; first != last; ++first, ++cur) new (cur) CParticleEmitterInfo(*first);
    return cur;
}
static double gStoreAligned[16 * 4];
static char* const gStore = (char*)gStoreAligned;
int main()
{

    SPointee a = { 0, 1 }, b = { 0, 1 };
    CParticleEmitterInfo src[3] = { CParticleEmitterInfo(&a, 0, 1), CParticleEmitterInfo(0, 0, 2), CParticleEmitterInfo(&b, 0, 3) };
    CParticleEmitterInfo* raw = (CParticleEmitterInfo*)gStore;
    CParticleEmitterInfo* end = UninitCopy(src, src + 3, raw);
    if (end != raw + 3 || a.IVRefCount != 2 || b.IVRefCount != 2) { std::printf("00af1c10_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (raw[i].Data.Object != src[i].Data.Object || raw[i].Index != src[i].Index) { std::printf("00af1c10_TEST FAIL\n"); return 1; }
    if (UninitCopy(src, src, raw) != raw) { std::printf("00af1c10_TEST FAIL\n"); return 1; }
    std::printf("00af1c10_TEST PASS\n");
    return 0;
}