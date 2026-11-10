// 008e7f80 NCreatureMode::CModeBriarRoseConcentrate::GetRandomImposter -- choose an alive imposter after counting those not hit.
// Leaf: ?GetRandomImposter@CModeBriarRoseConcentrate@NCreatureMode@@QBEPAVCThing@@XZ
// Flags: none
// Retail:008e7f80,223 bytes.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/NCreatureMode_CModeBriarRoseConcentrate.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#include "stlport/fable_count_if.h"
namespace {
class CIsImposterHit:public std::unary_function<const CIntelligentPointer<CThing>,bool> {
public:
 bool operator()(const CIntelligentPointer<CThing>& pthing)const;
};
}
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
bool CIsImposterHit::operator()(const CIntelligentPointer<CThing>& pthing)const
{
 CTCCreatureModeManager* ptc_cmm;
 if(IS_THING_ALIVE(pthing)&&pthing->GetTC(&ptc_cmm)&&ptc_cmm->IsModeActive(0x3f))return false;
 return true;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
CThing* NCreatureMode::CModeBriarRoseConcentrate::GetRandomImposter()const
{
 long num_not_hit_imposters=std::count_if(Imposters.begin(),Imposters.end(),std::not1(CIsImposterHit()));
 if(num_not_hit_imposters>0){
  long random_i=GFRandom(num_not_hit_imposters,GetWorld().GetWorldSeed());
  for(long c1=0;c1!=Imposters.size();++c1){
   if(IS_THING_ALIVE(Imposters[c1])){
    if(random_i--==0)return Imposters[c1].Get();
   }
  }
 }
 return 0;
}