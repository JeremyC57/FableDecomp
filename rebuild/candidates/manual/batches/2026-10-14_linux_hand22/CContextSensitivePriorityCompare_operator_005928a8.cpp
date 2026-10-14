// CContextSensitivePriorityCompare::operator()(const SEntry& left, const SEntry& right) const --
// strict-weak ordering by priority: an entry with an item uses the item's priority at +0x2c,
// otherwise the priority is looked up from the entry's id; `left` sorts first when it is higher.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005928A8, length 62 bytes.
// Callee: the __fastcall priority lookup @ 0x00581BDE (id in ecx; the manifest's
// "GetContextSensitivePreferredItemButtonIndex" label is propagated). `this` is unused.
// Shape: the left priority survives in esi across the right lookup; the bool result is formed
// through a full register (`xor ecx ; setg cl ; mov al,cl`), the plain `return a > b;` form.
#pragma optimize("s", on)
struct CContextItem_Retail {
    unsigned char _pad[0x2c];
    long m_priority;                           // +0x2c
};

struct SContextEntry_Retail {
    long                m_id;                  // +0x00
    CContextItem_Retail* m_pItem;              // +0x04
};

long __fastcall GetContextSensitivePriority_00581bde(long id);   // 0x00581BDE

struct CContextSensitivePriorityCompare {
    static __forceinline long GetPriority(const SContextEntry_Retail& entry)
    {
        if (entry.m_pItem)
            return entry.m_pItem->m_priority;
        return GetContextSensitivePriority_00581bde(entry.m_id);
    }
    bool operator()(const SContextEntry_Retail& left, const SContextEntry_Retail& right) const;
};

bool CContextSensitivePriorityCompare::operator()(const SContextEntry_Retail& left, const SContextEntry_Retail& right) const
{
    return GetPriority(left) > GetPriority(right);
}
