// CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing(const CThing& thing) -- bind the filter to the originator by copying the thing's 64-bit UID (+0x18) into the filter's UID
// Leaf: ??0CMessageEventFilter_IsOriginatedByThing@@QAE@ABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AF2B0, length 18 bytes.
// The second of two byte-identical constructor bodies (0x006AF290 takes the thing by pointer; this one by reference). PDB: the filter holds one `ulong64 UID`; CThing's UID is the 64-bit word at +0x18. Shape: `this` copied to eax, the two halves copied through edx/ecx, `ret 4`.
#include "engine/CThing.h"

class CMessageEventFilter_IsOriginatedByThing { public:
    unsigned __int64 UID;                          // +0x00 (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing& thing);
};

CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing(const CThing& thing)
    : UID(thing.GetUniqueID())
{
}