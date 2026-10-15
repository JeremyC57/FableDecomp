// C3DMeshFileConvexVolumeChunk::~C3DMeshFileConvexVolumeChunk() -- free the Planes array, destroy Name, then ~CChunkedFileSuperChunk
// Retail 0x00A9D3F0, 35 bytes. Called only from the deleting destructor 0x00AAB6E0
// (C3DMeshFileConvexVolumeChunk vtable 0x0129E634 slot 0); the manifest label "CTCInventoryItem::OnDie"
// is propagated.
// Members die in reverse order: Planes (CArray<CPlane>, retail +0x30) frees its block with
// `free` 0x00BFEA14 under a null test (`add esp,4`), Name (CCharString, +0x2C) runs ~CCharString
// 0x0099EAE0, then CChunkedFileSuperChunk::~CChunkedFileSuperChunk 0x00AA8490 is tail-jumped. No
// vtable store: `__declspec(novtable)` over the out-of-line base destructor.
// Retail offsets: the base is 0x2C bytes (12-byte CArrays, as the landed 0x00AA8490), so every member
// sits 8 bytes below Ego_r.pdb (Name +0x2C, Planes +0x30).
// Leaf: ??1C3DMeshFileConvexVolumeChunk

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CCharString {
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};

struct CPlane;
struct CPlaneArray {                               // CArray<CPlane>
    CPlane* m_pData;
    long m_count;
    long m_capacity;
    ~CPlaneArray() { if (m_pData) free(m_pData); }
};

struct CChunkedFileSuperChunk {
    virtual void v00();
    unsigned char _body[0x28];                     // chunk type/parent, SubChunks, RegisteredSubChunkTypes, CurrentChunkIndex
    ~CChunkedFileSuperChunk();                     // 0x00AA8490
};

struct __declspec(novtable) C3DMeshFileConvexVolumeChunk : CChunkedFileSuperChunk {
    CCharString Name;                              // +0x2C
    CPlaneArray Planes;                            // +0x30
    virtual void v00();
    ~C3DMeshFileConvexVolumeChunk();
};

C3DMeshFileConvexVolumeChunk::~C3DMeshFileConvexVolumeChunk()
{
}
