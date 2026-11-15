// Complete original named Transfer and GetDefaultVal bodies; portable callee fixtures.
#include <assert.h>
#include <stdio.h>
#include <string.h>
class CBaseObject{};class CThing:public CBaseObject{};
static int Alive,Constructed,Destroyed,Copied,Calls;
class CBaseIntelligentPointer {public:CBaseObject* PData;
 CBaseIntelligentPointer():PData(0){++Alive;++Constructed;}
 virtual ~CBaseIntelligentPointer(){--Alive;++Destroyed;}
 CBaseObject* GetPItem()const{return PData;}void SetPItem(CBaseObject* p){PData=p;}
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {public:
 CIntelligentPointer(){}
 CIntelligentPointer(const CIntelligentPointer& other){SetPItem(other.GetPItem());++Copied;}
};
template<class T>class CPersistTraits {public:T DefaultVal;CPersistTraits():DefaultVal(){}};
class CPersistContext {public:
 int Mode;const char* ExpectedName;CBaseObject* Input;CBaseObject* Saved;
 template<class T>void Transfer(const char*,T&,const T&);
 template<class T>void Transfer(const char* name,T& value){Transfer(name,value,GetDefaultVal<T>());}
protected:
 template<class T>static T GetDefaultVal(){CPersistTraits<T> traits;return traits.DefaultVal;}
};
template<class T>void CPersistContext::Transfer(const char* name,T& value,const T& defaultValue){
 ++Calls;assert(name==ExpectedName);assert(defaultValue.GetPItem()==0);assert(Alive==2);
 if(Mode==0 || Mode==2)value.SetPItem(Input);
 else if(Mode==4)value.SetPItem(defaultValue.GetPItem());
 else Saved=value.GetPItem();
}
int main(){
 CThing oldThing,newThing;
 for(int mode=0;mode<5;++mode)for(int populated=0;populated<2;++populated){
  CIntelligentPointer<CThing> value;value.SetPItem(populated?&oldThing:0);
  CPersistContext context;context.Mode=mode;context.Input=&newThing;context.Saved=0;context.ExpectedName="LastHero";
  Constructed=Destroyed=Copied=Calls=0;
  context.Transfer(context.ExpectedName,value);
  assert(Calls==1 && Copied>=1 && Constructed==Destroyed && Alive==1);
  if(mode==0 || mode==2)assert(value.GetPItem()==&newThing);
  else if(mode==4)assert(value.GetPItem()==0);
  else {assert(value.GetPItem()==(populated?&oldThing:0));assert(context.Saved==value.GetPItem());}
 }
 assert(Alive==0);puts("006c9c60_TEST PASS");return 0;
}