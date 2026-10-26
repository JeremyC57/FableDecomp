// CPersistTraits<CAttackHistoryCombo>::TransferBinaryOut(CDataOutputStream& stream, const CAttackHistoryCombo& value) -- write the combo's attack count, each (attack type, damage attribute) pair, then its multiplier
// Leaf: ?TransferBinaryOut@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D9EE0, length 82 bytes, thiscall `ret 8` (`this` = the traits object,
// unused). Single caller: GFSerialiseVectorBinaryOut<CAttackHistoryCombo> 0x006DC550 (__fastcall stream/vector; it
// walks the 16-byte elements and calls this on its frame-local traits object, `lea ecx,[esp+18h]`). The manifest
// labels "GFSerialiseVectorBinaryOut_WritePair" (this) and "GFSerialiseVectorBinaryOut<CMusicEntry>" (the caller)
// are descriptive/propagated.
// Callees: CDataOutputStream::WriteSLONG(long) @ 0x00993EB0, WriteFloat(float) @ 0x00993EE0 (manifest "WriteCBYTE" is
// propagated onto both; landed users name them).
// Layout: CAttackHistoryCombo (Ego_r.pdb; retail 0x10 with the 12-byte STLport vector): AttackCombo
// vector<pair<EAttackHistoryType, EDamageAttribute>> +0, float Multiplier +0xC.
// Shape: the count as `size()` (`sar eax,3`), then an iterator loop with end() re-read every pass, two WriteSLONG per
// pair; the float goes out by value from +0xC.
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteFloat(float);   // 0x00993EE0
    void WriteSLONG(long);   // 0x00993EB0
};

enum EAttackHistoryType { EAttackHistoryType_Force32 = 0x7fffffff };
enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

struct COutputStreamMethods : CDataOutputStream {
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
};
}

class CAttackHistoryCombo {                                 // Ego_r.pdb (retail 0x10 bytes)
public:
    std::vector<std::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;   // +0x00
    float Multiplier;                                                           // +0x0C
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CAttackHistoryCombo> {
    CAttackHistoryCombo DefaultVal;
    void TransferBinaryOut(COutputStreamMethods& stream, const CAttackHistoryCombo& value) const;
};

void CPersistTraits<CAttackHistoryCombo>::TransferBinaryOut(COutputStreamMethods& stream, const CAttackHistoryCombo& value) const
{
    stream.WriteSLONG(value.AttackCombo.size());
    for (const std::pair<EAttackHistoryType, EDamageAttribute>* it = value.AttackCombo.begin(); it != value.AttackCombo.end(); ++it) {
        stream.WriteSLONG(it->first);
        stream.WriteSLONG(it->second);
    }
    stream.WriteFloat(value.Multiplier);
}