// Standalone behavioural fixture. Caller body is copied verbatim; external services are observable stubs.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){printf("FAIL %d\n",__LINE__);abort();}}while(0)
static int liveStrings,lookups,conversions,comparisons,sets,baseCalls;
static const char* names[]={"","GET_UP_FAST","GET_UP_SLOW"," ","RECOVER_LEFT"};
static const char* expectedName;
class CCharString {
public:
 const char* Text;
 CCharString(const char* text,long length=-1):Text(text){CHECK(length==-1);++liveStrings;}
 CCharString(const CCharString& other):Text(other.Text){++liveStrings;}
 ~CCharString(){CHECK(liveStrings>0);--liveStrings;}
 bool operator!=(const char* text)const{++comparisons;return strcmp(Text,text)!=0;}
};
class CDefString {
public:
 long TablePos;
 CDefString():TablePos(-1){}
 operator CCharString()const{return GetString();}
 CCharString GetString()const{++conversions;CHECK(TablePos>=0 && TablePos<5);return CCharString(names[TablePos]);}
};
class CAnimComponentFollowingOnAnim {
public:
 CDefString NextAnimName;
 CDefString GetNextAnimName()const{return NextAnimName;}
};
class CAnimationEntry {
public:
 CAnimComponentFollowingOnAnim Component;
 template<class T>void GetComponent(const T** out)const{++lookups;*out=&Component;}
};
template<class T>class CCountedPointer {public:T* Data;CCountedPointer():Data(0){}T* Get()const{return Data;}};
class CCreatureActionBase {
public:
 CCountedPointer<CAnimationEntry> PPreceedingPAnimEntry;
 CCountedPointer<CAnimationEntry>& GetPPreceedingAnimationEntry(){return PPreceedingPAnimEntry;}
};
class CCombatActionBase:public CCreatureActionBase {
public:
 virtual void Initialise(){CHECK(sets==1 && liveStrings==0);++baseCalls;}
 void SetCombatAnimation(const CCharString& name){CHECK(baseCalls==0 && sets==0 && liveStrings>0);CHECK(strcmp(name.Text,expectedName)==0);++sets;}
};
class CCombatAction_GenericStrikeResponseKnockdownRecoverBase:public CCombatActionBase {
public:virtual void Initialise();
};
void CCombatAction_GenericStrikeResponseKnockdownRecoverBase::Initialise()
{
    CAnimationEntry* entry=GetPPreceedingAnimationEntry().Get();
    bool found=false;
    if(entry){
        const CAnimComponentFollowingOnAnim* data;
        entry->GetComponent(&data);
        const CCharString& name=data->GetNextAnimName();
        if(name!=""){
            SetCombatAnimation(name);
            found=true;
        }
    }
    if(!found){
        SetCombatAnimation("RESPONSE_GENERIC_HIT_KNOCKDOWN_RECOVER");
    }
    CCombatActionBase::Initialise();
}
int main(){
 for(int present=0;present<2;++present)for(int n=0;n<5;++n){
  CAnimationEntry entry;entry.Component.NextAnimName.TablePos=n;
  CCombatAction_GenericStrikeResponseKnockdownRecoverBase action;
  action.PPreceedingPAnimEntry.Data=present?&entry:0;
  expectedName=present && names[n][0]?names[n]:"RESPONSE_GENERIC_HIT_KNOCKDOWN_RECOVER";
  for(int repeat=0;repeat<2;++repeat){
   CHECK(liveStrings==0);lookups=conversions=comparisons=sets=baseCalls=0;
   action.Initialise();
   CHECK(sets==1 && baseCalls==1 && liveStrings==0);
   CHECK(lookups==present && conversions==present && comparisons==present);
   CHECK(entry.Component.NextAnimName.TablePos==n);
   CHECK(action.PPreceedingPAnimEntry.Data==(present?&entry:0));
  }
 }
 puts("00865eb0_TEST PASS");return 0;
}