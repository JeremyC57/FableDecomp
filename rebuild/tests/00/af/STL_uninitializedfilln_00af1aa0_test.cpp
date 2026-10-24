// behaviour test for 0x00AF1AA0 _STL::__uninitialized_fill_n over NParticleEngine::CParticleEmitterInfo: n counted copies of x into raw storage, return first + n
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

    SPointee data = { 0, 1 };
    int bank = 0;
    CParticleEmitterInfo x(&data, &bank, 5);
    CParticleEmitterInfo* raw = (CParticleEmitterInfo*)gStore;
    CParticleEmitterInfo* end = UninitFillN(raw, 3, x);
    if (end != raw + 3 || data.IVRefCount != 4) { std::printf("00af1aa0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (raw[i].Data.Object != &data || raw[i].Bank != &bank || raw[i].Index != 5) { std::printf("00af1aa0_TEST FAIL\n"); return 1; }
    if (UninitFillN(raw + 3, 0, x) != raw + 3 || data.IVRefCount != 4) { std::printf("00af1aa0_TEST FAIL\n"); return 1; }
    CParticleEmitterInfo empty(0, 0, 9);
    if (UninitFillN(raw + 3, 2, empty) != raw + 5 || raw[4].Data.Object != 0 || raw[4].Index != 9) { std::printf("00af1aa0_TEST FAIL\n"); return 1; }
    std::printf("00af1aa0_TEST PASS\n");
    return 0;
}