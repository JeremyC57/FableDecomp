class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();   // 0x00AA75C0
};
class C3DMeshFileXMappingInfoChunk { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall C3DMeshFileXMappingInfoChunk_scalar_deleting_destructor(C3DMeshFileXMappingInfoChunk* chunk, int edx_unused, unsigned int flags)
{
    ((CChunkedFileChunk*)(chunk))->CChunkedFileChunk::~CChunkedFileChunk();
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}