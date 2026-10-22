// CPersistContext::Transfer<COpinionTransientOffset>(const char* name, COpinionTransientOffset& value, const COpinionTransientOffset& defaultValue) -- persist one COpinionTransientOffset member: CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save, or reset to the default
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00685F80, 264 bytes incl. the 5-entry jump table, `ret 0Ch`.
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream vtable slot +0x10 (Write(const void*, long)), COpinionTransientOffset::TransferBinaryIn 0x004793F8 (an ILT thunk), CPersistContext::TransferObjectSaveText 0x009BAD60 and
//   TransferObjectLoadText 0x009BBA40 (both take the CPersistContext_TransferableComponent<COpinionTransientOffset> wrapper, vtable 0x0125BAB0).
// Shape: the speed-TU persist switch of the landed Transfer<EArcheryState> 0x00CDCC20 / Transfer<ENumAcceptedBoasts>
//   0x004B8D70 (mode +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset; `default: __assume(0)`
//   gives the bare table jump, the case bodies laid out 4,3,2,1,0). The 8-byte frame is the {vtable, value*} wrapper
//   of the two text arms.
//   COpinionTransientOffset is a 24-byte POD (six dwords: the reset and the seeded load copy it field by field); its
//   binary save is a raw 0x18-byte virtual Write of the save stream, its binary load the member TransferBinaryIn.
struct CDataOutputStream {
    virtual void _v00();
    virtual void _v04();
    virtual void _v08();
    virtual void _v0c();
    virtual void Write(const void* data, long size);   // slot +0x10
};
struct CMemoryDataInputStream;
struct COpinionTransientOffset {
    long m_data[6];                               // 0x18 bytes, copied as six dwords
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x004793F8
};

struct CPersistContext_TransferableObjectBase {
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableObjectBase {   // vtable 0x0125BAB0 for T = COpinionTransientOffset
    T* Value;
    CPersistContext_TransferableComponent(T& value) : Value(&value) {}
    virtual void TransferIn();
};

struct CPersistContext {
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
    void CheckCRC(const char* name);              // 0x00404500
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
        PSaveStream->Write(&value, sizeof(T));
        break;
    case 2: {
        value.TransferBinaryIn(PLoadStream);
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

template void CPersistContext::Transfer<COpinionTransientOffset>(const char* name, COpinionTransientOffset& value, const COpinionTransientOffset& defaultValue);