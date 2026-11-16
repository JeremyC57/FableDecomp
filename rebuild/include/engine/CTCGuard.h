#ifndef FABLE_ENGINE_CTCGUARD_H
#define FABLE_ENGINE_CTCGUARD_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCGUARD_WITNESSEDCRIME
class COpinionDeed;
#endif
// Declared callable interface only; no instance is constructed or sized by this row.
class CTCGuard:public CTCBase {public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_GUARD;}
 bool IsAttackingToFineRatherThanKill()const;
#ifdef FABLE_RETAIL_CTCGUARD_WITNESSEDCRIME
 // Retail opt-in (acctC wfA1, row 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed): Ego_r.pdb ?WitnessedCrime@CTCGuard@@QAEXABVCOpinionDeed@@J@Z,
 // out of line in retail at 0x007B2FD0 (call 0x00682C26). Declaration only.
 void WitnessedCrime(const COpinionDeed& deed, long handle);          // 0x007B2FD0
#endif
};
#endif
