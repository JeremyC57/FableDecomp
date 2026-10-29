// CPersistTraits<CHeroMarriageInfo>::TransferBinaryOut(CDataOutputStream*, const CHeroMarriageInfo*) -- write the two UIDs (8 bytes each), the marriage state and the gay-marriage flag
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A3CD, 63 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataOutputStream::WriteUInt64 @ 0x00993EC0 (manifest "WriteCBYTE"; writes 8 bytes from its argument slot,
//   so each UID goes out as `push [hi] ; push [lo]`), WriteLong @ 0x00993EB0, WriteEBOOL @ 0x00993E30.
// Layout: engine/CHeroMarriageInfo.h (PDB): WifeUID +0, MaritalHomeUID +8, State (EMarriageState) +0x10,
//   GayMarriage +0x14. The enum is an opaque 4-byte field in the generated header, read as a long.
// Size-optimised TU: #pragma optimize("s", on) (the `push dword ptr [esi+N]` argument form).
#pragma optimize("s", on)
#include "engine/CHeroMarriageInfo.h"
#include "engine/CDataOutputStream.h"


template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CHeroMarriageInfo> {
    void TransferBinaryOut(CDataOutputStream* stream, const CHeroMarriageInfo* value);
};

void CPersistTraits<CHeroMarriageInfo>::TransferBinaryOut(CDataOutputStream* stream, const CHeroMarriageInfo* value)
{
    CDataOutputStream* out = (CDataOutputStream*)stream;
    out->WriteUHUGE(value->WifeUID);
    out->WriteUHUGE(value->MaritalHomeUID);
    out->WriteSLONG(*(const long*)value->State);
    out->WriteEBOOL(value->GayMarriage);
}