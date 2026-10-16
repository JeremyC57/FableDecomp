// Grade: DIFFER(183v194) -- retail pushes ebx at entry (ours delays it into the primitive loop: lane gap) and strength-reduces the map walks into a second induction pointer (&it->BoneIndex in edx, add edx,14h / 38h) where ours re-addresses [eax+10h]; register roles differ from there. next: walk the maps as key/value pairs (it->second.BoneIndex) or with a bone-index pointer local
// C3DMesh2::GetUsedBones(CArray<bool>&) const -- mark the root, every primitive's bones and the bones the helper points and dummy objects hang off
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8B5F0, length 208 bytes (`ret 4`).
// MESH LAYER: Ego_r.pdb ?GetUsedBones@C3DMesh2@@ABEXAAV?$CArray@_N@@@Z.
// Callees: C3DPrimitive2::GetUsedBones(CArray<bool>&) const @ 0x00A8FCF0 (0xA0-byte retail primitives).
// Retail offsets: the C3DMeshStats base's pre-allocated maps HelperPoints (data +0x3c, count +0x40; 0x14-byte
// C3DHelperPoint, bone index +0x10) and DummyObjects (data +0x4c, count +0x50; 0x38-byte C3DDummyObject, bone index +0x34),
// as in C3DMeshStats::AllocateBuffers 0x00A2E760; PrimitiveCount +0x90, Primitives +0x94 (PDB +0x98/+0x9c).
// Shape: STLport `*(begin() + n)` stores into the bool array; the map walks re-read begin()/end() (data + count) every pass.
// Leaf: GetUsedBones@C3DMesh2
template<class T> struct CArray {                   // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    T* begin() const { return m_begin; }
    unsigned long size() const { return (unsigned long)(m_end - m_begin); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
template<class T> struct CPreAllocArray {           // the pre-allocated vector map's storage words
    T* Data;
    unsigned long Count;
    const T* begin() const { return Data; }
    const T* end() const { return Data + Count; }
};
struct C3DHelperPoint { unsigned char _00[0x10]; long BoneIndex; };            // 0x14
struct C3DDummyObject { unsigned char _00[0x34]; long BoneIndex; };            // 0x38
struct C3DPrimitive2 {
    void GetUsedBones(CArray<bool>& used) const;    // 0x00A8FCF0
    unsigned char _body[0xa0];
};
struct C3DMesh2 {
    unsigned char _00[0x3c];
    CPreAllocArray<C3DHelperPoint> HelperPoints;    // +0x3c
    unsigned char _44[0x08];
    CPreAllocArray<C3DDummyObject> DummyObjects;    // +0x4c
    unsigned char _54[0x3c];
    unsigned long PrimitiveCount;                   // +0x90
    C3DPrimitive2* Primitives;                      // +0x94
    void GetUsedBones(CArray<bool>& used) const;
};

void C3DMesh2::GetUsedBones(CArray<bool>& used) const
{
    if (used.size() != 0)
        used[0] = true;
    for (unsigned long i = 0; i < PrimitiveCount; ++i)
        Primitives[i].GetUsedBones(used);
    for (const C3DHelperPoint* helper = HelperPoints.begin(); helper != HelperPoints.end(); ++helper)
        if (helper->BoneIndex != -1)
            used[helper->BoneIndex] = true;
    for (const C3DDummyObject* dummy = DummyObjects.begin(); dummy != DummyObjects.end(); ++dummy)
        if (dummy->BoneIndex != -1)
            used[dummy->BoneIndex] = true;
}
