#pragma optimize("t", on)
class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();   // 0x00AA75C0
};
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
void* __fastcall C3DAnimFileTimingEventChunk_CompositeScalarDeletingDestructor(void* chunk, int, unsigned int flags)
{
    ((CCharString*)(static_cast<unsigned char*>(chunk) + 16))->~CCharString();
    ((CChunkedFileChunk*)(chunk))->CChunkedFileChunk::~CChunkedFileChunk();
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}