// NUISystem::CUILocaleGraphicsDef::CUILocaleGraphicsDef() -- the parent-def base (0x00430370), this class's vtable (0x01235344), the 12-byte member at +0x3c (its constructor @ 0x00453309 builds two allocator temporaries), then the three words at +0x48..+0x50 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045A0EE, length 37 bytes.
// The chunk label "CIcicleData::CIcicleData" is propagated; the vtable names the class (rtti-vtables.tsv). The member constructor label "CDisplayManager::CopyBackBufferToTexture" is propagated too.
struct CParentDefClassBase {
    virtual void v00();
    CParentDefClassBase();                         // 0x00430370
};
struct CMember12_00453309 {                        // 12 bytes
    unsigned char _body[0xc];
    CMember12_00453309();                          // 0x00453309
};
namespace NUISystem {
struct CUILocaleGraphicsDef : CParentDefClassBase {   // vtable 0x01235344
    unsigned char      _pad04[0x38];
    CMember12_00453309 m_member3c;                 // +0x3c
    long               m_48;                       // +0x48
    long               m_4c;                       // +0x4c
    long               m_50;                       // +0x50
    virtual void v00();
    CUILocaleGraphicsDef();
};

CUILocaleGraphicsDef::CUILocaleGraphicsDef()
    : CParentDefClassBase(), m_member3c(), m_48(0), m_4c(0), m_50(0)
{
}
}
