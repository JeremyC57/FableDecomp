// GFSerialiseVectorBinaryOut<CAttackHistoryCombo>(CDataOutputStream& stream, vector<CAttackHistoryCombo>& value) -- write the element count, then every 16-byte attack-history combo through its persist traits
// Leaf: ??$GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DC550, 61 bytes, `__fastcall` (stream in ecx, vector in edx), plain `ret`; a speed
//   TU (no frame pointer; the 16-byte `sub esp,10h` frame is the traits object, never initialised).
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0 (the "WriteCBYTE" label is propagated; it writes a long),
//   CPersistTraits<CAttackHistoryCombo>::TransferBinaryOut @ 0x006D9EE0.
// Callers: CHeroExperienceDef's transfer 0x006D91AB (its last member, +0xA8 = AttackHistoryCombos) and the two
//   CPersistContext::Transfer<vector<CAttackHistoryCombo> > arms 0x006DD62C / 0x006DD66C (key 0x0122D70E). The catalog's
//   <CAttackHistoryCombo> is propagated (same 16-byte stride); the CHeroExperienceDef TU owns this body, and its 16-byte vector
//   growth 0x006DCA50 copy-constructs the {vector<pair>, float} record.
// Layout: PDB CAttackHistoryCombo = { vector<pair<EAttackHistoryType, EDamageAttribute> > AttackCombo; float Multiplier; }
//   (16 bytes with the 12-byte retail STLport vector) and CPersistTraits<T> = { T DefaultVal; }. Retail never writes the
//   traits frame, so the DefaultVal is modelled as a plain record (the vector as its three raw words).
// Shape: the landed GFSerialiseVectorBinaryOut template (0x005C12A1) without the size pragmas: count = size()
//   (`sar 4`), WriteLong(count), then traits.TransferBinaryOut(stream, &value[i]) with the index strength-reduced
//   to a byte offset in edi and a down-counter in esi.
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

class CAttackHistoryCombo { public:                                // 16 bytes
    void* AttackCombo[3];                                   // +0x00 vector<pair<EAttackHistoryType, EDamageAttribute> > words
    float Multiplier;                                       // +0x0C
};

namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const T* value) const;   // 0x006D9EE0
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

template void __fastcall GFSerialiseVectorBinaryOut<CAttackHistoryCombo>(CDataOutputStreamWriter& stream, std::vector<CAttackHistoryCombo>& value);