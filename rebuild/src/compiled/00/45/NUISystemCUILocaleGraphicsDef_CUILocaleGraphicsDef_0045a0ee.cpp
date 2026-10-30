// NUISystem::CUILocaleGraphicsDef::CUILocaleGraphicsDef() -- the parent-def base (0x00430370), this class's vtable (0x01235344), the 12-byte member at +0x3c (its constructor @ 0x00453309 builds two allocator temporaries), then the three words at +0x48..+0x50 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045A0EE, length 37 bytes.
// The chunk label "CIcicleData::CIcicleData" is propagated; the vtable names the class (rtti-vtables.tsv). The member constructor label "CDisplayManager::CopyBackBufferToTexture" is propagated too.
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
struct CMember12_00453309 {                        // 12 bytes
    void* _body[3];                                // an STLport map: three pointer words (4-aligned)
    CMember12_00453309();                          // 0x00453309
};
namespace NUISystem {
class CUILocaleGraphicsDef : public CParentDefClassBase { public:   // vtable 0x01235344
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CUILocaleGraphicsDef@NUISystem@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CUILocaleGraphicsDef@NUISystem@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CUILocaleGraphicsDef@NUISystem@@UAEKXZ
    CMember12_00453309 m_member3c;                 // +0x3c (Ego_r.pdb: std::map<EWorldMapNameGraphic,unsigned long> WorldMapRegionName)
    long               m_48;                       // +0x48 (Ego_r.pdb: std::vector<unsigned long> HelpScreenGraphics, three words)
    long               m_4c;                       // +0x4c
    long               m_50;                       // +0x50
    CUILocaleGraphicsDef();
};

CUILocaleGraphicsDef::CUILocaleGraphicsDef()
    : CParentDefClassBase(), m_member3c(), m_48(0), m_4c(0), m_50(0)
{
}
}