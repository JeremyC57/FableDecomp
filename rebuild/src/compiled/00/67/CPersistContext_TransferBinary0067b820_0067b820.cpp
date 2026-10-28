// CPersistContext::TransferBinary_0067b820(CReactionFrequencyTraitsArray& value) -- unnamed binary transfer of a reaction-frequency traits array: CRC-check the empty key, then load or save it through a default CPersistTraits<CReactionFrequencyTraitsArray>
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067B820, length 140 bytes (144 with int3 padding).
// Leaf: TransferBinary_0067b820
// Callees: CPersistContext::CheckCRC(const char*) @ 0x00404500 (with the empty literal @ 0x0122D70E),
// CPersistTraits<CReactionFrequencyTraitsArray>::TransferBinaryIn @ 0x006778A0 (load stream +0x24) and
// TransferBinaryOut @ 0x00677840 (save stream +0x28), ~CReactionFrequencyTraitsArray @ 0x0067A520 (all manifest-named).
// Label correction: the manifest's "CPersistContext_TransferableComponent<CReactionFrequencyTraitsArray>::TransferOut"
// is propagated: `this` is the CPersistContext (Mode +0x18, PLoadStream +0x24, PSaveStream +0x28, as the landed
// CPersistContext::Transfer<CIntelligentPointer<CThing>> 0x006C9A10) and the single argument is the value
// (`ret 4`; callers 0x006798F4 / 0x0067BEE5 push only the value). FableWin.pdb has only the named
// Transfer<CReactionFrequencyTraitsArray>(const char*, T&[, const T&]) members; this body handles just the two binary
// modes and its key is a constant "", so it is either an LTCG-specialised copy of the named Transfer with the key
// folded away or an unnamed binary helper -- the name is descriptive.
// Layout: Ego_r.pdb CReactionFrequencyTraitsArray (632 bytes): CReactionFrequencyTraits* LowHigh[2][79]; its inline
// constructor zeroes both rows in one unsigned loop (the traits' DefaultVal fills the 0x278-byte frame).
// Shape: the mode test as `sub 2 ; je ; dec ; jne` (case 3 laid out first), each arm ending in the out-of-line
// destructor and its own return.
class CMemoryDataInputStream;
class CDataOutputStream;
class CReactionFrequencyTraits;

class CReactionFrequencyTraitsArray {
public:
    CReactionFrequencyTraits* LowHigh[2][79];              // +0x000 / +0x13c
    CReactionFrequencyTraitsArray()
    {
        for (unsigned int i = 0; i < 79; ++i) {
            LowHigh[0][i] = 0;
            LowHigh[1][i] = 0;
        }
    }
    ~CReactionFrequencyTraitsArray();                      // 0x0067A520
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);        // 0x006778A0 for this T
    void TransferBinaryOut(CDataOutputStream& stream, const T& value);      // 0x00677840 for this T
};

class CPersistContext { public:
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;          // +0x24
    CDataOutputStream*      PSaveStream;          // +0x28
    protected: void CheckCRC(const char* name); public:              // 0x00404500
    void TransferBinary_0067b820(CReactionFrequencyTraitsArray& value);
};

void CPersistContext::TransferBinary_0067b820(CReactionFrequencyTraitsArray& value)
{
    CheckCRC("");
    CPersistTraits<CReactionFrequencyTraitsArray> traits;
    switch (Mode) {
    case 2:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case 3:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}