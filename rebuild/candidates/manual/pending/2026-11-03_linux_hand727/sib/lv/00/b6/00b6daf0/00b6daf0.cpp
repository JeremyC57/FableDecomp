// CEngineWaterRenderer::LoadNonPatchData(CMemoryDataInputStream&, CMemoryDataInputStream&) -- read the sea static-map info block size from the header stream and load the info block
// Leaf: LoadNonPatchData
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6DAF0, 130 bytes (+14 int3 padding), `ret 8`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, CWaterSeaStaticMapInfoBlock::Load @ 0x00B6D6E0.
// Identity: the only caller is CEngineWorldMap::LoadWaterData (landed 0x00B41FA0), which calls it on the global
// water renderer (0x01436E54) with (dataStream, headerStream); the catalog label `CPersistTraits<float>::
// TransferBinaryIn` is propagated and wrong. Retail's body never reads `this` or the data stream: it reads the
// block size (a long, homed in the dead header-stream argument slot) from the header stream through the inlined
// CMemoryDataInputStream::Read (64-bit [0, 0x7FFFFFFF] position check, fast copy from the chunk pointer, else
// ReadWithSrcChunkOverflow) and hands it to CWaterSeaStaticMapInfoBlock::Load on an 8-byte local block
// (`sub esp,8`; the PDB's block is 12 bytes with CutoutBlockCRC, retail's Load reads only 8, so the retail-era
// block is modelled with its first two members). Both the fast path and the fall-through tail end in the Load call.
// `this` uses engine/CEngineWaterRenderer.h; the stream uses engine/CMemoryDataInputStream.h with an inline Read
// (the header's `position` is long; the sum is taken unsigned, matching retail's zero high dword).
#include "engine/CEngineWaterRenderer.h"
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { friend struct CMemoryDataInputStreamReader;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)((unsigned long)position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

struct CWaterSeaStaticMapInfoBlock {
    long SeaStaticMapVersionNumber;   // +0x00
    long DataStreamPosition;          // +0x04
    bool Load(CMemoryDataInputStream& stream, long blockSize);   // 0x00B6D6E0
};

struct CEngineWaterRendererLoader : CEngineWaterRenderer {
    void LoadNonPatchData(CMemoryDataInputStream& data, CMemoryDataInputStream& header);
};

void CEngineWaterRendererLoader::LoadNonPatchData(CMemoryDataInputStream& data, CMemoryDataInputStream& header)
{
    long blockSize;
    static_cast<CMemoryDataInputStreamReader&>(header).Read(&blockSize, sizeof(blockSize));
    CWaterSeaStaticMapInfoBlock info;
    info.Load(header, blockSize);
}