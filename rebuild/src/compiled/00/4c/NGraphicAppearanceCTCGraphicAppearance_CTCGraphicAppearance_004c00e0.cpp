// NGraphicAppearance::CTCGraphicAppearance::CTCGraphicAppearance(CThing&) -- the graphic-appearance component: CTCBase(parent), empty mesh-effect and
//   tint maps and primitive / component lists, default flags, persistence and occlusion / water flags from the parent's definition
// Leaf: ??0CTCGraphicAppearance@NGraphicAppearance@@QAE@AAVCThing@@@Z
// Respelled 2026-11-16: the base call is ??0CTCBase@@QAE@AAVCThing@@@Z (0x00686800; was bound as CTCBase(CGraphicThing*)) and this
//   constructor takes CThing& in NGraphicAppearance as both PDBs spell it (??0CTCGraphicAppearance@NGraphicAppearance@@QAE@AAVCThing@@@Z).
//   The local CTCBase still types ParentThing as the CGraphicThing view this body reads (Definition at +0x70); same bytes.
// Actual CTCGraphicAppearance constructor, VT123986C (manifest clock sprite wrong).
// Real donor headers retained. Native map12/list4 vs donor map16/list12 shrink
// PTCAnimation2C->24, Scale3C->34, flags71->59; native object size5C.
#define CTCGraphicAppearance CTCGraphicAppearanceDonor
#define CTCBase CTCBaseDonor
#define CThing CThingDonor
#include "engine/_quarantine/CTCGraphicAppearance.h"
#include "engine/CTCBase.h"
#include "engine/_quarantine/CThing.h"
#undef CThing
#undef CTCBase
#undef CTCGraphicAppearance
#include <stdlib.h>
struct CGraphicDefinition {unsigned char Prefix[0x43];unsigned char Flags;unsigned char BeforePersistence[8];unsigned short Persistence;};
struct CGraphicThing {unsigned char Prefix[0x70];CGraphicDefinition* Definition;};
class CThing;
class CTCBase { public:CGraphicThing* ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;CTCBase(CThing&);virtual ~CTCBase();}; //686800/686830
struct CMeshPriority {unsigned char Bytes[12];};
template<class V>struct CGraphicMap {
 struct Node {unsigned char Colour;unsigned char Pad[3];Node* Parent;Node* Left;Node* Right;void* Key;V Value;};
 Node* Head;unsigned Size;unsigned char Allocator[4];
 CGraphicMap():Head(0){Head=static_cast<Node*>(malloc(sizeof(Node)));Size=0;Head->Colour=0;Head->Parent=0;Head->Left=Head;Head->Right=Head;}
};
struct CGraphicPrimitive {unsigned char Bytes[16];};
template<class T>struct CGraphicList {
 struct Node {Node* Next;Node* Previous;T Value;};Node* Head;
 CGraphicList():Head(0){Node* node=static_cast<Node*>(malloc(sizeof(Node)));node->Next=node;node->Previous=node;Head=node;}
};
struct CGraphicFlags {
 unsigned char SelfIlluminating:1,ForceResendEveryRenderFrame:1,ResendPrimsEveryRenderFrame:1,DisableOcclusionTesting:1,DrawAfterWater:1,CachedStaticFocalPos:1,RenderedLastFrame:1,PrimitivesDrawn:1;
};
struct CGraphicChannel {unsigned char Channel0,Channel1,Blend,Pad;CGraphicChannel():Channel0(0),Channel1(0),Blend(0){}};
#pragma pack(push,1)
struct CGraphicValue {unsigned Bank;float AnimStep,RenderSize;bool Additive;unsigned char Type;CGraphicValue():Bank(0),AnimStep(0),RenderSize(0),Additive(false),Type(0){}};
#pragma pack(pop)
namespace NGraphicAppearance {
struct CTCGraphicAppearance:CTCBase {
 CGraphicMap<CMeshPriority> MeshEffects;CGraphicMap<unsigned> TintColours;void* PTCAnimation;float CachedFocal[3];float Scale;
 CGraphicList<CGraphicPrimitive> Primitives;CGraphicList<void*> Components;
 CGraphicChannel Lighting;CGraphicValue MainGraphic;unsigned short Persistence;
 int FrameUpdatedWasRenderedFlag;unsigned char UpdatePrimitiveReasons;CGraphicFlags Flags;unsigned char Alpha;signed char ByPassColorFilter;
 CTCGraphicAppearance(CThing&);virtual ~CTCGraphicAppearance();
};
}
#ifdef _MSC_VER
typedef char Graphic5C[sizeof(NGraphicAppearance::CTCGraphicAppearance)==0x5c?1:-1];
typedef char GraphicScale34[offsetof(NGraphicAppearance::CTCGraphicAppearance,Scale)==0x34?1:-1];
typedef char GraphicFlags59[offsetof(NGraphicAppearance::CTCGraphicAppearance,Flags)==0x59?1:-1];
#endif
NGraphicAppearance::CTCGraphicAppearance::CTCGraphicAppearance(CThing& parent):CTCBase(parent),PTCAnimation(0),Scale(1.0f),FrameUpdatedWasRenderedFlag(-1),UpdatePrimitiveReasons(15),Alpha(255),ByPassColorFilter(0){
 Flags.SelfIlluminating=false;Flags.ForceResendEveryRenderFrame=false;Flags.ResendPrimsEveryRenderFrame=false;Flags.CachedStaticFocalPos=false;Flags.RenderedLastFrame=false;Flags.PrimitivesDrawn=false;
 MainGraphic.Type=0;Persistence=ParentThing->Definition->Persistence;
 Flags.DisableOcclusionTesting=(ParentThing->Definition->Flags&1)!=0;
 Flags.DrawAfterWater=(ParentThing->Definition->Flags&2)!=0;
}