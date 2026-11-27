// Native x86 fixture for NCreatureMode::CModeDragonBase's generated deleting destructor.
// The checked shared declarations supply the complete mode, both filters and PTarget.
#define FABLE_RETAIL_DRAGONBASE_DONOR_PROTECTED_PASS29
#define FABLE_RETAIL_DRAGONBASE_LIFETIME_PASS29
#define FABLE_RETAIL_CMODEDRAGONBASE_MODEBASE
#define FABLE_RETAIL_CMODEBASE_CTOR
#define FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRINGDATA_FRIEND
#define FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "engine/ETCInterfaceType.h"
#include "engine/CModeDragonBase.h"
#include "engine/CBaseObjectPointer.h"

static unsigned scenario;
static void check(bool ok, int line) {
    if (!ok) { fprintf(stderr,"dragon case %u, line %d\n",scenario,line); exit(1); }
}
#define CHECK(x) check((x),__LINE__)
class CBaseObject {};
class CThing : public CBaseObject {};
typedef NCreatureMode::CModeDragonBase Dragon;
typedef CTCCreatureModeManager::CModeBase Mode;
enum Event { TARGET, FILTER_GROUP, WEAPON_GROUP, FREE_MODE };
static Event events[4];
static unsigned eventCount;
static unsigned char* modeStorage;
static void** dragonTable;
static bool tracing;
static CBaseObjectPointer* control;
static unsigned controlDeletes;
static CCharStringData* weaponData;
static CCharStringData* dragonData;
static unsigned weaponDeletes, dragonDeletes;

static void record(Event e) { CHECK(eventCount<4); events[eventCount++]=e; }
void __cdecl operator delete(void* p) {
    if (p && p==weaponData) { ++weaponDeletes; weaponData=0; }
    if (p && p==dragonData) { ++dragonDeletes; dragonData=0; }
    if (p && p==control) { ++controlDeletes; control=0; }
    if (tracing && p==modeStorage) record(FREE_MODE);
    free(p);
}

CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) {}
void CBaseIntelligentPointer::SetPItem(CBaseObject* target) {
    CHECK(PData==0 && target && control && control->object==target);
    PData=control; ++PData->refcount;
}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PData?PData->object:0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {
    if (tracing) { CHECK(static_cast<void*>(this)==modeStorage+0x28); record(TARGET); }
    if (PData) {
        --PData->refcount;
        if (!PData->refcount && !PData->object) delete PData;
        PData=0;
    }
}

// Exercise the native shared-string ownership protocol while observing the two
// member destructor boundaries. Text initialization belongs to this fixture.
class FixtureText : public CBasicString<char> {
public:
    explicit FixtureText(const char* text) {
        StringLength=static_cast<unsigned long>(strlen(text)); DataLength=StringLength+1;
        UseFastExtend=false; PData=new char[DataLength]; memcpy(PData,text,DataLength);
    }
    void transfer(CBasicString<char>& destination) {
        destination=*this;
        PData=0; StringLength=0; DataLength=0;
    }
};
const char CCharString::NullChar=0;
long CCharString::NoStrings=0;
CCharString::CCharString() : PStringData(0) { ++NoStrings; }
CCharString::CCharString(const char* text,long) : PStringData(0) {
    ++NoStrings;
    if (*text) {
        PStringData=new CCharStringData();
        FixtureText payload(text); payload.transfer(PStringData->Data);
        PStringData->NoRefs=1;
    }
}
void CCharString::UnassignString() {
    if (PStringData) {
        PStringData->DecRef();
        if (PStringData->GetRefs()<=0) delete PStringData;
        PStringData=0;
    }
}
const CCharString& CCharString::operator=(const CCharString& other) {
    if (PStringData!=other.PStringData) {
        UnassignString(); PStringData=other.PStringData;
        if (PStringData) ++PStringData->NoRefs;
    }
    if (modeStorage && static_cast<void*>(this)==modeStorage+0x0c) weaponData=PStringData;
    if (modeStorage && static_cast<void*>(this)==modeStorage+0x20) {
        dragonData=PStringData;
        memcpy(&dragonTable,modeStorage,sizeof(dragonTable));
    }
    return *this;
}
CCharString::~CCharString() {
    if (tracing) {
        if (static_cast<void*>(this)==modeStorage+0x20) record(FILTER_GROUP);
        else if (static_cast<void*>(this)==modeStorage+0x0c) record(WEAPON_GROUP);
        else CHECK(false);
    }
    UnassignString(); --NoStrings;
}
class FixtureString : public CCharString {
public:
    explicit FixtureString(const char* text) : CCharString(text) {}
    static long instances() { return NoStrings; }
};
unsigned long FABLE_FASTCALL CCRC::Calc(unsigned long seed,const void*,unsigned long) { return seed; }
bool CAnimFilterOfGroup::operator()(const CAnimationEntry&) const { return false; }
CAnimFilterBase* CAnimFilterOfGroup::Clone() const { return 0; }

Mode::CModeBase(CThing& thing) : ParentThing(thing), WeaponBasedFilter("WEAPON"),
    FramesBeforeRemovingSelf(-1), Active(false) {}
Mode::~CModeBase() {}
void Mode::AnimUpdate(CAnimateThing&) {}
void Mode::FrameUpdate() {}
const CAnimFilterBase& Mode::GetAnimFilter() const { return WeaponBasedFilter; }
void Mode::OnActivate() {}
void Mode::OnDeactivate() {}
float Mode::GetMovementSpeed(const C3DVector&,const C3DVector&,EMovementType) const { return 0; }
bool Mode::IsIdleMode() const { return false; }
bool Mode::IsAllowingPlayerSpecialMovementMoves() const { return false; }
bool Mode::IsHoldWeaponWithBothHands() const { return false; }
bool Mode::IsRemoveOnDeactivate() const { return false; }

Dragon::CModeDragonBase(CThing& thing,const CCharString& group_name)
    : CModeBase(thing), Filter(group_name) {}
const CAnimFilterBase& Dragon::GetAnimFilter() const { return Filter; }
void Dragon::OnActivate() {}
void Dragon::OnDeactivate() {}

class ConcreteDragon : public Dragon {
public:
    ConcreteDragon(CThing& thing,const CCharString& group) : Dragon(thing,group) {}
    NCreatureMode::EMode GetType() const { return NCreatureMode::MODE_NULL; }
    void attach(CThing& target) { PTarget.SetPItem(&target); }
    void checkLayout() {
        unsigned char* p=reinterpret_cast<unsigned char*>(this);
        CHECK(static_cast<void*>(&PTarget)==p+0x28);
        CHECK(static_cast<const void*>(&GetAnimFilter())==p+0x1c);
        CHECK(static_cast<const void*>(&Mode::GetAnimFilter())==p+8);
    }
};

static void exercise(unsigned flags,unsigned owners,bool attach,bool expired,bool throughBase,unsigned groupState) {
    ++scenario;
    CThing thing;
    FixtureString group(groupState==0 ? "" : "DRAGON");
    weaponData=0; dragonData=0; weaponDeletes=dragonDeletes=0;
    control=0; controlDeletes=0; eventCount=0; dragonTable=0;
    modeStorage=static_cast<unsigned char*>(::operator new(sizeof(ConcreteDragon)));
    ConcreteDragon* object=new(modeStorage) ConcreteDragon(thing,group);
    object->checkLayout();
    CHECK(weaponData && weaponData->GetRefs()==1);
    if (groupState!=0) CHECK(dragonData && dragonData->GetRefs()==2);
    if (groupState==2) {
        static_cast<CCharString&>(group)=CCharString();
        CHECK(dragonData && dragonData->GetRefs()==1);
    }
    void** finalTable=0;
    memcpy(&finalTable,object,sizeof(finalTable));
    CHECK(dragonTable && finalTable && dragonTable!=finalTable);
    if (attach) {
        control=new CBaseObjectPointer;
        control->object=&thing; control->refcount=owners;
        object->attach(thing); CHECK(control->refcount==owners+1);
        if (expired) control->object=0;
    }
    tracing=true;
    if (throughBase) {
        Mode* base=object;
        delete base;
    } else {
        // VC7.1 x86 thiscall puts this in ECX and the flag on the stack.
        // Slot zero is the actual compiler-generated CModeDragonBase scalar
        // deleting destructor, captured independently of the concrete table.
        typedef void* (Dragon::*ScalarDestructor)(unsigned);
        FABLE_STATIC_ASSERT(sizeof(ScalarDestructor)==sizeof(void*));
        ScalarDestructor destroy;
        memcpy(&destroy,&dragonTable[0],sizeof(destroy));
        CHECK((object->*destroy)(flags)==modeStorage);
    }
    tracing=false;
    CHECK(weaponDeletes==1 && !weaponData);
    CHECK(dragonDeletes==(groupState==2?1u:0u));
    if (groupState==1) CHECK(dragonData && dragonData->GetRefs()==1);
    bool freed=throughBase || (flags&1)!=0;
    CHECK(eventCount==(freed?4u:3u));
    CHECK(events[0]==TARGET && events[1]==FILTER_GROUP && events[2]==WEAPON_GROUP);
    if (freed) CHECK(events[3]==FREE_MODE);
    else ::operator delete(modeStorage);
    modeStorage=0;
    bool controlFreed=attach && expired && owners==0;
    CHECK(controlDeletes==(controlFreed?1u:0u));
    if (attach && !controlFreed) {
        CHECK(control && control->refcount==owners);
        CHECK(control->object==(expired?0:&thing));
        CBaseObjectPointer* remaining=control; control=0; delete remaining;
    }
}

int main() {
    const unsigned masks[]={0,1,2,3,0x80000000u,0x80000001u,0xfffffffeu,0xffffffffu};
    for (unsigned groupState=0;groupState<3;++groupState)
    for (unsigned owners=0;owners<3;++owners) {
        for (unsigned state=0;state<3;++state) {
            for (unsigned f=0;f<sizeof(masks)/sizeof(masks[0]);++f)
                exercise(masks[f],owners,state!=0,state==2,false,groupState);
            exercise(1,owners,state!=0,state==2,true,groupState);
        }
    }
    CHECK(scenario==243 && FixtureString::instances()==0);
    puts("00741d60_TEST PASS");
    return 0;
}