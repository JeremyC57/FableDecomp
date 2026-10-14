#include <cstdio>
void* g_pDebugBuffer = (void*)0x5555; unsigned long g_nextInstanceId = 7;
struct SInstanceEntry { void* m_pOwner; unsigned long m_id; SInstanceEntry() : m_pOwner(g_pDebugBuffer), m_id(g_nextInstanceId++) { } };
struct SInstanceRecord { unsigned char m_active; SInstanceEntry m_entry; SInstanceRecord() : m_active(1) { } };
static SInstanceRecord* g_registered = 0;
struct CInstanceOwner { virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual void v08() { }
    virtual void Register(SInstanceRecord* r) { g_registered = r; }
    unsigned char _pad[0x2c]; unsigned char m_dirty;
    SInstanceEntry* CreateNewInstance() { SInstanceRecord* r = new SInstanceRecord; Register(r); m_dirty = 0; return &r->m_entry; } };
int main()
{
    CInstanceOwner o; o.m_dirty = 1;
    SInstanceEntry* e = o.CreateNewInstance();
    if (sizeof(SInstanceRecord) != 12 || g_registered == 0 || e != &g_registered->m_entry || g_registered->m_active != 1 || e->m_pOwner != (void*)0x5555 || e->m_id != 7 || g_nextInstanceId != 8 || o.m_dirty != 0) { std::printf("00c30840_TEST FAIL\n"); return 1; }
    delete g_registered;
    std::printf("00c30840_TEST PASS\n"); return 0;
}