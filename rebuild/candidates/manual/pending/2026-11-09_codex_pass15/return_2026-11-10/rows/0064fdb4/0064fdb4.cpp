// 0064fdb4 NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar::DrawAt -- draw a quest status bar, target icons and interpolated fill.
// Grade: DIFFER(2593v2593) -- frame 0x164 versus 0x16c; local homes and final fill geometry/UV ordering differ
// Inline-budget: native 00650781..006507b4 expands Clear; plain size mode calls it. The larger budget expands the original0043dbf0 predicate too, so only its0064fe09 call site is scoped back to an out-of-line call.
// Leaf: ?DrawAt@CQuestInfoElementBar@CDrawQuestInfo@NPlayerGui@@UAEMABVC2DVector@@ABVCEnginePrimitivePrepareInfo@@@Z
// Retail: 0064fdb4,2593 bytes,thiscall.
// Flags: /Os /d2inlS200
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "fable_compiler.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCTargetingBase.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
// Original floor conversion: native dword stores at 0064ff98,0064ffd3,006500c1,006500f3,006501d1,0065020c,0065024a,0065027c,00650359,0065043c,00650544.
// The existing shared helper preserves the original rounding and integer-bit equality fixup.
#include "engine/GFFloatToLongFloor.h"
template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);
// Actual null/dead predicate at0043dbf0; its visible body proves the pointer reference does not escape.
template<> bool FABLE_FASTCALL IS_THING_ALIVE<const CThingPlayerCreature*>(const CThingPlayerCreature* const& thing)
{
    if(thing && !thing->IsDead())return true;
    return false;
}
template<class T> bool FABLE_FASTCALL operator==(const CBaseObject* item,const CIntelligentPointer<T>& pointer);
float NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar::DrawAt(const C2DVector& position,const CEnginePrimitivePrepareInfo& info)
{
    if(ThingValid && (!IS_THING_ALIVE(PThing) || PThing->IsInLimbo()))return 0.0f;
    const CThingPlayerCreature* hero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if(IS_THING_ALIVE(hero)) {
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    const CPlayerGuiDef& def=CPlayerGui::PeekGuiDef();
    PrimitiveHandles.resize(6);
    const CGraphicDataBank& bank=CPlayerGui::Get().PeekGraphicBank();
    float width=(float)bank.GetGraphicMaxWidth(def.HeroSprintBarLeftGraphic);
    float height=(float)bank.GetGraphicMaxHeight(def.HeroSprintBarLeftGraphic);
    float maxHeight=height;
    float iconWidth=0.0f,iconHeight=0.0f;
    if(Graphic1>0) {
        iconWidth=(float)bank.GetGraphicMaxWidth(Graphic1);
        iconHeight=(float)bank.GetGraphicMaxHeight(Graphic1);
        maxHeight=std::max(maxHeight,iconHeight);
    }
    float otherWidth=0.0f;
    if(Graphic2>0) {
        otherWidth=(float)bank.GetGraphicMaxWidth(Graphic2);
        float otherHeight=(float)bank.GetGraphicMaxHeight(Graphic2);
        maxHeight=std::max(maxHeight,otherHeight);
    }
    float available=def.QuestInfoGraphicWidth-6.0f;
    C2DVector iconPos(position),barPos(position),fillPos(position);
    if(Graphic1>0) {
        available-=GFFloatToLongFloor(iconWidth*Scale);
        barPos.X+=GFFloatToLongFloor(iconWidth*Scale);
        float drawWidth=iconWidth,drawHeight=iconHeight;
        EEngine2DLayer layer=ENGINE_2D_LAYER_6;
        if(IS_THING_ALIVE(PThing)) {
            const CTCTargetingBase* targeting;
            hero->PeekTC(&targeting);
            if(targeting->GetPTargetedThing()==PThing) {
                layer=ENGINE_2D_LAYER_5;
                drawWidth*=1.2f;drawHeight*=1.2f;
                iconPos.X-=drawWidth*0.1f;iconPos.Y-=drawHeight*0.1f;
            }
        }
        C2DVector size((float)GFFloatToLongFloor(drawWidth*Scale),(float)GFFloatToLongFloor(drawHeight*Scale));
        C2DVector end1=iconPos+size;
        CEnginePrimitive2DSprite sprite(0,C2DBoxF(iconPos,end1),layer,Graphic1,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[0],sprite,0xc0);
    }
    if(Graphic2>0) {
        available-=GFFloatToLongFloor(otherWidth*Scale);
        fillPos.X+=GFFloatToLongFloor(iconWidth*Scale)+available;
        C2DVector size((float)GFFloatToLongFloor(otherWidth*Scale),(float)GFFloatToLongFloor(iconHeight*Scale));
        C2DVector end2=fillPos+size;
        CEnginePrimitive2DSprite sprite(0,C2DBoxF(fillPos,end2),ENGINE_2D_LAYER_6,Graphic2,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[1],sprite,0xc0);
    }
    fillPos.X=barPos.X;
    C2DVector leftSize(width,(float)GFFloatToLongFloor(height*Scale));
    C2DVector end3=barPos+leftSize;
    CEnginePrimitive2DSprite leftSprite(0,C2DBoxF(barPos,end3),ENGINE_2D_LAYER_6,def.HeroSprintBarLeftGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
    info.Engine->SetPersistentPrimitive(PrimitiveHandles[2],leftSprite,0xc0);
    barPos.X+=width;
    float middleWidth;
    {
        C2DVector size(available-width*2.0f,(float)GFFloatToLongFloor(height*Scale));
        C2DVector end(barPos.X+size.X,barPos.Y+size.Y);
        middleWidth=end.X-barPos.X;
        if(middleWidth>DELTA_NOTIONAL_ZERO) {
            CEnginePrimitive2DSprite sprite(0,C2DBoxF(barPos,end),ENGINE_2D_LAYER_6,def.HeroSprintBarMiddleGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
            info.Engine->SetPersistentPrimitive(PrimitiveHandles[3],sprite,0xc0);
        }
    }
    barPos.X+=middleWidth;
    C2DVector rightSize(width,(float)GFFloatToLongFloor(height*Scale));
    C2DVector end4=barPos+rightSize;
    CEnginePrimitive2DSprite rightSprite(0,C2DBoxF(barPos,end4),ENGINE_2D_LAYER_6,def.HeroSprintBarRightGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().GetAlpha(),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
    info.Engine->SetPersistentPrimitive(PrimitiveHandles[4],rightSprite,0xc0);
    float percent=Value/(MaxValue-MinValue);
    if(percent>DELTA_NOTIONAL_ZERO) {
        CRGBColour colour((unsigned char)(Colour.R*percent+FadeColour.R*(1.0f-percent)),(unsigned char)(Colour.G*percent+FadeColour.G*(1.0f-percent)),(unsigned char)(Colour.B*percent+FadeColour.B*(1.0f-percent)),(unsigned char)(Colour.A*percent+FadeColour.A*(1.0f-percent)));
        fillPos.X+=4.0f;
        C2DVector size((available-8.0f)*percent,height*Scale);
        C2DVector end5=fillPos+size;
        CEnginePrimitive2DSprite sprite(0,C2DBoxF(fillPos,end5),ENGINE_2D_LAYER_5,def.HeroSprintBarInnerGraphic,0,ENGINE_PRIMITIVE_BLEND_ALPHA,ENGINE_INTERPOLATION_SET_DEFAULT,0,0,CPlayerGui::Get().ScaleAlpha(colour.A),colour.R,colour.G,colour.B,2,C2DVector(0,0),C2DVector(0,0));
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[5],sprite,0xc0);
    } else PrimitiveHandles[5].Clear();
    return maxHeight*Scale+2.0f;
    }
    return 0.0f;
}
