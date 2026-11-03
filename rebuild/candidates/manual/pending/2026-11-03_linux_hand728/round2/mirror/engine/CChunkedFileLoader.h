#ifndef FABLE_ENGINE_CCHUNKEDFILELOADER_H
#define FABLE_ENGINE_CCHUNKEDFILELOADER_H
// New header (chunk 725b; Codex pass-12 model): CChunkedFileLoader, Ego_r sizeof 0x10: vptr, ReadUnknownChunkTypes +4,
// SupportedFileType[5] +8. Constructor FableWin ??0CChunkedFileLoader@@QAE@PBD@Z; the scalar deleting destructor 0x00AA8D10
// (slot 0 of the C3DMeshFileXLoader vtable 0x0129E450 is the derived one). Root of C3DMeshFileLoader (rtti_bases.tsv).
#include "rebuild_abi.h"
class CChunkedFileLoader {
public:
    CChunkedFileLoader(const char*);
    virtual ~CChunkedFileLoader();
private:
    int ReadUnknownChunkTypes;      // +0x04
    char SupportedFileType[5];      // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CChunkedFileLoader) == 0x10);
#endif
