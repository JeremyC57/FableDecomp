// C3DMesh2::GetUsedBones(CBoolArray&) -- mark root, primitive, helper-point and dummy-object bones.
// Retail 0x00A8B5F0, 194 bytes. Primitive GetUsedBones 0x00A8FCF0.
// PDB helper/dummy names and record sizes; retail base C3DMeshStats is 0x7c, so PrimitiveCount/Primitives are +0x90/+0x94.
// No shared headers exist for these classes. Array values are byte bools, not packed vector<bool> bits.
class C3DMeshStats;
class CMovableResourceMemoryPool { public:
    bool Allocate(long*, C3DMeshStats*, unsigned long, unsigned long);
    char* GetBuffer(long);
};
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
    void SetBuffer(T* p, unsigned long count) { Elements = p; Size = count; }
};
struct CBoolArray {
    unsigned char* First;
    unsigned char* Last;
    unsigned char* End;
    unsigned long size() const { return (unsigned long)(Last - First); }
    unsigned char& operator[](unsigned long n) { return First[n]; }
};
class C3DHelperPoint { public:
 float Position[3]; long BoneIndex;
 void GetUsedBones(CBoolArray& used) const { if (BoneIndex != -1) used[BoneIndex] = true; }
};
struct C3DHelperPointEntry { unsigned long Key; C3DHelperPoint Value; };
class C3DDummyObject { public:
 float Transform[12]; long BoneIndex;
 void GetUsedBones(CBoolArray& used) const { if (BoneIndex != -1) used[BoneIndex] = true; }
};
struct C3DDummyObjectEntry { unsigned long Key; C3DDummyObject Value; };
class C3DMeshStats { public:
    void* __vftable;
    CMovableResourceMemoryPool* MemoryPool;
    long StatsMemoryHandle;
    CCharString Name;
    C3DBoundingSphere BoundingSphere;
    C3DBoundingBox BoundingBox;
    CPreAllocVectorMap<C3DHelperPointEntry> HelperPoints;
    CPreAllocVectorMap<C3DDummyObjectEntry> DummyObjects;
    unsigned char MeshVolumes[16];
    void* MeshGenerators[3];
    char* PackedNames;
    unsigned short PackedNamesSize;
    bool AnimatedFlag;
    bool AllocateBuffers(unsigned long helpers, unsigned long dummies, unsigned long names);
};

class C3DPrimitive2 { public:
    unsigned char _retailPrimitive[0xa0]; // PDB 0xac minus 3 retail vector shrinkages
    void GetUsedBones(CBoolArray&); // 0x00A8FCF0
};
class C3DMesh2 : public C3DMeshStats { public:
    long MemoryHandle;
    long PrimMemoryHandle;
    long BoneNameMemoryHandle;
    unsigned long MaterialCount;
    void* Materials;
    unsigned long PrimitiveCount;
    C3DPrimitive2* Primitives;
    void GetUsedBones(CBoolArray& used);
};
void C3DMesh2::GetUsedBones(CBoolArray& used)
{
    if (used.size()) used[0] = true;
    for (unsigned long n = 0; n < PrimitiveCount; ++n) Primitives[n].GetUsedBones(used);
    for (CPreAllocVectorMap<C3DHelperPointEntry>::iterator p = HelperPoints.begin(); p != HelperPoints.end(); ++p)
        p->Value.GetUsedBones(used);
    for (CPreAllocVectorMap<C3DDummyObjectEntry>::iterator p = DummyObjects.begin(); p != DummyObjects.end(); ++p)
        p->Value.GetUsedBones(used);
}