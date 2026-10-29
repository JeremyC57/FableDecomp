// CPersistContext::Transfer<CDefString>(const char* name, CDefString& value, const CDefString& defaultValue) -- persist one def string (a string-table index): CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save, or reset to the default
// Leaf: ??$Transfer@VCDefString@@@CPersistContext@@QAEXPBDAAVCDefString@@ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E39D0, 200 bytes (jump table 0x007E3A84 included), `ret 0Ch`.
// Identity: the text arms build the wrapper whose vtable 0x012704F8 has the RTTI type
//   `.?AV?$CPersistContext_TransferableComponent@VCDefString@@@@` (T = CDefString); Ego_r.pdb holds
//   `??$Transfer@VCDefString@@@CPersistContext@@QAEXPBDAAVCDefString@@ABV1@@Z` (and the 2-argument form). The catalog label
//   `Transfer<signed char>` was propagated across the byte-identical instantiations; this body is not byte-identical to
//   them (the binary-load arm is a thiscall on the value, not on a traits object).
// Callees: CPersistContext::CheckCRC 0x00404500 (protected); binary save CDataOutputStream::WriteSLONG 0x00993EB0 of
//   CDefString::TablePos through the inline CPersistTraits<CDefString>::TransferBinaryOut (Ego_r: `const CDefString&`) and
//   CDefString::TransferBinaryOut; binary load CDefString::TransferBinaryIn(CDataInputStream&) 0x0044FC00 (Ego_r
//   `?TransferBinaryIn@CDefString@@QAEXAAVCDataInputStream@@@Z`; the manifest's `CPersistTraits<float>` label is propagated)
//   through the inline CPersistTraits<CDefString>::TransferBinaryIn; CPersistContext::TransferObjectSaveText 0x009BAD60 and
//   TransferObjectLoadText 0x009BBA40 (both protected).
// Jump table (checked entry by entry): 0 -> 0x007E3A50 text load, 1 -> 0x007E3A2B text save, 2 -> 0x007E3A16 binary load,
//   3 -> 0x007E39FF binary save, 4 -> 0x007E39EB reset.
// Layout: CPersistContext Ego_r.pdb (0x2C); CDefString { long TablePos; } (4 bytes, Ego_r.pdb); engine/CDataInputStream.h;
//   CMemoryDataInputStream modelled locally on it (engine/CMemoryDataInputStream.h flattens its CDataInputStream base, so a
//   CMemoryDataInputStream& cannot bind the CDataInputStream& parameter: NOTES.md "Header debt").
// Shape: chunk 561's Transfer<T> template (switch on the closed persist mode with `default: __assume(0)`, cases 4,3,2,1,0,
//   block-scoped traits, the 8-byte {vtable, Object} wrapper of the text arms).
#include "engine/CDataInputStream.h"
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout { public:
    void WriteEBOOL(bool value);                 // 0x00993E30
    void WriteSLONG(long value);                 // 0x00993EB0
    void WriteUHUGE(unsigned __int64 value);     // 0x00993EC0
    void WriteFloat(float value);                // 0x00993EE0
};
class CMemoryDataInputStream : public CDataInputStream { public:
    unsigned char* StreamSource;                 // +0x18
};
class CCharString;
class CStringParser;
class CPersistContext;

class CDefString { public:
    long TablePos;                                                          // +0x00
    void TransferBinaryIn(CDataInputStream& stream);                       // 0x0044FC00
    void TransferBinaryOut(CDataOutputStream& stream) const { stream.WriteSLONG(TablePos); }
};

template <class T> class CPersistTraits;
template <> class CPersistTraits<CDefString> { public:
    CDefString DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, CDefString& value) { value.TransferBinaryIn(stream); }
    void TransferBinaryOut(CDataOutputStream& stream, const CDefString& value) { value.TransferBinaryOut(stream); }
};

class CPersistContext_TransferableComponentGenericType { public:
    CPersistContext_TransferableComponentGenericType() {}
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;      // slot 0
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;  // slot 1
};
template <class T> class CPersistContext_TransferableComponent : public CPersistContext_TransferableComponentGenericType { public:
    T* Object;
    CPersistContext_TransferableComponent(T& object) : Object(&object) {}
private:
    virtual void TransferOut(CPersistContext& context, CCharString& out);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
};

class CPersistContext { public:
    enum EMode { MODE_LOAD_TEXT, MODE_SAVE_TEXT, MODE_LOAD_BINARY, MODE_SAVE_BINARY, MODE_RESET };
    void*                   PDefinitionManager;   // +0x00
    void*                   PSymbolMap;           // +0x04
    CStringParser*          PParser;              // +0x08
    void*                   ErrorCallback;        // +0x0C
    CCharString*            PStringToClean;       // +0x10
    bool                    UseDefaults;          // +0x14
    bool                    UseCRCCheck;          // +0x15
    bool                    SingleTransfer;       // +0x16
    bool                    Finished;             // +0x17
    EMode                   Mode;                 // +0x18
    bool                    Binary;               // +0x1C
    CCharString*            PStringToSaveTo;      // +0x20
    CMemoryDataInputStream* PLoadStream;          // +0x24
    CDataOutputStream*      PSaveStream;          // +0x28
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
protected:
    void CheckCRC(const char* name);   // 0x00404500
    void TransferObjectSaveText(const char* name, CPersistContext_TransferableComponentGenericType& item);   // 0x009BAD60
    void TransferObjectLoadText(const char* name, CPersistContext_TransferableComponentGenericType& item);   // 0x009BBA40
};

template <class T>
void CPersistContext::Transfer(const char* name, T& value, const T& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_RESET:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY: {
        CPersistTraits<T> traits;
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
    case MODE_LOAD_BINARY: {
        CPersistTraits<T> traits;
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    }
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}

template void CPersistContext::Transfer<CDefString>(const char* name, CDefString& value, const CDefString& defaultValue);