// CPersistTraits<CPhysicalPrimitiveInit>::TransferBinaryIn(CDataInputStream*, CPhysicalPrimitiveInit*) -- read the primitive type, create the matching primitive and let it read itself
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067F870, 117 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (inline Read's slow path),
//   CPhysicalPrimitiveInit::SetPPrimitiveFromType(EPhysicalPrimitiveType) @ 0x0067F810 (walks the registered primitive
//   prototypes, clones the one whose GetType matches; returns false when none does), CPhysicalPrimitiveBase slot 5
//   (+0x14, TransferBinaryIn).
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0xC, SourceChunkBytesRemaining +0x14);
//   CPhysicalPrimitiveInit (PDB, 8 bytes) = CCountedPointer whose first word is the object pointer.
// Shape: inlined CDataInputStream::Read of a 4-byte type (64-bit position range check, intrinsic memcpy, `+=`
//   bookkeeping) into a local VC7.1 homes in the dead stream-argument slot. Speed-optimised (default) TU.
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CDataInputStreamReader : CDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

enum EPhysicalPrimitiveType { PHYSICAL_PRIMITIVE_NONE = 0 };

namespace NHitLocations {
class CPhysicalPrimitiveBase { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual long GetType() const;                               // +0x0C
    virtual void v04();
    virtual void TransferBinaryIn(CDataInputStream* stream);    // +0x14
};
}

template<class T> class CCountedPointer { public:
    T* Data;
    T* operator->() const { return Data; }
};

class CPhysicalPrimitiveInit { public:
    CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> PPrimitive;   // +0x00
    bool SetPPrimitiveFromType(EPhysicalPrimitiveType type);            // 0x0067F810
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CPhysicalPrimitiveInit> {
    void TransferBinaryIn(CDataInputStream* stream, CPhysicalPrimitiveInit* value);
};

void CPersistTraits<CPhysicalPrimitiveInit>::TransferBinaryIn(CDataInputStream* stream, CPhysicalPrimitiveInit* value)
{
    long type;
    ((CDataInputStreamReader*)stream)->Read(&type, sizeof(type));
    if (value->SetPPrimitiveFromType((EPhysicalPrimitiveType)type))
        value->PPrimitive->TransferBinaryIn(stream);
}