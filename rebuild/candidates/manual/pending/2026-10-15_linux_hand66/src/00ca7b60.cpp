// CStreamState_00ca7b60::Reset() -- __stdcall COM-style method: re-reads two values from the source object (+0xC) and clears the +0x4 counter; returns S_OK
// Leaf: Reset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA7B60, 40 bytes.
// Callees: the source's vtable slots 5 and 6 (`call [eax+14h]`, `call [edx+18h]`).
//
// The manifest label `CPhysicsFacesSegmentBranch::GetNoFaces` is propagated and wrong: `this` is the
// first STACK argument (`mov esi,[esp+8]` after `push esi`, `ret 4`) and the result is the constant
// 0 -- a `__stdcall` (COM-style) method. Twin of 0x00CA7B30 without the node list (that one also
// empties a list at +0x18); no direct code or data reference was found, so the class is named by
// address.
struct CStreamSource {
    virtual void Slot00();
    virtual void Slot04();
    virtual void Slot08();
    virtual void Slot0C();
    virtual void Slot10();
    virtual long GetValue14();             // slot 5
    virtual long GetValue18();             // slot 6
};

struct CStreamState_00ca7b60 {
    void*          m_vptr;                 // +0x00
    long           m_position;             // +0x04
    long           m_value8;               // +0x08
    CStreamSource* m_pSource;              // +0x0C
    long           m_value10;              // +0x10
    long __stdcall Reset();
};

long __stdcall CStreamState_00ca7b60::Reset()
{
    m_value10 = m_pSource->GetValue14();
    m_value8 = m_pSource->GetValue18();
    m_position = 0;
    return 0;
}
