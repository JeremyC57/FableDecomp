#pragma optimize("t", on)
class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();   // 0x00AA75C0
};
extern void __fastcall DestructMember_0099eae0(void* member);
void* __fastcall C3DAnimFileTimingEventChunk_CompositeScalarDeletingDestructor(void* chunk, int, unsigned int flags)
{
    DestructMember_0099eae0(static_cast<unsigned char*>(chunk) + 16);
    ((CChunkedFileChunk*)(chunk))->CChunkedFileChunk::~CChunkedFileChunk();
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}