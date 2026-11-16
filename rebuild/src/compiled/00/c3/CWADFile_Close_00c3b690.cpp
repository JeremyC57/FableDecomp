#include "rebuild_abi.h"
extern char* _afxPchNil;
struct C { void* f0; int f4; int f8; };
extern "C" C* FABLE_FASTCALL FableRet_c3b690(C* self){
    self->f0 = _afxPchNil;
    self->f4 = 0;
    self->f8 = 0;
    return self;
}