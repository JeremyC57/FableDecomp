class CChunkedFileSuperChunk {
public:
    virtual ~CChunkedFileSuperChunk();   // 0x00AA8490
};
class C3DMeshFileTransformationChunk { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall C3DMeshFileTransformationChunk_scalar_deleting_destructor(C3DMeshFileTransformationChunk* chunk, int edx_unused, unsigned int flags)
{
    ((CChunkedFileSuperChunk*)(chunk))->CChunkedFileSuperChunk::~CChunkedFileSuperChunk();
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}