// CPersistContext::TransferBinary<unsigned __int64>(unsigned __int64& value) -- binary-only persist of one 64-bit value under the empty key: CRC-check, then binary save (WriteUInt64) or binary load (the traits reader); the other modes do nothing
// Leaf: TransferBinary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CC520, length 78 bytes, `ret 4`. Unreferenced
// out-of-line instance (no callers), the 64-bit sibling of the landed TransferBinary<signed char> 0x004CC4D0.
// Callees: CPersistContext::CheckCRC @ 0x00404500 (the pooled empty key 0x0122D70E), CDataOutputStream::WriteUInt64
// @ 0x00993EC0, CPersistTraits<unsigned __int64>::TransferBinaryIn @ 0x0044B890 (landed names).
// Shape: a switch with only cases 3 and 2 (`sub eax,2 ; je ; dec eax ; jne`), case 3 laid out first; the binary-load
// arm's traits object (CPersistTraits<T> = { T DefaultVal; }, user ctor, nothing stored) is a block-scoped named local
// that needs its own 8-byte frame here (`sub esp,8`; the signed-char sibling homes its 1-byte traits in the dead
// argument slot).
struct CDataOutputStream {
    void WriteUInt64(unsigned __int64 value);    // 0x00993EC0
};
struct CMemoryDataInputStream;

template <class T> struct CPersistTraits {
    T DefaultVal;                                 // +0x00
    CPersistTraits() {}                           // user ctor: DefaultVal is left unset, nothing is stored
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* value);   // 0x0044B890 for T = unsigned __int64
};

struct CPersistContext {
    unsigned char           _pad00[0x18];
    long                    Mode;                 // +0x18 (2 binary load, 3 binary save)
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
    void CheckCRC(const char* name);              // 0x00404500
    template <class T> void TransferBinary(T& value);
};

template <class T>
void CPersistContext::TransferBinary(T& value)
{
    CheckCRC("");
    switch (Mode) {
    case 3:
        PSaveStream->WriteUInt64(value);
        break;
    case 2: {
        CPersistTraits<T> traits;
        traits.TransferBinaryIn(PLoadStream, &value);
        break;
    }
    }
}

template void CPersistContext::TransferBinary<unsigned __int64>(unsigned __int64& value);