// CPersistContext::TransferBinary<signed char>(signed char& value) -- binary-only persist of one signed char under the empty key: CRC-check, then binary save (sign-extended, as a long) or binary load; the other modes do nothing
// Leaf: TransferBinary
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CC4D0, 66 bytes, `ret 4`. Unreferenced out-of-line instance.
// Callees: CPersistContext::CheckCRC 0x00404500 (the pooled empty key 0x0122D70E), CDataOutputStream::WriteLong 0x00993EB0,
//   CPersistTraits<signed char>::TransferBinaryIn 0x004CBD50 (catalogued with the propagated <unsigned_char>).
// Shape: a switch with only cases 3 and 2 (`sub eax,2 ; je ; dec eax ; jne`), case 3 laid out first; the binary-load arm's
//   traits object (CPersistTraits<T> = { T DefaultVal; }, user ctor, nothing stored) is a block-scoped named local that
//   VC7.1 homes in the dead value-argument slot (`lea ecx,[esp+10h]`), as in the Transfer<long> sibling 0x00410BE0.
class CDataOutputStream { public:
    void WriteLong(long value);                   // 0x00993EB0
};
class CMemoryDataInputStream;

template <class T> struct CPersistTraits {
    T DefaultVal;                                 // +0x00
    CPersistTraits() {}                           // user ctor: DefaultVal is left unset, nothing is stored
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* value);   // 0x004CBD50 for T = signed char
};

class CPersistContext { public:
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
        PSaveStream->WriteLong(value);
        break;
    case 2: {
        CPersistTraits<T> traits;
        traits.TransferBinaryIn(PLoadStream, &value);
        break;
    }
    }
}

template void CPersistContext::TransferBinary<signed char>(signed char& value);