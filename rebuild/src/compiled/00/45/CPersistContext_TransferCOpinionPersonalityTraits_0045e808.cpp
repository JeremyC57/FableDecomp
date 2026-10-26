// CPersistContext::Transfer<COpinionPersonalityTraits*>(COpinionPersonalityTraits*& value) -- binary persist of the def's five-entry personality-traits array: CRC-check the empty key, then load (mode 2) or save (mode 3) every entry
// Leaf: ??$Transfer@PAVCOpinionPersonalityTraits@@@CPersistContext@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045E808, 91 bytes, `ret 4`; size TU without a frame pointer (like its landed caller
//         COpinionPersonalityDef::Transfer 0x0045E7CF, which passes PersonalityTraits at +0x3c; its only call site 0x0045E7DD).
// Callees: CPersistContext::CheckCRC @ 0x00404500 (key "" @ 0x0122D70E), COpinionPersonalityTraits::TransferBinaryOut
//          @ 0x0045E863 and ::TransferBinaryIn @ 0x0045E8ED (both landed; thiscall, the stream as the one stack argument).
// Identity: Ego_r.pdb names ??$Transfer@PAVCOpinionPersonalityTraits@@@CPersistContext@@QAEXPBDAAPAVCOpinionPersonalityTraits@@ABQAV1@@Z
//   and the CPersistTraits<COpinionPersonalityTraits*> TransferIn (name set only; the retail body takes just the value). PDB
//   COpinionPersonalityDef::PersonalityTraits is a COpinionPersonalityTraits* (the value is a reference to that pointer) and
//   CPersistTraits<COpinionPersonalityTraits*> is { COpinionPersonalityTraits* DefaultVal; }; COpinionPersonalityTraits is nine
//   floats (0x24 bytes), and the loop bound 0xB4 = 5 entries.
// Layout: CPersistContext Mode +0x18, PLoadStream +0x24, PSaveStream +0x28; local models.
// Shape: the two-case binary switch of 0x00458833 / 0x0045C8D3; the traits' inline transfers loop over the five entries (the
//   unsigned index strength-reduced to a byte offset in edi, `jb`; a signed index gives `jl`), re-reading the array pointer
//   through the reference every pass (the member call could change it); the unused traits local folds away. The size TU
//   keeps the traits transfers out of line (and homes the traits in the argument slot) unless they are __forceinline.
#pragma optimize("s", on)   // the def module is size-optimised

class CDataOutputStream;
class CMemoryDataInputStream;

class COpinionPersonalityTraits {
public:
    float BaseOffset, HeroStatScaling, VillageOpinionScaling, TimeScalingRunIn, TimeScalingRunOut;
    float PeakScalingIfPos, PeakScalingIfNeg, PersistScalingIfPos, PersistScalingIfNeg;   // 0x24 bytes
    void TransferBinaryOut(CDataOutputStream* stream) const;    // 0x0045E863
    void TransferBinaryIn(CMemoryDataInputStream* stream);      // 0x0045E8ED
};

const unsigned long OPINION_PERSONALITY_TRAITS_COUNT = 5;   // entries in COpinionPersonalityDef::PersonalityTraits (bound 0xB4 / 0x24)

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<COpinionPersonalityTraits*> {
    COpinionPersonalityTraits* DefaultVal;                                                      // +0x00
    __forceinline void TransferBinaryOut(CDataOutputStream* stream, COpinionPersonalityTraits* const* value)
    {
        for (unsigned long i = 0; i < OPINION_PERSONALITY_TRAITS_COUNT; ++i)
            (*value)[i].TransferBinaryOut(stream);
    }
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, COpinionPersonalityTraits** value)
    {
        for (unsigned long i = 0; i < OPINION_PERSONALITY_TRAITS_COUNT; ++i)
            (*value)[i].TransferBinaryIn(stream);
    }
};

class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    Mode;                           // +0x18 (2 = binary load, 3 = binary save)
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;                    // +0x24
    CDataOutputStream*      PSaveStream;                    // +0x28
    void CheckCRC(const char* name);                        // 0x00404500
    template <class T> void Transfer(T& value);
};

template <class T>
void CPersistContext::Transfer(T& value)
{
    CheckCRC("");
    CPersistTraits<T> traits;
    switch (Mode) {
    case 2:
        traits.TransferBinaryIn(PLoadStream, &value);
        break;
    case 3:
        traits.TransferBinaryOut(PSaveStream, &value);
        break;
    }
}

template void CPersistContext::Transfer<COpinionPersonalityTraits*>(COpinionPersonalityTraits*& value);