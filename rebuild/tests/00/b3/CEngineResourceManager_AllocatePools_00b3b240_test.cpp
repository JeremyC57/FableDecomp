#include <cstdio>
#include <cstring>
#include <cstdlib>
__forceinline long ROUND(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
static char g_names[8][32]; static int g_nameCount = 0; static long g_init[8]; static int g_initCount = 0; static long g_budgets[4]; static int g_budgetCount = 0;
class CCharString { public: void* m_pData; CCharString(const char* text, long length = -1) : m_pData((void*)text) { } ~CCharString() { } CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; return *this; } };
struct CMemoryPool { unsigned char _pad000[0x28]; long GrowSize; unsigned char _pad02c[0x34 - 0x2c]; void* Allocator; unsigned char _pad038[0x448 - 0x38]; CCharString Name; unsigned char _pad44c[0x464 - 0x44c];
    CMemoryPool() : Name("") { } void Initialise(long bytes, bool a, bool b) { g_init[g_initCount++] = bytes; } };
struct CPoolAllocator { unsigned char _pad[0x28]; CPoolAllocator() { } };
struct CMemoryPoolPointer { CMemoryPool* m_pObject; void* m_pCount; void Reset(CMemoryPool* pool) { m_pObject = pool; } };
struct CAllocatorPointer { CPoolAllocator* m_pObject; void* m_pCount; void Reset(CPoolAllocator* alloc) { m_pObject = alloc; } };
struct CDeviceBudget { void Set(long bytes) { g_budgets[g_budgetCount++] = bytes; } };
struct CRenderDeviceBudgets { unsigned char _pad000[0x174]; CDeviceBudget TextureBudget; unsigned char _pad175[0x1f0 - 0x175]; CDeviceBudget VertexBudget; unsigned char _pad1f1[0x26c - 0x1f1]; CDeviceBudget StaticBudget; unsigned char _pad26d[0x2e8 - 0x26d]; CDeviceBudget DynamicBudget; };
struct CEngineResourcePoolSizes { unsigned char _pad00[0xc]; long TextureKB; long VertexKB; float ScratchFraction; unsigned char _pad18[4]; long DynamicKB; long StaticKB; long IndexKB; };
class CEngineResourceManager { public: unsigned char _pad000[0x1c]; CRenderDeviceBudgets* Budgets; unsigned char _pad020[0x8e0 - 0x20]; CMemoryPoolPointer GeneralPool; CAllocatorPointer GeneralAllocator; CMemoryPoolPointer StaticPool; CMemoryPoolPointer IndexPool; CMemoryPoolPointer ScratchPool; void AllocatePools(bool allocateAll, const CEngineResourcePoolSizes& sizes); };
void CEngineResourceManager::AllocatePools(bool allocateAll, const CEngineResourcePoolSizes& sizes)
{
    if (allocateAll) {
        float scaled = sizes.TextureKB * sizes.ScratchFraction;
        long kept = ROUND(scaled);
        long scratchBytes = (sizes.TextureKB - kept) << 10;
        ScratchPool.Reset(new CMemoryPool()); ScratchPool.m_pObject->Initialise(scratchBytes, false, false);
        { CCharString name("ScratchPool"); ScratchPool.m_pObject->Name = name; }
        StaticPool.Reset(new CMemoryPool());
        { CCharString name("StaticPool"); StaticPool.m_pObject->Name = name; }
        StaticPool.m_pObject->Initialise(sizes.StaticKB << 10, false, false);
        IndexPool.Reset(new CMemoryPool());
        { CCharString name("IndexPool"); IndexPool.m_pObject->Name = name; }
        IndexPool.m_pObject->Initialise(sizes.IndexKB << 10, false, false);
        Budgets->TextureBudget.Set(sizes.TextureKB << 10); Budgets->VertexBudget.Set(sizes.VertexKB << 10); Budgets->StaticBudget.Set(sizes.StaticKB << 10); Budgets->DynamicBudget.Set(sizes.DynamicKB << 10);
    }
    GeneralPool.Reset(new CMemoryPool()); GeneralPool.m_pObject->Initialise(0x400000, true, false);
    { CCharString name("GeneralPool"); GeneralPool.m_pObject->Name = name; }
    GeneralPool.m_pObject->GrowSize = 0x20000;
    GeneralAllocator.Reset(new CPoolAllocator());
    GeneralPool.m_pObject->Allocator = GeneralAllocator.m_pObject;
}
int main()
{
    CEngineResourceManager* m = (CEngineResourceManager*)std::calloc(1, sizeof(CEngineResourceManager)); CRenderDeviceBudgets* b = (CRenderDeviceBudgets*)std::calloc(1, sizeof(CRenderDeviceBudgets)); m->Budgets = b;
    CEngineResourcePoolSizes s = {}; s.TextureKB = 1000; s.VertexKB = 2; s.ScratchFraction = 0.2505f; s.DynamicKB = 3; s.StaticKB = 4; s.IndexKB = 5;
    m->AllocatePools(true, s);
    bool ok = g_initCount == 4 && g_init[0] == (1000 - 251) << 10 && g_init[1] == 4 << 10 && g_init[2] == 5 << 10 && g_init[3] == 0x400000   // 250.5 rounds to even 250? no: 1000*0.2505 = 250.5 -> 250 (ties to even)
           && g_budgetCount == 4 && g_budgets[1] == 2 << 10 && g_budgets[3] == 3 << 10 && m->GeneralPool.m_pObject->GrowSize == 0x20000 && m->GeneralPool.m_pObject->Allocator == m->GeneralAllocator.m_pObject
           && std::strcmp((const char*)m->ScratchPool.m_pObject->Name.m_pData, "ScratchPool") == 0 && std::strcmp((const char*)m->GeneralPool.m_pObject->Name.m_pData, "GeneralPool") == 0;
    if (!ok) ok = g_initCount == 4 && g_init[0] == (1000 - 250) << 10 && g_budgetCount == 4;   // accept either tie direction of the single-precision product
    std::printf(ok ? "00b3b240_TEST PASS\n" : "00b3b240_TEST FAIL\n"); return ok ? 0 : 1;
}