// CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings(const C3DMesh& mesh) -- when the animation has a skeletal morph, size the cached bone-scaling array for this mesh to its bone count and fill each entry from the combined morph target
// Grade: DIFFER(197v218) -- draft (one grade): retail re-reads the morph pointer for the call, keeps `this` in edi, the mesh in ebx, the bone count in esi and a frame counter for the loop (`dec [esp+2Ch]`) with two strided induction pointers; the counted pointer is released through the frame copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD2A90, length 224 bytes (ret 4).
// Callees: C3DSkeletalMorph::PeekCombinedMorph() 0x00A45260 (CCountedPointer<const C3DSkeletalMorphTarget> by hidden pointer),
// the cache's find-or-insert 0x00BD9110 (keyed by the mesh's +0xE8 word, returns the vector<C3DVector>), vector<C3DVector>::
// resize(unsigned, const C3DVector&) 0x0075C040, C3DSkeletalMorphTarget::GetBoneScaling(C3DVector*, unsigned long) 0x00A44CD0;
// the counted pointer's release (destroy through the count block, operator delete 0x00BFE9BC).
// Layout: the animation's morph at this+0x78 (+0x18 of it the skeletal morph), Cache +0xD4; the mesh's bone count +0x98, bone
// records +0x9C (0x3C bytes, the id first), cache key +0xE8.
// Shape: draft (one grade); see the Grade line.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};

struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> struct CCountedPointer {
    T*           m_pObject;
    SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
    T* operator->() const { return m_pObject; }
};

struct C3DSkeletalMorphTarget {
    void GetBoneScaling(C3DVector* scaling, unsigned long boneID) const;   // 0x00A44CD0
};
struct C3DSkeletalMorph {
    CCountedPointer<const C3DSkeletalMorphTarget> PeekCombinedMorph();   // 0x00A45260
};
struct C3DAnimationMorphs {
    unsigned char     _pad0[0x18];
    C3DSkeletalMorph* SkeletalMorph;            // +0x18
};

struct C3DBone { unsigned long ID; unsigned char _rest[0x38]; };   // 0x3c bytes
struct C3DMesh {
    unsigned char _pad0[0x98];
    unsigned long BoneCount;                    // +0x98
    C3DBone*      Bones;                        // +0x9c
    unsigned char _pada0[0xe8 - 0xa0];
    unsigned long CacheKey;                     // +0xe8
};

struct CBoneScalingVector {                     // vector<C3DVector>
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    void resize(unsigned int size, const C3DVector& value);   // 0x0075C040
};
struct CBoneScalingCache {
    CBoneScalingVector& FindOrInsert(const unsigned long& key);   // 0x00BD9110
};

class CEngineInternalPrimitiveAnimatedMesh {
public:
    unsigned char       _pad00[0x78];
    C3DAnimationMorphs* Morphs;                 // +0x78
    unsigned char       _pad7c[0xd4 - 0x7c];
    CBoneScalingCache   Cache;                  // +0xd4
    void InitialiseBoneScalings(const C3DMesh& mesh);
};

void CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings(const C3DMesh& mesh)
{
    if (Morphs && Morphs->SkeletalMorph) {
        CCountedPointer<const C3DSkeletalMorphTarget> target = Morphs->SkeletalMorph->PeekCombinedMorph();
        unsigned long boneCount = mesh.BoneCount;
        unsigned long key = mesh.CacheKey;
        CBoneScalingVector& scalings = Cache.FindOrInsert(key);
        scalings.resize(boneCount, C3DVector());
        for (unsigned long i = 0; i < boneCount; ++i)
            target->GetBoneScaling(&scalings._M_start[i], mesh.Bones[i].ID);
    }
}
