// CMeshDataBank::NotifyBankSize(unsigned long noEntries, const vector<CBankEntryTypeSize>& entries) -- size the entry table for the bank and, for a read-only (retail) bank, pre-reserve the mesh, physics-mesh and animation info buffers from the per-type entry sizes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A26660, length 201 bytes (`ret 8`).
// Callees: CArray<CMeshDataBankEntry*>::resize(unsigned long, const T&) @ 0x00A2D0E0, CArray<C3DMeshInfo>::reserve @ 0x00A2C210,
// CArray<CPhysicsMeshInfo>::reserve @ 0x00A2C2B0, CArray<C3DAnimationInfo>::reserve @ 0x00A2C360 (labelled vector_reserve_generic*).
// Layout: engine/CMeshDataBank.h is 4 bytes off in retail (chunk 147) and carries the PDB's 16-byte CArrays, so the class is modelled
// locally with its PDB member names: retail CBankFile base is 0x10 below the PDB (ReadOnly +0x8D), MeshesAndAnimations +0x380,
// MeshInfoBuffer +0x38C, PhysicsMeshInfoBuffer +0x398, AnimationInfoBuffer +0x3A4 (12-byte retail CArrays).
// Shape: the resize fill value is a zero temporary in the frame (the same ebx zero starts the mesh total); the entry walk is an index loop
// over 8-byte {type, size} records strength-reduced to a pointer and a down-counter (`this` spilled to the frame); the type tests are
// if/else chains over two inline type predicates (types 1/2/4/5 meshes, 3 physics meshes, 6/10/7/8/9 animations; written inline
// the first chain places its add block at the end), the totals declared mesh, physics, animation.
class CMeshDataBankEntry;
class C3DMeshInfo;
class CPhysicsMeshInfo;
class C3DAnimationInfo;
template<class T> class CArray { public:               // retail CArray: 12 bytes
    T* First;
    T* Last;
    T* End;
    void resize(unsigned long size, const T& value);
    void reserve(unsigned long size);
};
struct CBankEntryTypeSize {                     // 8 bytes
    long          Type;
    unsigned long Size;
};
template<class T> struct vector {               // STLport vector
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long i) const { return *(_M_start + i); }
};
inline bool IsMeshType(long type) { return type == 1 || type == 2 || type == 4 || type == 5; }
inline bool IsAnimationType(long type) { return type == 6 || type == 10 || type == 7 || type == 8 || type == 9; }
class CMeshDataBank { public:
    void*         __vftable;                    // +0x000
    unsigned char _bankFile[0x88];              // +0x004 CBankFile
    bool          RetailMode;                   // +0x08c
    bool          ReadOnly;                     // +0x08d
    unsigned char _pad08e[0x380 - 0x8e];
    CArray<CMeshDataBankEntry*> MeshesAndAnimations;    // +0x380
    CArray<C3DMeshInfo>         MeshInfoBuffer;         // +0x38c
    CArray<CPhysicsMeshInfo>    PhysicsMeshInfoBuffer;  // +0x398
    CArray<C3DAnimationInfo>    AnimationInfoBuffer;    // +0x3a4
    void NotifyBankSize(unsigned long noEntries, const vector<CBankEntryTypeSize>& entries);
};

void CMeshDataBank::NotifyBankSize(unsigned long noEntries, const vector<CBankEntryTypeSize>& entries)
{
    MeshesAndAnimations.resize(noEntries, 0);
    if (ReadOnly) {
        unsigned long meshSize = 0;
        unsigned long physicsMeshSize = 0;
        unsigned long animationSize = 0;
        for (unsigned long i = 0; i < entries.size(); ++i) {
            long type = entries[i].Type;
            unsigned long size = entries[i].Size;
            if (IsMeshType(type))
                meshSize += size;
            else if (type == 3)
                physicsMeshSize += size;
            else if (IsAnimationType(type))
                animationSize += size;
        }
        MeshInfoBuffer.reserve(meshSize);
        PhysicsMeshInfoBuffer.reserve(physicsMeshSize);
        AnimationInfoBuffer.reserve(animationSize);
    }
}