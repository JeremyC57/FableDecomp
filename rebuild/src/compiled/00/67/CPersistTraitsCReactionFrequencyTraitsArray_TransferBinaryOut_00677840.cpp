// CPersistTraits<CReactionFrequencyTraitsArray>::TransferBinaryOut(CDataOutputStream*, const CReactionFrequencyTraitsArray*) -- write each of the 2 x 79 frequency traits as its type then its own data, or type 0 for an empty slot
// Retail: Fable.exe (TLC, VC7.1) @ 0x00677840, 91 bytes (+ int3 padding), `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0; CReactionFrequencyTraits virtuals slot 2 (+8, GetType) and
//   slot 4 (+0x10, TransferBinaryOut(CDataOutputStream*)).
// Layout: PDB CReactionFrequencyTraitsArray (0x278 bytes) = { CReactionFrequencyTraits* LowHigh[2][79] }. Local model.
// Shape: the nested index loops become two count-down loops over one walking element pointer (the outer counter lives
//   in the dead value-argument slot, the inner in ebx). Speed-optimised (default) TU.
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteSLONG(long);   // 0x00993EB0
};

struct CDataOutputStream_Methods : CDataOutputStream {
};

class CReactionFrequencyTraits { public:
    virtual void v00();
    virtual void v01();
    virtual long GetType() const;                                       // +0x08
    virtual void TransferBinaryIn(void* stream);                        // +0x0C
    virtual void TransferBinaryOut(CDataOutputStream* stream) const;    // +0x10
};

class CReactionFrequencyTraitsArray { public:
    CReactionFrequencyTraits* LowHigh[2][79];       // +0x000
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CReactionFrequencyTraitsArray> {
    void TransferBinaryOut(CDataOutputStream* stream, const CReactionFrequencyTraitsArray* value);
};

void CPersistTraits<CReactionFrequencyTraitsArray>::TransferBinaryOut(CDataOutputStream* stream, const CReactionFrequencyTraitsArray* value)
{
    CDataOutputStream_Methods* out = (CDataOutputStream_Methods*)stream;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 79; ++j) {
            CReactionFrequencyTraits* traits = value->LowHigh[i][j];
            if (traits != 0) {
                out->WriteSLONG(traits->GetType());
                traits->TransferBinaryOut(stream);
            }
            else
                out->WriteSLONG(0);
        }
    }
}