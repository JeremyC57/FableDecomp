// CPersistContext::Transfer<EGameflowPosition>(const char* name, EGameflowPosition& value, const EGameflowPosition& defaultValue) -- persist one enum member ("PostSavePosition"): CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save as a long, or reset to the default
// Leaf: ??$Transfer@W4EGameflowPosition@@@CPersistContext@@QAEXPBDAAW4EGameflowPosition@@ABW41@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDCA70, `ret 0Ch`. Called from the quest-script persist function at 0x00CDC070 with the key "PostSavePosition".
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0 (catalogued "WriteCBYTE"),
//   CPersistTraits<T>::TransferBinaryIn 0x00CDCB50, CPersistContext::TransferObjectSaveText 0x009BAD60 and TransferObjectLoadText
//   0x009BBA40 (both take the CPersistContext_TransferableComponent<T> wrapper, vtable 0x012C2EC4).
// Shape: the speed-optimised sibling of the landed size-TU CDefIndex instantiation 0x005CC1E8. The persist mode
//   (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a closed enum, so the switch
//   ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]`; the case bodies are laid out in source
//   order and retail has them 4,3,2,1,0. this module's traits carries a `T DefaultVal` member; declared at function scope it is built (zeroed) once before the jump, in the dead key-argument slot, and the binary-load arm passes its address. The 8-byte frame is the {vtable, value*} wrapper of the text arms.
//   The enum is a 4-byte enum written with WriteLong.
// Naming (NF170, q11755, 2026-11-20): T is the global EGameflowPosition (Ego_r.pdb + FableWin.pdb ??$Transfer@W4EGameflowPosition@@@CPersistContext@@QAEXPBDAAW4EGameflowPosition@@ABW41@@Z; FableWin also its
//   CPersistTraits / GetDefaultVal instances; the landed caller 0x00CEF8E0 transfers the master data PostSavePosition (EGameflowPosition, engine/CQ_SunnyvaleMasterData.h) and already binds this spelling). Was the descriptive EPostSavePosition.
class CDataOutputStream { public:
public:
    void WriteSLONG(long);   // 0x00993EB0
};
class CMemoryDataInputStream;
#include "engine/CQ_SunnyvaleMasterData.h"                // the global enum EGameflowPosition (Ego_r.pdb; CQ_SunnyvaleMasterData::PostSavePosition +4)

template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* value);   // 0x00CDCB50 for this T
};

class CPersistContext_TransferableComponentGenericType { public:
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {   // vtable 0x012C2EC4
    T* Value;
    CPersistContext_TransferableComponent(T& value) : Value(&value) {}
    virtual void TransferIn();
};

class CPersistContext { public:
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
    protected: void CheckCRC(const char* name); public:              // 0x00404500
    protected: void TransferObjectSaveText(const char* name, CPersistContext_TransferableComponentGenericType& item); public:   // 0x009BAD60
    protected: void TransferObjectLoadText(const char* name, CPersistContext_TransferableComponentGenericType& item); public:   // 0x009BBA40
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
        PSaveStream->WriteSLONG(value);
        break;
    case 2:
        traits.TransferBinaryIn(PLoadStream, &value);
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

template void CPersistContext::Transfer<EGameflowPosition>(const char* name, EGameflowPosition& value, const EGameflowPosition& defaultValue);