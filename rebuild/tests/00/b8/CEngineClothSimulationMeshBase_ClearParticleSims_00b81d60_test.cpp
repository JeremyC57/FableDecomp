// Behaviour test for CEngineClothSimulationMeshBase::ClearParticleSims @ 0x00B81D60
#include <cstdio>
#include <new>
struct SCountBlock { long m_references; };
template <class T> class CCountedPointer { public:
    T* Object; SCountBlock* Count;
    CCountedPointer() : Object(0), Count(0) {}
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    T* operator->() const { return Object; }
};
class CParticleSimulationRenderData { public: bool m_busy; virtual bool IsBusy() { return m_busy; } };
class CParticleSimulation { public: CCountedPointer<CParticleSimulationRenderData> RenderData; };
struct SFalseTag { SFalseTag() {} };
static int g_overflows = 0;
template <class T> struct CRetailVector {
    T* m_start; T* m_finish; T* m_endOfStorage;
    T* begin() const { return m_start; }
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    T& operator[](unsigned int n) { return *(begin() + n); }
    void InsertOverflow(T* position, const T& x, const SFalseTag&, unsigned int, bool) { ++g_overflows; new (position) T(x); m_finish = position + 1; m_endOfStorage = m_finish; }
    void clear() { m_finish = m_start; }
    void push_back(const T& x)
    {
        if (m_finish != m_endOfStorage) { new (m_finish) T(x); ++m_finish; }
        else InsertOverflow(m_finish, x, SFalseTag(), 1, true);
    }
};
static CCountedPointer<CParticleSimulation> g_storage[4];
CRetailVector<CCountedPointer<CParticleSimulation> > g_PendingReleaseParticleSims;
struct CEngineClothSimulationMeshBase {
    CRetailVector<CCountedPointer<CParticleSimulation> > ParticleSims;
    void ClearParticleSims()
    {
        for (unsigned int i = 0; i < ParticleSims.size(); ++i) {
            CCountedPointer<CParticleSimulation>& sim = ParticleSims[i];
            if (sim.Object && sim->RenderData.Object && sim->RenderData->IsBusy())
                g_PendingReleaseParticleSims.push_back(sim);
        }
        ParticleSims.clear();
    }
};
int main()
{
    CParticleSimulationRenderData busy, idle; busy.m_busy = true; idle.m_busy = false;
    CParticleSimulation s[4];
    s[0].RenderData.Object = &busy; s[1].RenderData.Object = &idle; s[2].RenderData.Object = 0; s[3].RenderData.Object = &busy;
    SCountBlock blocks[4] = { { 1 }, { 1 }, { 1 }, { 1 } };
    CCountedPointer<CParticleSimulation> sims[5];
    for (int i = 0; i < 4; ++i) { sims[i].Object = &s[i]; sims[i].Count = &blocks[i]; }
    // sims[4] stays null
    CEngineClothSimulationMeshBase m;
    m.ParticleSims.m_start = sims; m.ParticleSims.m_finish = sims + 5; m.ParticleSims.m_endOfStorage = sims + 5;
    g_PendingReleaseParticleSims.m_start = g_storage; g_PendingReleaseParticleSims.m_finish = g_storage; g_PendingReleaseParticleSims.m_endOfStorage = g_storage + 1;
    m.ClearParticleSims();
    if (g_PendingReleaseParticleSims.size() != 2 || g_storage[0].Object != &s[0] || g_storage[1].Object != &s[3] || g_overflows != 1 ||
        blocks[0].m_references != 2 || blocks[1].m_references != 1 || blocks[3].m_references != 2 || m.ParticleSims.size() != 0) {
        std::printf("00b81d60_TEST FAIL\n"); return 1;
    }
    std::printf("00b81d60_TEST PASS\n");
    return 0;
}