// Behaviour test for 00a26c60: src/00a26c60.cpp copied verbatim (including its real map find body); the resource list
// calls and BuildAnimationInstance are stubbed and recorded; a small hand-built tree drives the find.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
class CResourceList;
class CResource { public:
    virtual void v00();
    unsigned long IVRefCount;                  // +4
    CResourceList* ResourceList;           // +8
    CResource* PrevResource;               // +0xc
    CResource* NextResource;               // +0x10
    unsigned long ResourceSize;            // +0x14
    unsigned long LastUsedFrame;           // +0x18
    void ListRemove();                     // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);   // 0x009FBF90
};
class CResourceList { public:
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
class CCountedPointer { public:
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
class C3DAnimation2;
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
class C3DMesh2 { public:
    unsigned char _pad[0xe8];
    unsigned long MeshID;                  // +0xe8 the instance-map key
};
class C3DAnimationInfo { public:                  // engine/C3DAnimationInfo.h offsets
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

// ---- stubs ----
void CResource::v00() { }
void C3DAnimationInfo::v00() { }
static int gRemove = 0, gInsert = 0; static CResource* gRemoveThis = 0; static CResource* gInsPrev = 0; static CResource* gInsNext = 0;
void CResource::ListRemove() { ++gRemove; gRemoveThis = this; }
void CResource::ListInsert(CResource* prev, CResource* next) { ++gInsert; gInsPrev = prev; gInsNext = next; }
static int gBuild = 0; static const C3DMesh2* gBuildMesh = 0;
static SCountBlock gBuiltBlock; static struct { C3DAnimation2* Obj; SCountBlock* Count; } gBuiltHolder;
static int gDestroyed = 0;
static void __fastcall DestroyAnim(void*) { ++gDestroyed; }
CCountedPointer<C3DAnimation2> C3DAnimationInfo::BuildAnimationInstance(const C3DMesh2& mesh) const
{
    ++gBuild; gBuildMesh = &mesh;
    return *(CCountedPointer<C3DAnimation2>*)&gBuiltHolder;     // copy: +1 reference for the returned temporary
}
static void Reset() { gRemove = gInsert = 0; gRemoveThis = gInsPrev = gInsNext = 0; gBuild = 0; gBuildMesh = 0; }

int main()
{
    // the tree: header <-> root(20) with left(10) and right(30)
    static long store[4][sizeof(CAnimationNode) / sizeof(long) + 1];   // nodes have no default ctor: raw zeroed storage
    memset(store, 0, sizeof store);
    CAnimationNode& header = *(CAnimationNode*)store[0];
    CAnimationNode& n10 = *(CAnimationNode*)store[1];
    CAnimationNode& n20 = *(CAnimationNode*)store[2];
    CAnimationNode& n30 = *(CAnimationNode*)store[3];
    header._M_parent = &n20;
    n20.first = 20; n20._M_left = &n10; n20._M_right = &n30;
    n10.first = 10; n30.first = 30;
    SCountBlock b10 = { 1, DestroyAnim, 0 }, b20 = { 1, DestroyAnim, 0 }, b30 = { 1, DestroyAnim, 0 };
    n10.second.m_pObject = (C3DAnimation2*)0x1010; n10.second.m_pCount = &b10;
    n20.second.m_pObject = (C3DAnimation2*)0x2020; n20.second.m_pCount = &b20;
    n30.second.m_pObject = (C3DAnimation2*)0x3030; n30.second.m_pCount = &b30;
    gBuiltBlock.m_references = 1; gBuiltBlock.m_pfnDestroy = DestroyAnim; gBuiltBlock.m_pObject = 0;
    gBuiltHolder.Obj = (C3DAnimation2*)0x9999; gBuiltHolder.Count = &gBuiltBlock;

    CResourceList list; memset(&list, 0, sizeof list); list.CurrentFrame = 5;
    list.Head.PrevResource = (CResource*)0x4444;
    CAnimationData data; memset((char*)&data + 4, 0, sizeof data - 4);
    data.Animations.Header = &header;
    C3DAnimationInfo info; info.Data = &data;
    C3DMesh2 mesh; memset(&mesh, 0, sizeof mesh);

    // 1: cached (key 30), resource touched this frame already -> no list moves, the cached pointer add-ref'd
    Reset(); data.ResourceList = &list; data.LastUsedFrame = 5; mesh.MeshID = 30;
    {
        CCountedPointer<const C3DAnimation2> r = info.GetAnimation(mesh);
        CHECK(r.m_pObject == (const C3DAnimation2*)0x3030 && r.m_pCount == &b30 && b30.m_references == 2);
        CHECK(gRemove == 0 && gInsert == 0 && gBuild == 0);
    }
    CHECK(b30.m_references == 1);
    // 2: cached (key 10 then 20), stale frame -> moved to the list tail and restamped
    Reset(); data.LastUsedFrame = 3; mesh.MeshID = 10;
    {
        CCountedPointer<const C3DAnimation2> r = info.GetAnimation(mesh);
        CHECK(r.m_pObject == (const C3DAnimation2*)0x1010 && b10.m_references == 2);
        CHECK(gRemove == 1 && gRemoveThis == &data && gInsert == 1 && gInsPrev == (CResource*)0x4444 && gInsNext == &list.Head);
        CHECK(data.LastUsedFrame == 5 && gBuild == 0);
    }
    Reset(); mesh.MeshID = 20;
    { CCountedPointer<const C3DAnimation2> r = info.GetAnimation(mesh); CHECK(r.m_pObject == (const C3DAnimation2*)0x2020 && gBuild == 0); }
    // 3: key missing (between nodes, above all, below all) -> built; the build temporary released after the copy
    long keys[3] = { 15, 99, 1 };
    for (int k = 0; k < 3; ++k) {
        Reset(); data.ResourceList = 0; mesh.MeshID = keys[k];
        {
            CCountedPointer<const C3DAnimation2> r = info.GetAnimation(mesh);
            CHECK(r.m_pObject == (const C3DAnimation2*)0x9999 && gBuiltBlock.m_references == 2);
            CHECK(gBuild == 1 && gBuildMesh == &mesh && gRemove == 0);
        }
        CHECK(gBuiltBlock.m_references == 1);
    }
    // 4: empty map -> built
    Reset(); header._M_parent = 0; mesh.MeshID = 20;
    { CCountedPointer<const C3DAnimation2> r = info.GetAnimation(mesh); CHECK(gBuild == 1 && r.m_pObject == (const C3DAnimation2*)0x9999); }
    // 5: no data -> built straight away
    Reset(); info.Data = 0;
    { CCountedPointer<const C3DAnimation2> r = info.GetAnimation(mesh); CHECK(gBuild == 1 && gRemove == 0); }
    // 6: last reference released destroys through the count block
    {
        gDestroyed = 0; SCountBlock* blk = new SCountBlock; blk->m_references = 1; blk->m_pfnDestroy = DestroyAnim; blk->m_pObject = 0;
        { CCountedPointer<const C3DAnimation2>* p = (CCountedPointer<const C3DAnimation2>*)new char[sizeof(CCountedPointer<const C3DAnimation2>)];
          p->m_pObject = 0; p->m_pCount = blk; p->~CCountedPointer(); delete[] (char*)p; }
        CHECK(gDestroyed == 1);
    }
    if (fails == 0) printf("00a26c60_TEST PASS\n");
    return fails;
}