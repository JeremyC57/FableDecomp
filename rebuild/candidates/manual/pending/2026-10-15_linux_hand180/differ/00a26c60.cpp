// Grade: DIFFER(224v224) -- scheduling only: retail stores the mesh key into the dead mesh slot before re-reading Data and forms &Animations with lea esi,[eax+1Ch]; ours loads Data into esi, add esi,1Ch, then stores the key. tried: map reference local, inline GetAnimations() accessor, a by-value __forceinline Find(key) helper
// C3DAnimationInfo::GetAnimation(const C3DMesh2&) const -- touch the animation data, return the cached instance for the mesh, else build one
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
    unsigned char _M_links[0x10];          // color, parent, left, right
    unsigned long first;                   // +0x10
    CCountedPointer<C3DAnimation2> second; // +0x14
};
struct CAnimationMap {                     // map<unsigned long, CCountedPointer<C3DAnimation2> > (12 bytes in retail)
    CAnimationNode* Header;                // +0
    unsigned char _rest[8];
    struct iterator { CAnimationNode* Node; };
    iterator find(const unsigned long& key);   // 0x00A2ADC0
};
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
        CAnimationMap& animations = Data->Animations;
        CAnimationMap::iterator it = animations.find(key);
        if (it.Node != animations.Header)
            return it.Node->second;
    }
    return BuildAnimationInstance(mesh);
}
