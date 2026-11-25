// Behaviour model of ??_G?$pair@$$CBW4EObjectAugmentationType@@VCObjectAugmentationParticleSet@@@std@@QAEPAXI@Z 0x004E41A1: destroy
// the pair (its CObjectAugmentationParticleSet value, after the 4-byte key), then free the storage only when bit 0 of the flags is set;
// returns this.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_dtor_calls = 0, g_delete_calls = 0;
static const void* g_last_dtor_self = 0;
static const void* g_last_delete_self = 0;
struct CObjectAugmentationParticleSet { float PeriodSecs; ~CObjectAugmentationParticleSet() { g_dtor_calls++; g_last_dtor_self = this; } };
struct CAugmentationParticlePair {
    int first; CObjectAugmentationParticleSet second;
    static void operator delete(void* p) { g_delete_calls++; g_last_delete_self = p; std::free(p); }
};
static void* ScalarDeletingDestructor(CAugmentationParticlePair* self, unsigned int flags)
{
    self->~CAugmentationParticlePair();
    if (flags & 1)
        CAugmentationParticlePair::operator delete(self);
    return self;
}
int main()
{
    void* mem = std::malloc(sizeof(CAugmentationParticlePair));
    CAugmentationParticlePair* p = new (mem) CAugmentationParticlePair;
    void* r = ScalarDeletingDestructor(p, 0);
    if (r != p || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL flag0\n"); return 1; }
    if (g_last_dtor_self != &p->second) { std::printf("FAIL member\n"); return 1; }
    p = new (mem) CAugmentationParticlePair;
    r = ScalarDeletingDestructor(p, 2);
    if (r != p || g_dtor_calls != 2 || g_delete_calls != 0) { std::printf("FAIL flag2\n"); return 1; }
    p = new (mem) CAugmentationParticlePair;
    r = ScalarDeletingDestructor(p, 1);
    if (r != p || g_dtor_calls != 3 || g_delete_calls != 1 || g_last_delete_self != p) { std::printf("FAIL flag1\n"); return 1; }
    std::printf("VDD_OK\n");
    return 0;
}
