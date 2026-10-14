// CEntryVectorOwner_00a2ad10::Sizeof() const -- 12 bytes plus 4 per entry of the vector at +0x0 plus, for each non-null entry, its own Sizeof (virtual slot 2)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2AD10, length 71 bytes.
// Leaf: Sizeof
// The chunk label (CMeshDataBank::Sizeof) is propagated: `this` is the vector itself; named by address. Shape: the running total in ebx (`lea ebx,[ecx*4+0Ch]`), the entry pointer in esi against the last pointer re-read each iteration, the `? :` with a `mov eax,4` else-arm, `ret`.
struct CBankEntry_00a2ad10 {
    virtual void v000();
    virtual void Release();                        // slot 1 (+0x04)
    virtual unsigned long Sizeof();                // slot 2 (+0x08)
    virtual void Reload();                         // slot 3 (+0x0c)
};
struct CRetailVector_00a2ad10 {
    CBankEntry_00a2ad10** m_first;
    CBankEntry_00a2ad10** m_last;
    CBankEntry_00a2ad10** m_end;
    unsigned long size() const { return m_last - m_first; }
};

struct CEntryVectorOwner_00a2ad10 {
    CRetailVector_00a2ad10  m_entries;             // +0x0
    unsigned long Sizeof() const;
};

unsigned long CEntryVectorOwner_00a2ad10::Sizeof() const
{
    unsigned long total = 12 + m_entries.size() * 4;
    for (CBankEntry_00a2ad10** entry = m_entries.m_first; entry != m_entries.m_last; ++entry)
        total += *entry ? (*entry)->Sizeof() + 4 : 4;
    return total;
}
