// CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryOut(CDataOutputStream*, const CIntelligentPointer<CThing>*) -- write the pointed-to thing's UniqueID, or 0 when the pointer is empty
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C52D0, 52 bytes (+ int3 padding), `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (called twice: the null test, then the UID read),
//   CDataOutputStream::WriteUInt64 @ 0x00993EC0 (manifest "WriteCBYTE"; writes 8 bytes from its argument slot).
// Layout: CThing::UniqueID (ulong64) at +0x18 (PDB); CBaseIntelligentPointer is {vptr/pad, PData} (8 bytes), only
//   its GetPItem is used. The 64-bit zero lives in esi:edi across the branch. Speed-optimised (default) TU.
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteUHUGE(unsigned __int64);   // 0x00993EC0
};

struct CDataOutputStream_Methods : CDataOutputStream {
};

class CThing { public:
    unsigned char    _pad_0x00[0x18];   // CBaseObject + CThing bookkeeping
    unsigned __int64 UniqueID;          // +0x18
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CIntelligentPointer<CThing> > {
    void TransferBinaryOut(CDataOutputStream* stream, const CIntelligentPointer<CThing>* value);
};

void CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryOut(CDataOutputStream* stream, const CIntelligentPointer<CThing>* value)
{
    unsigned __int64 uid = 0;
    if (value->GetPItem() != 0)
        uid = value->GetPItem()->UniqueID;
    ((CDataOutputStream_Methods*)stream)->WriteUHUGE(uid);
}