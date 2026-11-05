#include "rebuild_abi.h"
#include "engine/CThing.h"
struct C {  };
extern "C" void FABLE_STDCALL Dest_5e9a60(C* p){ ((CThing*)p)->Kill(1); }