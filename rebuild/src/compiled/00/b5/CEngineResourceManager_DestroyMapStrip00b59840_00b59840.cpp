// CEngineResourceManager::DestroyMapStrip_00b59840(SMapEntry& entry) -- delete the entry's strip object at +4 (its out-of-line destructor, then operator delete) and clear the pointer; `this` is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B59840, length 39 bytes.
// Callees: the strip destructor @ 0x00BDED60 (the manifest label "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip"), operator delete @ 0x00BFE9BC. Shape:
// `if (p) { delete p; p = 0; }` keeps the clear inside the branch (before the join); the entry stays in edi, the object in esi. The chunk label
// "WorldFrameUpdate" is propagated; named by effect.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

namespace N_00bded60 { struct CPatchTesselationEdgeStrip {
    ~CPatchTesselationEdgeStrip();                 // 0x00BDED60
}; } using N_00bded60::CPatchTesselationEdgeStrip;

struct SMapEntry_00b59840 {
    long                        m_id;
    CPatchTesselationEdgeStrip* m_pStrip;          // +0x04
};

class CEngineResourceManager { public:
    void DestroyMapStrip_00b59840(SMapEntry_00b59840& entry);
};

void CEngineResourceManager::DestroyMapStrip_00b59840(SMapEntry_00b59840& entry)
{
    if (entry.m_pStrip) {
        delete entry.m_pStrip;
        entry.m_pStrip = 0;
    }
}