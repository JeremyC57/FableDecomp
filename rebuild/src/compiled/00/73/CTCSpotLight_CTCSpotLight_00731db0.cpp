// CTCSpotLight::~CTCSpotLight() -- spot-light component dtor: forget the light primitive, release the spot-light def and the shared light info, restore the appearance-component vtable, then ~CTCBase
// Leaf: ??1CTCSpotLight@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00731DB0, 98 bytes, tail jump to the base destructor.
// Identity (proven): the first stores are vtable 0x01266FBC (+0x00) and 0x01266FA4 (+0x0C), RTTI `.?AVCTCSpotLight@@`
//   (rtti_bases: CTCBase@0; NGraphicAppearance::CAGraphicAppearanceComponent@12); the sole caller 0x00732693 is the scalar
//   deleting dtor 0x00732690, slot 0 of 0x01266FBC. Ego_r.pdb ??1CTCSpotLight@@UAE@XZ. The final store 0x0125A170 is
//   CAGraphicAppearanceComponent's own vtable (its inline user dtor), as in the landed ~CTCLight 0x00640F80.
// Callees: the def's virtual Delete (slot 1), the info block's __fastcall DeleteFunc, operator delete 0x00BFE9BC,
//   CTCBase::~CTCBase 0x00686830 (tail jump).
// Layout: engine/CTCSpotLight.h offsets (flattened header: vptr words, counted pointer split in two words), so the class is a
//   local model with the PDB member names: PLightInfo +0x10 (CCountedPointer<CTCSpotLight::CSpotLightInfo>), PLightDef +0x18,
//   Overridden +0x1C, LightPrimitive +0x20, AccumilativeFlickerFactor +0x24, OldAccumilativeFlickerFactor +0x28.
//   CAGraphicAppearanceComponent's slot order (OnAppearanceDraw / OnAppearanceClear, both in CTCSpotLight's PDB set) is unproven.
// Shape: the body is `LightPrimitive = 0;` (one zero register for every clear); members in reverse order.
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
    class CSpotLightInfo;
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