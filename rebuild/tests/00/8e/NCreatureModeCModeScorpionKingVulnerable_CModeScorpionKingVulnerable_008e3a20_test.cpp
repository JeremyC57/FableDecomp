// Full implicit target destructor composition and original constructor body.
// Portable callee fixtures expose cleanup ordering and virtual dispatch.
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <string.h>
static bool Observe;
static int Events[64], EventCount, StringCount;
static void Note(int n) { if (Observe) Events[EventCount++] = n; }
class CThing { public: virtual ~CThing() {} };
class CThingCreatureBase : public CThing {};
class CBaseIntelligentPointer {
public:
    int Id; CThing* PData;
    CBaseIntelligentPointer():Id(0),PData(0) {}
    virtual ~CBaseIntelligentPointer() { Note(Id); PData=0; }
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {};
class CCharString {
public:
    char Text[32];
    CCharString() {Text[0]=0;++StringCount;}
    CCharString(const char* text) {strcpy(Text,text);++StringCount;}
    ~CCharString() {Note(6);--StringCount;}
};
class CAnimFilterBase {public:virtual ~CAnimFilterBase(){Note(5);}};
class CAnimFilterOfGroup : public CAnimFilterBase {
public:
    CCharString Group; unsigned long GroupCRC;
    CAnimFilterOfGroup(const CCharString& group) {Group=group;GroupCRC=0;}
};
class CTCCreatureModeManager {public:class CModeBase {public:virtual ~CModeBase(){Note(3);}};};
class CModeStandBase : public CTCCreatureModeManager::CModeBase {
public:
    CThing* Parent;
    CModeStandBase(CThing& thing):Parent(&thing) {}
    virtual ~CModeStandBase(){Note(4);}
};
namespace NCreatureMode {
class CModeScorpionKingVulnerable : public CModeStandBase {
public:
    CModeScorpionKingVulnerable(CThing& thing);
    long RecoveryCount;
    CAnimFilterOfGroup Filter;
    CThingCreatureBase* PCreature;
    bool bRecovering;
    CIntelligentPointer<CThingCreatureBase> PTarget;
    std::vector<CIntelligentPointer<CThing> > Obstructions;
    CIntelligentPointer<CThing> PEyeEmitter;
    CIntelligentPointer<CThing> PStingEmitter;
    long FrameCount; bool EffectsDone; long TailEffectIndex;
    float CollisionRadius; bool TailCurlFinished;
};
}
NCreatureMode::CModeScorpionKingVulnerable::CModeScorpionKingVulnerable(CThing& thing)
    : CModeStandBase(thing), Filter("VULNERABLE")
{
    PCreature = (CThingCreatureBase*)&thing;
}
int main() {
    for(int count=0;count<=5;++count) for(int linked=0;linked!=2;++linked) {
        CThingCreatureBase creature;
        NCreatureMode::CModeScorpionKingVulnerable* mode = new NCreatureMode::CModeScorpionKingVulnerable(creature);
        assert(mode->Parent==&creature && mode->PCreature==&creature);
        assert(strcmp(mode->Filter.Group.Text,"VULNERABLE")==0 && StringCount==1);
        assert(mode->Obstructions.empty());
        mode->Obstructions.resize(count);
        mode->PTarget.Id=7;mode->PEyeEmitter.Id=8;mode->PStingEmitter.Id=9;
        mode->PTarget.PData=mode->PEyeEmitter.PData=mode->PStingEmitter.PData=linked?&creature:0;
        for(int i=0;i<count;++i){mode->Obstructions[i].Id=10+i;mode->Obstructions[i].PData=(i&1)?&creature:0;}
        EventCount=0;Observe=true;
        CTCCreatureModeManager::CModeBase* base=mode;delete base;
        Observe=false;
        assert(EventCount==count+7 && StringCount==0);
        int pos=0;assert(Events[pos++]==9);assert(Events[pos++]==8);
        for(int j=0;j<count;++j)assert(Events[pos++]==10+j);
        assert(Events[pos++]==7);assert(Events[pos++]==6);assert(Events[pos++]==5);
        assert(Events[pos++]==4);assert(Events[pos++]==3);
    }
    puts("008e3a20_TEST PASS");return 0;
}