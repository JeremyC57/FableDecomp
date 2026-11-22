// Behaviour test for 004afdd0: src/004afdd0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CGroupList_004afdd0::ContainsMember(SMemberEntry key)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGroupList_004afdd0::ContainsMember(SMemberEntry key) -- true when any group of the pointer list at +0x38 lists the key in its member vector (+0x10 .. +0x14, SGI find @ 0x004B74E0 over 4-byte entries with the key wrapped in the entry type), false otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AFDD0, length 70 bytes.
// Callees: the SGI random-access __find @ 0x004B74E0 (`__fastcall`: first in ecx, last in edx, the value by const reference and the iterator-category tag by const reference on the stack; the tag is an empty temporary homed at the dead argument slot that also holds the converted value temporary, hence the self-copy `mov eax,[esp+10h] ; mov [esp+10h],eax` of the search value built once before the loop). The chunk label (CMessageEventManager::FindMostRecentMessage<IsOfType>) is propagated; the class is named by address. Shape: the sentinel in ebx, the node in esi, the group's end pointer in edi (read once into a local before the search), `xor al,al` / `mov al,1` tails, `ret 4`.
struct SMemberEntry_004afdd0 {                     // the 4-byte vector element: a key with a converting constructor (the search value is built from the plain key)
    long m_key;
    SMemberEntry_004afdd0(long key) : m_key(key) { }
    SMemberEntry_004afdd0(const SMemberEntry_004afdd0& other) : m_key(other.m_key) { }   // a user copy constructor: the by-value argument is copied into the local
};
struct random_access_iterator_tag_004afdd0 { random_access_iterator_tag_004afdd0() { } };   // a user constructor: no store for the empty temporary

SMemberEntry_004afdd0* __fastcall Find_004b74e0(SMemberEntry_004afdd0* first, SMemberEntry_004afdd0* last, const SMemberEntry_004afdd0& value, const random_access_iterator_tag_004afdd0& tag);   // 0x004B74E0
#pragma auto_inline(off)
// Same-TU body of the SGI random-access find instantiation 0x004B74E0 (template bodies are compiled in the caller's TU):
// compares each entry's key with the value read through its const reference; the empty iterator tag is never touched.
__declspec(noinline) SMemberEntry_004afdd0* __fastcall Find_004b74e0(SMemberEntry_004afdd0* first, SMemberEntry_004afdd0* last, const SMemberEntry_004afdd0& value, const random_access_iterator_tag_004afdd0& tag)
{
    for (; first != last; ++first)
        if (first->m_key == value.m_key)
            return first;
    return last;
}
#pragma auto_inline(on)

struct CMemberGroup_004afdd0 {
    unsigned char           _pad00[0x10];
    SMemberEntry_004afdd0*  m_pMembersBegin;       // +0x10
    SMemberEntry_004afdd0*  m_pMembersEnd;         // +0x14
};
struct SGroupNode {
    SGroupNode*            m_pNext;
    SGroupNode*            m_pPrev;
    CMemberGroup_004afdd0* m_value;                // +0x08
};
struct CGroupPtrList {                             // list<CMemberGroup*>: one sentinel pointer
    SGroupNode* m_pHead;
};

struct CGroupList_004afdd0 {
    unsigned char _pad00[0x38];
    CGroupPtrList m_groups;                        // +0x38
    bool ContainsMember(SMemberEntry_004afdd0 key);
};

bool CGroupList_004afdd0::ContainsMember(SMemberEntry_004afdd0 key)
{
    SGroupNode* head = m_groups.m_pHead;           // the sentinel read once (ebx)
    SGroupNode* node = head->m_pNext;
    if (node != head) {
        SMemberEntry_004afdd0 value(key);          // built once, homed in the dead `key` slot
        do {
            CMemberGroup_004afdd0* group = node->m_value;
            SMemberEntry_004afdd0* last = group->m_pMembersEnd;
            if (Find_004b74e0(group->m_pMembersBegin, last, value, random_access_iterator_tag_004afdd0()) != last)
                return true;
            node = node->m_pNext;
        } while (node != head);
    }
    return false;
}

// ---- stubs and driver ----
int main()
{
    SMemberEntry_004afdd0 m1[3] = { SMemberEntry_004afdd0(1), SMemberEntry_004afdd0(2), SMemberEntry_004afdd0(3) };
    SMemberEntry_004afdd0 m2[2] = { SMemberEntry_004afdd0(7), SMemberEntry_004afdd0(9) };
    static CMemberGroup_004afdd0 g1, g2; g1.m_pMembersBegin = m1; g1.m_pMembersEnd = m1 + 3; g2.m_pMembersBegin = m2; g2.m_pMembersEnd = m2 + 2;
    static SGroupNode head, n1, n2; head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &head; n1.m_value = &g1; n2.m_value = &g2;
    static CGroupList_004afdd0 list; list.m_groups.m_pHead = &head;
    CHECK(list.ContainsMember(SMemberEntry_004afdd0(2)) == true);
    CHECK(list.ContainsMember(SMemberEntry_004afdd0(9)) == true);
    CHECK(list.ContainsMember(SMemberEntry_004afdd0(5)) == false);
    g2.m_pMembersEnd = m2; CHECK(list.ContainsMember(SMemberEntry_004afdd0(9)) == false);
    head.m_pNext = &head; CHECK(list.ContainsMember(SMemberEntry_004afdd0(1)) == false);
    if (fails == 0) printf("004afdd0_TEST PASS\n"); else printf("004afdd0_TEST FAIL\n");
    return fails;
}