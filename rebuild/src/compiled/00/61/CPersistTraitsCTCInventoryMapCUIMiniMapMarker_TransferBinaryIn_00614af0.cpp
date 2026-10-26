// CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferBinaryIn(CMemoryDataInputStream* stream, CUIMiniMapMarker* value) -- binary read of one minimap marker: reset it to a default marker, then Region, DefNameLength, the UIDefName string of that length, Position, GraphicIndex and Angle
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCUIMiniMapMarker@CTCInventoryMap@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00614AF0, 688 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCUIMiniMapMarker@CTCInventoryMap@@@@QAEXAAVCMemoryDataInputStream@@
//   AAVCUIMiniMapMarker@CTCInventoryMap@@@Z and the default constructor ??0CUIMiniMapMarker@CTCInventoryMap@@QAE@XZ;
//   CUIMiniMapMarker (0x1C bytes) = { long Region +0, CCharString UIDefName +4, C2DVector Position +8,
//   long DefNameLength +0x10, long GraphicIndex +0x14, float Angle +0x18 }.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("" 0x0122D70E), CCharString::operator=(const CCharString&)
//   @ 0x0099EFB0, ~CCharString @ 0x0099EAE0, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0,
//   CDataInputStream::ReadString(long) @ 0x00996BE0 (by-value CCharString through a hidden pointer).
// Shape: `*value = CUIMiniMapMarker()` (the default temporary in the frame, its zero stores sunk past the operator=
//   argument push, the members copied one by one, the temporary's string destroyed), then CMemoryDataInputStream::Read
//   inlined six times (speed TU, `position += size`) through inline CPersistTraits<T>::TransferBinaryIn, each local in
//   the dead value-argument slot (`value` lives in edi); the name length read is forwarded in eax into
//   ReadString, whose result temporary also takes the value slot. No frame pointer.
#define CMemoryDataInputStream CMemoryDataInputStreamLayout
#include "engine/CMemoryDataInputStream.h"
#undef CMemoryDataInputStream
class CCharString;

class CDataInputStream
{
public:
    CCharString ReadString(unsigned long);   // 0x00996BE0
};
class CMemoryDataInputStream : public CDataInputStream, public CMemoryDataInputStreamLayout
{
};

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharString {
public:
    class CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

// Method view over the shared engine header (Ego_r.pdb layout: position +4, chunkBase +0xC = current chunk read
// pointer, chunkLength +0x14 = bytes left in the chunk); the overflow reader and ReadString are CDataInputStream's.
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
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

class C2DVector {
public:
    float X;
    float Y;
    C2DVector() : X(0.0f), Y(0.0f) {}
};

class CTCInventoryMap {
public:
    class CUIMiniMapMarker {                           // 0x1C bytes
    public:
        long        Region;                            // +0x00
        CCharString UIDefName;                         // +0x04
        C2DVector   Position;                          // +0x08
        long        DefNameLength;                     // +0x10
        long        GraphicIndex;                      // +0x14
        float       Angle;                             // +0x18
        CUIMiniMapMarker() : Region(0), UIDefName(""), DefNameLength(0), GraphicIndex(0), Angle(0.0f) {}
    };
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, long* value)
    {
        long number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<float> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, float* value)
    {
        float number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<CTCInventoryMap::CUIMiniMapMarker> {
    void TransferBinaryIn(CMemoryDataInputStream* stream, CTCInventoryMap::CUIMiniMapMarker* value);
};

void CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferBinaryIn(CMemoryDataInputStream* stream,
                                                                         CTCInventoryMap::CUIMiniMapMarker* value)
{
    *value = CTCInventoryMap::CUIMiniMapMarker();
    CPersistTraits<long> longTraits;
    CPersistTraits<float> floatTraits;
    longTraits.TransferBinaryIn(stream, &value->Region);
    longTraits.TransferBinaryIn(stream, &value->DefNameLength);
    value->UIDefName = static_cast<CMemoryDataInputStreamReader*>(stream)->ReadString(value->DefNameLength);
    floatTraits.TransferBinaryIn(stream, &value->Position.X);
    floatTraits.TransferBinaryIn(stream, &value->Position.Y);
    longTraits.TransferBinaryIn(stream, &value->GraphicIndex);
    floatTraits.TransferBinaryIn(stream, &value->Angle);
}