#include <stdio.h>
static int g_destroyed, g_ipDtors, g_ipCtors, g_blocksFreed;
class CCharString { public: void* m_pStringData; ~CCharString() { ++g_ipDtors; } };
struct CPointee { long m_tag; };
static void __fastcall DestroyPointee(CPointee* p) { ++g_destroyed; p->m_tag = -1; }
struct SCountBlock {
    long m_references; void (__fastcall* m_pfnDestroy)(CPointee*); CPointee* m_pObject;
    SCountBlock(CPointee* o, long refs) : m_references(refs), m_pfnDestroy(DestroyPointee), m_pObject(o) { }
    void operator delete(void* p) { ++g_blocksFreed; ::operator delete(p); }
};
struct CCountedPointer_CPointee {
    CPointee* m_pObject; SCountBlock* m_pCount;
    __forceinline ~CCountedPointer_CPointee()
    {
        if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } }
        m_pObject = 0; m_pCount = 0;
    }
};

struct CCountedRecord_00a16d70 {
    CCountedPointer_CPointee m_pItem; unsigned char _pad[0x14]; CCharString m_name;
    ~CCountedRecord_00a16d70();
};
CCountedRecord_00a16d70::~CCountedRecord_00a16d70() { }
int main()
{
    CPointee act; act.m_tag = 1;
    bool ok = sizeof(CCountedRecord_00a16d70) == 0x20;
    {   // last reference: the action is destroyed and the block freed
        CCountedRecord_00a16d70* o = new CCountedRecord_00a16d70; o->m_pItem.m_pObject = &act; o->m_pItem.m_pCount = new SCountBlock(&act, 1);
        o->~CCountedRecord_00a16d70(); ::operator delete(o);
        ok = ok && g_destroyed == 1 && act.m_tag == -1 && g_blocksFreed == 1 && g_ipDtors == 1;
    }
    {   // shared: only the count drops
        SCountBlock* blk = new SCountBlock(&act, 2);
        CCountedRecord_00a16d70* o = new CCountedRecord_00a16d70; o->m_pItem.m_pObject = &act; o->m_pItem.m_pCount = blk;
        o->~CCountedRecord_00a16d70(); ::operator delete(o);
        ok = ok && g_destroyed == 1 && blk->m_references == 1 && g_blocksFreed == 1 && g_ipDtors == 2;
        delete blk;
    }
    {   // null: nothing happens
        CCountedRecord_00a16d70* o = new CCountedRecord_00a16d70; o->m_pItem.m_pObject = 0; o->m_pItem.m_pCount = 0;
        o->~CCountedRecord_00a16d70(); ::operator delete(o);
        ok = ok && g_destroyed == 1 && g_blocksFreed == 2 && g_ipDtors == 3;
    }
    if (!ok) { printf("00a16d70_TEST FAIL\n"); return 1; }
    printf("00a16d70_TEST PASS\n"); return 0;
}