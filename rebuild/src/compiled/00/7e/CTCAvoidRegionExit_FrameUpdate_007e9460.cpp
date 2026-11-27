// 007e9460
// Leaf: ?FrameUpdate@CTCAvoidRegionExit@@UAEXXZ
// Flags: /O2 /Oy /Gr
// Inline-budget: The native ExitMarker assignment is called at the marker-selection site while the surrounding scan remains expanded.
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY_DISTANCE
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ARROW_ASSERTED_BODY
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_AVOID_REGION_FRAME_PASS29
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ADD2D
#define FABLE_RETAIL_C3DVECTOR_ADD2D_DIRECT
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_GETRIGHT_OUTOFLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CBASEOBJECT
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY_CALLS

#include "engine/C2DVector.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCAvoidRegionExit.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CTCDRegionExit.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
template<> inline bool FABLE_FASTCALL IS_THING_DEAD(const CIntelligentPointer<CThing>& thing)
{
    if (!(thing == 0)) {
        if (!thing->IsDead()) return false;
    }
    return true;
}
void CTCAvoidRegionExit::FrameUpdate()
{
    if (!MarkersAcquired) {MarkersAcquired=true; UpdateMarkers();}
    if (FrameLastUpdated!=CWorld::GetFrame()) {FrameLastUpdated=CWorld::GetFrame();UpdateBestDist();}
    if (NextFrameCheck>(unsigned long)CWorld::GetFrame()) return;
    NextFrameCheck=CWorld::GetFrame()+5;
    CTCAIScratchpad* scratchpad=0;
    if(GetParentThing().GetTC(&scratchpad)) scratchpad->SetTargetingDistanceOffset(0.0f);
    DistMap.erase(&GetParentThing());
    if(!ExitMarkers.size() || !EntranceMarkers.size()) return;
    float dist=0.0f;
    ExitMarker=CIntelligentPointer<CThing>(0); EntranceMarker=CIntelligentPointer<CThing>(0);
    if (false) {} else for(std::list<CIntelligentPointer<CThing> >::iterator it=ExitMarkers.begin();it!=ExitMarkers.end();++it) {
        if(IS_THING_DEAD(*it)){UpdateMarkers();return;}
        CTCDRegionExit* exit;
        if(!(*it)->GetTC(&exit))continue;
        float radius=exit->GetRadius(); CThing& parent=GetParentThing();
        if(!((parent.GetPos()-(*it)->GetPos()).GetMagnitude()<radius*2.0f))continue;
        C2DVector right((*it)->GetPhysicsTC()->GetRHSet().GetRight()*radius);
        C2DVector front((*it)->GetPos()+right),back((*it)->GetPos()-right);
        C2DVector pos(GetParentThing().GetPos());
        if((pos-back).Dot(right)>0.0f && (pos-front).Dot(right)<0.0f){ const CIntelligentPointer<CThing>& pick=*it;
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
 ExitMarker=pick;
#ifdef _MSC_VER
#pragma inline_depth(8)
#endif
}
    }
    if(ExitMarker==0)return;
    C2DVector forward(ExitMarker->GetPhysicsTC()->GetRHSet().GetForward());
    CThing& parent=GetParentThing(); if(!(C2DVector(parent.GetPos()-ExitMarker->GetPos()).Dot(forward)>0.0f)) return;
    if (false) {} else for(std::list<CIntelligentPointer<CThing> >::iterator it=EntranceMarkers.begin();it!=EntranceMarkers.end();++it) {
        if(IS_THING_DEAD(*it)){UpdateMarkers();return;}
        CThing* thing=(*it).operator->(); float candidate=(thing->GetPos()-ExitMarker->GetPos()).GetSquaredMagnitude();
        if(EntranceMarker==0 || dist>candidate){EntranceMarker=*it;dist=candidate;}
    }
    dist=GFRoot(dist);
    if(scratchpad)scratchpad->SetTargetingDistanceOffset(dist);
    DistMap[&GetParentThing()]=dist;
}