// C3DMesh2::InitialiseBoneNamePointers(long) -- when asked, resolves the bone-name handle through the pool at +4 and points BoneNameIndices (+0xa4) at it and BoneNameData (+0xa8) just past the BoneCount (+0x98) shorts
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A892E0, length 51 bytes.
// Callees: CHandleBasedMemoryPool::GetPointerFromHandle(long) @ 0x00A026B0.
// Retail C3DMesh2 sits 8 bytes below the Ego_r PDB layout here: BoneNameMemoryHandle +0x84,
// BoneCount +0x98, BoneNameIndices (unsigned short*) +0xa4, BoneNameData (char*) +0xa8. The
// `lea edx,[eax+ecx*2]` is the short-pointer arithmetic. The argument is tested as a dword.
struct CHandleBasedMemoryPool {
    void* GetPointerFromHandle(long handle);   // 0x00A026B0
};

struct C3DMesh2 {
    void* m_vptr;
    CHandleBasedMemoryPool* MemoryPool;    // +4
    unsigned char _pad_0x08[0x7c];
    long BoneNameMemoryHandle;             // +0x84
    unsigned char _pad_0x88[0x10];
    unsigned long BoneCount;               // +0x98
    unsigned char _pad_0x9c[0x8];
    unsigned short* BoneNameIndices;       // +0xa4
    char* BoneNameData;                    // +0xa8
    void InitialiseBoneNamePointers(long hasBoneNames);
};

void C3DMesh2::InitialiseBoneNamePointers(long hasBoneNames)
{
    if (hasBoneNames) {
        unsigned short* indices = (unsigned short*)MemoryPool->GetPointerFromHandle(BoneNameMemoryHandle);
        BoneNameIndices = indices;
        BoneNameData = (char*)(indices + BoneCount);
    }
}
