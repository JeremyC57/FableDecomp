// 004bc030 NGraphicAppearance::CTCGraphicAppearance::Copy
// Leaf: ?Copy@CTCGraphicAppearance@NGraphicAppearance@@UAEXABVCTCBase@@@Z
// Flags:
#include <string.h>
#define CTCGraphicAppearance CTCGraphicAppearanceFields
#include "engine/_quarantine/CTCGraphicAppearance.h"
#undef CTCGraphicAppearance
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#include "engine/CBaseClassNonCopyable.h"
class CPersistContext;class CTCParamsBase;class CEnginePrimitivePrepareInfo;
// Layout: real polymorphic prefix replaces the supplied flattened vptr. Original TPI names and reference types.
class CTCBase:public CBaseClassNonCopyable {
protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;protected:virtual void OnSerialise(CPersistContext&,bool);
public:virtual void OnRemove();virtual void ConstructFromParams(const CTCParamsBase&);virtual void OnCreate();virtual void InitialActivate();virtual void OnInitialActivate();virtual void RenderUpdate(float);virtual bool HasFrameUpdate()const;virtual void FrameUpdate();virtual void BulletTimeFrameUpdate();virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);virtual void Copy(const CTCBase&);
};
FABLE_STATIC_ASSERT(sizeof(CTCBase)==sizeof(CTCBaseFields));
namespace NGraphicAppearance {
// Layout: fields copied from the supplied retail header after its flattened CTCBase prefix.
class CTCGraphicAppearance:public CTCBase {protected:
    unsigned char     MeshEffects[0xc];                      // +0x0c map<void_*,NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry,std::less<void_*>,std::allocator<std::pair<void_*_const,NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry>_>_>
    unsigned char     TintColours[0xc];                      // +0x18 map<void_*,CRGBColour,std::less<void_*>,std::allocator<std::pair<void_*_const,CRGBColour>_>_>
    CTCAnimationBase* PTCAnimation;                          // +0x24
    float             CachedStaticFocalPosOffset_X;          // +0x28 C3DVector::X
    float             CachedStaticFocalPosOffset_Y;          // +0x2c C3DVector::Y
    float             CachedStaticFocalPosOffset_Z;          // +0x30 C3DVector::Z
    float             Scale;                                 // +0x34
    unsigned char     Primitives[0x4];                       // +0x38 list<NGraphicAppearance::CPrimitive,std::allocator<NGraphicAppearance::CPrimitive>_>
    unsigned char     Components[0x4];                       // +0x3c list<NGraphicAppearance::CAGraphicAppearanceComponent_*,std::allocator<NGraphicAppearance::CAGraphicAppearanceComponent_*>_>
    unsigned char     LightingChannel_ChannelIndex0;         // +0x40 CEnginePrimitiveThemeChannelDesc::ChannelIndex0
    unsigned char     LightingChannel_ChannelIndex1;         // +0x41 CEnginePrimitiveThemeChannelDesc::ChannelIndex1
    unsigned char     LightingChannel_ChannelBlendFactor;    // +0x42 CEnginePrimitiveThemeChannelDesc::ChannelBlendFactor
    unsigned char     LightingChannel_Pad;                   // +0x43 CEnginePrimitiveThemeChannelDesc::Pad
    unsigned long     MainGraphic_BankIndex;                 // +0x44 CEngineGraphic::BankIndex
    float             MainGraphic_AnimStep;                  // +0x48 CEngineGraphic::AnimStep
    float             MainGraphic_RenderSizeX;               // +0x4c CEngineGraphic::RenderSizeX
    bool              MainGraphic_AdditiveAlpha;             // +0x50 CEngineGraphic::AdditiveAlpha
    unsigned char     MainGraphic_Type;                      // +0x51 CEngineGraphic::Type
    unsigned short    ActivePersistenceFlags;                // +0x52
    long              FrameUpdatedWasRenderedFlag;           // +0x54
    unsigned char     UpdatePrimitiveReasons;                // +0x58
    unsigned char SelfIlluminating:1,ForceResendEveryRenderFrame:1,ResendPrimsEveryRenderFrame:1,DisableOcclusionTesting:1,DrawAfterWater:1,CachedStaticFocalPos:1,RenderedLastFrame:1,PrimitivesDrawn:1;
    unsigned char     Alpha;                                 // +0x5a
    signed char       ByPassColorFilter;                     // +0x5b
 public:bool IsSelfIlluminating()const{return SelfIlluminating!=0;}virtual void Copy(const CTCBase&);
};
FABLE_STATIC_ASSERT(sizeof(CTCGraphicAppearance)==sizeof(CTCGraphicAppearanceFields));
void CTCGraphicAppearance::Copy(const CTCBase&base){
 const CTCGraphicAppearance&other=static_cast<const CTCGraphicAppearance&>(base);
 ActivePersistenceFlags=other.ActivePersistenceFlags;
 memcpy(&MainGraphic_BankIndex,&other.MainGraphic_BankIndex,14);
 SelfIlluminating=other.IsSelfIlluminating();
 ForceResendEveryRenderFrame=other.ForceResendEveryRenderFrame;
 Scale=other.Scale;
 Alpha=other.Alpha;
}
}