// CTCDoor::Copy(const CTCBase& other) -- copy the four open-state flags and LastCreatureFoundWF from another door component
// Leaf: ?Copy@CTCDoor@@UAEXABVCTCBase@@@Z
// Naming wave 31 (family door): the own row is the retail virtual ?Copy@CTCDoor@@UAEXABVCTCBase@@@Z (Ego_r.pdb; slot 12 = CTCBase::Copy) on engine/CTCDoor.h's
//   retail branch (was the invented non-virtual CTCDoor_Methods::Copy under the bare Leaf "Copy")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007305A0, length 37 bytes.
// CTCDoor vtable 0x01266DE4 slot 12 (rtti-vtables.tsv; the chunk label is "CTCDoor::vfunc_12"); slot 12 of
// the CTCBase-derived vtables is named Copy in the manifest (4 classes); egor_signatures.tsv has
// `?Copy@CTCDoor@@UAEXABVCTCBase@@@Z` (the other component arrives as a CTCBase reference and is read as a door).
// engine/CTCDoor.h (PDB): CurrentOpen +0x10, OldOpen +0x11, Opening +0x12, Closing +0x13,
// LastCreatureFoundWF +0x14.
// Shape: `ret 4` (one argument); each byte goes through dl in member order, the long through eax last.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCDOOR
#include "engine/CTCDoor.h"                              // retail branch: CTCDoor : CTCBase, vtable 0x01266DE4

void CTCDoor::Copy(const CTCBase& other)
{
    const CTCDoor& door = static_cast<const CTCDoor&>(other);
    CurrentOpen = door.CurrentOpen;
    OldOpen = door.OldOpen;
    Opening = door.Opening;
    Closing = door.Closing;
    LastCreatureFoundWF = door.LastCreatureFoundWF;
}