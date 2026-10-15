// CTCLook::OnCreate() -- fetch the look and appearance sub-defs of the owning thing's def into PDef / PAppearanceDef, then centre the head and zero the eye angles
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753040, length 53 bytes.
// Callees: CDefClassBase::GetSubDef<CLookDef>(CDefPointer<CLookDef const>&) @ 0x007550F0 and
// GetSubDef<CAppearanceDef>(CDefPointer<CAppearanceDef const>&) @ 0x0042B0A2 (landed; the manifest's template arguments are
// propagated, the landed 0x0042B0A2 body names 'CAppearanceDef'), CTCLook::CentreHead @ 0x007528A0,
// CTCLook::ChangeEyeAngles(float, float) @ 0x00752200 (0.0f, 0.0f pushed as zero dwords).
// Layout (local model, no engine header): the CTCBase owner pointer at +4 (the thing, whose PBaseDef is at +0x70), PDef +0x14,
// PAppearanceDef +0x18 (PDB offsets). Shape: the owner chain is re-read for each lookup (the calls may change it).
struct CLookDefPointer { const void* m_pDef; };          // CDefPointer<CLookDef const>
struct CAppearanceDefPointer { const void* m_pDef; };    // CDefPointer<CAppearanceDef const>

struct CDefClassBase {
    bool GetSubDef(CLookDefPointer& out);                // 0x007550F0
    bool GetSubDef(CAppearanceDefPointer& out);          // 0x0042B0A2
};

struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PBaseDef;                             // +0x70 (PDB +0x80: the retail vector map and list are 8 bytes narrower each)
};

struct CTCLook {
    void*                 __vftable;                     // +0x00
    CThing*               m_pThing;                      // +0x04 (CTCBase owner)
    unsigned char         _pad_0x08[0x14 - 0x08];
    CLookDefPointer       PDef;                          // +0x14
    CAppearanceDefPointer PAppearanceDef;                // +0x18
    void CentreHead();                                   // 0x007528A0
    void ChangeEyeAngles(float angleXY, float angleYZ);  // 0x00752200
    void OnCreate();
};

void CTCLook::OnCreate()
{
    m_pThing->PBaseDef->GetSubDef(PDef);
    m_pThing->PBaseDef->GetSubDef(PAppearanceDef);
    CentreHead();
    ChangeEyeAngles(0.0f, 0.0f);
}