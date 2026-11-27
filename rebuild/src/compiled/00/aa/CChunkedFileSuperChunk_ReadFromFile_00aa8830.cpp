// 00aa8830 CChunkedFileSuperChunk::ReadFromFile
#define FABLE_RETAIL_CCHUNKEDFILECHUNK_GETCHUNKTYPE
// Flags: /GS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
#define FABLE_RETAIL_STLP_FILL_FASTCALL
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
// Leaf: ?ReadFromFile@CChunkedFileSuperChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z
#define FABLE_RETAIL_CHUNK_READER_PASS29
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_ADDSUBCHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_SUBCHUNKS_TYPED
#define FABLE_RETAIL_CCHUNKEDFILERAWDATACHUNK
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATASTREAM
#include "engine/CDataInputStream.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/CChunkedFileLoader.h"
#include "engine/CChunkedFileRawDataChunk.h"

void __cdecl operator delete[](void*);
CChunkedFileChunk::~CChunkedFileChunk() {}
CChunkedFileRawDataChunk::~CChunkedFileRawDataChunk() { delete[] Data; }
CChunkedFileChunk::CChunkedFileChunk(const char* chunktype) : Parent(0)
{
    memset(ChunkType, 0, sizeof(ChunkType));
    memcpy(ChunkType, chunktype, 4);
}
CChunkedFileRawDataChunk::CChunkedFileRawDataChunk(const char* chunktype)
    : CChunkedFileChunk(chunktype), Data(0), ByteSize(0)
{
}

const CChunkedFileChunk* CChunkedFileSuperChunk::FindRegisteredSubChunkType(const char* chunktype) const
{
    for (CArray<CCountedPointer<CChunkedFileChunk> >::const_iterator itr = RegisteredSubChunkTypes.begin(); itr != RegisteredSubChunkTypes.end(); ++itr) {
        const CCountedPointer<CChunkedFileChunk>& clonable_chunk = *itr;
        if (strcmp(chunktype, clonable_chunk->GetChunkType()) == 0)
            return clonable_chunk.Get();
    }
    return 0;
}

void CChunkedFileSuperChunk::AddSubChunk(const CCountedPointer<CChunkedFileChunk>& subchunk)
{
    subchunk->SetParent(this);
    SubChunks.push_back(subchunk);
}

void CChunkedFileSuperChunk::ReadFromFile(CDataInputStream& is, unsigned long bytes, CChunkedFileLoader& loader)
{
    unsigned long bytes_read = 0;
    while (bytes_read < bytes) {
        char chunktype[5] = {0};
        is.Read(chunktype, 4);
        chunktype[4] = 0;
        unsigned long chunksize = is.ReadULONG();
        bytes_read += 8;
        long oldpos = is.GetPosition();
        const CChunkedFileChunk* clonable_chunk = FindRegisteredSubChunkType(chunktype);
        if (clonable_chunk) {
            CCountedPointer<CChunkedFileChunk> new_chunk(clonable_chunk->CreateNewInstance());
            new_chunk->ReadFromFile(is, chunksize, loader);
            long chunksize_read = is.GetPosition() - oldpos;
            AddSubChunk(new_chunk);
        } else if (loader.GetReadUnknownChunkTypes()) {
            CCountedPointer<CChunkedFileChunk> new_chunk(new CChunkedFileRawDataChunk(chunktype));
            new_chunk->ReadFromFile(is, chunksize, loader);
            AddSubChunk(new_chunk);
        }
        is.SetPosition(oldpos + chunksize);
        bytes_read += chunksize;
    }
}