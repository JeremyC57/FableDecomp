// CTCLightningOrb::LoadDefAndFlagAppearance() -- take a counted copy of the owner thing's base def, fetch the CLightningOrbDef sub-def into PDef, release the copy, then set bit 1 of the byte at +0x59 of the owner's graphic appearance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F23D0, length 62 bytes.
// Leaf: LoadDefAndFlagAppearance
// Callees: CDefClassBase::GetSubDef<CLightningOrbDef>(CDefPointer<CLightningOrbDef const>&) @ 0x007F3C20 (manifest template
// argument propagated), the def's virtual slot 1 (Destroy, when the copy's release drops the count to zero).
// Vtable: slot 6 of CTCLightningOrb's vtable (catalog "vfunc_6"; the real slot name is not recorded, so the method is
// named from what it does). Layout: the CTCBase owner at +4; on the retail CThing the base def (CDefPointer<CThingBaseDef>)
// sits at +0x70 and PTCGraphicAppearance at +0x64 (PDB +0x80 / +0x74, 16 bytes of narrower retail containers before them);
// PDef +0x1c (as the constructor 0x004DB4D7). Local model.
// Shape: `CDefPointer def = m_pThing->PBaseDef;` is an inline copy constructor (null test + `inc [esi+4]`) whose destructor
// releases through the def object (null test + `dec [esi+4] ; jne ; call [edx+4]`, the dead null store dropped); the
// owner is re-read after the call and the flag set by `or cl,2` read-modify-write.
struct CLightningOrbDefPointer {                   // CDefPointer<CLightningOrbDef const>
    const void* m_pDef;
};

struct CThingBaseDef {                             // counted def (CDefClassBase): vptr, count at +4
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_count;                                  // +0x04
    void AddRef() { ++m_count; }
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
    bool GetSubDef(CLightningOrbDefPointer& out);  // 0x007F3C20
};

struct CThingBaseDefPointer {                      // CDefPointer<CThingBaseDef const>
    CThingBaseDef* m_pDef;
    CThingBaseDefPointer(const CThingBaseDefPointer& other) : m_pDef(other.m_pDef)
    {
        if (m_pDef)
            m_pDef->AddRef();
    }
    ~CThingBaseDefPointer()
    {
        if (m_pDef) {
            m_pDef->Release();
            m_pDef = 0;
        }
    }
    CThingBaseDef* operator->() const { return m_pDef; }
};

struct CTCGraphicAppearance {
    unsigned char _pad_0x00[0x59];
    unsigned char m_updateFlags;                   // +0x59
};

struct CThing {
    unsigned char         _pad_0x00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;    // +0x64
    unsigned char         _pad_0x68[0x70 - 0x68];
    CThingBaseDefPointer  PBaseDef;                // +0x70
};

struct CTCLightningOrb {
    void*                   __vftable;             // +0x00 (CTCBase)
    CThing*                 m_pThing;              // +0x04 (CTCBase owner)
    unsigned char           _pad_0x08[0x1c - 0x08];
    CLightningOrbDefPointer PDef;                  // +0x1c
    void LoadDefAndFlagAppearance();
};

void CTCLightningOrb::LoadDefAndFlagAppearance()
{
    {
        CThingBaseDefPointer def = m_pThing->PBaseDef;
        def->GetSubDef(PDef);
    }
    m_pThing->PTCGraphicAppearance->m_updateFlags |= 2;
}