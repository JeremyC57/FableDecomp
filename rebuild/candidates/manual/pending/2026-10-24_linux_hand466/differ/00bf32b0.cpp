// CEngineDynamic3DResourceManager::ReleaseResources() -- walk every leaf of the resource mapping tree; for each list header delete its pooled vertex buffers (taking their size off the free-pool and allocated totals), free the header and clear the leaf's entry
// Grade: DIFFER(194v196) -- FirstKey as the forwarder `return NextKey((unsigned long)BaseNode);` (both __forceinline) reproduces retail's merged `xor eax,eax` null exit and the `test eax,eax ; mov <key>,eax ; je` before the esi/edi pushes (the parked own-walk FirstKey threads the exits); left: retail re-tests the key at the loop top (`test ebp,ebp ; je`, reached by the NextKey success `mov ebp,eax ; jmp`) where the lane threads the back edge straight into the body, and this/key sit in ebp/ebx vs retail ebx/ebp. Tried while / for(;;)+break loops, plain out-of-line FirstKey/NextKey (called, 147 bytes), QFE-4035
// Leaf: ?ReleaseResources@?$CEngineDynamic3DResourceManager@V?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF32B0, 196 bytes (+12 int3), __thiscall, `ret`.
// Identity: FableWin.pdb ?ReleaseResources@?$CEngineDynamic3DResourceManager@V?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@@@QAEXXZ
//   (the catalog name is right); the instantiation is the vertex-buffer pool's (landed ctor 0x00BCA0D0).
// Callees: CVertexBufferWin32::Sizeof() const @ 0x00AC20D0 (out of line, x2: a forward to vtable slot 4), the resource's
//   virtual deleting destructor (slot 0), operator delete @ 0x00BFE9BC.
// Same-TU inlines: CEngineTreeFilerBase::FirstKey() (retail 0x00BF3210) and NextKey(unsigned long) (retail 0x00BF31D0) --
//   both are out of line just above this body and expanded here; their bodies below are written from those copies
//   (FirstKey carries its own copy of the walk: written as `return NextKey((unsigned long)BaseNode)` the nested
//   NextKey stays a call; with plain or `inline` definitions VC7.1 expands only the NextKey in the loop and calls
//   FirstKey, so both are `__forceinline`).
//   GetEntry/SetEntry (CEngineTreeFiler<T>, PDB) are the inline GetGenericObject/SetGenericObject on the node's Entry.
// Layout (Ego_r.pdb): manager {vptr, AllocCount +4, FreePoolMem +8, TotalMemAllocated +0xC, CEngineTreeFiler
//   ResourceMappingTree +0x10 (BaseNode, KeySize)}; CNodeHeader {IsLeaf +0, KeyOffset/KeyBitStart/KeyBitCount +1..+3,
//   Entry +4, PrevSibling +8, NextSibling +0xC, ChildListHead +0x10, ChildListTail +0x14, Parent +0x18};
//   CListHeader {Head +0, Tail +4}; CEngineDynamic3DResource<CVertexBufferWin32,3> : CVertexBufferWin32 (0x34) {Key[3]
//   +0x34, PoolNext +0x40}.
// Shape: a `for (key = FirstKey(); key; key = NextKey(key))` walk; the inner list loop re-reads header->Head after each
//   call, and the next pointer is read before the virtual delete's null test.
class CVertexBufferWin32 {                               // vtable 0x0129BA20, 0x34 bytes
public:
    unsigned char _body[0x30];                           // +0x04 device buffer, format, sizes
    virtual ~CVertexBufferWin32();
    unsigned long Sizeof() const;                        // 0x00AC20D0
};

template <class T, long N> class CEngineDynamic3DResource : public T {
public:
    unsigned long             Key[N];                    // +0x34
    CEngineDynamic3DResource* PoolNext;                  // +0x40
    virtual ~CEngineDynamic3DResource() { }
};

class CEngineTreeFilerBase {
public:
    struct CNodeHeader {
        unsigned char IsLeaf;                            // +0x00
        unsigned char KeyOffset;                         // +0x01
        unsigned char KeyBitStart;                       // +0x02
        unsigned char KeyBitCount;                       // +0x03
        void*         Entry;                             // +0x04
        CNodeHeader*  PrevSibling;                       // +0x08
        CNodeHeader*  NextSibling;                       // +0x0c
        CNodeHeader*  ChildListHead;                     // +0x10
        CNodeHeader*  ChildListTail;                     // +0x14
        CNodeHeader*  Parent;                            // +0x18
    };
    CNodeHeader* BaseNode;                               // +0x00
    long         KeySize;                                // +0x04
    unsigned long FirstKey();                            // 0x00BF3210
    unsigned long NextKey(unsigned long key);            // 0x00BF31D0
protected:
    void* GetGenericObject(unsigned long key) { return ((CNodeHeader*)key)->Entry; }
    void SetGenericObject(unsigned long key, void* object) { ((CNodeHeader*)key)->Entry = object; }
};

__forceinline unsigned long CEngineTreeFilerBase::NextKey(unsigned long key)
{
    CNodeHeader* node = (CNodeHeader*)key;
    if (node == 0)
        return 0;
    do {
        if (node->ChildListHead)
            node = node->ChildListHead;
        else if (node->NextSibling)
            node = node->NextSibling;
        else {
            node = node->Parent;
            if (node == 0)
                return 0;
            node = node->NextSibling;
            if (node == 0)
                return 0;
        }
    } while (!node->IsLeaf);
    return (unsigned long)node;
}

__forceinline unsigned long CEngineTreeFilerBase::FirstKey()
{
    return NextKey((unsigned long)BaseNode);
}

template <class T> class CEngineTreeFiler : public CEngineTreeFilerBase {
public:
    T* GetEntry(unsigned long key) { return (T*)GetGenericObject(key); }
    void SetEntry(unsigned long key, T* entry) { SetGenericObject(key, entry); }
};

template <class TResource> class CEngineDynamic3DResourceManager {
public:
    struct CListHeader {
        TResource* Head;                                 // +0x00
        TResource* Tail;                                 // +0x04
    };
    long AllocCount;                                     // +0x04
    long FreePoolMem;                                    // +0x08
    long TotalMemAllocated;                              // +0x0c
    CEngineTreeFiler<CListHeader> ResourceMappingTree;   // +0x10
    virtual long GetTotalBytesAllocated() const;         // vtable 0x012A69DC
    virtual long GetTotalBytesFree() const;
    void ReleaseResources();
private:
    virtual void FreeMemory();
};

template <class TResource>
void CEngineDynamic3DResourceManager<TResource>::ReleaseResources()
{
    for (unsigned long key = ResourceMappingTree.FirstKey(); key != 0; key = ResourceMappingTree.NextKey(key)) {
        CListHeader* header = ResourceMappingTree.GetEntry(key);
        if (header != 0) {
            while (header->Head != 0) {
                FreePoolMem -= header->Head->Sizeof();
                TotalMemAllocated -= header->Head->Sizeof();
                TResource* next = header->Head->PoolNext;
                delete header->Head;
                header->Head = next;
            }
            delete header;
            ResourceMappingTree.SetEntry(key, 0);
        }
    }
}

template void CEngineDynamic3DResourceManager<CEngineDynamic3DResource<CVertexBufferWin32, 3> >::ReleaseResources();
