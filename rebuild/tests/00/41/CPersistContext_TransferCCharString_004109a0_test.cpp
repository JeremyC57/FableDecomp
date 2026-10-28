// Behaviour test for 004109a0: src/004109a0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CPersistContext::Transfer<CCharString>(const char* name, CCharString& value, const CCharString& defaultValue)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPersistContext::Transfer<CCharString>(const char* name, CCharString& value, const CCharString& defaultValue) -- persist one string member: CRC-check the key, then by mode reset to the default, binary save, binary load, text save or text load (seeding the default first when asked)
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x004109A0, 247 bytes + 20-byte jump table, `ret 0Ch`.
// Label correction: catalogued as Transfer<CIntelligentPointer<CThing>>, but every arm works on a CCharString: the reset arm
//   calls CCharString::operator= 0x0099EFB0, the binary-save arm CDataOutputStream::WriteNullTerminatedString 0x00995AE0, the
//   binary-load arm CDataInputStream slot 6 (ReadNullTerminatedString 0x00994C80, returns CCharString by value) and the traits
//   object is a default-constructed CCharString (0x0099E4B0) destroyed with ~CCharString 0x0099EAE0: this is Transfer<CCharString>.
// Callees: CPersistContext::CheckCRC 0x00404500, CCharString::CCharString() 0x0099E4B0, ~CCharString 0x0099EAE0,
//   CCharString::operator= 0x0099EFB0, WriteNullTerminatedString 0x00995AE0, TransferObjectSaveText 0x009BAD60,
//   TransferObjectLoadText 0x009BBA40 (wrapper CPersistContext_TransferableComponent<CCharString>, vtable 0x0122DEFC).
// Shape: same template as the landed enum instantiations (0x00EB65A0): the persist mode (+0x18) is a closed enum, the switch
//   ends in `default: __assume(0)`, every case arm `return`s (so case 0 runs the traits destructor before popping the
//   shrink-wrapped ebx; the enum instantiation 0x00EB65A0 compiles the same either way) and the case bodies are laid out
//   in source order 4,3,2,1,0. CPersistTraits<T> holds a
//   `T DefaultVal`; declared at function scope it is constructed once before the jump in the dead `name` argument slot (the
//   name itself is kept in edi) and destroyed on every exit. The traits' TransferBinaryIn is inline here
//   (`value = stream->ReadNullTerminatedString()`): its by-value result lands in the dead `defaultValue` slot.
class CCharString {
public:
    void* PData;
    CCharString();                                     // 0x0099E4B0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

class CDataInputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual CCharString ReadNullTerminatedString();    // slot 6, 0x00994C80
};
class CDataOutputStream {
public:
    void WriteNullTerminatedString(const CCharString& value);   // 0x00995AE0
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() {}
    void TransferBinaryIn(CDataInputStream* stream, T* value) { *value = stream->ReadNullTerminatedString(); }
};

struct CPersistContext_TransferableObjectBase {
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableObjectBase {   // vtable 0x0122DEFC
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
        return;
    case 3:
        PSaveStream->WriteNullTerminatedString(value);
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

template void CPersistContext::Transfer<CCharString>(const char* name, CCharString& value, const CCharString& defaultValue);

// ---- stubs and driver ----
void CDataInputStream::v00() { }
void CDataInputStream::v01() { }
void CDataInputStream::v02() { }
void CDataInputStream::v03() { }
void CDataInputStream::v04() { }
void CDataInputStream::v05() { }
static int g_ctor = 0, g_dtor = 0, g_assign = 0, g_crc = 0, g_write = 0, g_read = 0, g_saveText = 0, g_loadText = 0;
static void* g_lastAssignFrom = 0; static const char* g_lastName = 0; static void* g_itemValue = 0;
CCharString::CCharString() { PData = (void*)0; ++g_ctor; }
CCharString::~CCharString() { ++g_dtor; }
const CCharString& CCharString::operator=(const CCharString& other) { ++g_assign; g_lastAssignFrom = other.PData; PData = other.PData; return *this; }
CCharString CDataInputStream::ReadNullTerminatedString() { ++g_read; CCharString s; s.PData = (void*)0x5555; return s; }
void CDataOutputStream::WriteNullTerminatedString(const CCharString& value) { ++g_write; g_lastAssignFrom = value.PData; }
void CPersistContext_TransferableObjectBase::TransferIn() { }
template <class T> void CPersistContext_TransferableComponent<T>::TransferIn() { }
void CPersistContext::CheckCRC(const char* name) { ++g_crc; g_lastName = name; }
void CPersistContext::TransferObjectSaveText(const char* name, CPersistContext_TransferableObjectBase& item)
{ ++g_saveText; g_lastName = name; g_itemValue = static_cast<CPersistContext_TransferableComponent<CCharString>&>(item).Value; }
void CPersistContext::TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item)
{ ++g_loadText; g_lastName = name; g_itemValue = static_cast<CPersistContext_TransferableComponent<CCharString>&>(item).Value; }
int main()
{
    CPersistContext c; memset(&c, 0, sizeof(c));
    CDataInputStream in; CDataOutputStream out; c.PLoadStream = &in; c.PSaveStream = &out;
    CCharString value; CCharString def; value.PData = (void*)0x1111; def.PData = (void*)0x2222;
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
    if (fails == 0) printf("004109a0_TEST PASS\n");
    return fails;
}
