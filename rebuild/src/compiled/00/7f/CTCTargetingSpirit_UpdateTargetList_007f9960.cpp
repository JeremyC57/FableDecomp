// CTCTargetingSpirit::UpdateTargetList() -- rebuilds the spirit's target choice: takes the parent thing's position, builds a
// 2D search box of half-size (long)(MaxTargetingDistance+1) around it, collects the four players' controlled creatures into an
// exclusion list, gathers every targetable thing in the box that is not a player creature and lies within MaxTargetingDistance
// in 3D (GFFilter_Bind3(IsNotList, IsTargetable(false,false), In3DRange)), sorts them with CThingCompare_BestForTargeting2
// (facing-weighted score) and targets the best one (or none).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F9960, 551 bytes, thiscall `ret`, protected non-virtual
//   (Ego_r.pdb + FableWin.pdb ?UpdateTargetList@CTCTargetingSpirit@@IAEXXZ; sole caller FrameUpdate 0x007F9B90 tail-jumps here).
// Callees: CThing::GetPos 0x004C73D0, CTCPhysicsBase::GetFacingVector (vslot +0x118; first result dead), ftol 0x00BFEA70,
//   CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970 (result dead) / GetPlayer 0x004498C0,
//   CPlayer::GetPControlledCreature 0x00487DC0, vector<const CThing*>::_M_insert_overflow 0x0049C370,
//   CTCBase::GetThingSearchTools 0x00686D80, CThingSearchTools::GetAllThingsInRoughArea<CThing,vector<CThing*>,CFilter_Bind3<IsNotList,IsTargetable,In3DRange> > 0x007F9C00,
//   std::sort<CThing**,CThingCompare_BestForTargeting2> 0x007FAE60 (__fastcall), CTCTargetingBase::SetPTargetedThing
//   (vslot +0x88), free 0x00BFEA14 (vector dtors). Float pool 1.0f @ 0x0122DED8.
// Shape: GetParentThing() spelled three times in the comparator (CSE'd by c2; a named reference local swaps edi/ebp);
//   the result pick is an if-assignment (a ?: emits an extra jump). Overlay headers: CThingFilter_IsNotList inline ctor,
//   new engine/CTCTargetingSpirit.h (full retail layout, sizeof 0x54).
// Leaf: ?UpdateTargetList@CTCTargetingSpirit@@IAEXXZ
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCSTOCKITEM_TARGETING
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_RAW_NOT_EQUAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#include "engine/EPlayerMode.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTargetingDef.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CWorld.h"
#include "engine/CCamera.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCCarrying.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_EnemyCreature.h"
#include "engine/CTCEnemy.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCStockItem.h"
#include "engine/TargetingPlayerSupport.h"
#include "stlport/targeting_algorithms.h"
extern const float DELTA_NOTIONAL_ZERO;
#include "engine/CMainGameComponent.h"
#define FABLE_RETAIL_THINGFILTER_ISNOTLIST_INLINE_CTOR
#include "engine/CThingFilter_IsNotList.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingCompare_BestForTargeting2.h"
#include "engine/CPlayerManager.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCTargetingSpirit.h"

// CThingFilter_In3DRange (no shared header; Ego_r: Pos C3DVector +0, Range float +0xC).
class CThingFilter_In3DRange : public std::unary_function<CThing,bool> {
public:
    CThingFilter_In3DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) {}
    bool operator()(const CThing& thing) const;
private:
    C3DVector Pos;
    float     Range;
};


void CTCTargetingSpirit::UpdateTargetList()
{
    C3DVector pos = GetParentThing().GetPos();
    C3DVector facing = GetParentThing().GetPhysicsTC()->GetFacingVector();
    long max_dist = (long)MaxTargetingDistance;
    C2DBoxI area(pos.ToC2DCoordI(), (long)(MaxTargetingDistance + 1.0f));
    CThingPlayerCreature* pmain = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    std::vector<CThing*> targets;
    std::vector<const CThing*> players;
    for (long i = 0; i < 4; ++i) {
        const CThing* pcreature = GetPlayerManager().GetPlayer(i).GetPControlledCreature();
        if (pcreature)
            players.push_back(pcreature);
    }
    GetThingSearchTools().GetAllThingsInRoughArea<CThing>(area,
        GFFilter_Bind3(CThingFilter_IsNotList(players), CThingFilter_IsTargetable(false, false),
                       CThingFilter_In3DRange(pos, (float)max_dist)), targets);
    std::sort(targets.begin(), targets.end(),
        CThingCompare_BestForTargeting2(GetParentThing(), GetParentThing().GetPos(), GetParentThing().GetPhysicsTC()->GetFacingVector(), 1.0f, 2.0f, 0.005f, 0.0f, 1.0f));
    CThing* pbest = 0;
    if (!targets.empty())
        pbest = targets[0];
    SetPTargetedThing(pbest);
}