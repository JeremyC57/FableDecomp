#ifndef FABLE_ENGINE_CTCACTIONUSEWINDOW_H
#define FABLE_ENGINE_CTCACTIONUSEWINDOW_H
#include "engine/CTCActionUseBase.h"
// Both original PDBs: CTCActionUseBase inheritance, no own fields, size20.
// Donor01b994cb constructs this exact class-name literal; donor024db630 calls it.
class CTCActionUseWindow:public CTCActionUseBase {public:
 static CCharString FABLE_FASTCALL StatGetClassTypeName(){return CCharString("CTCActionUseWindow",-1);}
};
FABLE_STATIC_ASSERT(sizeof(CTCActionUseWindow)==20);
#endif
