#include "rebuild_abi.h"
extern "C" void FABLE_FASTCALL Val_938ed0_A(void* stateGroup);
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
struct C { char p[0x3c]; int f3c; char Valid(); };
char C::Valid(){ Val_938ed0_A(this); int b=CWorld::GetFrame(); return (char)(b <= this->f3c); }