// CTCDoor::IsClosed() const -- true when the door is neither opening, closing nor open
// Leaf: IsClosed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082CF40, length 30 bytes.
// Callers: CThingBuilding::LockAllDoors @ 0x0082DB00 (on each door component found through the building's
// interface map; the result decides whether the door can be locked straight away). engine/CTCDoor.h: CurrentOpen
// +0x10, Opening +0x12, Closing +0x13. The manifest label "CTCFadeOutAndIn::IsFading" is propagated.
// Shape: `return !Opening && !Closing && !CurrentOpen;` -- three byte tests each jumping to the shared `xor eax,eax`
// exit placed last, the `mov eax,1` fall-through first (full-register 0/1 from the short-circuit chain).
#include "engine/CTCDoor.h"

struct CTCDoor_Methods : CTCDoor {
    bool IsClosed() const;
};

bool CTCDoor_Methods::IsClosed() const
{
    return !Opening && !Closing && !CurrentOpen;
}