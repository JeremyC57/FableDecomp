#ifndef FABLE_ENGINE_CGPUSCRATCHBUFFERTEXTUREMANAGER_H
#define FABLE_ENGINE_CGPUSCRATCHBUFFERTEXTUREMANAGER_H
// New header (chunk 767, row 0x00B3AFE0 CEngineResourceManager::OnReleaseDefaultPoolResources). The resource manager's
// GPUScratchBufferTextureManager member (retail +0x3190, 0x44 bytes: engine/CEngineResourceManager.h). Members are not modelled yet
// (one opaque block); only the members rows call are declared, with their Ego_r.pdb / FableWin.pdb spellings:
// ?BeginShutdown@CGPUScratchBufferTextureManager@@QAE_NXZ (retail 0x00F37170, landed).
#include "rebuild_abi.h"
class CGPUScratchBufferTextureManager {
public:
    bool BeginShutdown();                                    // 0x00F37170
#ifdef FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE
    // Opt-in (bwf2, row 0x00B72180): Ego_r.pdb 0x00B3C370 ?StartTextureUse@CGPUScratchBufferTextureManager@@QAEPAVCTexture@@J@Z
    // (retail 0x00F36EE0: returns the handle's entry +4 = the texture; 0x00F36F10 returns +8 = StartSurfaceUse).
    class CTexture* StartTextureUse(long handle);                        // 0x00F36EE0
#endif
private:
    unsigned char Unmodeled_0x00[0x44];
};
FABLE_STATIC_ASSERT(sizeof(CGPUScratchBufferTextureManager) == 0x44);
#endif // FABLE_ENGINE_CGPUSCRATCHBUFFERTEXTUREMANAGER_H
