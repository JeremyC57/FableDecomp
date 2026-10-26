// CPersistTraits<CPhysicalPrimitiveInit>::TransferBinaryOut(CDataOutputStream*, const CPhysicalPrimitiveInit*) -- write the primitive's type and let the primitive write itself, or write type 0 when there is none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067F7D0, 57 bytes (+ int3 padding), `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0; CPhysicalPrimitiveBase virtuals slot 3 (+0xC, GetType -- the
//   slot SetPPrimitiveFromType @ 0x0067F810 compares against the requested type) and slot 6 (+0x18, TransferBinaryOut).
// Layout (PDB CPhysicalPrimitiveInit, 8 bytes): PPrimitive (CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>)
//   +0, whose first word is the raw object pointer. No engine header exists; local model.
// Shape: the pointer is re-read through the counted pointer for the second virtual call (`mov ecx,[edi]` again);
//   speed-optimised (default) TU.
#include "engine/CDataOutputStream.h"

struct CDataOutputStream_Methods : CDataOutputStream {
    void WriteLong(long value);                     // 0x00993EB0
};

namespace NHitLocations {
class CPhysicalPrimitiveBase { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual long GetType() const;                               // +0x0C
    virtual void v04();
    virtual void TransferBinaryIn(void* stream);                // +0x14
    virtual void TransferBinaryOut(CDataOutputStream* stream) const;   // +0x18
};
}

template<class T> class CCountedPointer { public:
    T* Data;
    T* operator->() const { return Data; }
    T* Get() const { return Data; }
};

class CPhysicalPrimitiveInit { public:
    CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> PPrimitive;   // +0x00
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CPhysicalPrimitiveInit> {
    void TransferBinaryOut(CDataOutputStream* stream, const CPhysicalPrimitiveInit* value);
};

void CPersistTraits<CPhysicalPrimitiveInit>::TransferBinaryOut(CDataOutputStream* stream, const CPhysicalPrimitiveInit* value)
{
    if (value->PPrimitive.Get() != 0) {
        ((CDataOutputStream_Methods*)stream)->WriteLong(value->PPrimitive->GetType());
        value->PPrimitive->TransferBinaryOut(stream);
    }
    else
        ((CDataOutputStream_Methods*)stream)->WriteLong(0);
}