// CVtable_012b2240::Visit(void** key) -- "find the nth key" visitor: when the running count reaches the wanted index remember the key's address, then count it
// Leaf: ?Visit@CVtable_012b2240@@UAEXPAPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0EFE0, 22 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 0 of vtable 0x012B2240, whose constructor 0x00C0EFC0 (count +4 = 0, wanted index +8 =
//   argument, found +0xC = 0) derives from the visitor base of vtable 0x012B2238 (landed deleting destructor 0x00C0F010).
//   The red-black tree's in-order walk (landed PKCTRRedblackTree_00c18d50::VisitInOrder) calls slot 0 with each key's address, so
//   this picks the key at a position. libpk has no RTTI or PDB.
class CVtable_012b2240 {
public:
    virtual void Visit(void** key);             // slot 0 (PKCTRVisitor_00c18d50::Visit)
    long  m_count;                              // +0x04 items seen so far
    long  m_index;                              // +0x08 the position wanted
    void** m_found;                             // +0x0c the key at m_index, or 0
};

void CVtable_012b2240::Visit(void** key)
{
    if (m_count == m_index)
        m_found = key;
    ++m_count;
}