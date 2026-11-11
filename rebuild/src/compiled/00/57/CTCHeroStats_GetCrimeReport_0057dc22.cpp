// CTCHeroStats::GetCrimeReport -- append crimes, fines and expiry time.
// Retail:0057dc22,498 bytes,thiscall,ret12.
// Leaf: ?GetCrimeReport@CTCHeroStats@@QAEXABVCCharString@@AAVCWideString@@_N@Z
// Flags: /O1 /Oy-
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_CRIME_REPORT
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_COMPLETE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
// Original local data names/types from both PDBs; donor04abded8/df28/df30.
namespace {extern const CCharString CrimeText[10];extern CCharString CrimeTotalText;extern CCharString CrimeTimeLeftText;}
// Original helper: donor01e6f5c6, native0057daf5. Complete original arithmetic and ownership.
void CTCHeroStats::GetOutstandingFineAndFramesLeft(const CCharString& region,long& fine,long& frames_left)
{
 UpdateOutstandingCrimes(region);
 frames_left=0;fine=0;
 CTCHeroOpinionDeedLog* tc_opinion_deed_log=0;
 GetParentThing().GetTC(&tc_opinion_deed_log);
 for(long n=0;n<10;++n) {
  if(OutstandingCrimes[region].Crimes[n]!=0) {
   fine+=tc_opinion_deed_log->PeekPDeedEffectsDef((EOpinionDeedType)n)->CrimeFineAmount*OutstandingCrimes[region].Crimes[n];
   long expires_on=tc_opinion_deed_log->PeekPDeedEffectsDef((EOpinionDeedType)n)->KnowledgeLifeInSeconds*15+OutstandingCrimes[region].LastTimeSeenByGuard;
   frames_left=std::max(frames_left,expires_on-GetWorld().GetFrame());
  }
 }
}
void CTCHeroStats::GetCrimeReport(const CCharString& region,CWideString& report,bool include_summary)
{
 const NGameText::CDataBank& rTextBank=CMainGameComponent::Get()->GetTextBank();
 UpdateOutstandingCrimes(region);
 CTCHeroOpinionDeedLog* tc_opinion_deed_log=0;
 GetParentThing().GetTC(&tc_opinion_deed_log);
 CWideString crime_text;
 for(long n=0;n<10;++n) {
  if(OutstandingCrimes[region].Crimes[n]!=0) {
   crime_text=CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(CrimeText[n]);
   NGameText::ConstructString(crime_text,OutstandingCrimes[region].Crimes[n],tc_opinion_deed_log->PeekPDeedEffectsDef((EOpinionDeedType)n)->CrimeFineAmount);
   report+=crime_text+L"\n";
  }
 }
 if(include_summary) {
  long fine,frames_left;
  GetOutstandingFineAndFramesLeft(region,fine,frames_left);
  crime_text=CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(CrimeTotalText);
  NGameText::ConstructString(crime_text,fine);
  report+=crime_text+L"\n";
  if(fine) {
   crime_text=CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(CrimeTimeLeftText);
   // Native0057ddd3 calls the original float overload; preserve overload identity.
   float (FABLE_CDECL* round_up)(float)=ceil;
   NGameText::ConstructString(crime_text,(long)round_up(frames_left*(1.0f/900.0f)));
   report+=crime_text+L"\n";
  }
 }
}