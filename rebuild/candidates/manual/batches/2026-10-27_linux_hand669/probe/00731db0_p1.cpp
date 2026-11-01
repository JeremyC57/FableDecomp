// CTCSpotLight::~CTCSpotLight() -- spot-light component dtor: forget the light primitive, release the spot-light def and the shared light info, restore the appearance-component vtable, then ~CTCBase
// Leaf: ??1CTCSpotLight
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"

class CThing;
class CSpotLightDef;
class CEnginePrimitivePrepareInfo;
namespace NGraphicAppearance {
class CPrimitive;
class CTCGraphicAppearance;
class CGraphicAppearanceDrawInfo;
class CAGraphicAppearanceComponent {                        // vtable 0x0125A170
public:
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info) = 0;
    virtual void OnAppearanceClear() = 0;
    ~CAGraphicAppearanceComponent() { }
};
}

class CTCBase : public CBaseClassNonCopyable {              // retail 0x0C bytes
public:
    CTCBase(CThing& parent);                                // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCSpotLight : public CTCBase, public NGraphicAppearance::CAGraphicAppearanceComponent {   // vtables 0x01266FBC / 0x01266FA4
public:
    struct CSpotLightInfo;
    virtual ~CTCSpotLight();
    virtual bool OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance& appearance, const NGraphicAppearance::CGraphicAppearanceDrawInfo& info);
    virtual void OnAppearanceClear();
    CCountedPointer<CSpotLightInfo>     PLightInfo;                     // +0x10
    CDefPointer<const CSpotLightDef>    PLightDef;                      // +0x18
    bool                                Overridden;                     // +0x1C
    NGraphicAppearance::CPrimitive*     LightPrimitive;                 // +0x20
    float                               AccumilativeFlickerFactor;      // +0x24
    float                               OldAccumilativeFlickerFactor;   // +0x28
};

CTCSpotLight::~CTCSpotLight()
{
    LightPrimitive = 0;
}
