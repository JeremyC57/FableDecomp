// CPersistContext::Transfer(const char* name, CDefIndex& value, const CDefIndex& defaultValue) -- CRC-check the key, then by mode: text load (optionally seeding the default first), text save, binary load through CPersistTraits<CDefIndex>, binary save of the index; any other mode just takes the default
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CC1E8, length 150 bytes.
// Leaf: Transfer
// One of two byte-identical instantiations in different modules (0x00580371 and 0x005CC1E8; only the callees and the
// wrapper vtable differ); this one is called from 0x005CC312.
// Callees: CPersistContext::CheckCRC @ 0x00404500, CDataOutputStream::WriteSLONG @ 0x00993EB0 (catalogued "WriteCBYTE"; it
// writes a long), CPersistTraits<CDefIndex>::TransferBinaryIn @ 0x00452335, TransferObjectSaveText @ 0x009BAD60,
// TransferObjectLoadText @ 0x009BBA40; wrapper vtable 0x01253F18 (CPersistContext_TransferableComponent<CDefIndex>).
// Layout: CPersistContext (Ego_r.pdb, 0x2c bytes): use-defaults flag +0x14, mode +0x18, load stream +0x24, save stream +0x28;
// CDefIndex is one long.
// Shape: frame-pointer size TU (`#pragma optimize("y", off)` + `("s", on)`); a switch over cases 0..3 in source order lowers
// to `sub eax,0 ; je ; dec ; je ...` with the default arm first and the cases in reverse; the empty traits object of case 2
// lives in the value argument's slot (`lea ecx,[ebp+0Ch]` after the value is pushed); the 8-byte frame is the
// TransferableComponent wrapper {vtable, value*}; `ret 0Ch`.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CDefIndex { public: long Index; };
class CDataInputStream;
class CDataOutputStream {
public:
public:
    void WriteSLONG(long);   // 0x00993EB0
};

template <class T>
class CPersistTraits {
public:
    void TransferBinaryIn(CDataInputStream* stream, T& value);   // 0x00452335 for T = CDefIndex
};

class CPersistContext_TransferableObjectBase {
public:
    virtual void TransferIn();
};
template <class T>
class CPersistContext_TransferableComponent : public CPersistContext_TransferableObjectBase {   // vtable 0x01253F18
public:
    T* Value;
    CPersistContext_TransferableComponent(T& value) : Value(&value) { }
    virtual void TransferIn();
};

class CPersistContext {
public:
    unsigned char      _pad00[0x14];
    bool               UseDefaults;                // +0x14
    unsigned char      _pad15[3];
    long               Mode;                       // +0x18
    unsigned char      _pad1c[8];
    CDataInputStream*  LoadStream;                 // +0x24
    CDataOutputStream* SaveStream;                 // +0x28
    protected: void CheckCRC(const char* name); public:               // 0x00404500
    void TransferObjectSaveText(const char* name, CPersistContext_TransferableObjectBase& item);   // 0x009BAD60
    void TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item);   // 0x009BBA40
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};

template <class T>
void CPersistContext::Transfer(const char* name, T& value, const T& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case 0: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    case 1: {
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case 2: {
        CPersistTraits<T> traits;
        traits.TransferBinaryIn(LoadStream, value);
        break;
    }
    case 3:
        SaveStream->WriteSLONG(value.Index);
        break;
    default:
        value = defaultValue;
        break;
    }
}

template void CPersistContext::Transfer<CDefIndex>(const char* name, CDefIndex& value, const CDefIndex& defaultValue);