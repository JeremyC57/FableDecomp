// Grade: DIFFER(582v600) -- retail rounds TextureKB*ScratchFraction with a bare dword fistp after a float store/reload through the dead argument slots (inline-asm helper); C++ (long) calls _ftol. Lane gap: inline-asm fistp.
// CEngineResourceManager::AllocatePools(bool allocateAll, const CEngineResourcePoolSizes& sizes) -- when asked, create and size the three named texture/vertex memory pools and the four device budgets from the KB sizes; always create the 4 MB general pool and its allocator
// Leaf: AllocatePools
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B240, length 608 bytes, `ret 8`.
// Callees: operator new 0x00BFEA1A, CMemoryPool ctor 0x00A62E20 / Initialise 0x00A02570, CCountedPointer<CMemoryPool>::Reset
//   0x00B3E4A0, CCharString(const char*, long) 0x0099EBF0 / operator= 0x0099EFB0 / dtor 0x0099EAE0, device budget setter 0x009FC520,
//   allocator ctor 0x009D5EB0 + Reset 0x00B3E510.
// Layout: local model.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
struct CMemoryPool {
    unsigned char _pad000[0x28];
    long          GrowSize;                    // +0x28
    unsigned char _pad02c[0x34 - 0x2c];
    void*         Allocator;                   // +0x34
    unsigned char _pad038[0x448 - 0x38];
    CCharString   Name;                        // +0x448
    unsigned char _pad44c[0x464 - 0x44c];
    CMemoryPool();                             // 0x00A62E20
    void Initialise(long bytes, bool a, bool b);   // 0x00A02570
};
struct CPoolAllocator { unsigned char _pad[0x28]; CPoolAllocator(); };   // 0x009D5EB0
struct CMemoryPoolPointer { CMemoryPool* m_pObject; void* m_pCount; void Reset(CMemoryPool* pool); };          // 0x00B3E4A0
struct CAllocatorPointer { CPoolAllocator* m_pObject; void* m_pCount; void Reset(CPoolAllocator* alloc); };     // 0x00B3E510
struct CDeviceBudget { void Set(long bytes); };   // 0x009FC520
struct CRenderDeviceBudgets {
    unsigned char _pad000[0x174];
    CDeviceBudget TextureBudget;               // +0x174
    unsigned char _pad175[0x1f0 - 0x175];
    CDeviceBudget VertexBudget;                // +0x1f0
    unsigned char _pad1f1[0x26c - 0x1f1];
    CDeviceBudget StaticBudget;                // +0x26c
    unsigned char _pad26d[0x2e8 - 0x26d];
    CDeviceBudget DynamicBudget;               // +0x2e8
};
struct CEngineResourcePoolSizes {
    unsigned char _pad00[0xc];
    long  TextureKB;                           // +0x0c
    long  VertexKB;                            // +0x10
    float ScratchFraction;                     // +0x14
    unsigned char _pad18[4];
    long  DynamicKB;                           // +0x1c
    long  StaticKB;                            // +0x20
    long  IndexKB;                             // +0x24
};
struct CEngineResourceManager {
    unsigned char         _pad000[0x1c];
    CRenderDeviceBudgets* Budgets;             // +0x1c
    unsigned char         _pad020[0x8e0 - 0x20];
    CMemoryPoolPointer    GeneralPool;         // +0x8e0
    CAllocatorPointer     GeneralAllocator;    // +0x8e8
    CMemoryPoolPointer    StaticPool;          // +0x8f0
    CMemoryPoolPointer    IndexPool;           // +0x8f8
    CMemoryPoolPointer    ScratchPool;         // +0x900
    void AllocatePools(bool allocateAll, const CEngineResourcePoolSizes& sizes);
};

void CEngineResourceManager::AllocatePools(bool allocateAll, const CEngineResourcePoolSizes& sizes)
{
    if (allocateAll) {
        float scaled = sizes.TextureKB * sizes.ScratchFraction;
        long kept = (long)scaled;
        long scratchBytes = (sizes.TextureKB - kept) << 10;
        ScratchPool.Reset(new CMemoryPool());
        ScratchPool.m_pObject->Initialise(scratchBytes, false, false);
        ScratchPool.m_pObject->Name = CCharString("ScratchPool");
        StaticPool.Reset(new CMemoryPool());
        StaticPool.m_pObject->Name = CCharString("StaticPool");
        StaticPool.m_pObject->Initialise(sizes.StaticKB << 10, false, false);
        IndexPool.Reset(new CMemoryPool());
        IndexPool.m_pObject->Name = CCharString("IndexPool");
        IndexPool.m_pObject->Initialise(sizes.IndexKB << 10, false, false);
        Budgets->TextureBudget.Set(sizes.TextureKB << 10);
        Budgets->VertexBudget.Set(sizes.VertexKB << 10);
        Budgets->StaticBudget.Set(sizes.StaticKB << 10);
        Budgets->DynamicBudget.Set(sizes.DynamicKB << 10);
    }
    GeneralPool.Reset(new CMemoryPool());
    GeneralPool.m_pObject->Initialise(0x400000, true, false);
    GeneralPool.m_pObject->Name = CCharString("GeneralPool");
    GeneralPool.m_pObject->GrowSize = 0x20000;
    GeneralAllocator.Reset(new CPoolAllocator());
    GeneralPool.m_pObject->Allocator = GeneralAllocator.m_pObject;
}
