// CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing(const CThing* thing) -- bind the filter to the originator by copying the thing's 64-bit UID (+0x18) into the filter's UID
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AF290, length 18 bytes.
// No callees. PDB: the filter holds one `ulong64 UID`; CThing's UID is the 64-bit word at +0x18. Shape: `this` copied to eax, the two halves copied through edx/ecx, `ret 4`.
struct CThing {
    unsigned char      _pad00[0x18];
    unsigned __int64   UID;                        // +0x18
};

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;                          // +0x00 (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);
};

CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing(const CThing* thing)
    : UID(thing->UID)
{
}