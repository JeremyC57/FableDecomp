// Behaviour test for 0x00A3EC70 CSoundManagerLH::vfunc_6 (links the source object): true while the STLport list at +0x4c
// holds fewer entries than the limit at +0x18.
#include <cstdio>
#include <cstring>
struct SListNode_00a3ec70 { SListNode_00a3ec70* next; SListNode_00a3ec70* prev; };
struct CSTLportList_00a3ec70 { SListNode_00a3ec70* m_node; };
struct CSoundManagerLH {
    virtual void v00();
    unsigned char         _04[0x14];
    unsigned long         m_maxEntries;
    unsigned char         _1c[0x30];
    CSTLportList_00a3ec70 m_entries;
    virtual bool vfunc_6();
};
void CSoundManagerLH::v00() { }
int main()
{
    SListNode_00a3ec70 head, a, b;
    head.next = &head; head.prev = &head;
    CSoundManagerLH m; m.m_entries.m_node = &head; m.m_maxEntries = 2;
    bool ok = m.vfunc_6() == true;
    head.next = &a; a.next = &b; b.next = &head;
    ok = ok && m.vfunc_6() == false;
    m.m_maxEntries = 3;
    ok = ok && m.vfunc_6() == true;
    if (!ok) { std::printf("00a3ec70_TEST FAIL\n"); return 1; }
    std::printf("00a3ec70_TEST PASS\n");
    return 0;
}
