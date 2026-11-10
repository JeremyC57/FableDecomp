// Exact target body, original inline accessors and counted-definition lifetime.
// Controlled external pointer lookup and definition-return service; not a game-world test.
#include <stdio.h>
#include <bitset>
static int lookup_calls,def_calls,delete_calls,has_calls;
class CIVCountedPointeeBase {
public:
    mutable unsigned long IVRefCount;
    CIVCountedPointeeBase():IVRefCount(0){}
    virtual ~CIVCountedPointeeBase(){}
    virtual void Delete() const { ++delete_calls; }
    void AddRef() const { ++IVRefCount; }
    long Release() const { if (--IVRefCount == 0) { Delete(); return 0; } return IVRefCount; }
};
class CDefPointeeBase:public CIVCountedPointeeBase {};
class CSimBuildingDef:public CDefPointeeBase {
public: long Property;
};
template<class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    CDefPointer():Object(0){}
    CDefPointer(const CDefPointer& other):Object(other.Object){if(Object)Object->AddRef();}
    ~CDefPointer(){Release();}
    T* Get()const{return static_cast<T*>(Object);}
    T* operator->()const{return Get();}
    void Release(){if(Object){Object->Release();Object=0;}}
};
class CBaseObject { public: virtual ~CBaseObject(){} };
class CBaseIntelligentPointer {
public:
    CBaseObject* PItem;
    CBaseObject* GetPItem()const {++lookup_calls;return PItem;}
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {
public:T* Get()const{return static_cast<T*>(GetPItem());}
};
enum ETCInterfaceType {TCI_VILLAGE_MEMBER=35};
class CThing:public CBaseObject {
public:
    std::bitset<256> UsingComponents;
    bool HasTC(ETCInterfaceType type)const{++has_calls;return UsingComponents.test(type);}
};
class CThingBuilding:public CThing {
public:
    CSimBuildingDef* Definition;
    CDefPointer<const CSimBuildingDef> PeekPSimDefinition()const {
        ++def_calls;
        CDefPointer<const CSimBuildingDef> result;
        result.Object=Definition;
        return result;
    }
};
class CThingAICreature:public CThing {
public:
    CIntelligentPointer<CThingBuilding> WorkBuilding;
    const CThingBuilding* PeekWorkBuilding()const{return WorkBuilding.Get();}
};
class CAIBrain {
    CThingAICreature& Creature;
public:
    CAIBrain(CThingAICreature& creature):Creature(creature){}
    CThingAICreature& GetCreature()const{return Creature;}
};
class CAIStateGroupBase {
public: CAIBrain* PBrain;
protected: CThingAICreature& GetCreature()const{return PBrain->GetCreature();}
};
class CAIStateGroup_MinionWorkInInn:public CAIStateGroupBase {
public: virtual bool IsRunnable();
};
bool CAIStateGroup_MinionWorkInInn::IsRunnable()
{
    const CThingBuilding* pwork_building=GetCreature().PeekWorkBuilding();
    if(pwork_building && GetCreature().HasTC(TCI_VILLAGE_MEMBER)) {
        CDefPointer<const CSimBuildingDef> sim_building_def=pwork_building->PeekPSimDefinition();
        if(sim_building_def.Get())
            return (sim_building_def->Property & 0x20)!=0;
    }
    return false;
}

int main(){
    unsigned int cases=0;
    const long properties[]={0,1,16,31,32,33,63,64,96,127,128,255,-1,-32,-33};
    const unsigned long counts[]={1,2,3,9};
    for(int work=0;work<2;++work)for(int member=0;member<2;++member)
    for(int present=0;present<2;++present)for(unsigned pi=0;pi<sizeof(properties)/sizeof(properties[0]);++pi)
    for(unsigned ci=0;ci<sizeof(counts)/sizeof(counts[0]);++ci){
        CThingAICreature creature; CThingBuilding building; CSimBuildingDef definition;
        CAIBrain brain(creature);CAIStateGroup_MinionWorkInInn state;state.PBrain=&brain;
        creature.WorkBuilding.PItem=work?&building:0;
        creature.UsingComponents.reset();creature.UsingComponents.set(34);creature.UsingComponents.set(36);
        creature.UsingComponents.set(35,member!=0);
        building.Definition=present?&definition:0;
        definition.Property=properties[pi];definition.IVRefCount=counts[ci];
        lookup_calls=def_calls=delete_calls=has_calls=0;
        bool result=state.IsRunnable();
        bool entered=work && member;bool used=entered&&present;
        bool expected=used && (properties[pi]&32)!=0;
        if(result!=expected||lookup_calls!=1||has_calls!=(work?1:0)||def_calls!=(entered?1:0)
            ||delete_calls!=(used&&counts[ci]==1?1:0)||definition.IVRefCount!=counts[ci]-(used?1:0))return 1;
        ++cases;
    }
    if(cases!=480)return 2;
    puts("0090bbf0_TEST PASS");return 0;
}