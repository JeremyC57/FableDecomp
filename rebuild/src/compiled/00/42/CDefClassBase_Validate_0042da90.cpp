// Empty member hook. __fastcall this=ecx.
#include "engine/CDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefClassBase_Methods : CDefClassBase {
    void Hook(int init, int name);
};
void CDefClassBase_Methods::Hook(int init, int name) {}