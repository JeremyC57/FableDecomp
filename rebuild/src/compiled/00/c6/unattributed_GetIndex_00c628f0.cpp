// CHandleOwner_00c628f0::GetIndex() -- forwards to the attached object's slot-5 virtual, -1 when nothing is attached
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C628F0, length 19 bytes. No direct callees
// (virtual slot 5, +0x14, of the object at +4 via a tail jump).
// The manifest label "CAIBrain::GetCurrentStateGroupDebugText" is propagated (CAIBrain's +4 is
// not an object pointer) and is rejected; the class is named by the body. `or eax,-1` is the
// size-neutral -1 load VC7.1 emits for a long return; the sibling call becomes `jmp [eax+14h]`.
struct CIndexedObject {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual long GetIndex();                // slot 5 (+0x14)
};

struct CHandleOwner_00c628f0 {
    void*           __vftable;              // +0x00
    CIndexedObject* m_pObject;              // +0x04
    long GetIndex();
};

long CHandleOwner_00c628f0::GetIndex()
{
    if (m_pObject == 0)
        return -1;
    return m_pObject->GetIndex();
}