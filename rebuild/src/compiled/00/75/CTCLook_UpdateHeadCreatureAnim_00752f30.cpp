// CTCLook::UpdateHeadCreatureAnim -- maintain the selected head animation and frame history.
// Retail: 00752f30,226 bytes.
// Leaf: ?UpdateHeadCreatureAnim@CTCLook@@AAEXABVCCharString@@W4ECreatureAnim@@PAVCAnimateThing@@@Z
// Flags: none
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#include "engine/CTCLook.h"
#include "engine/CAnimateThing.h"
#include "engine/NAnimationEntryBodge_CAnimEntryLite.h"
void CTCLook::UpdateHeadCreatureAnim(const CCharString& anim_name,ECreatureAnim creature_anim,CAnimateThing* p_anims)
{
 long bank_index=-1;
 if(creature_anim==11 && PHeadTurnAnimH.Get())bank_index=PHeadTurnAnimH->BankIndex;
 else if(creature_anim==12 && PHeadTurnAnimV.Get())bank_index=PHeadTurnAnimV->BankIndex;
 if(bank_index!=-1) {
  CAnimSet* p_anim_state=p_anims->GetFirstCombinationAnimFromType(creature_anim);
  if(!p_anim_state) {
   CAnimInit anim_init((unsigned char)creature_anim,0,bank_index,(EAnimUpdateMode)0,false,0,100);
   anim_init.Frame=CAnimFrameInfo(0.5f);
   p_anims->AddCombinationAnim(anim_init,0,0,false,false);
  }
  p_anim_state=p_anims->GetFirstCombinationAnimFromType(creature_anim);
  if(p_anim_state) {
   CAnimFrameInfo frame;
   frame.Last=p_anim_state->GetFrame().Current;
   frame.Current=GetHeadCreatureAnimFrame(creature_anim);
   p_anim_state->SetFrame(frame);
  } else {
   HeadAngleRelToBodyXY=0.0f;HeadAngleRelToBodyYZ=0.0f;
   HeadVelocityXY=0.0f;HeadVelocityYZ=0.0f;
  }
 }
}