class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();   // 0x00AA75C0
};
class C3DMeshFileXBoundingSphereChunk { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall C3DMeshFileXBoundingSphereChunk_scalar_deleting_destructor(C3DMeshFileXBoundingSphereChunk* chunk, int edx_unused, unsigned int flags)
{
    ((CChunkedFileChunk*)(chunk))->CChunkedFileChunk::~CChunkedFileChunk();
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}