// CEngineSurfaceCompositionManager::BeginBankUpdate() -- free the engine shadow buffer, reopen the intermediate disk-cache bank read/write (create it when no morphs were generated yet) and start an entry update on it
// Leaf: ?BeginBankUpdate@CEngineSurfaceCompositionManager@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2B000, 74 bytes, tail jump to BeginUpdateEntries.
// Identity: Ego_r.pdb + FableWin.pdb ?BeginBankUpdate@CEngineSurfaceCompositionManager@@AAEXXZ (private).
// Callees: CIEngine::DeallocateShadowBuffer (slot 74 on the engine 0x01436EA4), CBankFile::OpenFileReadWrite (slot 4),
//   CBankFile::BeginUpdateEntries 0x009CE8D0 (Ego_r ?BeginUpdateEntries@CBankFile@@QAEXXZ).
// Layout: IntermediateBankFile +0x8c, IntermediateDiskCacheFileName +0x9b0, GeneratedMorphs map +0x9bc (node count +0x9c0).
// Shape: `unsigned long create = 0; if (GeneratedMorphs.empty()) create = 1;` (the branchy 0/1; `?:` and bool give sete).
// Header debt: the shared header's retail branch is opaque past +0x88, so `this` is a local model (as landed 0x00B2C8E0).
#define FABLE_RETAIL_CENGINE
#include "engine/CEngine.h"
#include "fable_string.h"

extern CEngine* g_pEngine_01436ea4;                     // 0x01436EA4 (the engine; CEngine::Initialise stores this)

class CGraphicDataBank { public:                        // vtable 0x0129C8CC, 0x30c bytes
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual bool OpenFileReadOnly(const CWideString& fileName, unsigned long flags);   // slot 3 (CBankFile)
    virtual void OpenFileReadWrite(const CWideString& fileName, unsigned long create, unsigned long flags);   // slot 4
    void BeginUpdateEntries();                          // 0x009CE8D0 ?BeginUpdateEntries@CBankFile@@QAEXXZ
    unsigned char _pad04[0x30c - 4];
};

struct SMorphMapStandIn {                               // STLport map<pair<long,long>,CEngineTextureMorph> (12 bytes)
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    bool empty() const { return _M_node_count == 0; }
};

class CEngineSurfaceCompositionManager {
    unsigned char    _pad[0x8c];
    CGraphicDataBank IntermediateBankFile;              // +0x8c
    unsigned char    _padbanks[0x9b0 - 0x8c - sizeof(CGraphicDataBank)];
    CWideString      IntermediateDiskCacheFileName;     // +0x9b0
    CWideString      CompletedDiskCacheFileName;        // +0x9b4
    CWideString      CustomTextureBankFileName;         // +0x9b8
    SMorphMapStandIn GeneratedMorphs;                   // +0x9bc
    void BeginBankUpdate();
};

void CEngineSurfaceCompositionManager::BeginBankUpdate()
{
    g_pEngine_01436ea4->DeallocateShadowBuffer();
    unsigned long create = 0;
    if (GeneratedMorphs.empty())
        create = 1;
    IntermediateBankFile.OpenFileReadWrite(IntermediateDiskCacheFileName, create, 0x1000);
    IntermediateBankFile.BeginUpdateEntries();
}