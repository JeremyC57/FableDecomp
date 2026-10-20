// CPersistContext::Transfer<CWideString>(const char* name, CWideString& value, const CWideString& defaultValue) -- persist one wide-string member: CRC-check the key, then by mode reset to the default, binary save, binary load, text save or text load (seeding the default first when asked)
// Grade: DIFFER(280v280) -- case 0 pops the shrink-wrapped ebx before the traits CWideString dtor (retail: dtor, then pop ebx); everything else matches
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00410AC0, 247 bytes + 20-byte jump table, `ret 0Ch`.
// Label correction: catalogued as Transfer<CIntelligentPointer<CThing>>, but every arm works on a CWideString: the reset arm
//   calls CWideString::operator= 0x0099B7D0, the binary-save arm CDataOutputStream::WriteNullTerminatedWString 0x00993F00, the
//   binary-load arm CDataInputStream slot 7 (ReadNullTerminatedWString 0x00994D20, returns CWideString by value) and the traits
//   object is a default-constructed CWideString (0x0099AED0, catalogued as a CCharString ctor) destroyed with ~CWideString
//   0x0099B510: this is Transfer<CWideString>, the twin of Transfer<CCharString> 0x004109A0 (called from 0x00411060).
// Callees: CPersistContext::CheckCRC 0x00404500, CWideString::CWideString() 0x0099AED0, ~CWideString 0x0099B510,
//   CWideString::operator= 0x0099B7D0, WriteNullTerminatedWString 0x00993F00, TransferObjectSaveText 0x009BAD60,
//   TransferObjectLoadText 0x009BBA40 (wrapper CPersistContext_TransferableComponent<CWideString>, vtable 0x0122DF2C).
// Shape: same template as the landed enum instantiations (0x00EB65A0): the persist mode (+0x18) is a closed enum, the switch
//   ends in `default: __assume(0)` and the case bodies are laid out in source order 4,3,2,1,0. CPersistTraits<T> holds a
//   `T DefaultVal`; declared at function scope it is constructed once before the jump in the dead `name` argument slot (the
//   name itself is kept in edi) and destroyed on every exit. The traits' TransferBinaryIn is inline here
//   (`value = stream->ReadNullTerminatedWString()`): its by-value result lands in the dead `defaultValue` slot.
class CWideString {
public:
    void* PData;
    CWideString();                                     // 0x0099AED0
    ~CWideString();                                    // 0x0099B510
    CWideString& operator=(const CWideString& other);  // 0x0099B7D0
};

class CDataInputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual CWideString ReadNullTerminatedWString();   // slot 7, 0x00994D20
};
class CDataOutputStream {
public:
    void WriteNullTerminatedWString(const CWideString& value);   // 0x00993F00
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() {}
    void TransferBinaryIn(CDataInputStream* stream, T* value) { *value = stream->ReadNullTerminatedWString(); }
};

struct CPersistContext_TransferableObjectBase {
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableObjectBase {   // vtable 0x0122DF2C
    T* Value;
    CPersistContext_TransferableComponent(T& value) : Value(&value) {}
    virtual void TransferIn();
};

struct CPersistContext {
    unsigned char      _pad00[0x14];
    bool               UseDefaults;          // +0x14
    unsigned char      _pad15[3];
    long               Mode;                 // +0x18
    unsigned char      _pad1c[8];
    CDataInputStream*  PLoadStream;          // +0x24
    CDataOutputStream* PSaveStream;          // +0x28
    void CheckCRC(const char* name);         // 0x00404500
    void TransferObjectSaveText(const char* name, CPersistContext_TransferableObjectBase& item);   // 0x009BAD60
    void TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item);   // 0x009BBA40
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
        PSaveStream->WriteNullTerminatedWString(value);
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

template void CPersistContext::Transfer<CWideString>(const char* name, CWideString& value, const CWideString& defaultValue);
