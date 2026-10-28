// CPersistTraits<COpinionDeed*>::TransferBinaryIn(CMemoryDataInputStream* stream, COpinionDeed** value) -- binary read of an opinion deed: allocate a default deed, then DeedType, DeedHandle, Position, FrameHappened (no longer happening), the reported/crime flags, ExtraData and the target pointer
// Leaf: ?TransferBinaryIn@?$CPersistTraits@PAVCOpinionDeed@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C88D0, 697 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb COpinionDeed (0x28 bytes) = { EOpinionDeedType DeedType +0, long DeedHandle +4, C3DVector Position +8,
//   long FrameHappened +0x14, CIntelligentPointer<const CThing> PTarget +0x18, bool WasSearchingForWitnesses +0x20,
//   bool IsStillHappening +0x21, bool IsCrimeReportedToVillage +0x22, bool IsCrime +0x23, long ExtraData +0x24 };
//   constructor ??0COpinionDeed@@QAE@W4EOpinionDeedType@@JABVC3DVector@@PBVCThing@@_N3J@Z (landed 0x006C54C0).
// Callees: operator new @ 0x00BFEA1A, COpinionDeed::COpinionDeed @ 0x006C54C0, CDataInputStream::ReadWithSrcChunkOverflow
//   @ 0x00993CA0, CDataInputStream::Read3DVector @ 0x009950F0 (by-value C3DVector through a hidden pointer),
//   CBaseIntelligentPointer() @ 0x00A01B10 / ~CBaseIntelligentPointer @ 0x00A01C10 (the pointer traits' DefaultVal,
//   CIntelligentPointer<CThing> vtable 0x01238C6C), CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryIn
//   @ 0x006C8780. Mirror of the landed text writer 0x006C7960.
// Shape: `*value = new COpinionDeed(0, 0, C3DVector(), 0, false, false, 0)` (null-checked new, the zeroed vector temporary
//   in the frame), then CMemoryDataInputStream::Read inlined (speed TU, `position += size`) through value-returning inline
//   reads assigned as `(*value)->X = Read()` (so `*value` is re-read AFTER each read; passing `&(*value)->X` into a
//   TransferBinaryIn-style helper computes the address first and keeps the deed in edi), each local in the dead
//   value-argument slot;
//   the bools read as bytes and normalised with `setne`; the pointer traits object is built last, in the frame.
//   No EH frame. No frame pointer.
#define CMemoryDataInputStream CMemoryDataInputStreamLayout
#include "engine/CMemoryDataInputStream.h"
#undef CMemoryDataInputStream
class C3DVector;

class CDataInputStream
{
public:
    C3DVector Read3DVector();   // 0x009950F0
private:
    friend struct CMemoryDataInputStreamReader;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream, public CMemoryDataInputStreamLayout
{
};

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
void* __cdecl operator new(unsigned int size);        // 0x00BFEA1A

class CThing;
class C3DVector {
public:
    float X;
    float Y;
    float Z;
    C3DVector() : X(0.0f), Y(0.0f), Z(0.0f) {}
};

// Method view over the shared engine header (Ego_r.pdb layout: position +4, chunkBase +0xC = current chunk read
// pointer, chunkLength +0x14 = bytes left in the chunk); the overflow reader and Read3DVector are CDataInputStream's.
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CBaseIntelligentPointer {
public:
    void* PItem;                                       // +0x04
    CBaseIntelligentPointer();                         // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // <CThing> 0x01238C6C, <const CThing> 0x01237F1C
public:
    CIntelligentPointer() {}
};

enum EOpinionDeedType { OPINION_DEED_TYPE_NONE = 0, OPINION_DEED_TYPE_FORCE_DWORD = 0x7fffffff };

class COpinionDeed {                                   // 0x28 bytes
public:
    EOpinionDeedType DeedType;                         // +0x00
    long             DeedHandle;                       // +0x04
    C3DVector        Position;                         // +0x08
    long             FrameHappened;                    // +0x14
    CIntelligentPointer<const CThing> PTarget;         // +0x18
    bool             WasSearchingForWitnesses;         // +0x20
    bool             IsStillHappening;                 // +0x21
    bool             IsCrimeReportedToVillage;         // +0x22
    bool             IsCrime;                          // +0x23
    long             ExtraData;                        // +0x24
    COpinionDeed(EOpinionDeedType type, long handle, const C3DVector& position, const CThing* target,
                 bool stillHappening, bool isCrime, long extraData);   // 0x006C54C0
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    static __forceinline long Read(CMemoryDataInputStream* stream)
    {
        long number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        return number;
    }
};
template <> struct CPersistTraits<bool> {
    static __forceinline bool Read(CMemoryDataInputStream* stream)
    {
        unsigned char byte;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&byte, sizeof(byte));
        return byte != 0;
    }
};
template <> struct CPersistTraits<CIntelligentPointer<CThing> > {
    CIntelligentPointer<CThing> DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, CIntelligentPointer<CThing>* value);   // 0x006C8780
};
template <> struct CPersistTraits<COpinionDeed*> {
    void TransferBinaryIn(CMemoryDataInputStream* stream, COpinionDeed** value);
};

void CPersistTraits<COpinionDeed*>::TransferBinaryIn(CMemoryDataInputStream* stream, COpinionDeed** value)
{
    *value = new COpinionDeed(OPINION_DEED_TYPE_NONE, 0, C3DVector(), 0, false, false, 0);
    (*value)->DeedType = (EOpinionDeedType)CPersistTraits<long>::Read(stream);
    (*value)->DeedHandle = CPersistTraits<long>::Read(stream);
    (*value)->Position = static_cast<CMemoryDataInputStreamReader*>(stream)->Read3DVector();
    (*value)->FrameHappened = CPersistTraits<long>::Read(stream);
    (*value)->IsStillHappening = false;
    (*value)->IsCrimeReportedToVillage = CPersistTraits<bool>::Read(stream);
    (*value)->IsCrime = CPersistTraits<bool>::Read(stream);
    (*value)->ExtraData = CPersistTraits<long>::Read(stream);
    CPersistTraits<CIntelligentPointer<CThing> > pointerTraits;
    pointerTraits.TransferBinaryIn(stream, (CIntelligentPointer<CThing>*)&(*value)->PTarget);
}