#include "rebuild_abi.h"
struct C { char p10[0x10]; int f10; char p14[0x1c-0x14]; int f1c; };
extern "C" char FABLE_FASTCALL FableRet_85b8a0(C *action){ return (char)((action->f1c + 1) < action->f10); }