#pragma optimize("t", on)
class CChunkedFileSuperChunk {
public:
    virtual ~CChunkedFileSuperChunk();   // 0x00AA8490
};
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
void* __fastcall C3DMeshFileSubMeshChunk_CompositeScalarDeletingDestructor(void* chunk, int, unsigned int flags)
{
    ((CCharString*)(static_cast<unsigned char*>(chunk) + 44))->~CCharString();
    ((CChunkedFileSuperChunk*)(chunk))->CChunkedFileSuperChunk::~CChunkedFileSuperChunk();
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}