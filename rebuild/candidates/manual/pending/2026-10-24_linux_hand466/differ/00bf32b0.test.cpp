// behaviour test for 0x00BF32B0 CEngineDynamic3DResourceManager::ReleaseResources: every leaf's pooled buffers deleted and their size taken off both totals
#include <cstdio>

static int g_deleted = 0;

class CVertexBufferWin32 {
public:
    unsigned long Size;
    CVertexBufferWin32(unsigned long size) : Size(size) { }
    virtual ~CVertexBufferWin32() { ++g_deleted; }
    unsigned long Sizeof() const { return Size; }        // stand-in for 0x00AC20D0
};

template <class T, long N> class CEngineDynamic3DResource : public T {
public:
    unsigned long             Key[N];
    CEngineDynamic3DResource* PoolNext;
    CEngineDynamic3DResource(unsigned long size, CEngineDynamic3DResource* next) : T(size), PoolNext(next) { }
    virtual ~CEngineDynamic3DResource() { }
};

class CEngineTreeFilerBase {
public:
    struct CNodeHeader {
        unsigned char IsLeaf;
        void*         Entry;
        CNodeHeader*  NextSibling;
        CNodeHeader*  ChildListHead;
        CNodeHeader*  Parent;
    };
    CNodeHeader* BaseNode;
    unsigned long FirstKey()
    {
        CNodeHeader* node = BaseNode;
        if (node == 0) return 0;
        return Walk(node);
    }
    unsigned long NextKey(unsigned long key)
    {
        CNodeHeader* node = (CNodeHeader*)key;
        if (node == 0) return 0;
        return Walk(node);
    }
    void* GetGenericObject(unsigned long key) { return ((CNodeHeader*)key)->Entry; }
    void SetGenericObject(unsigned long key, void* object) { ((CNodeHeader*)key)->Entry = object; }
private:
    static unsigned long Walk(CNodeHeader* node)
    {
        do {
            if (node->ChildListHead) node = node->ChildListHead;
            else if (node->NextSibling) node = node->NextSibling;
            else {
                node = node->Parent;
                if (node == 0) return 0;
                node = node->NextSibling;
                if (node == 0) return 0;
            }
        } while (!node->IsLeaf);
        return (unsigned long)node;
    }
};

template <class T> class CEngineTreeFiler : public CEngineTreeFilerBase {
public:
    T* GetEntry(unsigned long key) { return (T*)GetGenericObject(key); }
    void SetEntry(unsigned long key, T* entry) { SetGenericObject(key, entry); }
};

template <class TResource> class CEngineDynamic3DResourceManager {
public:
    struct CListHeader {
        TResource* Head;
        TResource* Tail;
    };
    long AllocCount;
    long FreePoolMem;
    long TotalMemAllocated;
    CEngineTreeFiler<CListHeader> ResourceMappingTree;
    void ReleaseResources()
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
};

typedef CEngineDynamic3DResource<CVertexBufferWin32, 3> TBuffer;
typedef CEngineDynamic3DResourceManager<TBuffer> TManager;

int main()
{
    // root -> {inner (not a leaf) -> {leafA, leafB}}; leafC is the inner node's sibling
    CEngineTreeFilerBase::CNodeHeader root, inner, leafA, leafB, leafC;
    root.IsLeaf = 0;  root.Entry = 0;  root.NextSibling = 0;      root.ChildListHead = &inner; root.Parent = 0;
    inner.IsLeaf = 0; inner.Entry = 0; inner.NextSibling = &leafC; inner.ChildListHead = &leafA; inner.Parent = &root;
    leafA.IsLeaf = 1; leafA.NextSibling = &leafB; leafA.ChildListHead = 0; leafA.Parent = &inner;
    leafB.IsLeaf = 1; leafB.NextSibling = 0;      leafB.ChildListHead = 0; leafB.Parent = &inner;
    leafC.IsLeaf = 1; leafC.NextSibling = 0;      leafC.ChildListHead = 0; leafC.Parent = &root;
    TManager manager;
    manager.AllocCount = 0;
    manager.FreePoolMem = 1000;
    manager.TotalMemAllocated = 2000;
    manager.ResourceMappingTree.BaseNode = &root;
    TManager::CListHeader* a = new TManager::CListHeader;
    a->Head = new TBuffer(10, new TBuffer(20, 0)); a->Tail = 0;
    leafA.Entry = a;
    leafB.Entry = 0;                                     // a leaf without a header is skipped
    TManager::CListHeader* c = new TManager::CListHeader;
    c->Head = new TBuffer(30, 0); c->Tail = 0;
    leafC.Entry = c;
    manager.ReleaseResources();
    if (g_deleted != 3 || manager.FreePoolMem != 940 || manager.TotalMemAllocated != 1940) { std::printf("00bf32b0_TEST FAIL\n"); return 1; }
    if (leafA.Entry != 0 || leafB.Entry != 0 || leafC.Entry != 0) { std::printf("00bf32b0_TEST FAIL\n"); return 1; }
    manager.ResourceMappingTree.BaseNode = 0;            // an empty tree does nothing
    manager.ReleaseResources();
    if (g_deleted != 3) { std::printf("00bf32b0_TEST FAIL\n"); return 1; }
    std::printf("00bf32b0_TEST PASS\n");
    return 0;
}
