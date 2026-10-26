// Behaviour test for 00a5c570: body copied from src/00a5c570.cpp (CSimpleMemoryPool::CSimpleMemoryPool), the base constructor stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_baseCalls = 0;
struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable() {}
    CBaseClassNonCopyable() { ++g_baseCalls; }
};
struct CMemoryBlock;
struct CArea {
    unsigned long Size;
    CArea*        NextFreeArea;
    long          Status;
};
class CSimpleMemoryPool : public CBaseClassNonCopyable { public:
    CMemoryBlock* FirstMemoryBlock;
    unsigned long Capacity;
    unsigned long GrowthGranularity;
    CArea         Root;
    CSimpleMemoryPool(unsigned long capacity, unsigned long growthGranularity);
    virtual ~CSimpleMemoryPool() {}
    void SetCapacity(unsigned long n) { Capacity = ((n + 0xf) / 16) * 16; }
};
CSimpleMemoryPool::CSimpleMemoryPool(unsigned long capacity, unsigned long growthGranularity)
    : GrowthGranularity(((growthGranularity + 0xf) / 16) * 16)
{
    Root.Size = 0;
    Root.NextFreeArea = 0;
    FirstMemoryBlock = 0;
    Capacity = 0x400;
    if (capacity != 0)
        SetCapacity(capacity);
}
int main()
{
    unsigned char raw[sizeof(CSimpleMemoryPool)]; memset(raw, 0xAA, sizeof(raw));
    // 1. zero capacity keeps the default 0x400; granularity rounded up to 16
    CSimpleMemoryPool* p = new (raw) CSimpleMemoryPool(0, 17);
    CHECK(g_baseCalls == 1); CHECK(p->FirstMemoryBlock == 0); CHECK(p->Capacity == 0x400); CHECK(p->GrowthGranularity == 32);
    CHECK(p->Root.Size == 0); CHECK(p->Root.NextFreeArea == 0);
    // 2. a capacity is rounded up to 16
    CSimpleMemoryPool q(100, 16);
    CHECK(q.Capacity == 112); CHECK(q.GrowthGranularity == 16);
    CSimpleMemoryPool r(16, 0);
    CHECK(r.Capacity == 16); CHECK(r.GrowthGranularity == 0);
    if (fails == 0) printf("00a5c570_TEST PASS\n"); else printf("00a5c570_TEST FAIL\n");
    return fails;
}