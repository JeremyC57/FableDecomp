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
 const CRGBColour & GetColour() const;
protected:
 long GetHaloGraphic() const{if(Overridden)return 0;return PLightDef->HaloGraphic;}
protected:
 float GetInnerRadius() const;
protected:
 float GetOuterRadius() const;
protected:
 float GetFlicker() const;
protected:
 float GetFlickerSpeed() const;
protected:
 bool IsInverted() const;
protected:
 void AttachParticles();
public:
 CTCLight & operator=(const CTCLight & p0);
protected:
 class CLightInfo;
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
