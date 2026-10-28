#include <stdio.h>
#include <stdlib.h>
#include <new>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CThing;class CBaseObjectPointer;
class CTCBase {public:virtual ~CTCBase();CThing* ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;};
// Layout: pointee count+4 and virtual Delete slot1 are confirmed by each assigned destructor. Generated pointee headers flatten the vptr, so actual polymorphism is represented here.
class CIVCountedPointeeBase {public:virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable long IVRefCount;};
template<class T> class CDefPointer {public:T* Object;__forceinline ~CDefPointer(){if(Object){const CIVCountedPointeeBase* base=(const CIVCountedPointeeBase*)Object;if(--base->IVRefCount==0)base->Delete();Object=0;}}};
class CBaseIntelligentPointer {public:virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {};
class C3DVector {public:float X,Y,Z;};
class CEnginePrimitiveThemeChannelDesc {public:long ID;};
enum EOracleMinigameState {};enum EOracleMinigameInput {};
namespace std {template<class T>class allocator{};template<class T,class A=allocator<T> >class list {public:void* _M_node;~list();};}
class CPerceivedThingDef;
class CTCPerceiveThing : public CTCBase {public:
    CIntelligentPointer<const CThing> PerceivedThing;
    float DistanceFromPerceivedThingSquared,AwarenessTimer;long FramesSinceDirectlyPerceivedThing;C3DVector LastPerceivedPos;
    bool Perceiving,LineOfSight,ExtendedLineOfSight,IgnoreFOVLineOfSight,DirectAwareness,Smelt,Heard,SearchingForThingToPerceive;
    CDefPointer<const CPerceivedThingDef> PDef;long UpdateOffset;float SoundRadius,SightRadius,ExtendedSightRadius,SmellRadius,GiveUpChaseRadius;bool PerceptionCombat;long FrameHitByPerceivedThing;
};
void Destroy_CTCPerceiveThing(CTCPerceiveThing* p){p->CTCPerceiveThing::~CTCPerceiveThing();}

static int baseCalls,memberCalls,deleteCalls,bad;static void* expectedOwner;static void* expectedMember;
CTCBase::~CTCBase(){++baseCalls;if(this!=expectedOwner || memberCalls!=1)bad=1;}
void CIVCountedPointeeBase::Delete()const{++deleteCalls;if(IVRefCount!=0 || memberCalls)bad=1;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){++memberCalls;if((void*)this!=expectedMember)bad=1;}

int main(){int ok=1;
    for(int count=0;count<3;++count){
        void* memory=malloc(sizeof(CTCPerceiveThing));if(!memory)return 1;CTCPerceiveThing* object=new(memory)CTCPerceiveThing;CIVCountedPointeeBase pointee;pointee.IVRefCount=count;
        object->PDef.Object=count?reinterpret_cast<const CPerceivedThingDef*>(&pointee):0;
        expectedOwner=object;expectedMember=&object->PerceivedThing;void* defWord=&object->PDef.Object;
        baseCalls=memberCalls=deleteCalls=bad=0;Destroy_CTCPerceiveThing(object);
        void* remaining;memcpy(&remaining,defWord,sizeof(remaining));
        ok=ok && !bad && baseCalls==1 && memberCalls==1 && deleteCalls==(count==1?1:0) && pointee.IVRefCount==(count?count-1:0) && !remaining;
        free(memory);
    }
    puts(ok ? "007462f0_TEST PASS" : "007462f0_TEST FAIL");return ok ? 0 : 1;
}