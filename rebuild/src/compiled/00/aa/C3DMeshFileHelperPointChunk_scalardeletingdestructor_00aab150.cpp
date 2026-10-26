#pragma optimize("t", on)
class CChunkedFileSuperChunk {
public:
    virtual ~CChunkedFileSuperChunk();   // 0x00AA8490
};
extern void __fastcall DestructMember_0099eae0(void* member);
void* __fastcall C3DMeshFileHelperPointChunk_CompositeScalarDeletingDestructor(void* chunk, int, unsigned int flags)
{
    DestructMember_0099eae0(static_cast<unsigned char*>(chunk) + 64);
    ((CChunkedFileSuperChunk*)(chunk))->CChunkedFileSuperChunk::~CChunkedFileSuperChunk();
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}