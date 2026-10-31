// CPersistContext::Transfer<EHeroCentreDoorType>(const char* name, EHeroCentreDoorType& value, const EHeroCentreDoorType& defaultValue) -- persist one hero-centre door type: CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save, or reset to the default
// Grade: RELOCATION_MATCH over the true 208-byte extent (retry wave 2 sec2b): over-spanned catalog row (240 bytes ran into the 2-argument Transfer<EHeroCentreDoorType> at 0x0077FEB0); BOUNDS_FIX.tsv + oracle_extra.tsv carry the 208-byte oracle
// Leaf: ??$Transfer@W4EHeroCentreDoorType@@@CPersistContext@@QAEXPBDAAW4EHeroCentreDoorType@@ABW41@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077FDE0, 208 bytes (jump table included, to 0x0077FEB0); the catalog row is 240 bytes, `ret 0Ch`.
// Identity: the text arms build the wrapper whose vtable 0x0126B038 has the RTTI type
//   `.?AV?$CPersistContext_TransferableComponent@W4EHeroCentreDoorType@@@@` (T = EHeroCentreDoorType), and Ego_r.pdb holds
//   `??$Transfer@W4EHeroCentreDoorType@@@CPersistContext@@QAEXPBDAAW4EHeroCentreDoorType@@ABW41@@Z` and the 2-argument
//   `...@@QAEXPBDAAW4EHeroCentreDoorType@@@Z`, which is the 0x0077FEB0 body right after the jump table (it calls this one with a
//   zero default from GetDefaultVal). Callers 0x0077F69D / 0x0077F6C2 (CTCHeroCentre persist, key string 0x0126B0C0).
//   The catalog label `Transfer<signed char>` was propagated across the byte-identical instantiations.
// Callees: CPersistContext::CheckCRC 0x00404500 (protected); binary save CDataOutputStream::WriteSLONG 0x00993EB0 through the
//   inline CPersistTraits<T>::TransferBinaryOut (Ego_r: by-value T); CPersistTraits<EHeroCentreDoorType>::TransferBinaryIn
//   0x0077FD10 (the manifest's `CPersistTraits<float>` label is propagated); CPersistContext::TransferObjectSaveText 0x009BAD60
//   and TransferObjectLoadText 0x009BBA40 (both protected, taking the CPersistContext_TransferableComponentGenericType base).
// Shape: chunk 561's Transfer<T> template unchanged (switch on the closed persist mode with `default: __assume(0)`, cases
//   4,3,2,1,0, block-scoped CPersistTraits<T> without a constructor, the 8-byte {vtable, Object} wrapper of the text arms).
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

enum EHeroCentreDoorType { };                              // values not needed here
template <class T> class CPersistTraits { public:
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);   // 0x0077FD10 for T = EHeroCentreDoorType
    void TransferBinaryOut(CDataOutputStream& stream, T value) { stream.WriteSLONG(value); }
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

template void CPersistContext::Transfer<EHeroCentreDoorType>(const char* name, EHeroCentreDoorType& value, const EHeroCentreDoorType& defaultValue);