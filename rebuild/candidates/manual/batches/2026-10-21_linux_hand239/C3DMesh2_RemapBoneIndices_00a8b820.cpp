// C3DMesh2::RemapBoneIndices(map<unsigned long, unsigned long>&) -- remap the bone indices of every primitive, bone parent, helper point and dummy object
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
struct CBoneMapNode {                               // STLport _Rb_tree_node<pair<const ulong, ulong> >
    long          _M_color;
    CBoneMapNode* _M_parent;
    CBoneMapNode* _M_left;
    CBoneMapNode* _M_right;
    unsigned long first;                            // +0x10
    unsigned long second;
};
struct CBoneMap {                                   // map<unsigned long, unsigned long>
    CBoneMapNode* _M_header;
    unsigned long _M_node_count;
    unsigned long& operator[](const unsigned long& key);   // 0x00553E60
    CBoneMapNode* end() const { return _M_header; }
    CBoneMapNode* find(const unsigned long& key) const
    {
        CBoneMapNode* y = _M_header;
        CBoneMapNode* x = _M_header->_M_parent;
        while (x != 0) {
            if (!(x->first < key)) {
                y = x;
                x = x->_M_left;
            }
            else
                x = x->_M_right;
        }
        if (y == end() || key < y->first)
            return end();
        return y;
    }
};

struct C3DMeshStats;
struct CCharString { void* PStringData; };
struct C3DBoundingSphere { float values[4]; };
struct C3DBoundingBox { float values[6]; };
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
struct C3DHelperPoint {
    float Position[3]; long BoneIndex;
    void RemapBoneIndices(CBoneMap& remap) { if (BoneIndex != -1) BoneIndex = remap[BoneIndex]; }
};
struct C3DHelperPointEntry { unsigned long Key; C3DHelperPoint Value; };
struct C3DDummyObject {
    float Transform[12]; long BoneIndex;
    void RemapBoneIndices(CBoneMap& remap) { if (BoneIndex != -1) BoneIndex = remap[BoneIndex]; }
};
struct C3DDummyObjectEntry { unsigned long Key; C3DDummyObject Value; };
struct C3DMeshStats {
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

struct C3DPrimitive2 {
    unsigned char _retailPrimitive[0xa0];
    void RemapBoneIndices(CBoneMap& remap);         // 0x00A90F00
};
struct C3DBone2 {                                   // 0x3c
    unsigned long NameCRC;
    long          Parent;                           // +0x04
    long          OriginalNoChildren;
    float         LocalizationMatrix[12];
};
struct C3DMesh2 : C3DMeshStats {
    long MemoryHandle;
    long PrimMemoryHandle;
    long BoneNameMemoryHandle;
    unsigned long MaterialCount;                    // +0x88
    void* Materials;                                // +0x8c
    unsigned long PrimitiveCount;                   // +0x90
    C3DPrimitive2* Primitives;                      // +0x94
    unsigned long BoneCount;                        // +0x98
    C3DBone2* Bones;                                // +0x9c
    void RemapBoneIndices(CBoneMap& remap);
};

void C3DMesh2::RemapBoneIndices(CBoneMap& remap)
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
