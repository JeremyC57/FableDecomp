// CReadableDef::Transfer(CPersistContext& context) -- persist the UseInfoDisplay flag: CRC check with the empty key, then read it in load mode (2) or write it in save mode (3)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E81B0, length 69 bytes (virtual, `ret 4`).
// Callees: CPersistContext::CheckCRC @ 0x00404500 (the empty key "" @ 0x0122D70E), CPersistTraits<bool>::TransferBinaryIn
// @ 0x00403EB0 (on a traits temporary, the load stream +0x24 and &value), CDataOutputStream::WriteBool @ 0x00993E30 (on
// the save stream +0x28).
// Layout: engine/CReadableDef.h (PDB): UseInfoDisplay +0x25 behind the 0x21-byte flattened def base.
// Shape: the inline CPersistContext::Transfer<bool> template -- `&value` is taken before the CRC call (kept in edi),
// the mode is a `switch` with cases 2 and 3 (`sub eax,2 ; je ; dec eax ; jne`), the save arm inlines the traits'
// TransferBinaryOut (`xor eax,eax ; mov al,[edi]`), and the load arm's empty traits temporary lives in the dead
// context-argument slot (`lea ecx,[esp+14h]`). The method is attached through a derived shell because the generated
// header is a plain layout struct.
// Leaf: Transfer
#include "engine/CReadableDef.h"

struct CDataOutputStream {
    void WriteBool(bool value);                         // 0x00993E30
};
struct CMemoryDataInputStream;

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<bool> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, bool* value);   // 0x00403EB0
    static void TransferBinaryOut(CDataOutputStream* stream, const bool& value) { stream->WriteBool(value); }
};

struct CPersistContext {
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);                    // 0x00404500
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value);
            break;
        }
    }
};

struct CReadableDef_Transfer : CReadableDef {
    void Transfer(CPersistContext& context);
};

void CReadableDef_Transfer::Transfer(CPersistContext& context)
{
    context.Transfer("", UseInfoDisplay);
}