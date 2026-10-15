// C3DAnimFileSequenceChunk::~C3DAnimFileSequenceChunk() -- free the Matrices array, destroy BoneName, then ~CChunkedFileSuperChunk
// Retail 0x00A55420, 35 bytes. Called only from the deleting destructor 0x00AAFFF0
// (C3DAnimFileSequenceChunk vtable 0x0129E8B8 slot 0); the manifest label "CTCInventoryItem::OnDie"
// is propagated.
// Members die in reverse order: Matrices (CArray<CMatrix3x4>, retail +0x40) frees its block with
// `free` 0x00BFEA14 under a null test (`add esp,4`), BoneName (CCharString, +0x34) runs ~CCharString
// 0x0099EAE0, then CChunkedFileSuperChunk::~CChunkedFileSuperChunk 0x00AA8490 is tail-jumped. No
// vtable store: `__declspec(novtable)` over the out-of-line base destructor.
// Retail offsets: the base is 0x2C bytes (12-byte CArrays, as the landed 0x00AA8490), so every member
// sits 8 bytes below Ego_r.pdb (BoneIndex +0x2C, ParentIndex +0x30, BoneName +0x34,
// SamplesPerSecond +0x38, BoneType +0x3C, Matrices +0x40).
// Leaf: ??1C3DAnimFileSequenceChunk

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CCharString {
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};

struct CMatrix3x4;
struct CMatrixArray {                              // CArray<CMatrix3x4>
    CMatrix3x4* m_pData;
    long m_count;
    long m_capacity;
    ~CMatrixArray() { if (m_pData) free(m_pData); }
};

struct CChunkedFileSuperChunk {
    virtual void v00();
    unsigned char _body[0x28];                     // chunk type/parent, SubChunks, RegisteredSubChunkTypes, CurrentChunkIndex
    ~CChunkedFileSuperChunk();                     // 0x00AA8490
};

struct __declspec(novtable) C3DAnimFileSequenceChunk : CChunkedFileSuperChunk {
    long BoneIndex;                                // +0x2C
    long ParentIndex;                              // +0x30
    CCharString BoneName;                          // +0x34
    float SamplesPerSecond;                        // +0x38
    long BoneType;                                 // +0x3C EBoneType
    CMatrixArray Matrices;                         // +0x40
    virtual void v00();
    ~C3DAnimFileSequenceChunk();
};

C3DAnimFileSequenceChunk::~C3DAnimFileSequenceChunk()
{
}
