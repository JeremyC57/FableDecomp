#include "engine/CVillageTaskTorch.h"  // retyped onto the PDB layout; byte parity re-verified
extern int __fastcall probe(void*);
bool __fastcall f(CVillageTaskTorch* task){ return probe(reinterpret_cast<char*>(task)+0x10) != 0; }