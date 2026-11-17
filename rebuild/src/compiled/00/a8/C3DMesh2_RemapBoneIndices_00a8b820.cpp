// C3DMesh2::RemapBoneIndices(map<unsigned long, unsigned long>&) -- remap the bone indices of every primitive, bone parent, helper point and dummy object
// Leaf: ?RemapBoneIndices@C3DMesh2@@AAEXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8B820, length 355 bytes (`ret 4`).
// Callees: C3DPrimitive2::RemapBoneIndices @ 0x00A90F00 (landed), map<unsigned long, unsigned long>::operator[] @ 0x00553E60
//          (catalog "std::map::insert"; as in the landed 0x00A90F00).
// Shape: primitives by index (count re-read each pass); each bone's Parent is looked up with the inlined STLport
//          find (lower_bound walk `cmp [x+10h],key ; jb right`, then the `j == end || key < j->key` miss test) and
//          replaced by remap[Parent] or -1 (the long -> unsigned long key temporary sits in the second frame word,
//          the spilled loop counter in the first); helper points and dummy objects walk their CPreAllocVectorMap
//          with the end (Elements + Size) re-read after every operator[] call.
// Layout: retail C3DMeshStats base as in the landed C3DMesh2::GetUsedBones 0x00A8B5F0 (HelperPoints +0x38, DummyObjects
//          +0x48), then PrimitiveCount +0x90, Primitives +0x94 (0xa0 each), BoneCount +0x98, Bones +0x9c (C3DBone2,
//          0x3c: Parent +4) -- the PDB's C3DMesh2 offsets minus 8. No shared headers exist for these classes.
// NF70 (round 7, land1117fu q10797): the bone remap is the Ego_r.pdb std::map<unsigned long, unsigned long> (stlport/fable_map.h, its
//   find visible and its operator[] the out-of-line COMDAT 0x00553E60), replacing the CBoneMap / CBoneMapNode stand-ins; the method is
//   private as in Ego_r.pdb / FableWin.pdb (?RemapBoneIndices@C3DMesh2@@AAEXAAV?$map@KK...@Z, which the landed caller 0x00A8B990 binds).
#include "stlport/fable_map.h"
class C3DMeshStats;
class CCharString { public: void* PStringData; };
class C3DBoundingSphere { public: float values[4]; };
class C3DBoundingBox { public: float values[6]; };
template<class T> struct CPreAllocVectorMap {
    unsigned long Compare;
    T* Elements;
    unsigned long Size;
    bool Dirty;
    struct iterator {
        T* Pointer;
        iterator(T* p) : Pointer(p) {}
        T* operator->() const { return Pointer; }
        iterator& operator++() { ++Pointer; return *this; }
        bool operator!=(const iterator& p) const { return Pointer != p.Pointer; }
    };
    iterator begin() { return iterator(Elements); }
    iterator end() { return iterator(Elements + Size); }
};
class C3DHelperPoint { public:
    float Position[3]; long BoneIndex;
    void RemapBoneIndices(std::map<unsigned long, unsigned long>& remap) { if (BoneIndex != -1) BoneIndex = remap[BoneIndex]; }
};
struct C3DHelperPointEntry { unsigned long Key; C3DHelperPoint Value; };
class C3DDummyObject { public:
    float Transform[12]; long BoneIndex;
    void RemapBoneIndices(std::map<unsigned long, unsigned long>& remap) { if (BoneIndex != -1) BoneIndex = remap[BoneIndex]; }
};
struct C3DDummyObjectEntry { unsigned long Key; C3DDummyObject Value; };
class C3DMeshStats { public:
    void* __vftable;
    void* MemoryPool;
    long StatsMemoryHandle;
    CCharString Name;
    C3DBoundingSphere BoundingSphere;
    C3DBoundingBox BoundingBox;
    CPreAllocVectorMap<C3DHelperPointEntry> HelperPoints;   // +0x38
    CPreAllocVectorMap<C3DDummyObjectEntry> DummyObjects;   // +0x48
    unsigned char MeshVolumes[16];
    void* MeshGenerators[3];
    char* PackedNames;
    unsigned short PackedNamesSize;
    bool AnimatedFlag;
};

class C3DPrimitive2 { public:
    unsigned char _retailPrimitive[0xa0];
    void RemapBoneIndices(std::map<unsigned long, unsigned long>& remap);         // 0x00A90F00
};
class C3DBone2 { public:                                   // 0x3c
    unsigned long NameCRC;
    long          Parent;                           // +0x04
    long          OriginalNoChildren;
    float         LocalizationMatrix[12];
};
class C3DMesh2 : public C3DMeshStats { public:
    long MemoryHandle;
    long PrimMemoryHandle;
    long BoneNameMemoryHandle;
    unsigned long MaterialCount;                    // +0x88
    void* Materials;                                // +0x8c
    unsigned long PrimitiveCount;                   // +0x90
    C3DPrimitive2* Primitives;                      // +0x94
    unsigned long BoneCount;                        // +0x98
    C3DBone2* Bones;                                // +0x9c
private:
    void RemapBoneIndices(std::map<unsigned long, unsigned long>& remap);
};

void C3DMesh2::RemapBoneIndices(std::map<unsigned long, unsigned long>& remap)
{
    for (unsigned long n = 0; n < PrimitiveCount; ++n)
        Primitives[n].RemapBoneIndices(remap);
    for (unsigned long b = 0; b < BoneCount; ++b) {
        C3DBone2& bone = Bones[b];
        if (remap.find(bone.Parent) != remap.end())
            bone.Parent = remap[bone.Parent];
        else
            bone.Parent = -1;
    }
    for (CPreAllocVectorMap<C3DHelperPointEntry>::iterator p = HelperPoints.begin(); p != HelperPoints.end(); ++p)
        p->Value.RemapBoneIndices(remap);
    for (CPreAllocVectorMap<C3DDummyObjectEntry>::iterator d = DummyObjects.begin(); d != DummyObjects.end(); ++d)
        d->Value.RemapBoneIndices(remap);
}