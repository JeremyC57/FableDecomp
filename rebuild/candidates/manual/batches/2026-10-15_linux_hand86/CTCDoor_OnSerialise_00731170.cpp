// CTCDoor::OnSerialise(CPersistContext& context, bool fullState) -- persist "Open" (and, for a full transfer, "OldOpen"), then re-open a door that was saved open
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00731170, length 95 bytes.
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) @ 0x004045C0 (manifest label
// Transfer<signed char>, a byte-identical instance); CTCDoor::Open(bool) @ 0x00730C00.
// CTCDoor vtable 0x01266DE4 slot 1 (rtti-vtables.tsv; chunk label "CTCDoor::vfunc_1"); slot 1 of the
// CTCBase-derived vtables is OnSerialise in 33 classes of the manifest. Literals: "Open" @ 0x01266E74,
// "OldOpen" @ 0x01266E6C.
// engine/CTCDoor.h (PDB): CurrentOpen +0x10, OldOpen +0x11. PDB CPersistContext: Mode (EMode) +0x18.
// Shape: the first default is a `false` temporary in the high byte of the `push ecx` frame slot
// (`lea eax,[esp+0Fh]` + the late `mov byte ptr [esp+1Bh],0`), the second default is CurrentOpen itself
// (its address in esi is pushed again); the context register ebx is recycled for the Mode load and
// both mode tests are against that one register before the CurrentOpen byte test.
#include "engine/CTCDoor.h"

struct CPersistContext {
    unsigned char _pad[0x18];                               // PDefinitionManager .. Finished
    long          Mode;                                     // +0x18 EMode
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
};

struct CTCDoor_Methods : CTCDoor {
    void Open(bool instant);                                // 0x00730C00
    void OnSerialise(CPersistContext& context, bool fullState);
};

void CTCDoor_Methods::OnSerialise(CPersistContext& context, bool fullState)
{
    context.Transfer("Open", CurrentOpen, false);
    if (fullState) {
        context.Transfer("OldOpen", OldOpen, CurrentOpen);
        if (context.Mode != 1 && context.Mode != 3 && CurrentOpen)
            Open(false);
    }
}
