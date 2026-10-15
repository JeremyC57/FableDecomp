// CTCDoor::OnCreate() -- seed Opening and Closing from CurrentOpen and clear LastCreatureFoundWF
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00730580, length 17 bytes.
// CTCDoor vtable 0x01266DE4 slot 4 (rtti-vtables.tsv; the chunk label is "CTCDoor::vfunc_4"). Slot 4 of the
// CTCBase-derived vtables is OnCreate in 39 classes of the manifest, so the slot is named that way here.
// engine/CTCDoor.h (PDB): CurrentOpen +0x10, Opening +0x12, Closing +0x13, LastCreatureFoundWF +0x14.
// Shape: one byte load reused for both stores, then the immediate zero store; no callees.
#include "engine/CTCDoor.h"

struct CTCDoor_Methods : CTCDoor {
    void OnCreate();
};

void CTCDoor_Methods::OnCreate()
{
    Opening = CurrentOpen;
    Closing = CurrentOpen;
    LastCreatureFoundWF = 0;
}
