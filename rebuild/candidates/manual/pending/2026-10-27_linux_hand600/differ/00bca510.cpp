// CEngineVertexBufferPool::GetBuffer(EVertexType type, long count, long stride, unsigned long usage, EVertexBufferPool pool) -- hand out a pooled dynamic vertex buffer: round the vertex count up to the pool granularity (at most 0x10000), build the three-word key, reuse the first idle buffer filed under that key (or file a new empty list for it), else build a new buffer and account for it
// Grade: DIFFER(405v398) -- register allocation: VC7.1 keeps the rounded count in edi across the inlined GetResource (this in ebp), where retail homes it back in its argument slot, keeps this in ebx and spends ebp on a zero register (`xor ebp,ebp`: the null tests, the CListHeader zero stores, PoolNext = 0); the shapes otherwise line up (inline GetKey / AddKey-FindID-FindNode / GetEntry-SetEntry / idle-buffer pop / AddNewResource). Tried: plain inline (169v398, GetResource out of line), the ternary GetGenericObject (411v398), a by-reference rounding helper (no change).
// Leaf: ?GetBuffer@CEngineVertexBufferPool@@QAEPAV?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@W4EVertexType@@JJKW4EVertexBufferPool@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCA510, 398 bytes, thiscall `ret 14h`.
// Identity (two sources): (1) FableWin.pdb ?GetBuffer@CEngineVertexBufferPool@@QAEPAV?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@W4EVertexType@@JJKW4EVertexBufferPool@@@Z
//   (five arguments = `ret 14h`); (2) the body is that member over the Ego_r.pdb layout: SizeGranularity +0x18, the
//   CEngineDynamic3DResourceManager base (FreePoolMem +0x08, TotalMemAllocated +0x0C, ResourceMappingTree +0x10), the
//   resource's Key[3] +0x34 / PoolNext +0x40, and it calls CEngineVertexBufferPool::BuildNewBuffer 0x00BF3380 with
//   (key, type, count, stride, usage, pool) exactly as FableWin.pdb's BuildNewBuffer signature. The manifest label
//   "CEngineVertexBufferPool_AllocateBuffer" is descriptive.
// Callees: CEngineTreeFilerBase::FindNode 0x00BC9B30 (bound) and AddNewKey 0x00BF3030, CVertexBufferBase::IsBusy
//   0x00AC20C0 and Sizeof 0x00AC20D0 (FableWin.pdb spellings; the landed bindings are wrong: NAME_FIXES.tsv), operator new
//   0x00BFEA1A, CEngineVertexBufferPool::BuildNewBuffer 0x00BF3380 (FableWin.pdb spelling; the landed leaf uses int
//   parameters: NAME_FIXES.tsv).
// Shape: GetKey, the manager's GetResource (all `__forceinline`: plain inline leaves GetResource out of line, 169v398) (AddKey = FindID with FindNode's first level, then AddNewKey; GetEntry /
//   SetEntry; the idle-buffer pop) and AddNewResource inline, as the landed FindID 0x00BC9B80 for the tree descent.
// Layout: local models (the engine headers are opaque: CVertexBufferWin32.h is a flat 0x34; no header for the pool, the
//   manager, the tree filer or the resource).
enum EVertexType { VERTEX_TYPE_FORCE_DWORD = 0x7fffffff };
enum EVertexBufferPool { VERTEX_BUFFER_POOL_FORCE_DWORD = 0x7fffffff };

class CVertexBufferBase {                          // Ego_r.pdb, 0x34 (polymorphic)
public:
    virtual ~CVertexBufferBase();
    void*         PVertexBuffer;                   // +0x04
    unsigned long Length;                          // +0x08
    EVertexType   Type;                            // +0x0c
    unsigned char Stride;                          // +0x10
    unsigned char _pad11[0x34 - 0x11];             // +0x11 .. CompressionParams +0x14
    bool IsBusy() const;                           // 0x00AC20C0 (DoIsBusy, slot 3)
    unsigned long Sizeof() const;                  // 0x00AC20D0 (DoSizeof, slot 4)
};
class CVertexBufferWin32 : public CVertexBufferBase {
};
template <class T, int N> class CEngineDynamic3DResource : public T {
public:
    unsigned long             Key[N];              // +0x34
    CEngineDynamic3DResource* PoolNext;            // +0x40
};

class CEngineTreeFilerBase {
public:
    struct CNodeHeader {                           // Ego_r.pdb, 28 bytes
        unsigned char IsLeaf;                      // +0x00
        unsigned char KeyOffset;                   // +0x01
        unsigned char KeyBitStart;                 // +0x02
        unsigned char KeyBitCount;                 // +0x03
        void*         Entry;                       // +0x04
        CNodeHeader*  PrevSibling;                 // +0x08
        CNodeHeader*  NextSibling;                 // +0x0c
        CNodeHeader*  ChildListHead;               // +0x10
        CNodeHeader*  ChildListTail;               // +0x14
        CNodeHeader*  Parent;                      // +0x18
    };
    CNodeHeader* BaseNode;                         // +0x00
    long         KeySize;                          // +0x04
    __forceinline unsigned long FindID(unsigned long* key)
    {
        CNodeHeader* node = FindNode(BaseNode, key);
        if (!node)
            return 0;
        unsigned long* nodeKey = (unsigned long*)(node + 1);
        for (long i = 0; i < KeySize; ++i)
            if (key[i] != nodeKey[i])
                return 0;
        return (unsigned long)node;
    }
    __forceinline unsigned long AddKey(unsigned long* key)
    {
        unsigned long id = FindID(key);
        if (id)
            return id;
        return AddNewKey(key);
    }
protected:
    void* GetGenericObject(unsigned long id)
    {
        void* object = 0;
        if (id)
            object = ((CNodeHeader*)id)->Entry;
        return object;
    }
    void SetGenericObject(unsigned long id, void* object)
    {
        if (id)
            ((CNodeHeader*)id)->Entry = object;
    }
private:
    CNodeHeader* FindNode(CNodeHeader* node, unsigned long* key);   // 0x00BC9B30
    unsigned long AddNewKey(unsigned long* key);                    // 0x00BF3030
};
template <class T> class CEngineTreeFiler : public CEngineTreeFilerBase {
public:
    T* GetEntry(unsigned long id) { return (T*)GetGenericObject(id); }
    void SetEntry(unsigned long id, T* entry) { SetGenericObject(id, entry); }
};

#pragma inline_recursion(on)
#pragma inline_depth(4)
__forceinline CEngineTreeFilerBase::CNodeHeader* CEngineTreeFilerBase::FindNode(CNodeHeader* node, unsigned long* key)
{
    if (node->IsLeaf)
        return node;
    CNodeHeader* child = ((CNodeHeader**)(node + 1))[(key[node->KeyOffset] >> node->KeyBitStart) & ((1 << node->KeyBitCount) - 1)];
    if (child)
        return FindNode(child, key);
    return 0;
}

template <class TResource> class CEngineDynamic3DResourceManager {
public:
    struct CListHeader {
        TResource* First;                          // +0x00
        TResource* Last;                           // +0x04
        CListHeader() : First(0), Last(0) { }
    };
    virtual long GetTotalBytesAllocated() const;   // vtable 0x012A69DC
    long AllocCount;                               // +0x04
    long FreePoolMem;                              // +0x08
    long TotalMemAllocated;                        // +0x0c
    CEngineTreeFiler<CListHeader> ResourceMappingTree;   // +0x10
protected:
    __forceinline TResource* GetResource(unsigned long* key)
    {
        unsigned long id = ResourceMappingTree.AddKey(key);
        CListHeader* header = ResourceMappingTree.GetEntry(id);
        if (!header) {
            header = new CListHeader;
            ResourceMappingTree.SetEntry(id, header);
        }
        if (header->First && !header->First->IsBusy()) {
            TResource* resource = header->First;
            header->First = resource->PoolNext;
            resource->PoolNext = 0;
            if (!header->First)
                header->Last = 0;
            FreePoolMem -= resource->Sizeof();
            return resource;
        }
        return 0;
    }
    __forceinline void AddNewResource(TResource* resource, unsigned long* key)
    {
        resource->Key[0] = key[0];
        resource->Key[1] = key[1];
        resource->Key[2] = key[2];
        TotalMemAllocated += resource->Sizeof();
    }
};

class CEngineVertexBufferPool : public CEngineDynamic3DResourceManager<CEngineDynamic3DResource<CVertexBufferWin32, 3> > {
public:
    long SizeGranularity;                          // +0x18
    __forceinline void GetKey(unsigned long* key, EVertexType type, long count, long stride, unsigned long usage, EVertexBufferPool pool)
    {
        key[0] = type;
        key[1] = (stride << 16) + count;
        key[2] = (pool << 16) + usage;
    }
    CEngineDynamic3DResource<CVertexBufferWin32, 3>* GetBuffer(EVertexType type, long count, long stride,
                                                              unsigned long usage, EVertexBufferPool pool);
    CEngineDynamic3DResource<CVertexBufferWin32, 3>* BuildNewBuffer(unsigned long* key, EVertexType type, long count,
                                                                   long stride, unsigned long usage,
                                                                   EVertexBufferPool pool);   // 0x00BF3380
};

CEngineDynamic3DResource<CVertexBufferWin32, 3>* CEngineVertexBufferPool::GetBuffer(EVertexType type, long count, long stride,
                                                                                   unsigned long usage, EVertexBufferPool pool)
{
    count = ((count - 1) / SizeGranularity + 1) * SizeGranularity;
    if (count >= 0x10000)
        count = 0x10000;
    unsigned long key[3];
    GetKey(key, type, count, stride, usage, pool);
    CEngineDynamic3DResource<CVertexBufferWin32, 3>* resource = GetResource(key);
    if (resource)
        return resource;
    resource = BuildNewBuffer(key, type, count, stride, usage, pool);
    if (resource)
        AddNewResource(resource, key);
    return resource;
}
