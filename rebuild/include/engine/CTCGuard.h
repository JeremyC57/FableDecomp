#ifndef FABLE_ENGINE_CTCGUARD_H
#define FABLE_ENGINE_CTCGUARD_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/CTCBase.h"
// Declared callable interface only; no instance is constructed or sized by this row.
class CTCGuard:public CTCBase {public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_GUARD;}
 bool IsAttackingToFineRatherThanKill()const;
};
#endif
