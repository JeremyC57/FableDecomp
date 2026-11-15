// Self-contained C++03 behavior fixture. Production header and root pasted below.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#define FABLE_RETAIL_CSOUNDEVENTGROUP
#if defined(_MSC_VER) && defined(_M_IX86)
#define FABLE_JOIN2(a,b) a##b
#define FABLE_JOIN(a,b) FABLE_JOIN2(a,b)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_JOIN(Layout_,__LINE__)[(x)?1:-1]
#else
#define FABLE_STATIC_ASSERT(x)
#endif
#ifndef FABLE_LIBPK_CSOUNDEVENTGROUP_H
#define FABLE_LIBPK_CSOUNDEVENTGROUP_H
struct CSoundEventManager;
class CSoundEventGroup;
// Descriptive audio-library identities: original names absent from all three PDBs.
// Native C20390 / C345B0: three coordinate words and a byte flag, sizeof16.
// Only used by the proven virtual prefix; no new implementation is supplied.
struct CSoundPosition;
#ifdef FABLE_RETAIL_CSOUNDEVENTGROUP
// Layout: native C21BA0 table012B3674; C23A1E array stride0xB4.
// The first nine virtual slots are declared; later unused slots are not modeled.
class CSoundEvent {
public:
    virtual long* GetIndexSlot();
    virtual void ResetLevel(CSoundEventManager* manager);
    virtual void SetUserGain(CSoundEventManager* manager,float gain);
    virtual void SetPlaybackParameter(CSoundEventManager* manager,float value);
    virtual void ResetPlaybackState(CSoundEventManager* manager);
    virtual void Set3DPosition(CSoundEventManager* manager,const CSoundPosition& position);
    virtual void SetPositionParameters(CSoundEventManager* manager,float first,float second);
    virtual void ApplyPan(CSoundEventManager* manager,float pan);
    virtual bool GetPositionFlag(CSoundEventManager* manager);
    unsigned char UnrecoveredState04[8];
    long Index;
    unsigned char UnrecoveredState10[32];
    CSoundEventGroup* Group;
    CSoundEvent* Next;
    unsigned char UnrecoveredState38[124];
};
// Layout: C240BF array stride12; ctorC29F50 vptr0, Index4=-1, First8=null.
// Slots012B40A4..012B40C4 correspond exactly to the concrete event prefix.
class CSoundEventGroup {
public:
    virtual long* GetIndexSlot();
    virtual void ResetLevel(CSoundEventManager* manager);
    virtual void SetUserGain(CSoundEventManager* manager,float gain);
    virtual void SetPlaybackParameter(CSoundEventManager* manager,float value);
    virtual void ResetPlaybackState(CSoundEventManager* manager);
    virtual void Set3DPosition(CSoundEventManager* manager,const CSoundPosition& position);
    virtual void SetPositionParameters(CSoundEventManager* manager,float first,float second);
    virtual void ApplyPan(CSoundEventManager* manager,float pan);
    virtual bool GetPositionFlag(CSoundEventManager* manager);
    long Index;
    CSoundEvent* First;
};
FABLE_STATIC_ASSERT(sizeof(CSoundEvent)==0xB4);
FABLE_STATIC_ASSERT(offsetof(CSoundEvent,Index)==0x0C);
FABLE_STATIC_ASSERT(offsetof(CSoundEvent,Group)==0x30);
FABLE_STATIC_ASSERT(offsetof(CSoundEvent,Next)==0x34);
FABLE_STATIC_ASSERT(sizeof(CSoundEventGroup)==12);
FABLE_STATIC_ASSERT(offsetof(CSoundEventGroup,Index)==4);
FABLE_STATIC_ASSERT(offsetof(CSoundEventGroup,First)==8);
#endif
#endif
bool CSoundEventGroup::GetPositionFlag(CSoundEventManager* manager)
{
    return First->GetPositionFlag(manager);
}


struct CSoundEventManager { unsigned Cookie; };
static CSoundEvent* ExpectedEvent;
static CSoundEvent* Replacement;
static CSoundEventManager* ExpectedManager;
static CSoundEventGroup* ObservedGroup;
static bool ExpectedFlag, ChangeHead;
static unsigned Calls;
static void Check(bool ok) { if(!ok) { fputs("dispatch fixture failure\n",stderr); exit(1); } }
#define UNUSED_CONTROL_DEFINITIONS(C) \
long* C::GetIndexSlot(){Check(false);return 0;} \
void C::ResetLevel(CSoundEventManager*){Check(false);} \
void C::SetUserGain(CSoundEventManager*,float){Check(false);} \
void C::SetPlaybackParameter(CSoundEventManager*,float){Check(false);} \
void C::ResetPlaybackState(CSoundEventManager*){Check(false);} \
void C::Set3DPosition(CSoundEventManager*,const CSoundPosition&){Check(false);} \
void C::SetPositionParameters(CSoundEventManager*,float,float){Check(false);} \
void C::ApplyPan(CSoundEventManager*,float){Check(false);}
UNUSED_CONTROL_DEFINITIONS(CSoundEvent)
UNUSED_CONTROL_DEFINITIONS(CSoundEventGroup)
bool CSoundEvent::GetPositionFlag(CSoundEventManager* manager)
{
    Check(this==ExpectedEvent && manager==ExpectedManager);
    ++Calls;
    if(ChangeHead) ObservedGroup->First=Replacement;
    return ExpectedFlag;
}
int main()
{
    CSoundEvent events[2]; CSoundEventGroup group; CSoundEventManager managers[4];
    for(unsigned i=0;i<4;++i) managers[i].Cookie=0x1200u+i;
    for(unsigned scenario=0;scenario<64;++scenario) {
        unsigned first=scenario&1u;
        for(unsigned i=0;i<2;++i) {
            events[i].Index=100+(long)i;events[i].Group=&group;events[i].Next=&events[1-i];
            memset(events[i].UnrecoveredState04,0x35,sizeof events[i].UnrecoveredState04);
            memset(events[i].UnrecoveredState10,0x57,sizeof events[i].UnrecoveredState10);
            memset(events[i].UnrecoveredState38,0x79,sizeof events[i].UnrecoveredState38);
        }
        group.Index=(long)scenario-32;group.First=&events[first];
        ExpectedEvent=&events[first];Replacement=&events[1-first];ObservedGroup=&group;
        ExpectedFlag=(scenario&2u)!=0;ChangeHead=(scenario&4u)!=0;
        ExpectedManager=(scenario&32u)?0:&managers[(scenario>>3)&3u];Calls=0;
        bool result=group.GetPositionFlag(ExpectedManager);
        Check(result==ExpectedFlag && Calls==1 && group.Index==(long)scenario-32);
        Check(group.First==(ChangeHead?Replacement:ExpectedEvent));
        for(unsigned i=0;i<2;++i) {
            Check(events[i].Index==100+(long)i && events[i].Group==&group && events[i].Next==&events[1-i]);
            for(unsigned j=0;j<sizeof events[i].UnrecoveredState04;++j)Check(events[i].UnrecoveredState04[j]==0x35);
            for(unsigned j=0;j<sizeof events[i].UnrecoveredState10;++j)Check(events[i].UnrecoveredState10[j]==0x57);
            for(unsigned j=0;j<sizeof events[i].UnrecoveredState38;++j)Check(events[i].UnrecoveredState38[j]==0x79);
        }
        for(unsigned i=0;i<4;++i)Check(managers[i].Cookie==0x1200u+i);
    }
    puts("00c29dc0_TEST PASS"); return 0;
}