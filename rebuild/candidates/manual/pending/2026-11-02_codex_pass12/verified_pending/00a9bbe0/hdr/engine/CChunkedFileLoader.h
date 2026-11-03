#ifndef FABLE_ENGINE_CCHUNKEDFILELOADER_H
#define FABLE_ENGINE_CCHUNKEDFILELOADER_H
// Retail opt-in (Codex pass 12): original PDB types and native chunk ctor/dtor offsets.
#include "rebuild_abi.h"
class CChunkedFileLoader {
public:CChunkedFileLoader(const char*);virtual ~CChunkedFileLoader();
private:int ReadUnknownChunkTypes;char SupportedFileType[5];
};
FABLE_STATIC_ASSERT(sizeof(CChunkedFileLoader)==16);
#endif
