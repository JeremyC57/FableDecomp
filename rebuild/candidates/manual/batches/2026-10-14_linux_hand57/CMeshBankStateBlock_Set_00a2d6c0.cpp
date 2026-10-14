// CMeshBankStateBlock::Set(const CBankStateBlock* other) -- copy another block's state: the 16-byte plain state at +4 assigned, the resource reference at +0x14 assigned (0x00A2CF50) and the text record at +0x28 assigned (0x006B9E00)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2D6C0, length 80 bytes.
// Leaf: ?Set@CMeshBankStateBlock@@MAEXPBVCBankStateBlock@@@Z
// Callees: 0x00A2CF50 (labelled CScriptGameResourceObjectScriptedThingBase::operator=), 0x006B9E00 (labelled LTextTreeWalkThrough_CopyAssign). Shape: `this` in esi, the source in edi (advanced to +0x28 for the last call), the state copied as four dwords, `ret 4`.
struct SBankState_00a2d6c0 {                       // 16 bytes of plain data
    long m_0, m_4, m_8, m_c;
};
struct CResourceRef_00a2d6c0 {                     // 0x14 bytes
    unsigned char m_body[0x14];
    CResourceRef_00a2d6c0& operator=(const CResourceRef_00a2d6c0& other);   // 0x00A2CF50
};
struct CTextRecord_00a2d6c0 {
    unsigned char m_body[4];
    CTextRecord_00a2d6c0& operator=(const CTextRecord_00a2d6c0& other);     // 0x006B9E00
};

struct CBankStateBlock {
    virtual void v00();
    SBankState_00a2d6c0   m_state;                 // +0x04
    CResourceRef_00a2d6c0 m_resource;              // +0x14
    CTextRecord_00a2d6c0  m_text;                  // +0x28
};

struct CMeshBankStateBlock : CBankStateBlock {
    virtual void Set(const CBankStateBlock* other);
};

void CMeshBankStateBlock::Set(const CBankStateBlock* other)
{
    m_state = other->m_state;
    m_resource = other->m_resource;
    m_text = other->m_text;
}
