// CFH_Prisoner::GetEndCutscene(CCharString&) const -- form cage cutscene and append history variant.
// Native00D3AB00,213 bytes. String helpers99F830,99F690,99EFB0,99EAE0,99F100; cancellationF35B30.
// Donor CFH_Prisoner MyCageNumber+20,ParentClass+14; native matches. Parent history at184 needs donor container-size mapping audit.
// Naming (NF129, 2026-11-20, q4627): 0x0099F100 is CCharString::operator+=(const char*) (Ego_r.pdb 0x00AFA9A0 + FableWin.pdb 0x02F619E0
//   ??YCCharString@@QAEABV0@PBD@Z, declared by fable_string.h); the field-free CharStringMethods adapter's Append stand-in is replaced by the
//   header operator (fable_string.h retail branch, FABLE_RETAIL_CCHARSTRING).
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
namespace NScript { class CPredicateBase; }
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
  if(ParentClass->MissionHistory==1) { if(!IsActiveThreadTerminating()) out += "S"; }
  else if(ParentClass->MissionHistory==2) { if(!IsActiveThreadTerminating()) out += "D"; }
  else if(ParentClass->MissionHistory==3) { if(!IsActiveThreadTerminating()) out += "X"; }
  else IsActiveThreadTerminating();
 }
}