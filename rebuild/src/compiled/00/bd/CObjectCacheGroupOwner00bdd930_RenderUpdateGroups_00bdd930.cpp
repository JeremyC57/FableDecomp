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
    void RenderUpdate(CEngineCamera* camera, float a, float b, unsigned long c, float d);   // 0x00BDD860
};

struct CObjectCacheGroupOwner_00bdd930 {
    unsigned char                _pad[0x20];
    CObjectCacheGroupCollection* m_pGroups;        // +0x20
    unsigned char                _pad24[0x12];
    unsigned char                m_renderEnabled : 1;   // +0x36 bit 0
    void RenderUpdateGroups(CEngineCamera* camera, float a, float b, unsigned long c, float d);
};

void CObjectCacheGroupOwner_00bdd930::RenderUpdateGroups(CEngineCamera* camera, float a, float b, unsigned long c, float d)
{
    if (m_renderEnabled) {
        for (CObjectCacheGroupCollection* group = m_pGroups; group; group = group->m_pNext)
            group->RenderUpdate(camera, a, b, c, d);
    }
}