// GFSerialiseVectorBinaryOut<ENavigatorType>(CDataOutputStream& stream, vector<ENavigatorType>& value) -- write the element count, then every navigator type as a signed long
// Leaf: ??$GFSerialiseVectorBinaryOut@W4ENavigatorType@@
//
// Retail 0x006724F0, 55 bytes (+ int3 padding to 64), `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the only caller is the binary transfer of CThingCreatureDef::NavigatorTypes (0x006724B0, called from
//   CThingCreatureDef::Transfer 0x00671600 with &def->NavigatorTypes = +0xC8; Ego_r.pdb CThingCreatureDef has
//   vector<ENavigatorType> NavigatorTypes at PDB +0xCC); its load arm is GFSerialiseVectorBinaryIn<ENavigatorType>
//   0x00672530.  The catalog's "<float>" is a propagated label: the elements go out through WriteSLONG.
// Callee: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count and every element).
// Shape: the landed generic template (0x0046AFF4 / 0x005C12A1): `count = value.size(); stream.WriteSLONG(count);
//   for (i = 0; i < count; ++i) traits.TransferBinaryOut(stream, &value[i]);` with the enum traits inline; speed TU
//   (aligned loop head, no frame).  `value._M_start` is re-read every pass because WriteSLONG is opaque.
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; the vector is STLport's three words.
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteSLONG(long);   // 0x00993EB0
};

struct CDataOutputStreamWriter : CDataOutputStream {
};

enum ENavigatorType { NAVIGATOR_TYPE_NONE = -1, NAVIGATOR_TYPE_0 = 0 };

namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<ENavigatorType> {
    ENavigatorType DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const ENavigatorType* value) const
    {
        stream.WriteSLONG(*value);
    }
};

template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, std::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}

template void __fastcall GFSerialiseVectorBinaryOut<ENavigatorType>(CDataOutputStreamWriter& stream,
                                                                    std::vector<ENavigatorType>& value);