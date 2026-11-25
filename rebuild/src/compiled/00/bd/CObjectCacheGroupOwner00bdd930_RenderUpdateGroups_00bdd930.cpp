// CObjectCacheGroupOwner_00bdd930::RenderUpdateGroups(CEngineCamera* camera, float a, float b, unsigned long c, float d) -- when bit 0 of the byte at +0x36 is set, render-updates every group of the chain at +0x20 (next at +0xc) with the five arguments
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDD930, length 66 bytes (80 with padding).
// Callees: CObjectCacheGroupCollection::RenderUpdate(CEngineCamera*, float, float, unsigned long, float) @ 0x00BDD860 (on each group). Shape: the flag test
// precedes any register save; the three last arguments are hoisted into ebx/ebp/edi once the chain is known non-empty, the first two are re-read from their
// slots each iteration (the floats are forwarded as dwords); `ret 14h` pops the five arguments. The chunk label (CEngineLandscapePatch::DeleteForegroundVertexBuffers)
// is propagated; the owner is modelled locally.
class CEngineCamera;

struct CObjectCacheGroupCollection {
    unsigned char                _pad[0xc];
    CObjectCacheGroupCollection* m_pNext;          // +0x0c
};

struct CObjectCacheGroupOwner_00bdd930 {
    unsigned char                _pad[0x20];
    CObjectCacheGroupCollection* m_pGroups;        // +0x20
    unsigned char                _pad24[0x12];
    unsigned char                m_renderEnabled : 1;   // +0x36 bit 0
    void RenderUpdateGroups(CEngineCamera* camera, float a, float b, unsigned long c, float d);
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00BDD860 ?RenderUpdate@CObjectTypeCollection@CLocalDetailCacheMap@@QAEXABVCEngineCamera@@MMKM@Z (was ?RenderUpdate@CObjectCacheGroupCollection@@QAEXPAVCEngineCamera@@MMKM@Z)
namespace CLocalDetailCacheMap { class CObjectTypeCollection { public: void RenderUpdate(CEngineCamera const&, float, float, unsigned long, float); }; }
void CObjectCacheGroupOwner_00bdd930::RenderUpdateGroups(CEngineCamera* camera, float a, float b, unsigned long c, float d)
{
    if (m_renderEnabled) {
        for (CObjectCacheGroupCollection* group = m_pGroups; group; group = group->m_pNext)
            reinterpret_cast<CLocalDetailCacheMap::CObjectTypeCollection*>(group)->RenderUpdate(*(camera), a, b, c, d);
    }
}