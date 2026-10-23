#include "rebuild_abi.h"
struct M { bool Check_0085f010(); };
extern "C" M* FABLE_FASTCALL Enemy_954130_A(void* self);
struct C { char p[0x14]; char s14; char Has(); };
char C::Has(){ M* m=Enemy_954130_A((char*)this+0x14); bool r=m->Check_0085f010(); return (char)(r!=false); }