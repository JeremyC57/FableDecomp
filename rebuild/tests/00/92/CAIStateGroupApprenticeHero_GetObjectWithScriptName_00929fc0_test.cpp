#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "engine/GFRandom.h"
static void check(bool ok){if(!ok)exit(2);}
class CCharString {
public: const char* text; explicit CCharString(const char* t):text(t){}
};
class CThingGameObject {
public: const char* name;unsigned char state;unsigned long flags;int id;
};
class CThingFilter_HasScriptName {
 const CCharString& name;
public: explicit CThingFilter_HasScriptName(const CCharString& n):name(n){}
 bool operator()(const CThingGameObject& x)const{return !strcmp(name.text,x.name);}
};
static unsigned rngCalls;
long FABLE_FASTCALL GFROR13(unsigned long x){
 ++rngCalls;const unsigned int word=static_cast<unsigned int>(x);return static_cast<long>((word>>13)|(word<<19));
}
class CThingSearchTools {
public: CThingGameObject object[8];unsigned n;mutable unsigned searches;
 template<class T,class F,class L>void GetAllThings(const F& filter,L& list)const{
  ++searches;
  for(unsigned i=0;i<n;++i){const CThingGameObject& item=object[i];
   if((item.state&4) && (item.state&8) && !(item.state&1) && !(item.state&32) && (item.flags&7)==7 && filter(item))
    list.push_back(const_cast<T*>(&item));
  }
 }
};
class CAIStateGroup_ApprenticeHero{
public:
 CThingSearchTools search; unsigned long seed;unsigned seedRequests;
 const CThingSearchTools& PeekThingSearchTools()const{return search;}
 unsigned long* GetPWorldSeed(){++seedRequests;return &seed;}
 CThingGameObject* GetObjectWithScriptName(const CCharString&);
};
CThingGameObject* CAIStateGroup_ApprenticeHero::GetObjectWithScriptName(const CCharString& name)
{
    std::list<CThingGameObject*> things;
    PeekThingSearchTools().GetAllThings<CThingGameObject>(CThingFilter_HasScriptName(name), things);
    if (things.size() != 0) {
        long thing_index = GFRandom(things.size(), GetPWorldSeed());
        if (false) {} else for (std::list<CThingGameObject*>::iterator thing_it = things.begin(); thing_it != things.end(); ++thing_it) {
            if (thing_index == 0)
                return *thing_it;
            --thing_index;
        }
    }
    return 0;
}
int main(){
 const char* names[]={"Library","Arena",""};CAIStateGroup_ApprenticeHero hero;
 for(unsigned count=0;count<=8;++count)for(unsigned state=0;state<64;++state)
 for(unsigned group=0;group<3;++group)for(unsigned pattern=0;pattern<4;++pattern)
 for(unsigned run=0;run<16;++run){
  unsigned ids[8],found=0;unsigned int initial=0x40302010U*run+0xfedcba98U;
  hero.search.n=count;hero.search.searches=hero.seedRequests=rngCalls=0;hero.seed=initial;
  for(unsigned i=0;i<count;++i){CThingGameObject& v=hero.search.object[i];
   v.id=i;v.name=names[(i+pattern)%3];v.state=static_cast<unsigned char>((i%3==0)?state:12);
   v.flags=(i%4==pattern)?3:7;
   if(!strcmp(v.name,names[group]) && (v.state&45)==12 && v.flags==7)ids[found++]=i;
  }
  unsigned int next=initial*0x24a1U+0x24dfU;next=(next>>13)|(next<<19);
  CThingGameObject* expected=found?&hero.search.object[ids[next%found]]:0;
  CThingGameObject* answer=hero.GetObjectWithScriptName(CCharString(names[group]));
  check(answer==expected && hero.seed==(found?next:initial) && hero.search.searches==1 && rngCalls==(found?1U:0U) && hero.seedRequests==rngCalls);
 }
 puts("00929fc0_TEST PASS");return 0;
}