// CPersistContext::Transfer<unsigned char>(const char* name, unsigned char& value, const unsigned char& defaultValue) -- persist one unsigned char: CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save, or reset to the default
// Grade: RELOCATION_MATCH over the true extent (oracle_extra.tsv, BOUNDS_FIX) -- over-spanned catalog row: the true extent (sub esp,8 .. end of the jump table, 208 bytes) is byte-exact with relocations masked (tools/pfx.py); the row runs on into the next function(s), re-split it and land this source
// Leaf: ??$Transfer@E@CPersistContext@@QAEXPBDAAEABE@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00405A60, 208 bytes (jump table included); the catalog row is 308 bytes and runs on into the next functions, `ret 0Ch`.
// Identity: the text arms build the wrapper whose vtable 0x0122DCD4 has the RTTI type
//   `.?AV?$CPersistContext_TransferableComponent@E@@` (T = unsigned char), and Ego_r.pdb holds `??$Transfer@E@CPersistContext@@QAEXPBDAAEABE@Z`.
//   The catalog label `Transfer<signed char>` was propagated across the byte-identical instantiations.
// Callees: CPersistContext::CheckCRC 0x00404500 (protected); binary save CDataOutputStream::WriteSLONG 0x00993EB0 through the generic inline CPersistTraits<T>::TransferBinaryOut (the unsigned char value widened to long);
//   CPersistTraits<unsigned char>::TransferBinaryIn 0x00405940; CPersistContext::TransferObjectSaveText 0x009BAD60 and
//   TransferObjectLoadText 0x009BBA40 (both protected, taking the CPersistContext_TransferableComponentGenericType base).
//   The writer run at 0x00993E30.. follows Ego_r order: EBOOL, UBYTE, SBYTE, CBYTE, UWORD, SWORD, ULONG, SLONG (0x993EB0),
//   UHUGE (0x993EC0), SHUGE, Float (0x993EE0), Double, NullTerminatedWString, Colour (0x993F30, also bound by landed code).
// Shape: the persist mode (+0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a closed enum,
//   so the switch ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]`; the case bodies are laid
//   out in source order and retail has them 4,3,2,1,0. CPersistTraits<T> has no constructor (no zero store) and is
//   block-scoped in each binary arm: retail homes it in a dead argument slot (a function-scope traits takes the key
//   slot instead). The 8-byte frame is the {vtable, Object} wrapper of the text arms.
#include "engine/CMemoryDataInputStream.h"
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout { public:
    void WriteEBOOL(bool value);                 // 0x00993E30
    void WriteSLONG(long value);                 // 0x00993EB0
    void WriteUHUGE(unsigned __int64 value);     // 0x00993EC0
    void WriteFloat(float value);                // 0x00993EE0
};
class CCharString;
class CStringParser;
class CPersistContext;

template <class T> class CPersistTraits { public:
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);   // 0x00405940 for T = unsigned char
    void TransferBinaryOut(CDataOutputStream& stream, const T& value) { stream.WriteSLONG(value); }
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

template void CPersistContext::Transfer<unsigned char>(const char* name, unsigned char& value, const unsigned char& defaultValue);