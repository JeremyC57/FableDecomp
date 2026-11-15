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
class CThingAICreature : public CThing {};
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
class CModeGuildmasterPowerUpShield : public CModeStandBase {
public:
    CModeGuildmasterPowerUpShield(CThing& thing);
    CAnimFilterOfGroup Filter;
    long Timer;
    std::vector<CIntelligentPointer<CThing> > LightningBeamParticleEmitters;
    std::vector<CIntelligentPointer<CThingAICreature> > Guards;
};
}
NCreatureMode::CModeGuildmasterPowerUpShield::CModeGuildmasterPowerUpShield(CThing& thing)
    : CModeStandBase(thing), Filter("POWER_UP"), Timer(0)
{
}
int main(){
 for(int beams=0;beams<=4;++beams)for(int guards=0;guards<=4;++guards){
  CThingAICreature creature;
  NCreatureMode::CModeGuildmasterPowerUpShield* mode=new NCreatureMode::CModeGuildmasterPowerUpShield(creature);
  assert(mode->Parent==&creature && mode->Timer==0);
  assert(strcmp(mode->Filter.Group.Text,"POWER_UP")==0 && StringCount==1);
  assert(mode->LightningBeamParticleEmitters.empty() && mode->Guards.empty());
  mode->LightningBeamParticleEmitters.resize(beams);mode->Guards.resize(guards);
  for(int i=0;i<beams;++i){mode->LightningBeamParticleEmitters[i].Id=10+i;mode->LightningBeamParticleEmitters[i].PData=(i&1)?&creature:0;}
  for(int j=0;j<guards;++j){mode->Guards[j].Id=100+j;mode->Guards[j].PData=(j&1)?&creature:0;}
  EventCount=0;Observe=true;
  CTCCreatureModeManager::CModeBase* base=mode;delete base;
  Observe=false;assert(StringCount==0 && EventCount==beams+guards+4);
  int pos=0;for(int k=0;k<guards;++k)assert(Events[pos++]==100+k);
  for(int m=0;m<beams;++m)assert(Events[pos++]==10+m);
  assert(Events[pos++]==6);assert(Events[pos++]==5);assert(Events[pos++]==4);assert(Events[pos++]==3);
 }
 puts("00743600_TEST PASS");return 0;
}