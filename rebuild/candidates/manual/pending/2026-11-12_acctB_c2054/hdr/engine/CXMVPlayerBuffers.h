#ifndef FABLE_ENGINE_CXMVPLAYERBUFFERS_H
#define FABLE_ENGINE_CXMVPLAYERBUFFERS_H
// Chunk 2051 (row 0x0042EC7C CNewFrontendGameComponent::Run). Ego_r.pdb CXMVPlayerBuffers (0x10 bytes): vptr +0x00 (Run deletes it
// through slot 0, 0x0042F232), InputBuffer +0x04, PlaybackBuffer +0x08, CurrentSize +0x0C. Ctor ??0CXMVPlayerBuffers@@QAE@XZ 0x0042DB40.
#include "rebuild_abi.h"
class CXMVPlayerBuffers {
public:
    CXMVPlayerBuffers();                                                      // 0x0042DB40
    virtual ~CXMVPlayerBuffers();                                             // slot 0
private:
    char* InputBuffer;                                                        // +0x04
    char* PlaybackBuffer;                                                     // +0x08
    unsigned long CurrentSize;                                                // +0x0C
};
FABLE_STATIC_ASSERT(sizeof(CXMVPlayerBuffers) == 0x10);
#endif
