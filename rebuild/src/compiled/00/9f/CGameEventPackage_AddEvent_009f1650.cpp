// CGameEventPackage::AddEvent(const CGameEvent& event) -- a replacement event overwrites the first queued event with the same type and player; otherwise (or when none matches) the event is appended and NoEvents bumped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F1650, length 105 bytes (+ int3 padding). `ret 4`. No callees.
// Layout: Ego_r.pdb CGameEventPackage (0x648 bytes): TimeStamp +0, NoEvents +4, Events[40] +8 (CGameEvent, engine/CGameEvent.h, 0x28 bytes).
// Shape: the event's type is hoisted into ebp, the loop walks &Events[i].Player (`lea ecx,[eax+0Ch]`, step 0x28) with NoEvents re-read
// every pass (unsigned `jb`); each event copy is the implicit 10-dword struct assignment (`rep movsd`), the match arm returns directly.
#include "engine/CGameEvent.h"

class CGameEventPackage { public:
    long          TimeStamp;                 // +0x00
    unsigned long NoEvents;                  // +0x04
    CGameEvent    Events[40];                // +0x08
    void AddEvent(const CGameEvent& event);
};

void CGameEventPackage::AddEvent(const CGameEvent& event)
{
    if (event.Replacement) {
        for (unsigned long i = 0; i < NoEvents; ++i) {
            if (Events[i].Type == event.Type && Events[i].Player == event.Player) {
                Events[i] = event;
                return;
            }
        }
    }
    Events[NoEvents] = event;
    ++NoEvents;
}