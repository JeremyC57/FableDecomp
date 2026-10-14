// CEngineInternalPrimitiveMeshBase::GetMeshEffect(CIVCountedPointer& effect) -- hands out the mesh effect counted pointer at +0x68 (counted-pointer assignment: release the caller's old pointee, take the new one and add a reference); always true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA8070, length 54 bytes.
// Callees: CIVCountedPointeeBase virtual slot 1 (Delete, +4) when the released count reaches zero. Shape: the inlined CIVCountedPointer::operator= compares
// the two pointees first (`cmp ecx,[esi+68h] ; je`), releases through a local copy of the old pointee (`dec [ecx+4] ; jne ; call [eax+4]`), zeroes the slot,
// then stores the new pointee and tests the stored value before `inc [eax+4]`; `mov al,1` is the bool. The pointee layout (vtable, IVRefCount at +4) and the
// pointer are the ones used by CEvent_0084b690 @ 0x0084B690 (landed); the mesh base's member at +0x68 has no PDB layout and is modelled locally.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

struct CEngineInternalPrimitiveMeshBase {
    unsigned char    _pad[0x68];
    CIVCountedPointer m_meshEffect;                // +0x68
    bool GetMeshEffect(CIVCountedPointer& effect);
};

bool CEngineInternalPrimitiveMeshBase::GetMeshEffect(CIVCountedPointer& effect)
{
    effect = m_meshEffect;
    return true;
}