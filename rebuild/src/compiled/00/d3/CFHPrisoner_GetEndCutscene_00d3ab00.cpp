// CFH_Prisoner::GetEndCutscene(CCharString&) const -- form cage cutscene and append history variant.
// Native00D3AB00,213 bytes. String helpers99F830,99F690,99EFB0,99EAE0,99F100; cancellationF35B30.
// Donor CFH_Prisoner MyCageNumber+20,ParentClass+14; native matches. Parent history at184 needs donor container-size mapping audit.
#include "fable_string.h"
namespace NScript { class CPredicateBase; }
struct CharStringMethods : CCharString { CCharString& Append(const char*); }; //0099F100, field-free method adapter
CCharString __fastcall GFIntToCharString(long); //0099F830
CCharString __fastcall operator+(const char*,const CCharString&); //0099F690
struct FireHeartRetailView { unsigned char OtherState[0x184]; long MissionHistory; };
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

struct CFH_Prisoner : public CEntityScriptBase {
 unsigned char ScriptBase[0x14]; FireHeartRetailView* ParentClass; void* PMasterData;
 bool Chosen; long MyCageNumber;
 void GetEndCutscene(CCharString&) const;
};
void CFH_Prisoner::GetEndCutscene(CCharString& out) const {
 out="CS_FIREHEART_KILL"+GFIntToCharString(MyCageNumber+1);
 if(MyCageNumber>0 && !IsActiveThreadTerminating()) {
  if(ParentClass->MissionHistory==1) { if(!IsActiveThreadTerminating()) static_cast<CharStringMethods&>(out).Append("S"); }
  else if(ParentClass->MissionHistory==2) { if(!IsActiveThreadTerminating()) static_cast<CharStringMethods&>(out).Append("D"); }
  else if(ParentClass->MissionHistory==3) { if(!IsActiveThreadTerminating()) static_cast<CharStringMethods&>(out).Append("X"); }
  else IsActiveThreadTerminating();
 }
}