#include <cstdio>
// Model of the novtable CTCBase-derived destructor @ 0x007B2720: release counted action, name, target pointer, base.
static int g_order[8], g_n, g_deleted;
struct CCountedObject { long m_refCount; void Delete() { ++g_deleted; } };
struct CCountedPointer { CCountedObject* m_pObject; ~CCountedPointer() { CCountedObject* o = m_pObject; if (o) { if (--o->m_refCount == 0) o->Delete(); m_pObject = 0; } g_order[g_n++] = 1; } };
struct CCharString { void* d; ~CCharString() { g_order[g_n++] = 2; } };
struct CBaseIntelligentPointer { void* p; ~CBaseIntelligentPointer() { g_order[g_n++] = 3; } };
struct CTCBase { ~CTCBase() { g_order[g_n++] = 4; } };
struct X : CTCBase { CBaseIntelligentPointer m_pTarget; CCharString m_name; CCountedPointer m_pAction; ~X() {} };
int main()
{
    CCountedObject obj = { 1 };
    { X x; x.m_pAction.m_pObject = &obj; }
    if (g_n != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 4 || g_deleted != 1 || obj.m_refCount != 0) { std::printf("007b2720_TEST FAIL\n"); return 1; }
    g_n = 0; obj.m_refCount = 2; { X x; x.m_pAction.m_pObject = &obj; }
    if (g_deleted != 1 || obj.m_refCount != 1) { std::printf("007b2720_TEST FAIL\n"); return 1; }
    std::printf("007b2720_TEST PASS\n"); return 0;
}
