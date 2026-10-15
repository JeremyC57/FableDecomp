// C3DMeshFileGroupChunk::GetChunkInfo() const -- "Bone Index: %li, Vertex Count: %lu" into a 1 KB buffer, returned as a CCharString
// Grade: DIFFER(100v98) -- /GS lane gap: the lane loads the hidden result pointer into esi in the prologue (and copies this to eax), retail re-reads its argument slot after the sprintf call (mov esi,[esp+41Ch]); optimize(s) worse
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABAE50, 98 bytes. Callees: sprintf 0x00BFEB10 (format @ 0x0129EBD0),
// CCharString(const char*, long) 0x0099EBF0, __security_check_cookie 0x00BFE9F9 (cookie @ 0x0139C8A8).
// The PDB header (engine/C3DMeshFileGroupChunk.h) sits 8 bytes high (its C3DMeshFileChunk base is 0x34,
// retail TLC's is 0x2C, as in the landed C3DMeshFile*Chunk family), so the retail layout is modelled here
// with the PDB names: BoneIndex +0x2C, VertexList +0x30 (CArray<C3DVertexBlend2>, 8-byte elements: `sar 3`).
extern "C" int __cdecl sprintf(char* buffer, const char* format, ...);   // 0x00BFEB10

struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
};

struct C3DVertexBlend2 { unsigned short m_index; float m_weight; };   // 8 bytes
template <class T> struct CArray {                  // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return m_end - m_begin; }
};

struct C3DMeshFileGroupChunk {
    virtual ~C3DMeshFileGroupChunk();
    unsigned char _base_body[0x28];                 // +0x04 .. +0x2B (C3DMeshFileChunk base, retail size)
    long BoneIndex;                                 // +0x2C
    CArray<C3DVertexBlend2> VertexList;             // +0x30
    CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileGroupChunk::GetChunkInfo() const
{
    char buffer[1024];
    sprintf(buffer, "Bone Index: %li, Vertex Count: %lu", BoneIndex, VertexList.Size());
    return CCharString(buffer, -1);
}
