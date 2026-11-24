// CAIBrain::SetAllStateGroupsEnabled(bool)
// Leaf: ?SetAllStateGroupsEnabled@CAIBrain@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088B8C0, oracle length 36 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// engine/CAIBrain.h (PDB): +0x28 CVectorMap<long, CAIStateGroupBase*> StateGroupList. The vector
// map's first two dwords are begin/end of its {key, value} pairs (8 bytes each). Every state
// group's Enabled byte (+0x12 in CAIStateGroupBase) is set to the argument; the end pointer is
// re-read each iteration exactly as the retail loop does.
// Name (NF378): Ego_r.pdb / FableWin.pdb ?SetAllStateGroupsEnabled@CAIBrain@@QAEX_N@Z; BSim 1.0 and Ego_r ai_brain.obj order (was the
// stand-in CAIBrain_Methods). Declared by the engine/CAIBrain.h opt-in below.
#define FABLE_RETAIL_CAIBRAIN_SETALLSTATEGROUPSENABLED
#include "engine/CAIBrain.h"

struct CAIStateGroupBaseCalls {
    unsigned char _pad_0x00[0x12];
    bool          Enabled;          // +0x12
};

struct CStateGroupEntry {
    long                    Key;      // +0x00 state group id
    CAIStateGroupBaseCalls* Group;    // +0x04
};

struct CStateGroupVectorMap {
    CStateGroupEntry* Begin;          // +0x00
    CStateGroupEntry* End;            // +0x04
};

void CAIBrain::SetAllStateGroupsEnabled(bool enabled)
{
    CStateGroupVectorMap* groups = reinterpret_cast<CStateGroupVectorMap*>(&this->StateGroupList[0]);
    for (CStateGroupEntry* it = groups->Begin; it != groups->End; ++it)
        it->Group->Enabled = enabled;
}