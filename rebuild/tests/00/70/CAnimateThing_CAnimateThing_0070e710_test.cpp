#define FABLE_RETAIL_STLP_LIST
// Original complete shared owner/member classes and actual CThing& parameter; no stand-in list implementation.
// Leaf: ??0CAnimateThing@@QAE@AAVCThing@@@Z
// Flags: (none)
#define FABLE_RETAIL_CANIMATETHING_CTOR_PASS26
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_CTOR_BODY_PASS26
#define FABLE_RETAIL_CANIMFROMFILEINFO_CTOR_PASS26
#include "engine/CAnimateThing.h"
CAnimateThing::CAnimateThing(CThing& parent_thing)
 :ChangeAlphaBy(1.0f),AnimDelay(2),Interruptable(true),ParentThing(parent_thing)
{
    CurrentAnimRefreshStamp=1;
    LastAnimRefreshStamp=0;
    while(DelayedAnimQueue.size()!=3)
        DelayedAnimQueue.push_back(CDelayedAnimation());
}

#include "engine/CThing.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
static int failed=0,base_constructions=0,wide_constructions=0;
#define CHECK(x) do{if(!(x)){++failed;std::printf("FAIL %d\n",__LINE__);}}while(0)
CBaseClass::CBaseClass(){++base_constructions;}
CBaseClass::~CBaseClass(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}
CBaseObject::~CBaseObject(){}
void CBaseObject::Validate()const{}
void CClass::Save(CCharString&){}
CCharString::CCharString(){}
CCharString::CCharString(const CCharString&){}
CCharString::~CCharString(){}
CWideString::CWideString(const wchar_t* text){CHECK(text&&text[0]==0);++wide_constructions;}
CWideString::~CWideString(){}
C3DAnimationBlendState::~C3DAnimationBlendState(){}
CAnimSet::CAnimSet(const CAnimSet&){}
CAnimateThing::CCompleteAnimation::CCompleteAnimation():MainAnimCanTransitionOut(false){}
static unsigned Word(const CAnimateThing& a,unsigned off){unsigned n;std::memcpy(&n,(const unsigned char*)&a+off,4);return n;}
static float Float(const CAnimateThing&a,unsigned off){float n;std::memcpy(&n,(const unsigned char*)&a+off,4);return n;}
int main(){
 CThing parent;
 for(int run=0;run<3;++run){
  void* allocation=std::malloc(sizeof(CAnimateThing));CHECK(allocation!=0);if(!allocation)return 1;std::memset(allocation,0xa4,sizeof(CAnimateThing));
  int base_before=base_constructions,wide_before=wide_constructions;
  CAnimateThing* anim=new(allocation) CAnimateThing(parent);
  CHECK(base_constructions==base_before+4&&wide_constructions==wide_before+1);
  CHECK(Word(*anim,0x88)==0xa4a4a4a4u&&Float(*anim,0x8c)==1.0f&&Word(*anim,0x90)==2);
  CHECK((((const unsigned char*)anim)[0x94]&1)!=0&&Word(*anim,0x98)==1&&Word(*anim,0x9c)==0);
  CThing* stored;std::memcpy(&stored,(const unsigned char*)anim+0xa0,sizeof(stored));CHECK(stored==&parent);
  CHECK(((const unsigned char*)anim)[0xa4]==0&&Float(*anim,0xac)==0&&Float(*anim,0xb0)==0&&Float(*anim,0xb4)==0);
  CHECK(Word(*anim,0xb8)==0xa4a4a4a4u);
  for(unsigned i=0;i<4;++i)CHECK(Word(*anim,0x6c+i*8)==0);
  std::list<CAnimateThing::CDelayedAnimation>& queue=*(std::list<CAnimateThing::CDelayedAnimation>*)((unsigned char*)anim+0x60);
  CHECK(queue.size()==3);unsigned n=0;
  for(std::list<CAnimateThing::CDelayedAnimation>::iterator it=queue.begin();it!=queue.end();++it){CHECK(it->CombinationAnims.empty());++n;}
  CHECK(n==3);
  std::list<CAnimSet>& delayed=*(std::list<CAnimSet>*)((unsigned char*)anim+0x64);CHECK(delayed.empty());
  anim->~CAnimateThing();std::free(allocation);
 }
 std::puts(failed?"0070e710_TEST FAIL":"0070e710_TEST PASS");return failed?1:0;
}