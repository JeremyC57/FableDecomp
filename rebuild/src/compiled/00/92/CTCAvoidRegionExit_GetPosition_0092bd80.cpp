// CTCAvoidRegionExit::GetPosition() -- the entrance marker's position while the marker is set and alive, else the zero vector
// Leaf: ?GetPosition@CTCAvoidRegionExit@@QAE?AVC3DVector@@XZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092BD80, length 107 bytes. Returns C3DVector through the hidden pointer; `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3: the inline HasPosition() test, then the position fetch),
// CThing::GetPosition @ 0x004C73D0 (const C3DVector&; catalog "CVertexBufferWin32::DoSizeof").
// Shape: one conditional expression `HasPosition() ? marker->GetPosition() : C3DVector(0,0,0)`: both arms yield an address in eax
// (the reference, or a frame temporary at esp+4) and one shared three-dword copy into the result through edx (the memberwise
// user copy ctor; the implicit copy goes through esi).
// Layout: PDB CTCAvoidRegionExit (0x20): EntranceMarker +0xC, ExitMarker +0x14 (CIntelligentPointer<CThing>), NextFrameCheck +0x1C;
// retail CThing Dead = bit 0 of +0x91.
#include "engine/C3DVector.h"
#include "engine/CThing.h"
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCAvoidRegionExit : public CTCBase {
public:
    CIntelligentPointer<CThing> EntranceMarker;     // +0x0c CIntelligentPointer<CThing>
    CIntelligentPointer<CThing> ExitMarker;         // +0x14
    unsigned long NextFrameCheck;               // +0x1c
    bool HasPosition() { return EntranceMarker.Get() && !EntranceMarker.Get()->IsDead(); }
    C3DVector GetPosition();
};

C3DVector CTCAvoidRegionExit::GetPosition()
{
    return HasPosition() ? EntranceMarker.Get()->GetPos() : C3DVector(0.0f, 0.0f, 0.0f);
}