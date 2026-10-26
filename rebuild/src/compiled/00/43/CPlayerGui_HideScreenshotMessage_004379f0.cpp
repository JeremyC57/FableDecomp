// Store -1 into a field of the object pointed to by this+0x248. __fastcall this=ecx.
// Layout: engine/CPlayerGui.h (retail STLport 4.6 layout) has PDrawNPCName_Data at +0x248 (PDrawMouseCursor_Data is +0x240;
//   the Ego_r layout had it at +0x248).
#include "engine/CPlayerGui.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CDrawNPCName { char pad[0x2c]; int field; };
struct CPlayerGui_Methods : CPlayerGui {
    void Hide();
};
#pragma pack(pop)
void CPlayerGui_Methods::Hide() { this->PDrawNPCName_Data->field = -1; }