// CTCCarrying::IsSlotPrimarySlot(long slot) -- fetches the slot's definition from the game definition manager and returns its IsPrimary flag (+0x30), releasing the definition pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A1330, length 60 bytes.
// Callees: CGameDefinitionManager::Get @ 0x0044C6B0 (a static returning the global manager [0x013B879C]), the
// manager's GetDef<T>(long, CDefPointer<T>*) @ 0x006A3060 (manifest "GetDef<CThingPhysicalSwitchDef>": assigns
// through the out pointer with add-ref / release), the definition's virtual slot 1 (destroy) on the last release.
// Shape: the def pointer is a 4-byte local in the `push ecx` slot, null-initialised before the call and
// passed by address; the flag is read into bl before the inlined, null-test-free release
// (`mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax ; jne ; call [edx+4]`).
// Tried: `if (--m_p->m_refCount == 0) m_p->Destroy();` (re-reads the def from its address-taken slot after
// the store, 69v60), a local pointer copy with `--p->m_refCount` and a Release() on the pointee (both give
// the `add eax,4 ; test edx,edx` form, 66v60), and the explicit count local below (62v60). Next: the
// count-local release written as a member of the pointee (so `this` is allocated to ecx), else lane gap
// (register choice).
struct CCarrySlotDef {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long          m_refCount;                       // +0x04
    unsigned char _pad08[0x30 - 0x08];
    bool          IsPrimary;                        // +0x30
};

template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        T* p = m_p;
        long count = p->m_refCount - 1;
        p->m_refCount = count;
        if (count == 0)
            p->Destroy();
    }
    T* operator->() const { return m_p; }
};

struct CDefinitionManager {
    void GetDef(long index, CDefPointer<CCarrySlotDef>* out);   // 0x006A3060
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) void CDefinitionManager::GetDef(long index, CDefPointer<CCarrySlotDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return;
    *(void**)out = def;
    
}
#pragma auto_inline(on)

struct CGameDefinitionManager {
    static CDefinitionManager* Get();               // 0x0044C6B0
};

struct CTCCarrying {
    virtual void v00();
    bool IsSlotPrimarySlot(long slot);
};

bool CTCCarrying::IsSlotPrimarySlot(long slot)
{
    CDefPointer<CCarrySlotDef> def;
    CGameDefinitionManager::Get()->GetDef(slot, &def);
    return def->IsPrimary;
}