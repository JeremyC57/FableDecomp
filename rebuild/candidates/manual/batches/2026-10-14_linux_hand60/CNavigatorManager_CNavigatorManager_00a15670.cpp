// CNavigatorManager::CNavigatorManager(void* a, void* owner, IProvider* provider) -- the base constructed with 5 (0x00A373B0), this class's vtable (0x0129CA84), the owner at +0x1c, the capacity 800 at +0x20, the two sentinel lists at +0x24 (0x28-byte nodes) and +0x2c (0x10-byte nodes) built (malloc @ 0x00BFEA0E, each node linked to itself before it is stored), then the provider's virtual slot 2 answers the dword at +0x28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A15670, length 96 bytes.
// Callees: 0x00A373B0 (the base constructor; the manifest label is propagated), malloc @ 0x00BFEA0E. Vtable 0x0129CA84 names the class. Shape: `this` in esi, the two mallocs' stack cleanup merged into one `add esp,8`, `ret 0Ch`.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CNavigatorBase_00a373b0 {
    virtual void v00();
    unsigned char _body[0x18];
    CNavigatorBase_00a373b0(long mode);            // 0x00A373B0
};
struct IProvider_00a15670 {
    virtual void v000(); virtual void v001();
    virtual long GetValue();                       // slot 2 (+0x08)
};
struct CRetailAllocator { CRetailAllocator() { } };
template<unsigned int N> struct CRetailList {      // VC7.1 Dinkumware list: one sentinel-node pointer
    struct node { node* next; node* prev; unsigned char value[N - 8]; };
    node* head;
    CRetailList(CRetailAllocator allocator) : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

struct CNavigatorManager : CNavigatorBase_00a373b0 {   // vtable 0x0129CA84
    void*                 m_pOwner;                // +0x1c
    long                  m_capacity;              // +0x20
    CRetailList<0x28>     m_maps;                  // +0x24
    long                  m_28;                    // +0x28
    CRetailList<0x10>     m_navigators;            // +0x2c
    CNavigatorManager(void* a, void* owner, IProvider_00a15670* provider);
};

CNavigatorManager::CNavigatorManager(void* a, void* owner, IProvider_00a15670* provider)
    : CNavigatorBase_00a373b0(5), m_pOwner(owner), m_capacity(800), m_maps(CRetailAllocator()), m_navigators(CRetailAllocator())
{
    m_28 = provider->GetValue();
}
