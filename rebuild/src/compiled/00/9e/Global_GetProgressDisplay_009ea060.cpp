// NProgressDisplay::GetProgressDisplay() -- return a counted reference to the global progress display
// Retail 0x009EA060, 28 bytes. Ego_r.pdb: ?GetProgressDisplay@NProgressDisplay@@YI?AV?$CCountedPointer@
// VCAProgressDisplay@@@@XZ, i.e. `CCountedPointer<CAProgressDisplay> __fastcall GetProgressDisplay()`:
// the by-value counted pointer comes back through the hidden result pointer in ecx (`mov eax,ecx`
// returns it). The body is `return PProgressDisplay;` -- the namespace-scope static counted
// pointer NProgressDisplay::PProgressDisplay (PDB: "NProgressDisplay::`dynamic initializer for
// 'PProgressDisplay''"; retail storage 0x013CAA38 Data / 0x013CAA3C Info, the pair
// SetProgressDisplay 0x009E9FD0 assigns and the atexit release 0x0122A890 drops).
// Shape: CCountedPointer's add-ref copy constructor copied out of the global object: Data, then
// Info (loaded into edx, copied to ecx for the null test), then `++Info->owners`. Building the
// result from the two words through a separate two-argument constructor hoists the Info load
// above the Data store (26 v 28), so the global must be the counted-pointer object itself.
// Leaf: ?GetProgressDisplay@NProgressDisplay@@YI?AV?$CCountedPointer@VCAProgressDisplay@@@@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).

class CAProgressDisplay;

typedef void (__fastcall *FableDestroyReferencedObject)(void* object);

struct FableReferenceCount              // CCountedPointer count block
{
    long owners;                        // +0x00
    FableDestroyReferencedObject destroy;   // +0x04
    void* object;                       // +0x08
};

#include "engine/CCountedPointer.h"

namespace NProgressDisplay
{
    extern CCountedPointer<CAProgressDisplay> PProgressDisplay;      // 0x013CAA38 {Data, Info}

    CCountedPointer<CAProgressDisplay> __fastcall GetProgressDisplay();

    CCountedPointer<CAProgressDisplay> __fastcall GetProgressDisplay()
    {
        return PProgressDisplay;
    }
}
