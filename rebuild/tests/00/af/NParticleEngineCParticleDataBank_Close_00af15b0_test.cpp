// Behaviour test for 0x00AF15B0 NParticleEngine::CParticleDataBank::Close
#include <cstdio>
struct CParticleEmitterInfo { long id; };
static int g_destroyed = 0;
static int g_closed = 0;
CParticleEmitterInfo* Move(CParticleEmitterInfo* first, CParticleEmitterInfo* last, CParticleEmitterInfo* dest, bool)
{
    for (; first != last; ++first, ++dest) *dest = *first;
    return dest;
}
void DestroyRange(CParticleEmitterInfo* first, CParticleEmitterInfo* last)
{
    for (; first != last; ++first) ++g_destroyed;
}
class CBankFile { public: void Close() { ++g_closed; } };
struct CArray { CParticleEmitterInfo* _M_start; CParticleEmitterInfo* _M_finish; CParticleEmitterInfo* _M_end_of_storage; };
struct CParticleDataBank : CBankFile {
    CArray ParticleEmitters;
    void Close()
    {
        CParticleEmitterInfo* last = ParticleEmitters._M_finish;
        CParticleEmitterInfo* first = ParticleEmitters._M_start;
        CParticleEmitterInfo* kept = Move(last, last, first, false);
        DestroyRange(kept, ParticleEmitters._M_finish);
        ParticleEmitters._M_finish = kept;
        CBankFile::Close();
    }
};
int main()
{
    CParticleEmitterInfo items[3];
    CParticleDataBank bank;
    bank.ParticleEmitters._M_start = items;
    bank.ParticleEmitters._M_finish = items + 3;
    bank.ParticleEmitters._M_end_of_storage = items + 3;
    bank.Close();
    if (g_destroyed != 3 || g_closed != 1 || bank.ParticleEmitters._M_finish != items
        || bank.ParticleEmitters._M_end_of_storage != items + 3) { std::printf("00af15b0_TEST FAIL\n"); return 1; }
    bank.Close();
    if (g_destroyed != 3 || g_closed != 2) { std::printf("00af15b0_TEST FAIL\n"); return 1; }
    std::printf("00af15b0_TEST PASS\n");
    return 0;
}