// CEngineLandscapePatch::DeleteLayerMeshes() -- deletes the polymorphic layer mesh at +8 (clearing its slot and the three-bit layer count at +0x3c) and returns the virtual-memory block at +0xc to the engine's landscape memory manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF3790, length 65 bytes.
// Callees: the layer mesh's virtual slot 0 (scalar deleting destructor, flag 1: `delete`), CEngineLandscapeMemoryManager::FreeVirtual(void*) @ 0x00BD9B90 (on the
// manager at engine +0x6b0; the engine @ 0x01436EA8). Shape: each member is tested once (the `delete` under the test needs no test of its own); the bitfield
// clear is `and al,0F8h` around the pointer's zero store. The patch's +8 / +0x3c members are the ones CEngineLandscapePatch::Clear @ 0x00BF38F0 uses; no PDB
// layout for this class, modelled locally.
struct CLayerMesh_00bf3790 {
    virtual ~CLayerMesh_00bf3790();
};

class CEngineLandscapeMemoryManager { public:
    void FreeVirtual(void* block);                 // 0x00BD9B90
};

struct CEngineGlobal_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemoryManager;   // +0x6b0
};
extern CEngineGlobal_01436ea8* g_pEngine_01436ea8;   // 0x01436EA8

class CEngineLandscapePatch { public:
    unsigned char        _pad[8];
    CLayerMesh_00bf3790* m_pLayerMesh;             // +0x08
    void*                m_pLayerBlock;            // +0x0c
    unsigned char        _pad10[0x2c];
    unsigned char        m_layerCount : 3;         // +0x3c bits 0-2
    void DeleteLayerMeshes();
};

void CEngineLandscapePatch::DeleteLayerMeshes()
{
    if (m_pLayerMesh) {
        delete m_pLayerMesh;
        m_pLayerMesh = 0;
        m_layerCount = 0;
    }
    if (m_pLayerBlock) {
        g_pEngine_01436ea8->m_pLandscapeMemoryManager->FreeVirtual(m_pLayerBlock);
        m_pLayerBlock = 0;
    }
}