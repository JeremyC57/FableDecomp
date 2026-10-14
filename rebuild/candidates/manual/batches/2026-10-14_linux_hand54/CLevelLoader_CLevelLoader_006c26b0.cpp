// CLevelLoader::CLevelLoader(void* a, void* b, void* c) -- the loader base constructed with (0, 0xFA00, 0.1f) (0x00A44740), this class's vtable (0x0125D9B0), the list sentinel at +0x14 allocated (one 0x10-byte node linked to itself, malloc @ 0x00BFEA0E) and the three arguments stored at +0x18 / +0x1c / +0x20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C26B0, length 78 bytes.
// Callees: 0x00A44740 (the base constructor; the chunk label is propagated), malloc @ 0x00BFEA0E. Shape: `this` in esi, the sentinel linked to itself before it is stored, `ret 0Ch`.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CLevelLoaderBase_00a44740 {
    virtual void v00();
    unsigned char _body[0x10];
    CLevelLoaderBase_00a44740(long a, long size, float fraction);   // 0x00A44740
};

struct CRetailAllocator { CRetailAllocator() { } };

struct CRetailList_node0x10 {                      // VC7.1 Dinkumware list: one sentinel-node pointer
    struct node { node* next; node* prev; unsigned char value[0x10 - 8]; };
    node* head;
    CRetailList_node0x10(CRetailAllocator allocator) : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

struct CLevelLoader : CLevelLoaderBase_00a44740 {   // vtable 0x0125D9B0
    CRetailList_node0x10 m_pending;                // +0x14
    void*                m_18;                     // +0x18
    void*                m_1c;                     // +0x1c
    void*                m_20;                     // +0x20
    CLevelLoader(void* a, void* b, void* c);
};

CLevelLoader::CLevelLoader(void* a, void* b, void* c)
    : CLevelLoaderBase_00a44740(0, 0xfa00, 0.1f), m_pending(CRetailAllocator()), m_18(a), m_1c(b), m_20(c)
{
}
