// Behavior-only fixture: exact selected target, actual observer/action objects.
// External AI actions are checked boundaries with controlled return/state changes.
// Unused interfaces are reduced; no native layout or dispatch claim is made.
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
static int cases,trace[32],traceCount,liveHandles,liveActions,terminates,moves;
static int hasMessage,initialBody,nearBody,curiosityResult,retries,moveResult,deathPhase,deathMode,lookResult,posResult,sweepResult;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
static void event(int x){CHECK(traceCount<32);trace[traceCount++]=x;}
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
class CBaseObject;class CBaseObjectPointer{public:CBaseObject*Object;};class CBaseObject{public:CBaseObjectPointer Pointer;CBaseObject(){Pointer.Object=this;}virtual~CBaseObject(){Pointer.Object=0;}};
class CThing:public CBaseObject{public:bool Dead;unsigned long Flags;C3DVector Position;CThing():Dead(false),Flags(0){}const C3DVector&GetPos()const{return Position;}bool IsDead()const{return Dead;}unsigned long GetTypeFlags()const{return Flags;}static unsigned long __fastcall StatGetTypeFlags(){return 0;}};
class CThingCreatureBase:public CThing{};class CThingAICreature:public CThingCreatureBase{public:virtual float GetFOV()const{return 180.0f;}};
class CBaseIntelligentPointer{CBaseObjectPointer*PData;public:CBaseIntelligentPointer():PData(0){++liveHandles;}virtual~CBaseIntelligentPointer(){--liveHandles;}CBaseObject*GetPItem()const{return PData?PData->Object:0;}void SetPItem(CBaseObject*p){PData=p?&p->Pointer:0;}};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:__forceinline CIntelligentPointer(T*p){SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(p)));}CIntelligentPointer(const CIntelligentPointer&p){SetPItem(p.GetPItem());}T*Get()const{return static_cast<T*>(GetPItem());}T*operator->()const{return Get();}};
class CMessageEvent:public CBaseObject{public:C3DVector Pos;CIntelligentPointer<CThing>POriginator;CMessageEvent(CThing*p):Pos(4.0f,8.0f,12.0f),POriginator(p){}template<class T>T*GetPOriginator()const{T*result=0;if(POriginator.Get()){unsigned long mask=T::StatGetTypeFlags();const unsigned long&flags=POriginator.Get()->GetTypeFlags();if((flags&mask)==mask)result=static_cast<T*>(POriginator.Get());}return result;}};
static CThing*bodyThing;static CThingAICreature*actor;static CMessageEvent*message;
static void killBody(){if(deathMode)bodyThing->Dead=true;else bodyThing->Pointer.Object=0;}
class CMessageEventManager{public:void RemoveMessage(const CMessageEvent*p){CHECK(p==message);event(1);message->Pos=C3DVector(-4.0f,-8.0f,-12.0f);}};
class CWorld{public:CMessageEventManager Manager;CMessageEventManager&GetMessageEventManager(){return Manager;}};static CWorld world;
class CCreatureActionBase:public CBaseObject{public:int Kind;CCreatureActionBase(int k):Kind(k){++liveActions;}virtual~CCreatureActionBase(){--liveActions;}};
class CCreatureAction_Curiosity:public CCreatureActionBase{public:CCreatureAction_Curiosity(CThingCreatureBase&c):CCreatureActionBase(0){CHECK(&c==actor);}};
class CCreatureAction_OrderSecuritySweep:public CCreatureActionBase{public:CCreatureAction_OrderSecuritySweep(CThingCreatureBase&c):CCreatureActionBase(1){CHECK(&c==actor);}};
enum EAIActionReturn{};enum EAIActionRepeat{};enum EMovementType{};
class CAIStateGroupBase{public:CThingAICreature&GetCreature()const{return *actor;}CWorld&GetWorld()const{return world;}void Terminate(){CHECK(liveActions==0);++terminates;event(8);}
 EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase&a,bool flag){CHECK(liveActions==1&&!flag);if(a.Kind==0){event(3);if(deathPhase==1)killBody();return(EAIActionReturn)curiosityResult;}event(7);return(EAIActionReturn)sweepResult;}
 EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat rep,CIntelligentPointer<const CThing>body,float radius,bool&arrived,EMovementType movement,bool a,bool b,bool c,bool d,float extra){CHECK((int)rep==2&&body.Get()==bodyThing&&!bodyThing->Dead&&radius==2.5f&&(int)movement==5&&a&&!b&&c&&d&&extra==0.0f&&liveHandles==3);event(4);int result=moves++<retries?4:moveResult;arrived=result==2;if((deathPhase==2&&result==4)||(deathPhase==3&&result!=4))killBody();return(EAIActionReturn)result;}
 EAIActionReturn AIAction_LookAtThing(EAIActionRepeat rep,CIntelligentPointer<const CThing>body,long duration,float fov){CHECK((int)rep==1&&body.Get()==bodyThing&&!bodyThing->Dead&&duration==15&&fov==0.5f&&liveHandles==3);event(5);return(EAIActionReturn)lookResult;}
 EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat rep,const C3DVector pos,float radius,bool&arrived,EMovementType movement,bool a,bool b,bool c,const CThing*thing,bool d,float extra){CHECK((int)rep==1&&pos.X==4.0f&&pos.Y==8.0f&&pos.Z==12.0f&&radius==1.5f&&!arrived&&(int)movement==5&&a&&!b&&c&&!thing&&d&&extra==0.0f);event(6);arrived=posResult==2;return(EAIActionReturn)posResult;}
};
class CAIStateGroup_MinionGuardReactToBodyFoundMessage:public CAIStateGroupBase{public:const CMessageEvent*FindBodyFoundEvent(){event(0);return hasMessage?message:0;}virtual void Update();};
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector&a,const C3DVector&b,float radius){CHECK(&a==&actor->Position&&&b==&bodyThing->Position&&radius==2.5f);event(2);return nearBody!=0;}
void CAIStateGroup_MinionGuardReactToBodyFoundMessage::Update()
{
    const CMessageEvent* message = FindBodyFoundEvent();
    if (message) {
        CIntelligentPointer<const CThing> body(message->GetPOriginator<CThing>());
        if ((!body.Get() || body.Get()->IsDead())) {
            Terminate();
            return;
        }
        C3DVector bodyPos = message->Pos;
        CMessageEventManager* manager = &GetWorld().GetMessageEventManager();
        manager->RemoveMessage(message);
        if (!GFWorld3DDistanceSmallerThan(GetCreature().GetPos(), body.Get()->GetPos(), 2.5f)) {
            if (AIAction_PerformActionAndWait(CCreatureAction_Curiosity(GetCreature()), false) != 2) {
                Terminate();
                return;
            }
            bool arrived = false;
            EAIActionReturn result;
            do {
                if ((!body.Get() || body.Get()->IsDead())) {
                    Terminate();
                    return;
                }
                result = AIAction_MoveToThingProximity((EAIActionRepeat)2, body, 2.5f, arrived, (EMovementType)5, true, false, true, true, 0.0f);
            } while (result == 4);
            if (result != 2) {
                Terminate();
                return;
            }
        }
        if ((body.Get() && !body.Get()->IsDead())) {
            if (AIAction_LookAtThing((EAIActionRepeat)1, body, 0xf, GetCreature().GetFOV() * 0.0027777778f) == 2) {
                bool arrived = false;
                if (AIAction_MoveToPosProximity((EAIActionRepeat)1, bodyPos, 1.5f, arrived, (EMovementType)5, true, false, true, 0, true, 0.0f) != 2) {
                    Terminate();
                    return;
                }
                if (AIAction_PerformActionAndWait(CCreatureAction_OrderSecuritySweep(GetCreature()), false) != 2) {
                    Terminate();
                    return;
                }
            }
        }
    }
    Terminate();
}

static void run(){
 CHECK(liveHandles==0&&liveActions==0);CThing body;CThingAICreature creature;bodyThing=&body;actor=&creature;body.Dead=initialBody==2;body.Position=C3DVector(16,32,64);CMessageEvent msg(initialBody?&body:0);message=&msg;CAIStateGroup_MinionGuardReactToBodyFoundMessage group;
 int expected[32],n=0;expected[n++]=0;bool proceeds=hasMessage&&initialBody==1;
 if(proceeds){expected[n++]=1;expected[n++]=2;if(!nearBody){expected[n++]=3;proceeds=curiosityResult==2&&deathPhase!=1;if(proceeds){int count=(deathPhase==2&&retries>0)?1:retries+1;for(int i=0;i<count;++i)expected[n++]=4;proceeds=!(deathPhase==2&&retries>0)&&moveResult==2&&deathPhase!=3;}}if(proceeds){expected[n++]=5;if(lookResult==2){expected[n++]=6;if(posResult==2)expected[n++]=7;}}}expected[n++]=8;
 traceCount=terminates=moves=0;group.Update();CHECK(terminates==1&&liveHandles==1&&liveActions==0&&traceCount==n);for(int j=0;j<n;++j)CHECK(trace[j]==expected[j]);++cases;
}
int main(){
 hasMessage=0;initialBody=1;nearBody=0;curiosityResult=moveResult=lookResult=posResult=sweepResult=2;retries=deathPhase=deathMode=0;run();hasMessage=1;initialBody=0;run();initialBody=2;run();initialBody=1;
 for(int near=0;near<2;++near)for(int curiosity=0;curiosity<2;++curiosity)for(int retry=0;retry<3;++retry)for(int result=0;result<2;++result)for(int death=0;death<4;++death)for(int kind=0;kind<2;++kind)for(int look=0;look<2;++look)for(int pos=0;pos<2;++pos)for(int sweep=0;sweep<2;++sweep){nearBody=near;curiosityResult=curiosity?2:0;retries=retry;moveResult=result?2:0;deathPhase=death;deathMode=kind;lookResult=look?2:0;posResult=pos?2:0;sweepResult=sweep?2:0;run();}
 CHECK(cases==1539&&liveHandles==0&&liveActions==0);puts("00911300_TEST PASS");return 0;}
