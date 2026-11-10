#ifndef FABLE_ENGINE_C3DANIMFILELOADER_H
#define FABLE_ENGINE_C3DANIMFILELOADER_H
// Retail opt-in (Codex pass19): original Ego_r/FableWin types and donor calls; native animation rows 00a24d70/00a26ad0. See NAMING_EVIDENCE.md.
#include "rebuild_abi.h"
#include "engine/CChunkedFileLoader.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
class C3DAnimFile;class CDataInputStream;class CHandleBasedMemoryPool;class CChunkedFileChunk;class C3DAnimFileRootChunk;
class C3DAnimFileLoader:public CChunkedFileLoader {
public:
 C3DAnimFileLoader(const C3DAnimFileLoader&);C3DAnimFileLoader();virtual ~C3DAnimFileLoader();
 CCountedPointer<C3DAnimFile> LoadAnimFile(CDataInputStream&);
 CCountedPointer<C3DAnimFile> LoadAnimFileCompressed(const void*,unsigned long,CHandleBasedMemoryPool&);
 CCountedPointer<CChunkedFileChunk> RegisterRootSubChunkType(const CChunkedFileChunk&);
 CCountedPointer<CChunkedFileChunk> RegisterObjectSubChunkType(const CChunkedFileChunk&);
 CCountedPointer<CChunkedFileChunk> RegisterHelpersSubChunkType(const CChunkedFileChunk&);
 CCountedPointer<CChunkedFileChunk> RegisterMovementVectorSubChunkType(const CChunkedFileChunk&);
private:
 CCountedPointer<C3DAnimFileRootChunk> RootChunkType;
 CCountedPointer<CChunkedFileChunk> ObjectChunkType,SequenceChunkType,BoneMaskChunkType,HelpersChunkType,MovementVectorChunkType,MovementTrackChunkType,TimingEventChunkType;
 std::vector<CChunkedFileChunk*> ChunkTypes;
public:C3DAnimFileLoader& operator=(const C3DAnimFileLoader&);
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileLoader)==0x5c);
#endif
