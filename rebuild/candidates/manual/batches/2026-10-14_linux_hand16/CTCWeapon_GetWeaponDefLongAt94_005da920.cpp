// CTCWeapon::GetWeaponDefLongAt94() -- reads a long from the weapon definition (retail CWeaponDef +0x94) through
// a scoped CDefPointer reference (AddRef, read, Release / destroy on zero).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DA920, length 35 bytes; /O2 TU (the release is `mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax`).
// Callees: CWeaponDef virtual slot 1 (+0x04, destroy) when the count reaches zero.
// PDB CTCWeapon: +0x0C CDefPointer<const CWeaponDef> WeaponDef (the neighbours 0x005DA950 IsAnalogueLoading /
// 0x005DA980 IsProjectileWeaponLoaded read the same pointer). The shipped CWeaponDef is laid out differently
// from Ego_r.pdb (IsAnalogueLoading reads the byte at +0xC4 where the PDB puts UseAnalogueLoading at +0x9C),
// so the member at +0x94 cannot be named from the PDB; the manifest label "CCategory::GetCategoryIdentifier"
// is propagated and rejected.
struct CWeaponDef_Retail {
    virtual void v00();
    virtual void Destroy();                      // slot 1 (+0x04)
    long          m_refCount;                    // +0x04
    unsigned char _pad_0x08[0x8C];               // +0x08
    long          m_value94;                     // +0x94 (name not recoverable, see above)
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};

struct CDefRef {                                 // scoped reference returned by CDefPointer::operator->
    CWeaponDef_Retail* m_p;
    CDefRef(CWeaponDef_Retail* p) : m_p(p) { if (m_p) m_p->AddRef(); }
    ~CDefRef() { m_p->Release(); }
    CWeaponDef_Retail* operator->() const { return m_p; }
};

struct CDefPointer {
    CWeaponDef_Retail* m_pObject;
    CDefRef operator->() const { return CDefRef(m_pObject); }
};

struct CTCWeapon_Retail {
    unsigned char _pad_0x00[0xC];
    CDefPointer   WeaponDef;                     // +0x0C
    long GetWeaponDefLongAt94();
};

long CTCWeapon_Retail::GetWeaponDefLongAt94()
{
    return WeaponDef->m_value94;
}
