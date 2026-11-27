// 00694890 CCreatureActionBase::UpdateActionAnimation
// Flags: none
#define FABLE_RETAIL_CANIMSET_TYPE
#define FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24
// Leaf: ?UpdateActionAnimation@CCreatureActionBase@@QAEXXZ
#define FABLE_RETAIL_UPDATE_ACTION_ANIMATION_PASS29
#define FABLE_RETAIL_CTHING_PEEKWORLD_FRIEND_CREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_UPDATEANIM
#define FABLE_RETAIL_CCREATUREACTIONBASE_PEEKWORLD
#define FABLE_RETAIL_CTHINGCREATUREBASE_UPDATEANIMATION
#define FABLE_RETAIL_CANIMATETHING_SETASFINISHED
#define FABLE_RETAIL_CANIMATETHING_REMOVE_COMBINATION_CALL_PASS24
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CANIMINIT_STRAFE_CALLED_PASS24
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CWORLD_PEEKMESHBANK
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#define FABLE_RETAIL_C3DANIMATIONINFO_LOADING
#define FABLE_RETAIL_CANIMATIONENTRY
#define FABLE_RETAIL_CANIMATIONENTRY_GETFLAGS
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CAnimationEntry.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CAnimInit.h"
#include "engine/CAnimSet.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DAnimationInfo.h"
#include "engine/NAnimationEvents_CManager.h"
#include "engine/GFLimit.h"
// Inline budget: retail calls make_pair at 00694AA4 and the vector destructor at 00694C0E.
// Keeping both original bodies visible preserves the event_frame stack slot and their standalone copies.
#ifdef _MSC_VER
#define FABLE_ACTION_ANIM_NOINLINE __declspec(noinline)
#else
#define FABLE_ACTION_ANIM_NOINLINE
#endif
namespace std {
template<> FABLE_ACTION_ANIM_NOINLINE inline vector<CCharString>::~vector()
{
    _Destroy(this->_M_start, this->_M_finish);
}
template<> FABLE_ACTION_ANIM_NOINLINE inline pair<long,CCharString> FABLE_CDECL make_pair(const long& a, const CCharString& b)
{
    return pair<long,CCharString>(a,b);
}
}
#undef FABLE_ACTION_ANIM_NOINLINE

extern const CCharString PERFORM_ACTION;
extern const CCharString CUT_SCENE_EVENT_PREFIX;
namespace {
void FABLE_FASTCALL GetActionAnimSetList(std::vector<CAnimSet*>& anim_sets,CThingCreatureBase& creature,long anim_index) {
    CTCAnimationComplex* ptc_anim;
    creature.GetTC(&ptc_anim);
    CAnimateThing& anim = ptc_anim->GetAnimation();
    if (creature.IsAnimCombinationAnim(anim_index)) {
        std::vector<CAnimSet*> comb_anims;
        anim.GetAllCombinationAnimsFromType(1,comb_anims);
        if (false) {} else for (std::vector<CAnimSet*>::const_iterator it = comb_anims.begin(); it != comb_anims.end(); ++it) {
            CAnimSet* pcomb_anim = *it;
            if (pcomb_anim->GetAnimIndex() == anim_index)
                anim_sets.push_back(pcomb_anim);
        }
    } else {
        CAnimSet& main_anim_set = anim.GetMainAnim();
        if (main_anim_set.GetAnimIndex() == anim_index && main_anim_set.GetType() == 1)
            anim_sets.push_back(&main_anim_set);
    }
}
CAnimSet* FABLE_FASTCALL GetPActionAnimSet(CThingCreatureBase& creature,long anim_index) {
    std::vector<CAnimSet*> anim_sets;
    GetActionAnimSetList(anim_sets,creature,anim_index);
    long size_guy = anim_sets.size();
    if (size_guy == 1)
        return *anim_sets.begin();
    return 0;
}
}
inline const CWorld& CCreatureActionBase::PeekWorld() const {return PCreature->PeekWorld();}
void CCreatureActionBase::UpdateActionAnimation()
{
    if (!PAnimEntry.Get() || !PAnimEntry->BankIndex)
        return;
    long const_fps = CMainGameComponent::GetConstantFPS();
    CTCAnimationComplex* ptc_anim;
    PCreature->GetTC(&ptc_anim);
    CAnimateThing& anim = ptc_anim->GetAnimation();
    bool combination_anim = PCreature->IsAnimCombinationAnim(PAnimEntry->BankIndex);
    if (!combination_anim && !AddedActionAnim)
        anim.SetAsFinished();
    if (!AddedActionAnim) {
        bool waitForMesh = false;
        if (WaitForMeshToLoad() && !PCreature->GetTCGraphicAppearance()->IsResourcesLoaded(true))
            waitForMesh = true;
        if (!waitForMesh && PeekWorld().PeekMeshBank()->PeekAnimation(PAnimEntry->BankIndex)->IsLoaded()) {
            InitialiseActionPause();
            InitialiseActionTime();
            InitialiseActionEvents();
            const NAnimationEvents::CManager& game_events_manager = NAnimationEvents::GetGameEventManager();
            long anim_index = GetAnimationIndex();
            if (game_events_manager.AnimHasEvent(anim_index, PERFORM_ACTION)) {
                ActionTime = (long)(game_events_manager.GetEventTimeInSeconds(anim_index, PERFORM_ACTION) * const_fps);
                ActionTime += GetAnimDelayTime();
                GFLimit(ActionTime, 0, TotalTime);
            }
            std::vector<CCharString> anim_event_names;
            game_events_manager.GetEventNames(anim_index, anim_event_names);
            if (false) {} else for (std::vector<CCharString>::const_iterator event_it = anim_event_names.begin();
                 event_it != anim_event_names.end(); ++event_it) {
                const CCharString& event_name = *event_it;
                if (event_name.StartsWith(CUT_SCENE_EVENT_PREFIX)) {
                    long event_frame = (long)(game_events_manager.GetEventTimeInSeconds(anim_index, event_name) *
                                              const_fps + GetAnimDelayTime());
                    CutSceneEvents.insert(std::make_pair(event_frame, event_name));
                }
            }
            long trans_time = GetAnimationTransitionInTime();
            long flags = GetAnimExtraFlags();
            if (combination_anim) {
                CAnimInit anim_init(1, 0, PAnimEntry->BankIndex, ANIM_UPDATE_NONE, false, flags, 100);
                anim.AddCombinationAnim(anim_init, trans_time, 0, true, false);
            } else {
                long anim_flags = PAnimEntry->GetFlags();
                if (!CanAnimationTransitionOut())
                    flags |= 2;
                if (!AllowsCombinationAnims())
                    flags |= 1;
                if (OverrideFaceWithDefault())
                    flags |= 4;
                CAnimInit anim_init(1, 0, PAnimEntry->BankIndex, ANIM_UPDATE_NONE, false, flags, 100);
                anim.SetMainAnim(anim_init, trans_time, 0);
            }
            std::vector<CAnimSet*> anim_sets;
            GetActionAnimSetList(anim_sets, *PCreature, PAnimEntry->BankIndex);
            if (anim_sets.size() == 1) {
                AddedActionAnim = true;
                OnAnimationLoaded();
            } else {
                if (false) {} else for (std::vector<CAnimSet*>::iterator it = anim_sets.begin(); it != anim_sets.end(); ++it) {
                    CAnimSet* panim_set = *it;
                    anim.RemoveCombinationAnim(panim_set);
                }
            }
        } else if (!PeekWorld().PeekMeshBank()->PeekAnimation(PAnimEntry->BankIndex)->IsLoading()) {
            PeekWorld().PeekMeshBank()->PeekAnimation(PAnimEntry->BankIndex)->BeginLoading(false);
        }
    } else {
        CAnimSet* panim_set = GetPActionAnimSet(*PCreature, PAnimEntry->BankIndex);
        if (panim_set) {
            const CAnimFrameInfo& frame_info = panim_set->GetFrame();
            float delay_time = (float)GetAnimDelayTime() / const_fps;
            float total_time = (float)TotalTime / const_fps;
            float assumed_running_time = RunningTimeSeconds;
            float cur_frame = (assumed_running_time - delay_time) / (total_time - delay_time);
            GFLimit(cur_frame, 0.0f, 1.0f);
            float last_frame = frame_info.Current;
            panim_set->SetFrame(CAnimFrameInfo(cur_frame, last_frame));
        }
    }
}