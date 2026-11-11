#include <stdio.h>
#include <string.h>
static int failed,line_failed,tn,has_calls,base_calls,peek_calls,get_sound_calls,quick_index_calls,context_index_calls,quick_event_calls,context_event_calls,play_calls,string_calls,string_dtors;
static int live_strings,string_instances,string_copies;
static char trace[64];static void note(char c){trace[tn++]=c;trace[tn]=0;}
#define CHECK(x) do{if(!(x)){failed=1;line_failed=__LINE__;return;}}while(0)
enum EInputButtonEventType{INPUT_EVENT_TEST_VALUE=2};
class CCharString {public:int id;bool alive;CCharString(int i):id(i),alive(true){++string_instances;++live_strings;}CCharString(const CCharString&s):id(s.id),alive(true){if(!s.alive)failed=1;++string_copies;++string_instances;++live_strings;}~CCharString(){if(!alive)failed=1;alive=false;--live_strings;++string_dtors;}};
class CDefString {public:int id;bool operator!=(const char*text)const{note('N');if(*text)failed=1;return id!=0;}CCharString GetString()const{++string_calls;note('G');return CCharString(id);}operator CCharString()const{return GetString();}};
class CExpressionDef {public:CDefString SoundCriteria,LoopSoundCriteria;};
static long expected_item,expected_device,quick_button,context_button;static bool quick_yes,context_yes,inventory_present,sound_present;static unsigned long handle_value;static int expected_sound;
class CTCInventory {public:long GetQuickAccessItemButtonIndex(long item)const{++quick_index_calls;note('q');if(item!=expected_item)failed=1;return quick_button;}long GetContextSensitiveItemButtonIndex(long item)const{++context_index_calls;note('c');if(item!=expected_item)failed=1;return context_button;}};
class CGamePlayerInterface{public:bool IsQuickAccessEventInQueue(long button,EInputButtonEventType type,long device)const{++quick_event_calls;note('Q');if(button!=quick_button||type!=2||device!=expected_device)failed=1;return quick_yes;}bool IsContextSensitiveEventInQueue(long button,EInputButtonEventType type,long device)const{++context_event_calls;note('C');if(button!=context_button||type!=2||device!=expected_device)failed=1;return context_yes;}};
class CPlayer{public:CGamePlayerInterface*PlayerInterface;long GetJoystickDeviceNumber()const{return expected_device;}CGamePlayerInterface*GetPlayerInterface(){return PlayerInterface;}};
class CTCSoundPlayer{public:unsigned long PlayAttachedCriteria(const CCharString&name,bool a,bool b,bool c,long delay){++play_calls;note('P');if(!name.alive||live_strings<=0||name.id!=expected_sound||!a||b||c||delay!=-1)failed=1;return handle_value;}};
class CThingCreatureBase{public:CPlayer*player;CTCInventory*inventory;CTCSoundPlayer*sound;CPlayer&GetPlayer(){return *player;}bool PeekTC(const CTCInventory**out)const{++peek_calls;note('I');if(inventory_present)*out=inventory;return inventory_present;}bool GetTC(CTCSoundPlayer**out){++get_sound_calls;note('T');if(sound_present)*out=sound;return sound_present;}};
template<class T>class CIntelligentPointer {public:T*item;T*operator->()const{return item;}};
template<class T>class CDefPointer{public:T*Object;T*operator->()const{return Object;}};
class CCreatureActionBase{public:CIntelligentPointer<CThingCreatureBase>PCreature;bool performed,after;bool HasPerformedAction()const{++has_calls;note('H');return performed;}virtual void FrameUpdate();};
class CCreatureAction_PerformExpression:public CCreatureActionBase {public:CDefPointer<const CExpressionDef>PExpressionDef;long ExpressionItemDefIndex;bool ButtonHeld;unsigned long SoundEffectID;virtual void FrameUpdate();};
static int base_held_mode;
void CCreatureActionBase::FrameUpdate(){++base_calls;note('B');performed=after;if(base_held_mode)static_cast<CCreatureAction_PerformExpression*>(this)->ButtonHeld=base_held_mode==2;}
void CCreatureAction_PerformExpression::FrameUpdate()
{
 ButtonHeld=false;
 if(ExpressionItemDefIndex>0) {
  const CTCInventory* ptc_inventory=0;
  if(PCreature->PeekTC(&ptc_inventory)) {
   long button_index=ptc_inventory->GetQuickAccessItemButtonIndex(ExpressionItemDefIndex);
   if(button_index!=-1) {
    if(PCreature->GetPlayer().GetPlayerInterface()->IsQuickAccessEventInQueue(button_index,(EInputButtonEventType)2,PCreature->GetPlayer().GetJoystickDeviceNumber()))ButtonHeld=true;
   }
   if(!ButtonHeld) {
    button_index=ptc_inventory->GetContextSensitiveItemButtonIndex(ExpressionItemDefIndex);
    if(button_index!=-1) {
     if(PCreature->GetPlayer().GetPlayerInterface()->IsContextSensitiveEventInQueue(button_index,(EInputButtonEventType)2,PCreature->GetPlayer().GetJoystickDeviceNumber()))ButtonHeld=true;
    }
   }
  }
 }
 if(!HasPerformedAction()) {
  CCreatureActionBase::FrameUpdate();
  if(HasPerformedAction()) {
   CTCSoundPlayer* ptc_sound=0;
   if(PCreature->GetTC(&ptc_sound)) {
    if(ButtonHeld && PExpressionDef->LoopSoundCriteria!="") {
     CCharString sound=PExpressionDef->LoopSoundCriteria.GetString();
     SoundEffectID=ptc_sound->PlayAttachedCriteria(sound,true,false,false,-1);
    }
    else ptc_sound->PlayAttachedCriteria(PExpressionDef->SoundCriteria,true,false,false,-1);
   }
  }
 } else CCreatureActionBase::FrameUpdate();
}

static void one(long item,int inv,long qb,long cb,int events,int before,int after,int sound,int loop,int held_mode,int handle,int initial_held)
{
 CTCInventory inventory;CTCSoundPlayer player_sound;CGamePlayerInterface input;CPlayer player;player.PlayerInterface=&input;CThingCreatureBase creature;creature.player=&player;creature.inventory=&inventory;creature.sound=&player_sound;
 CExpressionDef def;def.SoundCriteria.id=11;def.LoopSoundCriteria.id=loop?22:0;
 CCreatureAction_PerformExpression obj;obj.PCreature.item=&creature;obj.PExpressionDef.Object=&def;obj.ExpressionItemDefIndex=item;obj.ButtonHeld=initial_held!=0;obj.SoundEffectID=123;obj.performed=before!=0;obj.after=after!=0;
 expected_item=item;expected_device=17;quick_button=qb;context_button=cb;quick_yes=(events&1)!=0;context_yes=(events&2)!=0;inventory_present=inv!=0;sound_present=sound!=0;base_held_mode=held_mode;unsigned long handles[]={0,128,0xffffffffUL};handle_value=handles[handle];
 bool valid=item>0&&inv;bool quick_check=valid&&qb!=-1;bool held=quick_check&&quick_yes;bool context_lookup=valid&&!held;bool context_check=context_lookup&&cb!=-1;if(context_check&&context_yes)held=true;
 if(held_mode)held=held_mode==2;bool play=!before&&after&&sound;bool looping=play&&held&&loop;expected_sound=looping?22:11;
 failed=0;tn=0;trace[0]=0;has_calls=base_calls=peek_calls=get_sound_calls=quick_index_calls=context_index_calls=quick_event_calls=context_event_calls=play_calls=string_calls=string_dtors=0;live_strings=string_instances=string_copies=0;
 obj.FrameUpdate();CHECK(!failed);CHECK(obj.ButtonHeld==held);CHECK(obj.SoundEffectID==(looping?handle_value:123));CHECK(obj.performed==(after!=0));CHECK(has_calls==(before?1:2));CHECK(base_calls==1);CHECK(peek_calls==(item>0?1:0));CHECK(quick_index_calls==(valid?1:0));CHECK(context_index_calls==(context_lookup?1:0));CHECK(quick_event_calls==(quick_check?1:0));CHECK(context_event_calls==(context_check?1:0));CHECK(get_sound_calls==(!before&&after?1:0));CHECK(play_calls==(play?1:0));CHECK(string_calls==(play?1:0));CHECK(live_strings==0);CHECK(string_dtors==string_instances);CHECK(string_instances==string_calls+string_copies);
 char expected[64]="";if(item>0){strcat(expected,"I");if(inv){strcat(expected,"q");if(quick_check)strcat(expected,"Q");if(context_lookup){strcat(expected,"c");if(context_check)strcat(expected,"C");}}}strcat(expected,"HB");if(!before){strcat(expected,"H");if(after){strcat(expected,"T");if(sound){if(held)strcat(expected,"N");strcat(expected,"GP");}}}CHECK(strcmp(trace,expected)==0);CHECK(obj.PCreature.item==&creature&&obj.PExpressionDef.Object==&def);CHECK(def.SoundCriteria.id==11&&def.LoopSoundCriteria.id==(loop?22:0));
}
int main(){long items[]={-1,0,1,7};long buttons[]={-1,0,3};int cases=0;for(int i=0;i<4;++i)for(int v=0;v<2;++v)for(int q=0;q<3;++q)for(int c=0;c<3;++c)for(int e=0;e<4;++e)for(int b=0;b<2;++b)for(int a=0;a<2;++a)for(int s=0;s<2;++s)for(int l=0;l<2;++l)for(int h=0;h<3;++h)for(int r=0;r<3;++r)for(int t=0;t<2;++t){one(items[i],v,buttons[q],buttons[c],e,b,a,s,l,h,r,t);if(failed){printf("00846bc0_TEST FAIL line%d case%d\n",line_failed,cases);return 1;}++cases;}if(cases!=82944)return 2;puts("00846bc0_TEST PASS");return 0;}