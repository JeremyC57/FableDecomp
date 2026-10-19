// Grade: DIFFER(197v218) -- register allocation: retail keeps this in edi, the mesh in ebx, the bone count in esi (plus a frame copy feeding a count-down loop counter homed in the dead mesh slot) and re-reads Anim.StartState.Data before PeekCombinedMorph; ours keeps this in esi, caches the shared-data pointer and spills the scalings reference (tried early returns, &&, a null-safe accessor)
// CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings(const C3DMesh2&) const -- when the start animation state carries a skeletal morph, fill the cached per-mesh bone-scaling array (keyed by the mesh ID, sized to its bone count) from the combined morph target, one bone at a time
// Leaf: InitialiseBoneScalings
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD2A90, 218 bytes (+ int3 padding), `ret 4`. Mangled
//   ?InitialiseBoneScalings@CEngineInternalPrimitiveAnimatedMesh@@QBEXABVC3DMesh2@@@Z.
// Callees: C3DSkeletalMorph::PeekCombinedMorph() const 0x00A45260 (CCountedPointer<const C3DSkeletalMorphTarget> by hidden
//   pointer), map<unsigned long, CArray<C3DVector>>::operator[](const unsigned long&) 0x00BD9110, vector<C3DVector>::resize(
//   size_type, const C3DVector&) 0x0075C040, C3DSkeletalMorphTarget::GetBoneScaling(C3DVector&, unsigned long bone)
//   0x00A44CD0, operator delete 0x00BFE9BC (the counted pointer's info block).
// Layout (retail): this: Anim (C3DAnimationInterpolator +0x70) StartState.Data +0x78 -> CSharedData whose SkeletalMorph
//   counted pointer is at +0x18 (Ego_r.pdb +0x1c, the CArray before it is 12 bytes in retail); Cache.BoneScalings +0xd4 (PDB
//   agrees). C3DMesh2 sits 8 bytes below the PDB: BoneCount +0x98, Bones +0x9c (C3DBone2, 0x3c bytes, ID first), MeshID +0xe8.
// Shape: two early returns (the shared-data pointer re-read for the second test); the morph target is a by-value counted
//   pointer local released on the way out; the bone count is a named local; the map key is a by-value copy of MeshID passed by
//   address through a __forceinline by-value helper (homed in the dead `mesh` slot); the default resize value is an uninitialised C3DVector temporary; the loop reads
//   the scaling array's first pointer and the bone array each pass.
void __cdecl operator delete(void* block);             // 0x00BFE9BC

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};

struct CCPPointerInfo {
    long  References;                                  // +0x00
    void (__fastcall* Destroy)(void* object);          // +0x04
    void* Object;                                      // +0x08
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
    }
};

struct C3DSkeletalMorphTarget {
    void GetBoneScaling(C3DVector& scaling, unsigned long bone) const;   // 0x00A44CD0
};
struct C3DSkeletalMorph {
    CCountedPointer<const C3DSkeletalMorphTarget> PeekCombinedMorph() const;   // 0x00A45260
};

struct C3DBone2 {                                      // 0x3c bytes
    unsigned long ID;                                  // +0x00
    unsigned char _body[0x38];
};
struct C3DMesh2 {
    unsigned char  _pad00[0x98];
    unsigned long  BoneCount;                          // +0x98 (retail)
    C3DBone2*      Bones;                              // +0x9c
    unsigned char  _pada0[0xe8 - 0xa0];
    unsigned long  MeshID;                             // +0xe8
};

struct CVector3DArray {                                // CArray<C3DVector> (retail vector, 12 bytes)
    C3DVector* m_pFirst;
    C3DVector* m_pLast;
    C3DVector* m_pEnd;
    void resize(unsigned int newSize, const C3DVector& value = C3DVector());   // 0x0075C040
    C3DVector& operator[](unsigned long index) { return m_pFirst[index]; }
};
struct CBoneScalingMap {                               // map<unsigned long, CArray<C3DVector>>
    void* _M_words[3];
    CVector3DArray& operator[](const unsigned long& key);   // 0x00BD9110
};

struct CAnimationSharedData {                          // C3DAnimationBlendState::CSharedData (retail)
    unsigned char                           _pad00[0x18];
    CCountedPointer<const C3DSkeletalMorph> SkeletalMorph;   // +0x18
};

struct CEngineInternalPrimitiveAnimatedMesh {
    unsigned char           _pad00[0x78];
    CAnimationSharedData*   StartStateData;            // +0x78 (Anim.StartState.Data)
    unsigned char           _pad7c[0xd4 - 0x7c];
    mutable CBoneScalingMap BoneScalings;              // +0xd4 (Cache.BoneScalings)
    __forceinline CVector3DArray& GetBoneScalings(unsigned long meshID) const { return BoneScalings[meshID]; }
    void InitialiseBoneScalings(const C3DMesh2& mesh) const;
};

void CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings(const C3DMesh2& mesh) const
{
    if (StartStateData && StartStateData->SkeletalMorph.Data) {
        CCountedPointer<const C3DSkeletalMorphTarget> target = StartStateData->SkeletalMorph->PeekCombinedMorph();
        unsigned long boneCount = mesh.BoneCount;
        CVector3DArray& scalings = GetBoneScalings(mesh.MeshID);
        scalings.resize(boneCount);
        for (unsigned long i = 0; i < boneCount; ++i)
            target->GetBoneScaling(scalings[i], mesh.Bones[i].ID);
    }
}
