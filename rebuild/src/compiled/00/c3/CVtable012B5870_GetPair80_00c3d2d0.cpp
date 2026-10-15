// CVtable_012B5870::GetPair80(long* first, long* second) -- const getter: copies the two words at +0x80 and +0x84 out through two pointer arguments
// Leaf: GetPair80
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3D2D0, length 27 bytes (`ret 8`, `this` in ecx).
// Callees: none.
// Identity: the manifest label "CParentDefClassBase::GetInstantiationName" is propagated -- the real
// CParentDefClassBase::GetInstantiationName (0x004303B0, landed) returns the +0x25 CDefString by
// hidden pointer. The only caller (0x00C42E15) runs it on the static object at 0x0143BB58
// (`mov ecx,143BB58h`), whose constructor 0x00C3D7D0 stores the no-RTTI vtable 0x012B5870 and zeroes
// +0x80/+0x84; it writes the two values into its own +0xdc/+0xe0. The class has no RTTI and no PDB
// match (the landed constructor's "SExpSpendUndo" label is an 84-byte PDB struct), so it is named
// after its vtable and the members by offset.
// Shape: two out-parameter stores; the first member is loaded before the first argument, the
// second argument reuses ecx after `this` is dead.

struct CVtable_012B5870 {
    virtual void v00();
    unsigned char _body[0x7c];                                            // +0x04..+0x80
    long m_value80;                                                       // +0x80
    long m_value84;                                                       // +0x84
    void GetPair80(long* first, long* second) const;
};

void CVtable_012B5870::GetPair80(long* first, long* second) const
{
    *first = m_value80;
    *second = m_value84;
}