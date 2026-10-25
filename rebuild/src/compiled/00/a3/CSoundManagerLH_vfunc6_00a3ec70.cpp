// CSoundManagerLH::vfunc_6() -- true while the number of entries on the list at +0x4c is below the limit at +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3EC70, length 32 bytes.
// Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 6; its sibling slot 10 (0x00A3EC90) returns
// the room left (`limit - count`) from the same walk.
// Shape: the list is the 4-byte STLport one (+0x4c holds the sentinel node pointer); its size() is the SGI
// `__distance` walk (`first = first->next` then `++n`, the loop head aligned with `lea esp,[esp]`), the
// sentinel kept in edx and the count in esi. The unsigned `count < limit` comes back as `cmp ; sbb eax,eax ; neg eax`:
// VC7.1's form for a plain `return a < b;` bool return too (the `?:` form is a byte shorter). The result is a bool: its
// caller, slot 4 0x00A3ECB0, tests only al (`call [eax+18h] ; test al,al`).
struct SListNode_00a3ec70 {
    SListNode_00a3ec70* next;
    SListNode_00a3ec70* prev;
};

struct CSTLportList_00a3ec70 {                  // STLport list: the sentinel node pointer only
    SListNode_00a3ec70* m_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        SListNode_00a3ec70* first = m_node->next;
        SListNode_00a3ec70* last = m_node;
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
    CSTLportList_00a3ec70 m_entries;            // +0x4c
    virtual bool vfunc_6();
};

bool CSoundManagerLH::vfunc_6()
{
    return m_entries.size() < m_maxEntries;
}