// 008575c0 CCombatActionBase::GetNextPos
// Leaf: ?GetNextPos@CCombatActionBase@@UBE?AVC3DVector@@XZ
// Flags: /d2inlT100
#define FABLE_RETAIL_COMBAT_NEXT_POS_PASS27
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CRESOURCE_RESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CBANKFILEASYNC
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CBASEOBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMDURATION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_PEEKWORLD
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKMESHBANK
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#define FABLE_RETAIL_C3DANIMATIONINFO
#define FABLE_RETAIL_CCREATUREACTIONBASE_PRECEEDING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CMAIN_GAME_COMPONENT
#define FABLE_RETAIL_C3DANIMATIONSTATS_MOVEMENTVECTOR
#include "fable_assert.h"
#include "fable_string.h"
class CMeshDataBank; class CAnimationEntry;
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CRightHandedSet.h"
#include "engine/CQuaternion.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CMatrix3x4.h"
#include "engine/CAnimationEntry.h"
#include "engine/C3DAnimationStats.h"
#include "engine/C3DAnimationInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCFlourishTarget.h"

inline const CMeshDataBank* CCombatActionBase::PeekMeshBank() const { return PeekWorld().PeekMeshBank(); }
inline float CCombatActionBase::GetTimeThroughCurrentAnimation() const {
    float time = RunningTimeSeconds - (float)GetAnimDelayTime() / CMainGameComponent::GetConstantFPS();
    if (time < 0.0f) time = 0.0f;
    return time;
}
inline float CCombatActionBase::GetLastTimeThroughCurrentAnimation() const {
    float time = LastRunningTimeSeconds - (float)GetAnimDelayTime() / CMainGameComponent::GetConstantFPS();
    if (time < 0.0f) time = 0.0f;
    return time;
}
C3DVector CCombatActionBase::GetNextPos() const {
    const long fps=CMainGameComponent::GetConstantFPS();
    C3DVector movement_vector;
    long anim_index=GetAnimationIndex();
    if (PreferredFinalPosSet) {
        long world_tick_anim_duration=(long)(PCreature->GetAnimDuration(anim_index)*fps);
        movement_vector=PreferredMovementVector/(float)TotalTime;
    } else {
        const CAnimationEntry* panim=GetPAnimationEntry().Get();
        const C3DAnimationInfo* info=PeekMeshBank()->PeekAnimation(anim_index);
        CCountedPointer<const C3DAnimationStats> stats=info->GetAnimationStats();
        float current_time=GetTimeThroughCurrentAnimation();
        float last_time=GetLastTimeThroughCurrentAnimation();
        const C3DAnimationStats* anim_stats=stats.Get();
        CMatrix3x4 delta;
        anim_stats->GetDeltaMovement(delta,last_time,current_time);
        C3DVector delta_position_movement=delta.GetTranslation();
        PCreature->GetTCGraphicAppearance()->GetPositionMatrix(1.0f).TransformVector(delta_position_movement);
        CTCPhysicsBase* ptc_physics=PCreature->GetPhysicsTC();
        const CRightHandedSet& rhs=ptc_physics->GetRHSet();
        C3DVector forward=rhs.GetForward();
        C3DVector up=rhs.GetUp();
        if (anim_stats->HasRotation()) {
            delta.TransformVector(forward);
            delta.TransformVector(up);
            // The debug build checks each transformed component for NaN.
            FABLE_RETAIL_ASSERT(forward.X == forward.X);
            FABLE_RETAIL_ASSERT(forward.Y == forward.Y);
            FABLE_RETAIL_ASSERT(forward.Z == forward.Z);
            FABLE_RETAIL_ASSERT(up.X == up.X);
            FABLE_RETAIL_ASSERT(up.Y == up.Y);
            FABLE_RETAIL_ASSERT(up.Z == up.Z);
        }
        ptc_physics->SetRHSet(CRightHandedSet(up,forward));
        movement_vector=delta_position_movement;
        float xy=PCreature->GetPhysicsTC()->GetFacingAngleXY();
        C3DVector facing=PCreature->GetPhysicsTC()->GetFacingVector();
    }
    const CTCFlourishTarget* ptc_flourish_target;
    if (PCreature->PeekTC(&ptc_flourish_target)) ptc_flourish_target->ProcessActionMovementDelta(movement_vector);
    C3DVector next_pos=PCreature->GetPos()+movement_vector;
    return next_pos;
}