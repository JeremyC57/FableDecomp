// CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryOut(CDataOutputStream*, const CIntelligentPointer<CThing>*) -- write the pointed-to thing's UniqueID, or 0 when the pointer is empty
// Leaf: ?TransferBinaryOut@?$CPersistTraits@V?$CIntelligentPointer@VCThing@@@@@@QAEXPAVCDataOutputStream@@PBV?$CIntelligentPointer@VCThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C52D0, 52 bytes (+ int3 padding), `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (called twice: the null test, then the UID read),
//   CDataOutputStream::WriteUInt64 @ 0x00993EC0 (manifest "WriteCBYTE"; writes 8 bytes from its argument slot).
// Layout: CThing::UniqueID (ulong64) at +0x18 (PDB); CBaseIntelligentPointer is {vptr/pad, PData} (8 bytes), only
//   its GetPItem is used. The 64-bit zero lives in esi:edi across the branch. Speed-optimised (default) TU.
#include "engine/CDataOutputStream.h"

#include "engine/CThing.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CIntelligentPointer<CThing> > {
    void TransferBinaryOut(CDataOutputStream* stream, const CIntelligentPointer<CThing>* value);
};

void CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryOut(CDataOutputStream* stream, const CIntelligentPointer<CThing>* value)
{
    unsigned __int64 uid = 0;
    if (value->Get() != 0)
        uid = value->Get()->GetUniqueID();
    ((CDataOutputStream*)stream)->WriteUHUGE(uid);
}