// CPersistContext::Transfer<short>(const char* name, short& value, const short& defaultValue) -- persist one short member: CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save as a long, or reset to the default
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CC3F0, 212 bytes incl. the 5-entry jump table, `ret 0Ch`.
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0, CPersistTraits<short>::TransferBinaryIn 0x004CBC70, CPersistContext::TransferObjectSaveText 0x009BAD60 and
//   TransferObjectLoadText 0x009BBA40 (both take the CPersistContext_TransferableComponent<short> wrapper, vtable 0x01239A74).
// Shape: the speed-TU persist switch of the landed Transfer<EArcheryState> 0x00CDCC20 / Transfer<ENumAcceptedBoasts>
//   0x004B8D70 (mode +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset; `default: __assume(0)`
//   gives the bare table jump, the case bodies laid out 4,3,2,1,0). The 8-byte frame is the {vtable, value*} wrapper
//   of the two text arms.
//   The binary save sign-extends the value to a long (`movsx edx,word ptr [ecx]`). The binary-load arm's traits
//   object (PDB-style CPersistTraits<short> = { short DefaultVal; }, user ctor, nothing stored) is a named local in
//   the arm's block, homed in the dead value-argument slot (`lea ecx,[esp+20h]`).
class CDataOutputStream { public:
public:
    void WriteSLONG(long);   // 0x00993EB0
};
class CMemoryDataInputStream;
template <class T> struct CPersistTraits {
    T DefaultVal;                                 // +0x00 (PDB: CPersistTraits<T> is { T DefaultVal; })
    CPersistTraits() {}                           // user ctor: DefaultVal is left unset, nothing is stored
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* value);   // 0x004CBC70 for T = short
};

struct CPersistContext_TransferableObjectBase {
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableObjectBase {   // vtable 0x01239A74 for T = short
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
    void TransferObjectSaveText(const char* name, CPersistContext_TransferableObjectBase& item);   // 0x009BAD60
    void TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item);   // 0x009BBA40
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
    case 2: {
        CPersistTraits<T> traits;
        traits.TransferBinaryIn(PLoadStream, &value);
        break;
    }
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

template void CPersistContext::Transfer<short>(const char* name, short& value, const short& defaultValue);