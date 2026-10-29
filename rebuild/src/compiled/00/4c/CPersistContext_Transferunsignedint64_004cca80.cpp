// CPersistContext::Transfer<unsigned __int64>(const char* name, unsigned __int64& value) -- the no-default transfer: binary-only, so it ignores the name and runs TransferBinary (empty-key CRC check, then WriteUInt64 on save or the traits reader on load)
// Leaf: ??$Transfer@_K@CPersistContext@@QAEXPBDAA_K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CCA80, length 78 bytes, `ret 8` (thiscall: an unused
// first argument, then the value reference). Unreferenced out-of-line instance (no callers).
// Identity: the bytes equal TransferBinary<unsigned __int64> 0x004CC520 (chunk 372) except for one extra, never-read
// 4-byte leading argument. The only two-argument CPersistContext member over unsigned __int64 in Ego_r.pdb is
// `??$Transfer@_K@CPersistContext@@QAEXPBDAA_K@Z` (const char*, unsigned __int64&), so the row is modelled as that
// Transfer forwarding to an inlined TransferBinary(value); the forwarding body is inferred (probable, not proven).
// The catalog "CPersistTraits_uint64_TransferBinary_WithArg" is behavioural.
// Callees: CPersistContext::CheckCRC @ 0x00404500 (the pooled empty key 0x0122D70E), CDataOutputStream::WriteUHUGE
// @ 0x00993EC0, CPersistTraits<unsigned __int64>::TransferBinaryIn @ 0x0044B890 (landed names).
// Shape: as 0x004CC520 -- cases 3 and 2 only (`sub eax,2 ; je ; dec eax ; jne`), case 3 first; the load arm's traits
// object a block-scoped named local in its own 8-byte frame.
class CDataOutputStream { public:
public:
    void WriteUHUGE(unsigned __int64);   // 0x00993EC0
};
class CMemoryDataInputStream;

template <class T> struct CPersistTraits {
    T DefaultVal;                                 // +0x00
    CPersistTraits() {}                           // user ctor: DefaultVal is left unset, nothing is stored
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);   // 0x0044B890 for T = unsigned __int64
};

class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    Mode;                 // +0x18 (2 binary load, 3 binary save)
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
    protected: void CheckCRC(const char* name); public:              // 0x00404500
    template <class T> void TransferBinary(T& value);
    template <class T> void Transfer(const char* name, T& value) { TransferBinary(value); }
};

template <class T>
void CPersistContext::TransferBinary(T& value)
{
    CheckCRC("");
    switch (Mode) {
    case 3:
        PSaveStream->WriteUHUGE(value);
        break;
    case 2: {
        CPersistTraits<T> traits;
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    }
    }
}

template void CPersistContext::Transfer<unsigned __int64>(const char* name, unsigned __int64& value);