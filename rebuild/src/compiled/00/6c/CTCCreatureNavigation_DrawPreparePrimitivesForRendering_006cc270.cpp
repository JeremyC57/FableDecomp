// 006cc270 CTCCreatureNavigation::DrawPreparePrimitivesForRendering
// Flags: none
// Leaf: ?DrawPreparePrimitivesForRendering@CTCCreatureNavigation@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
#define FABLE_RETAIL_CREATURE_NAV_ROUTE_PASS29
#define FABLE_RETAIL_STLP_LIST_CONST_FRONT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTZBELOW
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_FRAMEUPDATE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVELINE_INLINE_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#include "engine/CNavigatorAStar.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CIEngine.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/GFDebugPDynamicCast.h"
extern const unsigned long NAVIGATOR_TYPE_A_STAR;
void CTCCreatureNavigation::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    static float vertical_offset = 1.0f;
    if (!DrawNavRoutes || GetNavigatorType() != NAVIGATOR_TYPE_A_STAR) return;
    CNavigatorAStar* a_star = GFDebugPDynamicCast(PNavigator,(CNavigatorAStar*)0);
    const std::list<CNavWayPoint>* route = a_star->GetRoute();
    if (!route->size()) return;
    if(false) {} else for (std::list<CNavWayPoint>::const_iterator point=route->begin(); point!=route->end(); ++point) {
        std::list<CNavWayPoint>::const_iterator next=point;
        if (++next==route->end()) break;
 C3DVector start,end;
 start.X=point->Position.X; start.Y=point->Position.Y; start.Z=1000.0f; start.Z=GetParentCreature().GetHeightZBelowAt(start,false,true)+vertical_offset;
 end.X=next->Position.X; end.Y=next->Position.Y; end.Z=1000.0f; end.Z=GetParentCreature().GetHeightZBelowAt(end,false,true)+vertical_offset;
 CEnginePrimitiveLine line(0,start,end,CRGBColour(0,255,0),0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_0,ENGINE_PRIMITIVE_BLEND_OP_ADD);
 info.Engine.AddPrimitive(line);
 end=start; end.Z+=0.4f; line.Start=start; line.End=end; line.Colour=CRGBColour(0,0,255); info.Engine.AddPrimitive(line);
 }
 C3DVector start,end;
 start.X=GetParentCreature().GetPos().X; start.Y=GetParentCreature().GetPos().Y; start.Z=1000.0f; start.Z=GetParentCreature().GetHeightZBelowAt(start,false,true)+vertical_offset;
 end.X=route->front().Position.X; end.Y=route->front().Position.Y; end.Z=1000.0f; end.Z=GetParentCreature().GetHeightZBelowAt(end,false,true)+vertical_offset;
 CEnginePrimitiveLine line(0,start,end,CRGBColour(255,0,0),0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_0,ENGINE_PRIMITIVE_BLEND_OP_ADD);
 info.Engine.AddPrimitive(line);
 end=start; end.Z+=vertical_offset; line.Start=start; line.End=end; line.Colour=CRGBColour(255,0,0); info.Engine.AddPrimitive(line);
 std::vector<C3DVector> line_list; a_star->CreateLineList(line_list);
 if(false) {} else for (std::vector<C3DVector>::iterator line_it=line_list.begin();line_it!=line_list.end();++line_it) {
 C3DVector start_pos=*line_it++; start_pos.Z=1000.0f; start_pos.Z=GetParentCreature().GetHeightZBelowAt(start_pos,false,true)+vertical_offset;
 C3DVector end_pos=*line_it; end_pos.Z=1000.0f; end_pos.Z=GetParentCreature().GetHeightZBelowAt(end_pos,false,true)+vertical_offset;
 CEnginePrimitiveLine loop_line(0,start_pos,end_pos,CRGBColour(0,255,0),0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_0,ENGINE_PRIMITIVE_BLEND_OP_ADD);
 info.Engine.AddPrimitive(loop_line);
 }
}