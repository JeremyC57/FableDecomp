// CFH_Prisoner::GetEndSpeech(CCharString&). Actual string storage/lifetime header.
// Retail quest fields88/184, owner14. No matching quest/handler header available.
// Naming (NF129, 2026-11-20, q4627): 0x0099F100 is CCharString::operator+=(const char*) (Ego_r.pdb 0x00AFA9A0 + FableWin.pdb 0x02F619E0
//   ??YCCharString@@QAEABV0@PBD@Z, declared by fable_string.h); the field-free CPrisonerSpeechString adapter's Append stand-in is replaced by the
//   header operator (fable_string.h retail branch, FABLE_RETAIL_CCHARSTRING).
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
// No-data extension recovers missing genuine string methods. Copy constructor
// is PDB egor_pdb_names.tsv30802; actual inherited storage and destructor retained.
struct CPrisonerSpeechString:CCharString {
 CPrisonerSpeechString(const char*,long);
 CPrisonerSpeechString(const CPrisonerSpeechString&);
};
CCharString __fastcall GFIntToCharString(long); //99F830 resultECX, numberEDX; ?GFIntToCharString@@YI?AVCCharString@@J@Z (Ego_r / FableWin)
CCharString __fastcall operator+(const char*,const CCharString&); //99F690 ??H@YI?AVCCharString@@PBDABV0@@Z (Ego_r / FableWin)
struct CPrisonerQuestState {unsigned char Prefix[0x88];int FreedCount;unsigned char BeforeOutcome[0xf8];int Outcome;};
class CEntityScriptBase { public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ)
};
struct CFH_Prisoner {unsigned char Prefix[0x14];CPrisonerQuestState* Quest;void GetEndSpeech(CPrisonerSpeechString&)const;}; //F35B30
void CFH_Prisoner::GetEndSpeech(CPrisonerSpeechString& out)const{
 static_cast<CCharString&>(out)="TEXT_QST_B01_PROPHET"+GFIntToCharString(Quest->FreedCount+1);
 if(Quest->FreedCount==0){if(((CEntityScriptBase*)this)->IsActiveThreadTerminating())return;static_cast<CCharString&>(out) += "_FREED";return;}
 if(((CEntityScriptBase*)this)->IsActiveThreadTerminating())return;
 if(Quest->Outcome==1){if(((CEntityScriptBase*)this)->IsActiveThreadTerminating())return;static_cast<CCharString&>(out) += "_FREED_ASEXPECTED";return;}
 if(Quest->Outcome==2){if(((CEntityScriptBase*)this)->IsActiveThreadTerminating())return;static_cast<CCharString&>(out) += "_FREED_DEFAULT";return;}
 if(Quest->Outcome==3){if(((CEntityScriptBase*)this)->IsActiveThreadTerminating())return;static_cast<CCharString&>(out) += "_FREED_SURPRISE";return;}
 ((CEntityScriptBase*)this)->IsActiveThreadTerminating();
}