#include "engine/CTCWife.h"  // retyped onto the PDB layout; byte parity re-verified
class CTCWife; class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
bool __fastcall IsMarriedToThePlayer(CTCWife* wife){ return wife->FrameGotMarriedToThePlayer <= CWorld::GetFrame() ? true : false; }