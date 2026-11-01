// CPersistContext::Transfer<EscortTraderQuestStage>(const char* name, EscortTraderQuestStage& value, const EscortTraderQuestStage& defaultValue) -- persist the Escort Trader quest stage ("QuestStage"): CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save as a long, or reset to the default
// Leaf: ??$Transfer@W4EscortTraderQuestStage@CQR_EscortTraderScript@NScript@@@CPersistContext@@QAEXPBDAAW4EscortTraderQuestStage@CQR_EscortTraderScript@NScript@@ABW4123@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1DF60, 216 bytes (the switch table follows the code), thiscall, `ret 0Ch`.
// Identity (proven): the sole caller is the landed NScript::CQR_EscortTraderScript::OnPersist 0x00E1DCF0 (call at 0x00E1DD16)
//   with the key "QuestStage" 0x012E1490, the member +0x11C and the GetDefaultVal<EscortTraderQuestStage> 0x00E1DF50 result;
//   reloc_bindings binds 0x00E1DF60 under this exact symbol, which Ego_r.pdb (donor 0x00E698A0) and FableWin.pdb both list.
//   The manifest label "Transfer<signed char>" is propagated across byte-identical bodies.
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0 (the inlined
//   CPersistTraits<T>::TransferBinaryOut), CPersistTraits<T>::TransferBinaryIn 0x00E1E040 (this T's instance, the only
//   caller), CPersistContext::TransferObjectSaveText 0x009BAD60 and TransferObjectLoadText 0x009BBA40 (both take the
//   CPersistContext_TransferableComponent<T> wrapper, vtable 0x012E149C).
// Shape: the landed CPersistContext::Transfer<EPostSavePosition> 0x00CDCA70 (byte-identical twin). The persist mode
//   (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a closed enum, so the switch
//   ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]`; the case bodies are laid out in source
//   order and retail has them 4,3,2,1,0. The traits object carries a `T DefaultVal` member; declared at function scope it is
//   built (zeroed) once before the jump, in the dead key-argument slot, and the binary-load arm passes its address. The
//   8-byte frame is the {vtable, value*} wrapper of the text arms.
// Signatures: FableWin.pdb's CPersistTraits<EscortTraderQuestStage> (ctor, dtor, TransferBinaryIn(CMemoryDataInputStream&, T&),
//   TransferBinaryOut(CDataOutputStream&, T)) and CPersistContext_TransferableComponent<EscortTraderQuestStage> (ctor(T&),
//   private virtual TransferIn / TransferOut).
// Layout: CPersistContext from Ego_r.pdb (0x2C bytes): UseDefaults +0x14, Mode +0x18, PLoadStream +0x24, PSaveStream +0x28
//   (no engine header).
#include "engine/CDataOutputStream.h"                  // WriteSLONG 0x00993EB0
class CMemoryDataInputStream;
class CStringParser;
class CCharString;
class CPersistContext;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQR_EscortTraderScript { public:
    enum EscortTraderQuestStage { };                  // Ego_r.pdb nested enum
};
}

template <class T> class CPersistTraits { public:
    T DefaultVal;
    CPersistTraits() : DefaultVal((T)0) {}       // `DefaultVal()` on a nested enum trips VC7.1 C2064
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);   // 0x00E1E040 for this T
    void TransferBinaryOut(CDataOutputStream& stream, T value) { stream.WriteSLONG(value); }
};

class CPersistContext_TransferableComponentGenericType { public:
private:
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;
    virtual void TransferOut(CPersistContext& context, CCharString& text) = 0;
};
template <class T> class CPersistContext_TransferableComponent : public CPersistContext_TransferableComponentGenericType {   // vtable 0x012E149C
public:
    T* Value;
    CPersistContext_TransferableComponent(T& value) : Value(&value) {}
private:
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
    virtual void TransferOut(CPersistContext& context, CCharString& text);
};

class CPersistContext { public:
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
protected:
    void CheckCRC(const char* name);                                                                       // 0x00404500
    void TransferObjectSaveText(const char* name, CPersistContext_TransferableComponentGenericType& item);   // 0x009BAD60
    void TransferObjectLoadText(const char* name, CPersistContext_TransferableComponentGenericType& item);   // 0x009BBA40
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};

template <class T>
void CPersistContext::Transfer(const char* name, T& value, const T& defaultValue)
{
    CheckCRC(name);
    CPersistTraits<T> traits;
    switch (Mode) {
    case 4:
        value = defaultValue;
        break;
    case 3:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    case 2:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case 1: {
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case 0: {
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

template void CPersistContext::Transfer<NScript::CQR_EscortTraderScript::EscortTraderQuestStage>(const char* name,
    NScript::CQR_EscortTraderScript::EscortTraderQuestStage& value, const NScript::CQR_EscortTraderScript::EscortTraderQuestStage& defaultValue);