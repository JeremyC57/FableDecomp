// CEnvironmentCombatAction::CEnvironmentCombatAction() -- the CAIStateGroup_CreatureUseCoverBase constructor (0x00C42D60), this class's vtable (0x012B7B88, no RTTI), then the two self-pointers at +0x11c / +0x124 aimed at the embedded records at +0x128 / +0x168
// Vendor naming V2 (plan phase 1, one address, one name): 0x00C42D60 ??0CIcicleData@@QAE@XZ (was ??0CAIStateGroup_CreatureUseCoverBase_00c42d60@@QAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44580, length 48 bytes.
// The chunk label "CIcicleData::Initialize" is propagated; named by the base. Shape: both `lea`s before the vtable store, `mov eax,esi`.
struct CIcicleData {   // 0xf8 bytes (landed 0x00C42D60)
    virtual void v00();
    unsigned char _body[0xf4];
    CIcicleData();   // 0x00C42D60
};
struct SCoverRecord_00c44580 { unsigned char _body[0x40]; };
struct CAIStateGroup_Derived_00c44580 : CIcicleData {   // vtable 0x012B7B88
    unsigned char           _padf8[0x24];
    SCoverRecord_00c44580*  m_pRecordA;            // +0x11c
    long                    m_120;
    SCoverRecord_00c44580*  m_pRecordB;            // +0x124
    SCoverRecord_00c44580   m_recordA;             // +0x128
    SCoverRecord_00c44580   m_recordB;             // +0x168
    virtual void v00();
    CAIStateGroup_Derived_00c44580();
};

CAIStateGroup_Derived_00c44580::CAIStateGroup_Derived_00c44580()
    : CIcicleData(), m_pRecordA(&m_recordA), m_pRecordB(&m_recordB)
{
}