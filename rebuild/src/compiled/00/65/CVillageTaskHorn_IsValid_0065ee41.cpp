#include "engine/CVillageTaskHorn.h"  // retyped onto the PDB layout; byte parity re-verified
extern int __fastcall probe(void*);
bool __fastcall f(CVillageTaskHorn* task){ return probe(reinterpret_cast<char*>(task)+8) != 0; }