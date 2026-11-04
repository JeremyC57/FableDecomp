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
private:
    unsigned char Unmodeled_0x00[0x44];
};
FABLE_STATIC_ASSERT(sizeof(CGPUScratchBufferTextureManager) == 0x44);
#endif // FABLE_ENGINE_CGPUSCRATCHBUFFERTEXTUREMANAGER_H
