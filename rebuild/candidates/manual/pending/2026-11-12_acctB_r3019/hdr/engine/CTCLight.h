#ifndef FABLE_ENGINE_CTCLIGHT_H
#define FABLE_ENGINE_CTCLIGHT_H
#include "engine/CTCBase.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CLightDef.h"
#include "stlport/fable_containers.h"
class CGuiVarTransferStruct;
namespace NGraphicAppearance {class CPrimitive;}
// Layout: complete both-PDB CTCLight; two native STLport vectors give size68.
class CTCLight:public CTCBase,public NGraphicAppearance::CAGraphicAppearanceComponent {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
public:
 virtual ETCInterfaceType GetInterfaceType() const;
public:
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
public:
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
public:
 virtual CCharString GetClassTypeName() const;
public:
 CTCLight(const CTCLight & p0);
public:
 CTCLight(CThing & p0);
public:
 virtual ~CTCLight();
public:
 virtual void OnAppearanceClear();
public:
 virtual bool OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance & p0,const NGraphicAppearance::CGraphicAppearanceDrawInfo & p1);
public:
 virtual void FrameUpdate();
public:
 virtual bool HasFrameUpdate() const;
public:
 virtual void OnInitialActivate();
public:
 virtual bool IsComponentSerialisable() const;
public:
 virtual void OnSerialise(CPersistContext & p0,bool p1);
public:
 virtual void GetPropertiesStruct(CGuiVarTransferStruct & p0) const;
public:
 virtual void SetFromPropertiesStruct(const CGuiVarTransferStruct & p0);
public:
 bool IsActive() const;
public:
 void SetActive(bool p0);
public:
 bool IsOverridden() const;
public:
 void SetOverridden(bool p0);
public:
 void SetColour(const CRGBColour & p0);
public:
 void SetInnerRadius(float p0);
public:
 void SetOuterRadius(float p0);
public:
 void SetFlicker(float p0);
public:
 void SetInverted(bool p0);
public:
 void SetLightDef(const CDefPointer<CLightDef const > & p0);
protected:
 void DrawLight(long p0,const C3DVector & p1,NGraphicAppearance::CTCGraphicAppearance & p2,const NGraphicAppearance::CGraphicAppearanceDrawInfo & p3);
protected:
 void DrawFlare(long p0,const C3DVector & p1,NGraphicAppearance::CTCGraphicAppearance & p2,const NGraphicAppearance::CGraphicAppearanceDrawInfo & p3);
protected:
#ifdef FABLE_RETAIL_CTCLIGHT_DRAWLIGHT
 const CRGBColour & GetColour() const{if(Overridden)return PLightInfo->Colour;return PLightDef->Colour;}
#else
 const CRGBColour & GetColour() const;
#endif
protected:
 long GetHaloGraphic() const{if(Overridden)return 0;return PLightDef->HaloGraphic;}
protected:
#ifdef FABLE_RETAIL_CTCLIGHT_DRAWLIGHT
 float GetInnerRadius() const{if(Overridden)return PLightInfo->InnerRadius;return PLightDef->InnerRadius;}
#else
 float GetInnerRadius() const;
#endif
protected:
#ifdef FABLE_RETAIL_CTCLIGHT_DRAWLIGHT
 float GetOuterRadius() const{if(Overridden)return PLightInfo->OuterRadius;return PLightDef->OuterRadius;}
#else
 float GetOuterRadius() const;
#endif
protected:
 float GetFlicker() const;
protected:
 float GetFlickerSpeed() const;
protected:
#ifdef FABLE_RETAIL_CTCLIGHT_DRAWLIGHT
 bool IsInverted() const{if(Overridden)return PLightInfo->Inverted;return PLightDef->Inverted;}
#else
 bool IsInverted() const;
#endif
protected:
 void AttachParticles();
public:
 CTCLight & operator=(const CTCLight & p0);
protected:
#ifdef FABLE_RETAIL_CTCLIGHT_DRAWLIGHT
 // Opt-in (chunk 2020, row 0x006414A0 DrawLight): the nested CTCLight::CLightInfo (Ego_r.pdb, 0x18 bytes; FableWin ??0CLightInfo@CTCLight@@QAE@XZ)
 // with its members at the offsets DrawLight and the landed setters read (Colour +0, InnerRadius +4, OuterRadius +8, Flicker +0xC,
 // LightingChannelsEffected +0x10, Inverted +0x14), and the protected getters' bodies above: FableWin.pdb ?GetColour@CTCLight@@IBEABVCRGBColour@@XZ,
 // ?GetInnerRadius@CTCLight@@IBEMXZ, ?GetOuterRadius@CTCLight@@IBEMXZ, ?IsInverted@CTCLight@@IBE_NXZ -- inline in retail (no out-of-line copy
 // bound; DrawLight expands each as `Overridden ? PLightInfo->X : PLightDef->X`, 0x0064151E / 0x00641986 / 0x0064199F / 0x0064191E).
 class CLightInfo {
 public:
  CRGBColour Colour;
  float InnerRadius;
  float OuterRadius;
  float Flicker;
  unsigned long LightingChannelsEffected;
  bool Inverted;
 };
#else
 class CLightInfo;
#endif
 CCountedPointer<CLightInfo> PLightInfo;
 CDefPointer<const CLightDef> PLightDef;
 bool Active,Overridden;
 float AccumilativeFlickerFactor,OldAccumilativeFlickerFactor;
 bool Initialised;
private:
 std::vector<NGraphicAppearance::CPrimitive*> FlareSprites,Lights;
};
FABLE_STATIC_ASSERT(sizeof(CTCLight)==68);
#endif
