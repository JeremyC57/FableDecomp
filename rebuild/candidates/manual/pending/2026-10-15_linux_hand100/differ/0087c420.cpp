// CTCSinglePersonOccupiable::IsOfType(unsigned long) -- true when the owner's def carries the occupiable sub-def and that sub-def's type flags (+0x28) share a bit with the mask
// Grade: DIFFER(91v76) -- ours reads the def pointer back into eax (retail ecx) after GetSubDef, so each release becomes a load/dec/store with `mov ecx,eax` (retail folds to `dec dword ptr [ecx+4] ; jne ; mov edx,[ecx] ; call [edx+4]`), and the prologue loads ParentThing into ecx with the zero store before the push (retail eax, zero store after the push). VC7.1 does drop the null test on the success path in both. Tried: member-test destructor (91, best), a local pointer copy in the destructor (95), Release() on the pointee (95), optimize("s") (86 bytes but a frame + `dec [eax] ; cmp [eax],0`).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087C420, length 76 bytes (+4 int3 padding).
// Callees: CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) @ 0x0087C490 (bool; the manifest instantiation name
// "CAICreatureWillPowerIndicatorDef" is a folded twin), the sub-def's virtual slot 1 (+0x04, destroy when its reference
// count at +4 drops to zero). Callers: 0x006A8974, 0x006A8A1C, 0x007037B9.
// Layout: ParentThing at +4 (CTCBase); the thing's def pointer at +0x70.
// Shape: the 4-byte CDefPointer local lives in the `push ecx` slot, zeroed by its constructor after its address is
// pushed; its inline destructor (`dec [p+4] ; jne ; call [vt+4]`) is duplicated into both return paths; the def pointer
// is read into ecx once after the call (`mov ecx,[esp]`) and serves the flag test and both releases.
struct CSubDef {
    virtual void v00();
    virtual void Destroy();                  // slot 1 (+0x04)
    long          m_refCount;                // +0x04
    unsigned char _pad08[0x20];
    unsigned long m_typeFlags;               // +0x28
};

struct CDefPointer_CSubDef {
    CSubDef* m_pDef;
    CDefPointer_CSubDef() : m_pDef(0) { }
    ~CDefPointer_CSubDef()
    {
        if (m_pDef && --m_pDef->m_refCount == 0)
            m_pDef->Destroy();
    }
    CSubDef* operator->() const { return m_pDef; }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer_CSubDef* out); // 0x0087C490
};

struct CThing {
    unsigned char  _pad[0x70];
    CDefClassBase* m_pDef;                   // +0x70
};

struct CTCSinglePersonOccupiable {
    void*   m_vtable;                        // +0x00
    CThing* ParentThing;                     // +0x04 (CTCBase)
    bool IsOfType(unsigned long typeMask);
};

bool CTCSinglePersonOccupiable::IsOfType(unsigned long typeMask)
{
    CDefPointer_CSubDef subDef;
    if (ParentThing->m_pDef->GetSubDef(&subDef) && (subDef->m_typeFlags & typeMask))
        return true;
    return false;
}
