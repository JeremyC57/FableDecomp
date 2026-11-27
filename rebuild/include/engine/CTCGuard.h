#ifndef FABLE_ENGINE_CTCGUARD_H
#define FABLE_ENGINE_CTCGUARD_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_GUARD_REACT_TO_BRIBE_PASS27
#include "engine/CDefPointer.h"
class CTCVillage; class CGuardDef;
#ifdef FABLE_RETAIL_GUARD_CRIMINAL_UPDATE_PASS29
enum ECrimeSeverity {
 CRIME_SEVERITY_NONE, CRIME_SEVERITY_MINOR, CRIME_SEVERITY_MODERATE, CRIME_SEVERITY_SERIOUS
};
#else
enum ECrimeSeverity;
#endif
#endif
#ifdef FABLE_RETAIL_CTCGUARD_WITNESSEDCRIME
class COpinionDeed;
#endif
#ifdef FABLE_RETAIL_GUARD_FRAMEUPDATE_PASS29
#include "stlport/fable_deque.h"
#include "engine/CCrimeWaitingForBribe.h"
#endif
// Declared callable interface only; no instance is constructed or sized by this row.
class CTCGuard:public CTCBase {public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_GUARD;}
#ifdef FABLE_RETAIL_GUARD_BRIBE_FIXTURE_PASS27
    CTCGuard(CThing&);
#endif
#ifdef FABLE_RETAIL_CONTEXTSENSITIVEITEM_QUERY_PASS28
 bool PlayerIsCriminalAndCanApologise(); // Both donor PDBs and retail 0x007B2BF0.
#endif
#ifdef FABLE_RETAIL_GUARD_FRAMEUPDATE_PASS29
 virtual void FrameUpdate();
private:
 void ReportCrime(CTCVillage&,long,EOpinionDeedType,bool);
 long GetTimeAgoThatCrimesCanBeApologisedFor() const;
public:
#endif
 bool IsAttackingToFineRatherThanKill()const;
#ifdef FABLE_RETAIL_GUARD_REACT_TO_BRIBE_PASS27
#ifdef FABLE_RETAIL_GUARD_CRIMINAL_UPDATE_PASS29
 long GetFrameLastCrimeSeen()const{return FrameLastCrimeSeen;}
 ECrimeSeverity GetLastCrimeSeenSeverity()const{return LastCrimeSeenSeverity;}
#endif
 long GetBribePool() const { return BribePool; }
 long GetFramePendingCrimesAdded() const { return FramePendingCrimesAdded; }
 long GetFrameLastBribeAdded() const { return FrameLastBribeAdded; }
 long GetWaitForRebribeInSeconds() const;                                // 0x007B2990
 void ReportAllPendingCrimes(CTCVillage*);                              // 0x007B2E20
private:
 CDefPointer<const CGuardDef> PDef;
 long BribePool;
#ifdef FABLE_RETAIL_GUARD_FRAMEUPDATE_PASS29
 std::deque<CCrimeWaitingForBribe> PendingCrimes;
#else
 unsigned char PendingCrimes[0x28];
#endif
 //                                    // STLport deque<CCrimeWaitingForBribe>
 long FramePendingCrimesAdded, FrameLastBribeAdded;
 ECrimeSeverity LastCrimeSeenSeverity;
 long FrameLastCrimeSeen, FrameLastReceivedApology;
 bool WaitingForRegionKickConfirmation;
public:
#endif
#ifdef FABLE_RETAIL_CTCGUARD_WITNESSEDCRIME
 // Retail opt-in (acctC wfA1, row 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed): Ego_r.pdb ?WitnessedCrime@CTCGuard@@QAEXABVCOpinionDeed@@J@Z,
 // out of line in retail at 0x007B2FD0 (call 0x00682C26). Declaration only.
 void WitnessedCrime(const COpinionDeed& deed, long handle);          // 0x007B2FD0
#endif
};
#endif
