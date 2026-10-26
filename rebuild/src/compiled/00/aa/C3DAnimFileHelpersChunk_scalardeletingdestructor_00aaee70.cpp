class CChunkedFileSuperChunk {
public:
    virtual ~CChunkedFileSuperChunk();   // 0x00AA8490
};
class C3DAnimFileHelpersChunk { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall C3DAnimFileHelpersChunk_scalar_deleting_destructor(C3DAnimFileHelpersChunk* chunk, int edx_unused, unsigned int flags)
{
    ((CChunkedFileSuperChunk*)(chunk))->CChunkedFileSuperChunk::~CChunkedFileSuperChunk();
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}