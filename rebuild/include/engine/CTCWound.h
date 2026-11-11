#ifndef FABLE_ENGINE_CTCWOUND_H
#define FABLE_ENGINE_CTCWOUND_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/CTCBase.h"
// Declared callable interface only; no instance is constructed or sized by this row.
class CTCWound:public CTCBase {public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_WOUND;}
 bool AddPossibleWoundGivenStrikePoint(const C3DVector&);
};
#endif
