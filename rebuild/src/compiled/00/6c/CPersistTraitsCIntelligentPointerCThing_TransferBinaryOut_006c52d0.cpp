// CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryOut(CDataOutputStream*, const CIntelligentPointer<CThing>*) -- write the pointed-to thing's UniqueID, or 0 when the pointer is empty
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C52D0, 52 bytes (+ int3 padding), `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (called twice: the null test, then the UID read),
//   CDataOutputStream::WriteUInt64 @ 0x00993EC0 (manifest "WriteCBYTE"; writes 8 bytes from its argument slot).
// Layout: CThing::UniqueID (ulong64) at +0x18 (PDB); CBaseIntelligentPointer is {vptr/pad, PData} (8 bytes), only
//   its GetPItem is used. The 64-bit zero lives in esi:edi across the branch. Speed-optimised (default) TU.
#include "engine/CDataOutputStream.h"

struct CDataOutputStream_Methods : CDataOutputStream {
    void WriteUInt64(unsigned __int64 value);       // 0x00993EC0
};

class CThing { public:
    unsigned char    _pad_0x00[0x18];   // CBaseObject + CThing bookkeeping
    unsigned __int64 UniqueID;          // +0x18
};

struct CBaseIntelligentPointer {
    void* _padding_;                    // +0x00
    void* PData;                        // +0x04 CBaseObjectPointer*
    CThing* GetPItem() const;           // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CIntelligentPointer<CThing> > {
    void TransferBinaryOut(CDataOutputStream* stream, const CIntelligentPointer<CThing>* value);
};

void CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryOut(CDataOutputStream* stream, const CIntelligentPointer<CThing>* value)
{
    unsigned __int64 uid = 0;
    if (value->GetPItem() != 0)
        uid = value->GetPItem()->UniqueID;
    ((CDataOutputStream_Methods*)stream)->WriteUInt64(uid);
}