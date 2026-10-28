// CCarriedReadableDef::Transfer(CPersistContext& context) -- persist the readable item's two text tags (title and body CDefString table positions) through the context: load or save each after its CRC check
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E1E30, length 119 bytes (ret 4).
// Callees: CPersistContext::CheckCRC(const char*) @ 0x00404500 (with the empty literal 0x0122D70E), the CDefString
// binary read @ 0x0044FC00 (`__fastcall` on the tag, the load stream as argument; manifest label
// "CPersistTraits<float>::TransferBinaryIn" is propagated), CDataOutputStream's long write @ 0x00993EB0 (manifest
// label "WriteCBYTE" is propagated).
// Layout: rebuild/include/engine/CCarriedReadableDef.h (Ego_r.pdb): TitleTextTag_TablePos +0x28,
// BodyTextTag_TablePos +0x2c (each a CDefString = one TablePos long). The context's mode is at +0x18 (2 = load from
// the stream at +0x24, 3 = save to the stream at +0x28), as the landed CPersistContext::TransferBinary bodies.
// Shape: two inlined per-tag transfers (`CheckCRC("")` then a two-case `switch` lowered to `sub eax,2 ; je ;
// dec eax ; jne`); the first tag's address is formed up front (`lea ebx,[edi+28h]`), the second by `add edi,2Ch`;
// the second save path returns directly (tail-duplicated epilogue).
#include "engine/CCarriedReadableDef.h"

class CMemoryDataInputStream;
class CDataOutputStream { public:
public:
    void WriteSLONG(long);   // 0x00993EB0
};
class CDefString { public:                           // the TablePos word of a CDefString member
    long TablePos;
    void ReadBinary(CMemoryDataInputStream* stream);   // 0x0044FC00
};
class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    Mode;             // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* LoadStream;       // +0x24
    CDataOutputStream*      SaveStream;       // +0x28
    protected: void CheckCRC(const char* name); public:          // 0x00404500
    void Transfer(CDefString& value)
    {
        CheckCRC("");
        switch (Mode) {
        case 2:
            value.ReadBinary(LoadStream);
            break;
        case 3:
            SaveStream->WriteSLONG(value.TablePos);
            break;
        }
    }
};

struct CCarriedReadableDef_Methods : CCarriedReadableDef {
    void Transfer(CPersistContext& context);
};

void CCarriedReadableDef_Methods::Transfer(CPersistContext& context)
{
    context.Transfer(*reinterpret_cast<CDefString*>(&TitleTextTag_TablePos));
    context.Transfer(*reinterpret_cast<CDefString*>(&BodyTextTag_TablePos));
}