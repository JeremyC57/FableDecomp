// Behaviour test for 00410ac0: src/00410ac0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CPersistContext::Transfer<CWideString>(const char* name, CWideString& value, const CWideString& defaultValue)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPersistContext::Transfer<CWideString>(const char* name, CWideString& value, const CWideString& defaultValue) -- persist one wide-string member: CRC-check the key, then by mode reset to the default, binary save, binary load, text save or text load (seeding the default first when asked)
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
//   ends in `default: __assume(0)`, every case arm `return`s (so case 0 runs the traits destructor before popping the
//   shrink-wrapped ebx; the enum instantiation 0x00EB65A0 compiles the same either way) and the case bodies are laid out
//   in source order 4,3,2,1,0. CPersistTraits<T> holds a
//   `T DefaultVal`; declared at function scope it is constructed once before the jump in the dead `name` argument slot (the
//   name itself is kept in edi) and destroyed on every exit. The traits' TransferBinaryIn is inline here
//   (`value = stream->ReadNullTerminatedWString()`): its by-value result lands in the dead `defaultValue` slot.
class CWideString {
public:
    void* PData;
    CWideString();                                     // 0x0099AED0
    ~CWideString();                                    // 0x0099B510
    const CWideString& operator=(const CWideString& other);  // 0x0099B7D0
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

class CPersistContext_TransferableComponentGenericType { public:
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {   // vtable 0x0122DF2C
    T* Value;
    CPersistContext_TransferableComponent(T& value) : Value(&value) {}
    virtual void TransferIn();
};

class CPersistContext { public:
    unsigned char      _pad00[0x14];
    bool               UseDefaults;          // +0x14
    unsigned char      _pad15[3];
    long               Mode;                 // +0x18
    unsigned char      _pad1c[8];
    CDataInputStream*  PLoadStream;          // +0x24
    CDataOutputStream* PSaveStream;          // +0x28
    protected: void CheckCRC(const char* name); public:         // 0x00404500
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
        return;
    case 3:
        PSaveStream->WriteNullTerminatedWString(value);
        return;
    case 2:
        traits.TransferBinaryIn(PLoadStream, &value);
        return;
    case 1: {
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectSaveText(name, item);
        return;
    }
    case 0: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectLoadText(name, item);
        return;
    }
    default:
        __assume(0);
    }
}

template void CPersistContext::Transfer<CWideString>(const char* name, CWideString& value, const CWideString& defaultValue);

// ---- stubs and driver ----
void CDataInputStream::v00() { }
void CDataInputStream::v01() { }
void CDataInputStream::v02() { }
void CDataInputStream::v03() { }
void CDataInputStream::v04() { }
void CDataInputStream::v05() { }
void CDataInputStream::v06() { }
static int g_ctor = 0, g_dtor = 0, g_assign = 0, g_crc = 0, g_write = 0, g_read = 0, g_saveText = 0, g_loadText = 0;
static void* g_lastAssignFrom = 0; static const char* g_lastName = 0; static void* g_itemValue = 0;
CWideString::CWideString() { PData = (void*)0; ++g_ctor; }
CWideString::~CWideString() { ++g_dtor; }
const CWideString& CWideString::operator=(const CWideString& other) { ++g_assign; g_lastAssignFrom = other.PData; PData = other.PData; return *this; }
CWideString CDataInputStream::ReadNullTerminatedWString() { ++g_read; CWideString s; s.PData = (void*)0x5555; return s; }
void CDataOutputStream::WriteNullTerminatedWString(const CWideString& value) { ++g_write; g_lastAssignFrom = value.PData; }
void CPersistContext_TransferableComponentGenericType::TransferIn() { }
template <class T> void CPersistContext_TransferableComponent<T>::TransferIn() { }
void CPersistContext::CheckCRC(const char* name) { ++g_crc; g_lastName = name; }
void CPersistContext::TransferObjectSaveText(const char* name, CPersistContext_TransferableComponentGenericType& item)
{ ++g_saveText; g_lastName = name; g_itemValue = static_cast<CPersistContext_TransferableComponent<CWideString>&>(item).Value; }
void CPersistContext::TransferObjectLoadText(const char* name, CPersistContext_TransferableComponentGenericType& item)
{ ++g_loadText; g_lastName = name; g_itemValue = static_cast<CPersistContext_TransferableComponent<CWideString>&>(item).Value; }
int main()
{
    CPersistContext c; memset(&c, 0, sizeof(c));
    CDataInputStream in; CDataOutputStream out; c.PLoadStream = &in; c.PSaveStream = &out;
    CWideString value; CWideString def; value.PData = (void*)0x1111; def.PData = (void*)0x2222;
    int base = g_ctor;
    c.Mode = 4; c.Transfer("key", value, def);
    CHECK(g_crc == 1 && g_lastName && g_lastName[0] == 'k' && value.PData == (void*)0x2222 && g_assign == 1);
    CHECK(g_ctor - base == 1 && g_dtor == 1);   // the traits default value
    value.PData = (void*)0x1111;
    c.Mode = 3; c.Transfer("key", value, def);
    CHECK(g_write == 1 && g_lastAssignFrom == (void*)0x1111 && g_dtor == 2);
    c.Mode = 2; c.Transfer("key", value, def);
    CHECK(g_read == 1 && value.PData == (void*)0x5555);
    c.Mode = 1; c.Transfer("abc", value, def);
    CHECK(g_saveText == 1 && g_itemValue == &value && g_lastName[0] == 'a');
    value.PData = (void*)0x1111; c.UseDefaults = false; c.Mode = 0; c.Transfer("key", value, def);
    CHECK(g_loadText == 1 && g_itemValue == &value && value.PData == (void*)0x1111);
    c.UseDefaults = true; c.Transfer("key", value, def);
    CHECK(g_loadText == 2 && value.PData == (void*)0x2222);
    if (fails == 0) printf("00410ac0_TEST PASS\n");
    return fails;
}
