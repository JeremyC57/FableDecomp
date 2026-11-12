#ifndef FABLE_ENGINE_CTCELECTROCUTEONTOUCH_H
#define FABLE_ENGINE_CTCELECTROCUTEONTOUCH_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/CTCBase.h"
class CTCElectrocuteOnTouch:public CTCBase {public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_ELECTROCUTE_ON_TOUCH;}
 void OnHit(CThing&);
};
#endif
