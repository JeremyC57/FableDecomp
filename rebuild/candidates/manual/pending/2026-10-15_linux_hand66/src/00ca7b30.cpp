// CStreamState_00ca7b30::Reset() -- __stdcall COM-style method: re-reads two values from the source object (+0xC), clears the +0x4 counter and empties the node list at +0x18; returns S_OK
// Leaf: Reset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA7B30, 48 bytes.
// Callee: 0x00CB1340 CNodeList::RemoveAll (frees every node through `next` at +0x4, then zeroes
// count, last and first); the source's vtable slots 5 and 6 (`call [eax+14h]`, `call [edx+18h]`).
//
// The manifest label `CPhysicsFacesSegmentBranch::GetNoFaces` is propagated and wrong: `this` is the
// first STACK argument (`mov esi,[esp+8]` after `push esi`, `ret 4`), the result is the constant 0,
// and the only reference is the vtable word 0x012BF6B0 of an audio/streaming-module class whose
// neighbouring slots are thiscall methods and a __stdcall Release (0x00CA7B00:
// `InterlockedDecrement(&m_14)`, then the deleting destructor in slot 7) -- a COM-style object, so
// this is a `__stdcall` virtual returning HRESULT. The +0x18 list is a DirectShow-BaseClasses-like
// {first, last, count} node list. The class is named by address (no RTTI in the module).
struct CStreamSource {
    virtual void Slot00();
    virtual void Slot04();
    virtual void Slot08();
    virtual void Slot0C();
    virtual void Slot10();
    virtual long GetValue14();             // slot 5
    virtual long GetValue18();             // slot 6
};

struct CNodeList {
    struct SNode* m_pFirst;
    struct SNode* m_pLast;
    long          m_count;
    void RemoveAll();                      // 0x00CB1340
};

struct CStreamState_00ca7b30 {
    void*          m_vptr;                 // +0x00
    long           m_position;             // +0x04
    long           m_value8;               // +0x08
    CStreamSource* m_pSource;              // +0x0C
    long           m_value10;              // +0x10
    long           m_refCount;             // +0x14
    CNodeList      m_nodes;                // +0x18
    long __stdcall Reset();
};

long __stdcall CStreamState_00ca7b30::Reset()
{
    m_value10 = m_pSource->GetValue14();
    m_value8 = m_pSource->GetValue18();
    m_position = 0;
    m_nodes.RemoveAll();
    return 0;
}
