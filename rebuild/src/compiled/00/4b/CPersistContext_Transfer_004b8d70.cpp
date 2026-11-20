// CPersistContext::Transfer<int>(const char* name, int& value, const int& defaultValue) -- persist one int member ("NumAcceptedBoasts"): CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save as a long, or reset to the default
// Leaf: ??$Transfer@H@CPersistContext@@QAEXPBDAAHABH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B8D70, `ret 0Ch`. Called from CQuestManager::TransferBoasts (0x004B05C0) with the key "NumAcceptedBoasts".
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0 (catalogued "WriteCBYTE"),
//   CPersistTraits<T>::TransferBinaryIn 0x004B85D0, CPersistContext::TransferObjectSaveText 0x009BAD60 and TransferObjectLoadText
//   0x009BBA40 (both take the CPersistContext_TransferableComponent<T> wrapper, vtable 0x01239378).
// Shape: the speed-optimised sibling of the landed size-TU CDefIndex instantiation 0x005CC1E8. The persist mode
//   (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a closed enum, so the switch
//   ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]`; the case bodies are laid out in source
//   order and retail has them 4,3,2,1,0. the traits object of the binary-load arm is empty and lives in the dead value-argument slot (`lea ecx,[esp+20h]` once the value is in eax). The 8-byte frame is the {vtable, value*} wrapper of the text arms.
// Naming (NF108, 2026-11-19, q4738): was Transfer<ENumAcceptedBoasts> over a descriptive enum found in no PDB. Ego_r.pdb 0x004BCB00 +
//   FableWin.pdb 0x01C718B0 ??$Transfer@H@CPersistContext@@QAEXPBDAAHABH@Z (T = int);
//   the caller 0x004B05C0 already binds it. The callees respelled with T: FableWin.pdb ?TransferBinaryIn@?$CPersistTraits@H@@QAEXAAVCMemoryDataInputStream@@AAH@Z
//   (0x004B85D0, references) and the CPersistContext_TransferableComponent<int> vtable (0x01239378). In-place respelling of the row's
//   pre-existing local CPersistContext model (the landed Transfer<long> 0x00410BE0 form).
class CDataOutputStream { public:
public:
    void WriteSLONG(long);   // 0x00993EB0
};
class CMemoryDataInputStream;

template <class T> struct CPersistTraits {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);   // 0x004B85D0 for T = int
};

class CPersistContext_TransferableComponentGenericType { public:
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {   // vtable 0x01239378
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
    switch (Mode) {
    case 4:
        value = defaultValue;
        break;
    case 3:
        PSaveStream->WriteSLONG(value);
        break;
    case 2:
        CPersistTraits<T>().TransferBinaryIn(*PLoadStream, value);
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

template void CPersistContext::Transfer<int>(const char* name, int& value, const int& defaultValue);