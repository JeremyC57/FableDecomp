#include "engine/CTCWife.h"  // retyped onto the PDB layout; byte parity re-verified
struct CTCWife; extern int __fastcall imp_helper(CTCWife*);
bool __fastcall IsMarriedToThePlayer(CTCWife* wife){ return wife->FrameGotMarriedToThePlayer <= imp_helper(wife) ? true : false; }