// CSoundManagerLH::vfunc_10() -- the room left: the limit at +0x18 minus the number of entries on the list at +0x4c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3EC90, length 30 bytes (32 with padding).
// Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 10; its sibling slot 6 (0x00A3EC70) tests
// `count < limit` from the same walk.
// Shape: the list is the 4-byte STLport one (+0x4c holds the sentinel node pointer); its size() is the SGI
// `__distance` walk (`first = first->next` then `++n`, the loop head aligned with `lea esp,[esp]`), the
// sentinel kept in edx and the count in esi; the limit is loaded into eax and the count subtracted.
struct SListNode_00a3ec90 {
    SListNode_00a3ec90* next;
    SListNode_00a3ec90* prev;
};

struct CSTLportList_00a3ec90 {                  // STLport list: the sentinel node pointer only
    SListNode_00a3ec90* m_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        SListNode_00a3ec90* first = m_node->next;
        SListNode_00a3ec90* last = m_node;
        while (first != last) {
            first = first->next;
            ++n;
        }
        return n;
    }
};

struct CSoundManagerLH {                        // vtable 0x0129D2C4
    virtual void v00();
    unsigned char         _04[0x14];            // +0x04..+0x17
    unsigned long         m_maxEntries;         // +0x18
    unsigned char         _1c[0x30];            // +0x1c..+0x4b
    CSTLportList_00a3ec90 m_entries;            // +0x4c
    virtual unsigned long vfunc_10();
};

unsigned long CSoundManagerLH::vfunc_10()
{
    return m_maxEntries - m_entries.size();
}
