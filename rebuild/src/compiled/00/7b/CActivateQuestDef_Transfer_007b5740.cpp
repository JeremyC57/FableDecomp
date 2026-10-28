// CActivateQuestDef::Transfer(CPersistContext&) -- persist ScriptName (a def-string table position) and LoadResources
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B5740, length 124 bytes (virtual, `ret 4`).
// Callees: CPersistContext::CheckCRC @ 0x00404500 (x2, the empty key "" @ 0x0122D70E), CDefString::
// TransferBinaryIn(CMemoryDataInputStream*) @ 0x0044FC00 (manifest label "CPersistTraits<float>::
// TransferBinaryIn" is propagated across a byte-identical body), CDataOutputStream::WriteSLONG @ 0x00993EB0
// (manifest "WriteCBYTE"; it receives the whole dword), CPersistTraits<bool>::TransferBinaryIn @ 0x00403EB0,
// CDataOutputStream::WriteEBOOL @ 0x00993E30.
//
// Shape: two inline CPersistContext::Transfer<T> calls of the landed CReadableDef::Transfer (0x007E81B0)
// template -- each member address formed before its CRC call (ebx = &ScriptName, then `add edi,2Ch` for
// LoadResources), the mode switch (`sub eax,2 ; je ; dec eax ; jne`) with the save arm inline. The def string's
// traits forward to the string's own load (on the member, the load stream pushed) and save (its table position
// as a dword); the bool load uses the empty traits temporary in the dead argument slot. `this` is the PDB header
// layout (ScriptName_TablePos +0x28, LoadResources +0x2c).
// Leaf: Transfer
#include "engine/CActivateQuestDef.h"

class CMemoryDataInputStream;

class CDataOutputStream { public:
public:
    void WriteEBOOL(bool);   // 0x00993E30
    void WriteSLONG(long);   // 0x00993EB0
};

class CDefString { public:                                     // a name stored as its def-string table position
    long TablePos;
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x0044FC00
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<bool> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, bool* value);   // 0x00403EB0
    static void TransferBinaryOut(CDataOutputStream* stream, const bool& value) { stream->WriteEBOOL(value); }
};
template<> struct CPersistTraits<CDefString> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, CDefString* value) { value->TransferBinaryIn(stream); }
    static void TransferBinaryOut(CDataOutputStream* stream, const CDefString& value) { stream->WriteSLONG(value.TablePos); }
};

class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    protected: void CheckCRC(const char* name); public:                    // 0x00404500
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

struct CActivateQuestDef_Transfer : CActivateQuestDef {
    CDefString& ScriptName() { return *(CDefString*)&ScriptName_TablePos; }
    void Transfer(CPersistContext& context);
};

void CActivateQuestDef_Transfer::Transfer(CPersistContext& context)
{
    context.Transfer("", ScriptName());
    context.Transfer("", LoadResources);
}