// CGroupList_004afdd0::ContainsMember(SMemberEntry key) -- true when any group of the pointer list at +0x38 lists the key in its member vector (+0x10 .. +0x14, SGI find @ 0x004B74E0 over 4-byte entries with the key wrapped in the entry type), false otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AFDD0, length 70 bytes.
// Callees: the SGI random-access __find @ 0x004B74E0 (`__fastcall`: first in ecx, last in edx, the value by const reference and the iterator-category tag by const reference on the stack; the tag is an empty temporary homed at the dead argument slot that also holds the converted value temporary, hence the self-copy `mov eax,[esp+10h] ; mov [esp+10h],eax` of the search value built once before the loop). The chunk label (CMessageEventManager::FindMostRecentMessage<IsOfType>) is propagated; the class is named by address. Shape: the sentinel in ebx, the node in esi, the group's end pointer in edi (read once into a local before the search), `xor al,al` / `mov al,1` tails, `ret 4`.
// Grade: DIFFER(ours v retail) -- the search value temporary: retail homes it in the dead `key` argument slot (`mov eax,[esp+10h] ; mov [esp+10h],eax`, shared with the empty tag), the lane gives it a fresh `push ecx` frame slot (plain long key, by-value key with a user copy constructor, a per-iteration temporary and a once-built local tried); the rest of the body is identical.
struct SMemberEntry_004afdd0 {                     // the 4-byte vector element: a key with a converting constructor (the search value is built from the plain key)
    long m_key;
    SMemberEntry_004afdd0(long key) : m_key(key) { }
    SMemberEntry_004afdd0(const SMemberEntry_004afdd0& other) : m_key(other.m_key) { }   // a user copy constructor: the by-value argument is copied into the local
};
struct random_access_iterator_tag_004afdd0 { random_access_iterator_tag_004afdd0() { } };   // a user constructor: no store for the empty temporary

SMemberEntry_004afdd0* __fastcall Find_004b74e0(SMemberEntry_004afdd0* first, SMemberEntry_004afdd0* last, const SMemberEntry_004afdd0& value, const random_access_iterator_tag_004afdd0& tag);   // 0x004B74E0

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
