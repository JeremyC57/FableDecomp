// 00813d00 CTCBoastingPodium::FrameUpdate() -- updates boasting positions from the quest manager's boast list.
// Inline-budget: original ordinary LowerBound scores59 at the native expanded group loop and71 at the native called new-boast loop; default74 expands both. T70 is the first measured row-budget probe.
// Retail:0x00813D00,1372 bytes including the native switch table; code returns at0081424A.
// Flags: /d2inlT70
// Leaf: ?FrameUpdate@CTCBoastingPodium@@UAEXXZ
#include "fable_compiler.h"
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_POSTBOAST
#define FABLE_RETAIL_CQUESTMANAGER_PODIUM
#define FABLE_RETAIL_CTCBOASTINGPOSITION_PODIUM
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CVECTORMAP_PODIUM_UPPERBOUND
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CWORLD
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBoastingPosition.h"
#include "engine/CTCBoastingPodium.h"
#include "engine/CQuestManager.h"
#include "engine/CWorld.h"

// Native std::vector<long>::push_back,00485FF3..00486024,49 bytes; full original shared body.
// The actual external instantiation uses size optimization and a frame pointer.
#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
namespace std {
template<> void vector<long>::push_back(const long& x)
{
    if(this->_M_finish!=this->_M_end_of_storage._M_data){
        _Construct(this->_M_finish,x);++this->_M_finish;
    }else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
}
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif

void CTCBoastingPodium::FrameUpdate()
{
    if(FrameBoastListFrom==0||CQuestManager::Get().GetFrameBoastsLastUpdated()>=FrameBoastListFrom){
        const std::list<CQuestManager::CBoast>& boasts=CQuestManager::Get().GetBoasts();
        long number_of_boasts=boasts.size();
        BoastingActive=number_of_boasts>0;
        if(number_of_boasts>GetNumberOfActiveBoastingPositions()){
            for(long c1=0;c1<BoastingPositions.size();++c1){
                CTCBoastingPosition* ptc_boasting_position;
                BoastingPositions[c1].second->GetTC(&ptc_boasting_position);
                ptc_boasting_position->DeactivatePosition();
            }
            LastPositionsWithBoastCount=0;
            std::vector<long> position_indexes;
            switch(number_of_boasts){
                case 1:position_indexes.push_back(1);break;
                case 2:position_indexes.push_back(2);break;
                case 3:position_indexes.push_back(1);position_indexes.push_back(3);break;
                case 4:position_indexes.push_back(2);position_indexes.push_back(4);break;
            }
            std::list<CQuestManager::CBoast>::const_iterator boast_it=boasts.begin();
            for(long c1=0;c1<position_indexes.size();++c1){
                CVectorMap<long,CIntelligentPointer<CThing> >::iterator lower_bound=BoastingPositions.LowerBound(position_indexes[c1]);
                CVectorMap<long,CIntelligentPointer<CThing> >::iterator upper_bound=BoastingPositions.UpperBound(position_indexes[c1]);
                for(CVectorMap<long,CIntelligentPointer<CThing> >::iterator position_it=lower_bound;position_it!=upper_bound;++position_it){
                    CTCBoastingPosition* ptc_boasting_position;
                    position_it->second->GetTC(&ptc_boasting_position);
                    const CQuestManager::CBoast& boast=*boast_it;
                    ptc_boasting_position->ActivatePosition(boast.GetID(),boast.GetCost());
                    ++LastPositionsWithBoastCount;
                    ++boast_it;
                }
            }
        }else if(number_of_boasts>LastPositionsWithBoastCount){
            std::list<const CQuestManager::CBoast*> newboasts;
            {
            for(std::list<CQuestManager::CBoast>::const_iterator boast_it=boasts.begin(),end_it=boasts.end();boast_it!=end_it;++boast_it){
                bool is_new=true;
                for(long c1=0;c1<BoastingPositions.size();++c1){
                    CTCBoastingPosition* ptc_boasting_position;
                    BoastingPositions[c1].second->GetTC(&ptc_boasting_position);
                    if(ptc_boasting_position->HasBoast()&&boast_it->GetID()==ptc_boasting_position->GetBoastIndex()){
                        is_new=false;
                        break;
                    }
                }
                if(is_new)newboasts.push_back(&*boast_it);
            }
            }
            std::list<const CQuestManager::CBoast*>::iterator boast_it=newboasts.begin();
            for(long c1=0;c1<BoastingPositions.size();++c1){
                if(boast_it==newboasts.end())break;
                CTCBoastingPosition* ptc_boasting_position;
                BoastingPositions[c1].second->GetTC(&ptc_boasting_position);
                if(ptc_boasting_position->IsPositionActivated()&&!ptc_boasting_position->HasBoast()){
                    ptc_boasting_position->ActivatePosition((*boast_it)->GetID(),(*boast_it)->GetCost());
                    ++boast_it;
                    ++LastPositionsWithBoastCount;
                }
            }
        }else if(number_of_boasts==0){
            for(long c1=0;c1<BoastingPositions.size();++c1){
                CTCBoastingPosition* ptc_boasting_position;
                BoastingPositions[c1].second->GetTC(&ptc_boasting_position);
                if(ptc_boasting_position->IsPositionActivated())ptc_boasting_position->RemoveBoast();
            }
            LastPositionsWithBoastCount=0;
        }
        FrameBoastListFrom=PeekWorld().GetFrame();
    }
    GetParentThing().SetAsFrameUpdated();
    GetParentThing().SetAsRenderedThisFrame(4);
}
