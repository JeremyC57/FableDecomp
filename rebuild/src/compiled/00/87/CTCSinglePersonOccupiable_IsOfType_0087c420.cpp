// CTCSinglePersonOccupiable::IsOfType(unsigned long) -- true when the owner's def carries the occupiable sub-def and that sub-def's type flags (+0x28) share a bit with the mask
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087C420, length 76 bytes (+4 int3 padding).
// Callees: CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) @ 0x0087C490 (bool; the manifest instantiation name
// "CAICreatureWillPowerIndicatorDef" is a folded twin), the sub-def's virtual slot 1 (+0x04, destroy when its reference
// count at +4 drops to zero). Callers: 0x006A8974, 0x006A8A1C, 0x007037B9.
// Layout: ParentThing at +4 (CTCBase); the thing's def pointer at +0x70.
// Shape: the 4-byte CDefPointer local lives in the `push ecx` slot, zeroed by its constructor after its address is
// pushed; its inline destructor (`dec [p+4] ; jne ; call [vt+4]`) is duplicated into both return paths; the def pointer
// is read into ecx once after the call (`mov ecx,[esp]`) and serves the flag test and both releases. GetSubDef has a
// same-TU out-slot-only stand-in body (the out word does not escape), and the def class is read into a local first
// (`mov eax,[ecx+4] ; mov ecx,[eax+0x70]`).
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
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer_CSubDef* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->m_pDef = (CSubDef*)def;
    return true;
}
#pragma auto_inline(on)

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
    CDefClassBase* defClass = ParentThing->m_pDef;
    CDefPointer_CSubDef subDef;
    if (defClass->GetSubDef(&subDef) && (subDef->m_typeFlags & typeMask))
        return true;
    return false;
}