// CVtable012b2ab0::CVtable012b2ab0(void* source) -- EH-framed constructor: vtable 0x012B2AB0, the source at +4, then a 0xa4-byte helper built from that source (null-checked `new`, freed if its constructor throws) stored at +8
// Leaf: ??0CVtable012b2ab0
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C165A0, length 111 bytes. Built under /EHsc.
// Callees: operator new @ 0x00BFEA1A, the helper's constructor @ 0x00C0EDF0 (vtable 0x012B25A8; it keeps the source at +8).
// Label: the catalog row says CIsVariableFalse::Clone, but the body stores its own vtable 0x012B2AB0 and returns `this`
// (`ret 4`), and Ego_r.pdb's CIsVariableFalse is 8 bytes (vptr + bool* Variable) while this object has a word at +8: it is
// the constructor of an unnamed 12-byte class (no RTTI row for 0x012B2AB0).
// Shape: the members are initialisers (stored before `new`); the new'd pointer is homed in the dead argument slot and the
// EH state goes 0 -> -1 around the constructor; the base has no vtable store of its own (no base).
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
struct CHelper_00c0edf0 {                          // 0xa4 bytes
    unsigned char _body[0xa4];
    CHelper_00c0edf0(void* source);                // 0x00C0EDF0
};
struct CVtable012b2ab0 {
    virtual void v00();
    void*             m_pSource;                   // +0x04
    CHelper_00c0edf0* m_pHelper;                   // +0x08
    CVtable012b2ab0(void* source);
};

CVtable012b2ab0::CVtable012b2ab0(void* source)
    : m_pSource(source), m_pHelper(0)
{
    m_pHelper = new CHelper_00c0edf0(m_pSource);
}