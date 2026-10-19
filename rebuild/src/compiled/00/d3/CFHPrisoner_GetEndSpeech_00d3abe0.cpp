// CFH_Prisoner::GetEndSpeech(CCharString&). Actual string storage/lifetime header.
// Retail quest fields88/184, owner14. No matching quest/handler header available.
#include "fable_string.h"
// No-data extension recovers missing genuine string methods. Copy constructor
// is PDB egor_pdb_names.tsv30802; actual inherited storage and destructor retained.
struct CPrisonerSpeechString:CCharString {
 CPrisonerSpeechString(const char*,long);
 CPrisonerSpeechString(const CPrisonerSpeechString&);
 const CCharString& Append(const char*); //99F100
};
CPrisonerSpeechString __fastcall PrisonerNumberString(int); //99F830 resultECX, numberEDX
CPrisonerSpeechString __fastcall operator+(const char*,const CPrisonerSpeechString&); //99F690
struct CPrisonerQuestState {unsigned char Prefix[0x88];int FreedCount;unsigned char BeforeOutcome[0xf8];int Outcome;};
struct CFH_Prisoner {unsigned char Prefix[0x14];CPrisonerQuestState* Quest;bool IsActiveThreadTerminating()const;void GetEndSpeech(CPrisonerSpeechString&)const;}; //F35B30
void CFH_Prisoner::GetEndSpeech(CPrisonerSpeechString& out)const{
 static_cast<CCharString&>(out)="TEXT_QST_B01_PROPHET"+PrisonerNumberString(Quest->FreedCount+1);
 if(Quest->FreedCount==0){if(IsActiveThreadTerminating())return;out.Append("_FREED");return;}
 if(IsActiveThreadTerminating())return;
 if(Quest->Outcome==1){if(IsActiveThreadTerminating())return;out.Append("_FREED_ASEXPECTED");return;}
 if(Quest->Outcome==2){if(IsActiveThreadTerminating())return;out.Append("_FREED_DEFAULT");return;}
 if(Quest->Outcome==3){if(IsActiveThreadTerminating())return;out.Append("_FREED_SURPRISE");return;}
 IsActiveThreadTerminating();
}