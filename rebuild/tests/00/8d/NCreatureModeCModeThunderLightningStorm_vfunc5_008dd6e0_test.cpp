// BEGIN exact source snapshot SHA256 5fcac364a5a028505828563379e33931403c5be22425b7bb519357f939199abf
//008dd6e0 NCreatureMode::CModeThunderLightningStorm::OnActivate; original PDB egor20086.
// Fidelity correction only; direct helper/RTTI evidence: advisor/008dd6e0_main_audit.md.
#include "fable_string.h"
#define CModeStandBase CModeStandBasePdb
#include "engine/CModeStandBase.h"
#undef CModeStandBase
#define CDefClassBase CDefClassBasePdb
#include "engine/CDefClassBase.h"
#undef CDefClassBase
#define CParentDefClassBase CParentDefClassBasePdb
#include "engine/CParentDefClassBase.h"
#undef CParentDefClassBase
#define CDefinitionManager CDefinitionManagerPdb
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#define CThingAICreature CThingAICreaturePdb
#include "engine/CThingAICreature.h"
#undef CThingAICreature
#include "engine/CTCBase.h"
#define CDefStringTable CDefStringTablePdb
#include "fable_definition_table.h"
#undef CDefStringTable

class CThing;
class CAnimateThing;
class CAnimFilterBase;
class CTCCreatureModeManager { public:
    class CModeBase { public:
        virtual ~CModeBase();
        virtual unsigned long GetType()const=0;
        virtual void AnimUpdate(CAnimateThing&); //00739e40, base no-op
        virtual void FrameUpdate(); //0073ac50
        virtual const CAnimFilterBase& GetAnimFilter()const; //00739e50, PDB egor6786/debug0051c5d0
        virtual void OnActivate(); //0073aac0; original virtual slot5
        CThing* ParentThing;
        unsigned char BaseFields[0x14]; //WeaponBasedFilter+8, FramesBeforeRemovingSelf+14, Active+18
    };
};
// Layout: shared StandBase donor is flattened. This method view replaces its
// single vfptr with the original virtual base, retaining size68 and ParentThing+4.
// Partial method view: only slots through OnActivate are declared; the remaining
// StandBase state is opaque shared-donor storage, not newly inferred fields.
struct CModeStandBase:CTCCreatureModeManager::CModeBase {
    unsigned char StandFields[0x4c];
    virtual void OnActivate(); //008621a0; global owner, not NCreatureMode
};

// Layout: raw9fbec0 confirms resource prefix28B, RefCount+4; raw430370 confirms
// CDefClassBase remains packed37B (manager1c/index20/flags24), matching donor.
#pragma pack(push,1)
class CDefPointeeBase { public:
    virtual ~CDefPointeeBase(); //RTTI01230ba0 slot0=0042d9a0; fablewin3958
    virtual void Delete() const;
    mutable long RefCount;
    unsigned char ResourceFields[0x14];
    void AddRef()const {++RefCount;}
    void Release()const {if(--RefCount==0)Delete();}
};
class CDefClassBase:public CDefPointeeBase { public:
    CDefinitionManager* PDefManager;
    unsigned long GlobalIndex;
    unsigned char DefinitionFlags;
};
class CDefString { public:long TablePos;}; //original PDB type,4B, not an owned CCharString
class CParentDefClassBase:public CDefClassBase { public:
    CDefString InstantiationName; //+25
    unsigned char SubDefInfoMap[0x10]; //+29: retail vector12+comparator; debug20B->retail16B
};
#pragma pack(pop)
class CSpecialAbilitiesThunderLightningStormDef:public CParentDefClassBase { public:
    long NumBeams; //+3c: packed parent39 rounded for derived long; debug+40
    float StartRadius,EndRadius,SkyBeamStartTime,MainBeamsStartTime;
    float BeamRotationStartSpeed,BeamRotationEndSpeed,TotalLifetime,Damage;
    CDefString InitSound,LoopingSound; //+60/+64, raw457f73 and Transfer457f91
};
FABLE_STATIC_ASSERT(sizeof(CDefPointeeBase)==0x1c);
FABLE_STATIC_ASSERT(sizeof(CDefClassBase)==0x25);
FABLE_STATIC_ASSERT(sizeof(CParentDefClassBase)==0x39);
FABLE_STATIC_ASSERT(sizeof(CSpecialAbilitiesThunderLightningStormDef)==0x68);
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesThunderLightningStormDef,StartRadius)==0x40);
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesThunderLightningStormDef,InitSound)==0x60);

template<class T> class CDefPointer { public:
    const CDefPointeeBase* Object;
    CDefPointer():Object(0){}
    CDefPointer(const CDefPointer& p):Object(p.Object){if(Object)Object->AddRef();}
    ~CDefPointer(){if(Object)Object->Release();}
    const T* operator->()const{return static_cast<const T*>(Object);}
    CDefPointer& operator=(const CDefPointer& other){
        if(Object!=other.Object){if(Object){Object->Release();Object=0;}Object=other.Object;if(Object)Object->AddRef();}return *this;
    }
};
// Layout: shared definition-manager donor retained; only missing original methods added.
class CDefinitionManager:public CDefinitionManagerPdb { public:
    long GetGlobalIndexFromClassIndex(const CCharString&,long)const; //009ad390
    template<class T> bool GetDef(long,CDefPointer<const T>*)const; //008e31d0 Storm specialization
};
class CGameDefinitionManager:public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get(); //0044c6b0
};
struct CDefStringTable:CDefStringTablePdb {
    CCharString GetString(long)const; //009d49b0
};
extern CDefStringTable g_DefStringTable;
extern unsigned long g_InvalidSoundHandle;

enum ETCInterfaceType {TC_SOUND_PLAYER=16};
template<class K,class V> class CKeyPairCompareLess { public:};
template<class K,class V,class Compare> class CVectorMap { public:
    struct Entry {K first;V second;};
    typedef Entry* iterator; //retail one-pointer iterator ABI: EAX, no hidden return
    Entry* First;Entry* Last;Entry* Limit;
    iterator LowerBound(const K&); //0040f020 ORIGINAL NONCONST specialization; const overload is004365b0
};
// Layout: no shared CThing header exists; this retail view uses UsingComponents+20,
// interface vector+44; fields outside this operation are not asserted.
class CThing { public:
    unsigned char BeforeComponents[0x20];
    unsigned long UsingComponents[9];
    CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Interfaces;
    template<class T> __forceinline bool GetTC(T** out,ETCInterfaceType interfaceType){
        if(UsingComponents[interfaceType>>5]&(1UL<<(interfaceType&31))){
            ETCInterfaceType type=interfaceType;
            typedef CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Map;
            Map* map=&Interfaces;Map::iterator it=map->LowerBound(type);
            *out=static_cast<T*>((it==map->Last||interfaceType<it->first?map->Last:it)->second);return true;
        }return false;
    }
};
// Layout: no extra fields needed in this method view; shared AICreature donor
// records AISuspended/Suspenders. Retail helper uses flags1c8,set1cc (debug-30).
class CThingAICreature:public CThing { public:void SuspendAI(bool,void*);}; //00833a10
class CTCSoundPlayer:public CTCBase { public:
    unsigned long PlayAttachedCriteria(const CCharString&,bool,bool,bool,long); //0066bd00
};
namespace NCreatureMode {
struct CModeThunderLightningStorm:CModeStandBase {
    unsigned char StormFields[0x3c];
    CDefPointer<const CSpecialAbilitiesThunderLightningStormDef> PLightningStormDef; //+a4
    void* HitThingsFirst;void* HitThingsLast;void* HitThingsLimit;
    bool MainBeamsCreated;long Timer;float StormRotation,StormRadius;unsigned long LoopingSound;
    virtual void OnActivate();
private:
    void StartLoopingSound(); //008da990, original private helper
};
FABLE_STATIC_ASSERT(offsetof(CModeThunderLightningStorm,PLightningStormDef)==0xa4);
FABLE_STATIC_ASSERT(offsetof(CModeThunderLightningStorm,LoopingSound)==0xc4);
void CModeThunderLightningStorm::OnActivate(){
    CModeStandBase::OnActivate();
    static_cast<CThingAICreature*>(ParentThing)->SuspendAI(true,0);
    Timer=0;MainBeamsCreated=false;
    long defIndex;
    {CCharString defName("SPECIAL_ABILITIES_THUNDER_LIGHTNING_STORM_DEF",-1);
     defIndex=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(defName,1);}
    CGameDefinitionManager::Get().GetDef<CSpecialAbilitiesThunderLightningStormDef>(defIndex,&PLightningStormDef);
    const CSpecialAbilitiesThunderLightningStormDef* def=PLightningStormDef.operator->();
    StormRotation=0.0f;StormRadius=def->StartRadius;
    CTCSoundPlayer* sound;
    if(ParentThing->GetTC(&sound,TC_SOUND_PLAYER)){
        CCharString initSound=g_DefStringTable.GetString(def->InitSound.TablePos);
        sound->PlayAttachedCriteria(initSound,false,false,false,-1);
    }
    LoopingSound=0;LoopingSound=g_InvalidSoundHandle;StartLoopingSound();
}
}

// END exact source snapshot
#include <stdio.h>
#include <string.h>
static int events[32],eventCount,errors,deleted,lookups,lowerBounds;
static void event(int value){events[eventCount++]=value;}
static NCreatureMode::CModeThunderLightningStorm* activeMode;
static CCharStringData stringData[32];static unsigned stringCount;
CWideString::CWideString():storage_(0){}
CWideString::~CWideString(){}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char* text,long length):storage_(&stringData[stringCount++]){
    if(length!=-1)++errors;storage_->text=const_cast<char*>(text);
}
CCharString::~CCharString(){}
CCharString::operator const char*()const{return storage_?storage_->text:"";}
CTCCreatureModeManager::CModeBase::~CModeBase(){}
void CTCCreatureModeManager::CModeBase::AnimUpdate(CAnimateThing&){}
void CTCCreatureModeManager::CModeBase::FrameUpdate(){}
const CAnimFilterBase& CTCCreatureModeManager::CModeBase::GetAnimFilter()const{++errors;throw 4;}
void CTCCreatureModeManager::CModeBase::OnActivate(){++errors;}
void CModeStandBase::OnActivate(){event(1);}
void CThingAICreature::SuspendAI(bool suspended,void* suspender){event(2);if(!suspended||suspender!=0)++errors;}
CDefPointeeBase::~CDefPointeeBase(){}
void CDefPointeeBase::Delete()const{++deleted;}
static CSpecialAbilitiesThunderLightningStormDef stormDef,oldDef;
static CGameDefinitionManager manager;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get(){return manager;}
long CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString& name,long classIndex)const{
    event(3);++lookups;
    if(classIndex!=1||strcmp((const char*)name,"SPECIAL_ABILITIES_THUNDER_LIGHTNING_STORM_DEF"))++errors;
    return 23;
}
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* out)const{
    event(4);if(index!=23){++errors;return false;}
    // Faithful dependency ownership: acquired temporary, output assignment, temporary release.
    CDefPointer<const T> acquired;
    acquired.Object=&stormDef;acquired.Object->AddRef();*out=acquired;return true;
}
CDefStringTable g_DefStringTable;unsigned long g_InvalidSoundHandle=0xffffffffUL;
CCharString CDefStringTable::GetString(long id)const{
    if(activeMode->Timer!=0||activeMode->MainBeamsCreated||activeMode->StormRotation!=0.0f||activeMode->StormRadius!=123.5f)++errors;
    if(id==27){event(5);return CCharString("INIT_SOUND",-1);}
    if(id==31){event(8);return CCharString("LOOP_SOUND",-1);}
    ++errors;return CCharString("BAD",-1);
}
template<class K,class V,class C> typename CVectorMap<K,V,C>::iterator CVectorMap<K,V,C>::LowerBound(const K& key){
    ++lowerBounds;iterator first=First;long count=Last-First;
    while(count>0){long half=count/2;iterator middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}return first;
}
unsigned long CTCSoundPlayer::PlayAttachedCriteria(const CCharString& name,bool a,bool b,bool c,long last){
    if(a||b||c||last!=-1)++errors;
    if(!strcmp((const char*)name,"INIT_SOUND")){event(6);return 0x1234;}
    if(!strcmp((const char*)name,"LOOP_SOUND")){event(9);return 0x5678;}
    ++errors;return g_InvalidSoundHandle;
}
void NCreatureMode::CModeThunderLightningStorm::StartLoopingSound(){
    event(7);if(LoopingSound!=g_InvalidSoundHandle){++errors;return;}
    // This is a dependency double for the independently audited008da990 body.
    CTCSoundPlayer* sound;
    if(ParentThing->GetTC(&sound,TC_SOUND_PLAYER)){
        CCharString name=g_DefStringTable.GetString(PLightningStormDef->LoopingSound.TablePos);
        LoopingSound=sound->PlayAttachedCriteria(name,false,false,false,-1);
    }
}
static bool sequence(const int* expected,int count){if(eventCount!=count)return false;for(int i=0;i<count;++i)if(events[i]!=expected[i])return false;return true;}
struct FixtureMode:NCreatureMode::CModeThunderLightningStorm {virtual unsigned long GetType()const{return 0x36;}};
int main(){
    CThingAICreature owner;CTCSoundPlayer sound;
    typedef CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Map;
    Map::Entry entries[4];memset(&owner,0,sizeof(owner));
    entries[0].first=(ETCInterfaceType)2;entries[0].second=0;
    entries[1].first=(ETCInterfaceType)8;entries[1].second=0;
    entries[2].first=TC_SOUND_PLAYER;entries[2].second=&sound;
    entries[3].first=(ETCInterfaceType)0x7fffffff;entries[3].second=0;
    owner.Interfaces.First=entries;owner.Interfaces.Last=entries+3;owner.Interfaces.Limit=entries+4;
    owner.UsingComponents[0]=1UL<<16;
    stormDef.RefCount=1;oldDef.RefCount=1;stormDef.StartRadius=123.5f;
    stormDef.InitSound.TablePos=27;stormDef.LoopingSound.TablePos=31;
    {
        FixtureMode mode;activeMode=&mode;
        mode.ParentThing=&owner;mode.PLightningStormDef.Object=&oldDef;
        mode.Timer=998;mode.MainBeamsCreated=true;mode.LoopingSound=5;
        CTCCreatureModeManager::CModeBase* polymorphic=&mode;
        polymorphic->OnActivate(); //actual virtual slot5 dispatch, not direct fixture substitute
        const int first[]={1,2,3,4,5,6,7,8,9};
        if(errors||!sequence(first,9)||lookups!=1||lowerBounds!=2||deleted!=1||oldDef.RefCount!=0||stormDef.RefCount!=2||mode.PLightningStormDef.Object!=&stormDef||mode.LoopingSound!=0x5678)return 1;
        eventCount=0;owner.UsingComponents[0]=0;mode.Timer=42;mode.MainBeamsCreated=true;
        polymorphic->OnActivate();
        const int second[]={1,2,3,4,7};
        if(errors||!sequence(second,5)||lookups!=2||lowerBounds!=2||deleted!=1||stormDef.RefCount!=2||mode.LoopingSound!=g_InvalidSoundHandle||mode.Timer||mode.MainBeamsCreated)return 2;
    }
    if(stormDef.RefCount!=1||deleted!=1)return 3;
    puts("008dd6e0_TEST PASS");return 0;
}