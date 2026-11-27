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
#ifdef FABLE_RETAIL_CCHUNKEDFILELOADER_LOADCHUNKEDFILE
    // Per-TU opt-in (NF422, the loaders 0x00A99420 / 0x00A9B640): Ego_r.pdb / FableWin.pdb
    // ?LoadChunkedFile@CChunkedFileLoader@@IAEXAAVCChunkedFile@@AAVCDataInputStream@@@Z (protected), retail 0x00AA79C0 (reloc_bindings).
protected:
    void LoadChunkedFile(class CChunkedFile&, class CDataInputStream&);   // 0x00AA79C0
#endif
#ifdef FABLE_RETAIL_CHUNK_READER_PASS29
public:
int GetReadUnknownChunkTypes() const { return ReadUnknownChunkTypes; }
#endif
private:
    int ReadUnknownChunkTypes;      // +0x04
    char SupportedFileType[5];      // +0x08
};
FABLE_STATIC_ASSERT_AT(15, sizeof(CChunkedFileLoader) == 0x10);
#endif
