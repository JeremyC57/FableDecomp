// CAIStateGroup_JackDragon::Cleanup() -- kill the invisible helper creature if it is still alive.
// Leaf: ?Cleanup@CAIStateGroup_JackDragon@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009500A0, length 49 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, on PInvisibleBodgeCreature at +0x44),
// CThing::Kill(bool) @ 0x004C9B80.
//
// Shape: the landed IS_THING_DEAD pattern (GetPItem null, then the dead bit +0x91 & 1) guards a
// `GetPItem()->Kill(true)`; `this` is only needed to form the member address (`lea esi,[ecx+44h]`). The
// engine header keeps the pointer as an opaque byte array, so the layout is modelled locally with its name.
#include "engine/CThing.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CAIStateGroupBase { public:
    virtual void v00();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    unsigned char _pointers[0x30];              // +0x14 PEnemy, PLandedMarker, PHoverMarker1/2, PStrafeMarker1/2
    CIntelligentPointer<CThing> PInvisibleBodgeCreature;   // +0x44 CIntelligentPointer<CThingAICreature>
    void Cleanup();
};

void CAIStateGroup_JackDragon::Cleanup()
{
    if (PInvisibleBodgeCreature.Get() != 0) {
        if (!PInvisibleBodgeCreature.Get()->IsDead())
            PInvisibleBodgeCreature.Get()->Kill(true);
    }
}