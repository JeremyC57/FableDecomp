#include <stdio.h>
class CBaseObject{public:int Marker;};class CThing:public CBaseObject{};
class CThing;class CThingBuilding;
class CBaseObject;
class CBaseObjectPointer {                             // Ego_r.pdb, 8 bytes
public:
    CBaseObject*  object;                              // +0x00 the observed object, 0 once it died
    unsigned long refcount;                            // +0x04 intelligent pointers sharing this record
};
class CBaseIntelligentPointer {                        // vtable 0x0129C95C
public:
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
    CBaseObject* GetPItem() const;                     // 0x00A01B50
private:
    friend struct TestTracking;
    void ClearData() const                             // 0x00A01B20 (private const in Ego_r), inlined here
    {
        if (PData != 0) {
            PData->refcount--;
            if (PData->refcount == 0 && PData->object == 0)
                delete PData;
            PData = 0;
        }
    }
    mutable CBaseObjectPointer* PData;                 // +0x04
};

#pragma auto_inline(off)
CBaseObject* CBaseIntelligentPointer::GetPItem() const
{
    if (PData != 0) {
        if (PData->object != 0)
            return PData->object;
        ClearData();
    }
    return 0;
}

#pragma auto_inline(on)
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}};
class CThingAICreature{public:unsigned char _unmodeled00[0x168];CIntelligentPointer<CThingBuilding> HomeBuilding,WorkBuilding;};class CAIBrain{public:unsigned char _unmodeled00[24];CThingAICreature* Creature;CThingAICreature& GetCreature()const{return *Creature;}};
class CBaseClass{public:virtual ~CBaseClass(){} };class CAIStateGroupBase:public CBaseClass{public:CAIBrain* PBrain;};class CAIStateGroup_UsesSeat:public CAIStateGroupBase{};class CAIStateGroup_MinionSitOnSeatBase:public CAIStateGroup_UsesSeat{};
class CAIStateGroup_MinionSitAtHome:public CAIStateGroup_MinionSitOnSeatBase{protected:virtual bool GetPOwnerObject(CThing**);};
bool CAIStateGroup_MinionSitAtHome::GetPOwnerObject(CThing** out){CThing* owner=(CThing*)((CIntelligentPointer<CThingBuilding>&)PBrain->GetCreature().HomeBuilding).GetPItem();if(owner){*out=owner;return true;}return false;}
class Harness:public CAIStateGroup_MinionSitAtHome{public:bool Query(CThing** out){return GetPOwnerObject(out);}};
CBaseIntelligentPointer::~CBaseIntelligentPointer(){ClearData();}
struct TestTracking{static void Set(CBaseIntelligentPointer&p,CBaseObjectPointer*q){p.PData=q;}static CBaseObjectPointer* Get(CBaseIntelligentPointer&p){return p.PData;}};
int main(){Harness group;CAIBrain brain;CThingAICreature creature;CThing old,target;CBaseObjectPointer live,dead;group.PBrain=&brain;brain.Creature=&creature;TestTracking::Set(creature.WorkBuilding,0);TestTracking::Set(creature.HomeBuilding,0);CThing*out=&old;if(group.Query(&out)||out!=&old)goto fail;live.object=&target;live.refcount=2;TestTracking::Set(creature.HomeBuilding,&live);if(!group.Query(&out)||out!=&target||live.refcount!=2)goto fail;dead.object=0;dead.refcount=2;TestTracking::Set(creature.HomeBuilding,&dead);out=&old;if(group.Query(&out)||out!=&old||dead.refcount!=1||TestTracking::Get(creature.HomeBuilding))goto fail;puts("00938ef0_TEST PASS");return 0;fail:TestTracking::Set(creature.HomeBuilding,0);puts("00938ef0_TEST FAIL");return 1;}