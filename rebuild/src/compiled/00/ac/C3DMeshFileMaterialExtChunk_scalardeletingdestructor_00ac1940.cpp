class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();   // 0x00AA75C0
};
class C3DMeshFileMaterialExtChunk { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall C3DMeshFileMaterialExtChunk_scalar_deleting_destructor(C3DMeshFileMaterialExtChunk* chunk, int edx_unused, unsigned int flags)
{
    ((CChunkedFileChunk*)(chunk))->CChunkedFileChunk::~CChunkedFileChunk();
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}