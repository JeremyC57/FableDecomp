// C3DAnimationInfo::GetAnimation(const C3DMesh2&) const -- touch the animation data, return the cached instance for the mesh, else build one
// Grade: RELOCATION_MATCH (retry helper C) -- the instance map's _Rb_tree::find 0x00A2ADC0 given its real same-TU noinline
// body behind an inline find() wrapper (the 00b83750 lever), then -- on that base -- Data->Animations read at the call and
// at the end() test instead of through a map reference local: retail's early key store into the dead mesh slot and
// `lea esi,[eax+1Ch]` (was DIFFER(224v224); either change alone stays DIFFER).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A26C60, length 224 bytes (`ret 8`: hidden result + mesh).
// Ego_r.pdb: ?GetAnimation@C3DAnimationInfo@@QBE?AV?$CCountedPointer@$$CBVC3DAnimation2@@@@ABVC3DMesh2@@@Z.
// Callees: CResource::ListRemove @ 0x009FBFB0, CResource::ListInsert @ 0x009FBF90, the instance map's find(const unsigned long&)
// @ 0x00A2ADC0 (by hidden pointer; catalog "RBTree_FindNearestLE_uint"), C3DAnimationInfo::BuildAnimationInstance(const C3DMesh2&)
// @ 0x00A26AD0 (by value), operator delete @ 0x00BFE9BC.
// Model: the landed GetAnimationStats 0x00A269C0 (Data +0xc; CAnimationData : CResource with the inline Touch(); the
// instance map at +0x1c, 12 bytes in retail). Shape: the mesh key (+0xe8) copied into a local (homed in the dead mesh
// slot) for find() on a map reference local (esi); `it.Node != Header` compared directly; both returns convert CCountedPointer<C3DAnimation2> to the
// const pointer (copy + add-ref), the built instance through a temporary released after the copy.
// Leaf: GetAnimation@C3DAnimationInfo
struct CResourceList;
struct CResource {
    virtual void v00();
    unsigned long m_pad4;                  // +4
    CResourceList* ResourceList;           // +8
    CResource* PrevResource;               // +0xc
    CResource* NextResource;               // +0x10
    unsigned long ResourceSize;            // +0x14
    unsigned long LastUsedFrame;           // +0x18
    void ListRemove();                     // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);   // 0x009FBF90
};
struct CResourceList {
    long m_pad0;                           // +0
    CResource Head;                        // +4
    unsigned long ResourceCount;           // +0x20
    long AllocatedMemory;                  // +0x24
    long MaximumMemory;                    // +0x28
    unsigned long CurrentFrame;            // +0x2c
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T>
struct CCountedPointer {
    T* m_pObject;                          // +0
    SCountBlock* m_pCount;                 // +4
    template<class U> CCountedPointer(const CCountedPointer<U>& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
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
};
struct C3DAnimation2;
struct CAnimationNode {                    // STLport _Rb_tree_node<pair<const unsigned long, CCountedPointer<C3DAnimation2> > >
    long            _M_color;              // +0x00
    CAnimationNode* _M_parent;             // +0x04
    CAnimationNode* _M_left;               // +0x08
    CAnimationNode* _M_right;              // +0x0c
    unsigned long first;                   // +0x10
    CCountedPointer<C3DAnimation2> second; // +0x14
};
struct CAnimationMap {                     // map<unsigned long, CCountedPointer<C3DAnimation2> > (12 bytes in retail)
    CAnimationNode* Header;                // +0
    unsigned char _rest[8];
    struct iterator { CAnimationNode* Node; iterator() {} iterator(CAnimationNode* n) : Node(n) {} };
    iterator TreeFind(const unsigned long& key);   // 0x00A2ADC0 (_Rb_tree::find)
    iterator find(const unsigned long& key) { return TreeFind(key); }
};
#pragma auto_inline(off)
// Same-TU body of the instance map's _Rb_tree::find 0x00A2ADC0 (the 0x006699C0 shape): lower-bound walk from the root,
// then end() unless the key matches.
__declspec(noinline) CAnimationMap::iterator CAnimationMap::TreeFind(const unsigned long& key)
{
    CAnimationNode* result = Header;
    CAnimationNode* n = Header->_M_parent;
    while (n) {
        if (!(n->first < key)) {
            result = n;
            n = n->_M_left;
        }
        else
            n = n->_M_right;
    }
    CAnimationNode* found = result;
    if (found == Header || key < found->first)
        found = Header;
    return iterator(found);
}
#pragma auto_inline(on)
struct CAnimationData : CResource {
    CAnimationMap Animations;              // +0x1c
    void Touch()
    {
        if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
            ListRemove();
            ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
            LastUsedFrame = ResourceList->CurrentFrame;
        }
    }
};
struct C3DMesh2 {
    unsigned char _pad[0xe8];
    unsigned long MeshID;                  // +0xe8 the instance-map key
};
struct C3DAnimationInfo {                  // engine/C3DAnimationInfo.h offsets
    virtual void v00();
    void* Bank;                            // +4
    unsigned long Index;                   // +8
    CAnimationData* Data;                  // +0xc
    CCountedPointer<C3DAnimation2> BuildAnimationInstance(const C3DMesh2& mesh) const;   // 0x00A26AD0
    CCountedPointer<const C3DAnimation2> GetAnimation(const C3DMesh2& mesh) const;
};

CCountedPointer<const C3DAnimation2> C3DAnimationInfo::GetAnimation(const C3DMesh2& mesh) const
{
    if (Data) {
        Data->Touch();
        unsigned long key = mesh.MeshID;
        CAnimationMap::iterator it = Data->Animations.find(key);
        if (it.Node != Data->Animations.Header)
            return it.Node->second;
    }
    return BuildAnimationInstance(mesh);
}