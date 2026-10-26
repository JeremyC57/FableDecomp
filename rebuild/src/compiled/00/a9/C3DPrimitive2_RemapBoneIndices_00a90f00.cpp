// C3DPrimitive2::RemapBoneIndices(map<unsigned long, unsigned long>&) -- remap the bone indices of the device-independent data, every cloth primitive and every animated block's group table
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A90F00, length 192 bytes (`ret 4`).
// MESH LAYER: Ego_r.pdb ?RemapBoneIndices@C3DPrimitive2@@QAEXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z.
// Callees: N3DPrimitive::CDeviceIndependent::RemapBoneIndices @ 0x00A90D60, the cloth primitive's RemapBoneIndices @ 0x00AC3AF0
// (catalog "_Cons_val<...CParticleMorphs...>" is a propagated twin), map<unsigned long, unsigned long>::operator[] @ 0x00553E60
// (catalog "std::map::insert").
// Retail offsets (PDB - 4: 12-byte CArrays): ClothPrimitives (CArray<CCountedPointer<C3DClothPrimitive> >) +0x20, Data
// (scoped_ptr<N3DPrimitive::CDeviceIndependent>) +0x2c, AnimatedBlockCount +0x48, AnimatedBlocks +0x4c (0x30-byte
// CAnimatedBlock: GroupCount (uchar) +0x17, Groups uchar[18] +0x18).
// Shape: STLport `*(begin() + i)` over the cloth array (size re-read each pass); each group byte widened into the
// operator[] key temporary and replaced by the low byte of the mapped value.
// Leaf: RemapBoneIndices@C3DPrimitive2
struct CBoneMap {                                   // map<unsigned long, unsigned long>
    unsigned long& operator[](const unsigned long& key);   // 0x00553E60
};
class C3DClothPrimitive { public:
    void RemapBoneIndices(CBoneMap& remap);         // 0x00AC3AF0
};
namespace N3DPrimitive {
class CDeviceIndependent { public:
    void RemapBoneIndices(CBoneMap& remap);         // 0x00A90D60
};
}
template<class T> class CCountedPointer { public:
    T* Object; void* Count;
    T* operator->() const { return Object; }
};
template<class T> class CArray { public:                   // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    T* begin() const { return m_begin; }
    unsigned long size() const { return (unsigned long)(m_end - m_begin); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CAnimatedBlock {                             // 0x30
    unsigned char _00[0x0c];
    unsigned long BoneConstantsFixupOffset;         // +0x0c
    unsigned long VertexCount;                      // +0x10
    unsigned short BonesPerVertex;                  // +0x14
    bool PalettedFlag;                              // +0x16
    unsigned char GroupCount;                       // +0x17
    unsigned char Groups[18];                       // +0x18
    void* Data;                                     // +0x2c
};
class C3DPrimitive2 { public:
    unsigned char _pad[0x20];
    CArray<CCountedPointer<C3DClothPrimitive> > ClothPrimitives;   // +0x20
    N3DPrimitive::CDeviceIndependent* Data;         // +0x2c scoped_ptr
    unsigned char _30[0x18];
    unsigned long AnimatedBlockCount;               // +0x48
    CAnimatedBlock* AnimatedBlocks;                 // +0x4c
    void RemapBoneIndices(CBoneMap& remap);
};

void C3DPrimitive2::RemapBoneIndices(CBoneMap& remap)
{
    Data->RemapBoneIndices(remap);
    for (unsigned long i = 0; i < ClothPrimitives.size(); ++i)
        ClothPrimitives[i]->RemapBoneIndices(remap);
    for (unsigned long b = 0; b < AnimatedBlockCount; ++b) {
        CAnimatedBlock& block = AnimatedBlocks[b];
        for (unsigned long g = 0; g < block.GroupCount; ++g)
            block.Groups[g] = (unsigned char)remap[block.Groups[g]];
    }
}