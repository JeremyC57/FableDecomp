// Behaviour test for CPersistContext::Transfer<ECrimeSeverity> (0x007B45A0): CRC-check the key, then per mode:
// 0 text load (seeding the default first only when UseDefaults), 1 text save, 2 binary load via the traits object,
// 3 WriteLong(value) to PSaveStream, 4 reset to the default.
#include <stdio.h>
#include <string.h>

// ---- reconstruction under test (verbatim copy of src/007b45a0.cpp) ----
// Leaf: Transfer
// Grade: RELOCATION_MATCH 208/208 on the corrected span 0x007B45A0+208 (code + embedded jump table; bounds audit) -- catalog row 224 B also held the 3-byte `ret 4` stub at 0x007B4670
// CPersistContext::Transfer<ECrimeSeverity>(const char* name, ECrimeSeverity& value, const ECrimeSeverity& defaultValue) -- persist one enum member ("LastCrimeSeenSeverity"): CRC-check the key, then by mode text load (seeding the default first when asked), text save, binary load, binary save as a long, or reset to the default
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B45A0, `ret 0Ch`. Called from Save_TransferCriminalRecord (0x007B3760) with the key "LastCrimeSeenSeverity".
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteLong 0x00993EB0 (catalogued "WriteCBYTE"),
//   CPersistTraits<T>::TransferBinaryIn 0x0047926C, CPersistContext::TransferObjectSaveText 0x009BAD60 and TransferObjectLoadText
//   0x009BBA40 (both take the CPersistContext_TransferableComponent<T> wrapper, vtable 0x0126D7EC).
// Shape: the speed-optimised sibling of the landed size-TU CDefIndex instantiation 0x005CC1E8. The persist mode
//   (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a closed enum, so the switch
//   ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]`; the case bodies are laid out in source
//   order and retail has them 4,3,2,1,0. the traits object of the binary-load arm is empty and lives in the dead value-argument slot (`lea ecx,[esp+20h]` once the value is in eax). The 8-byte frame is the {vtable, value*} wrapper of the text arms.
//   The enum's name is descriptive (from the key); it is a 4-byte enum written with WriteLong.
class CDataOutputStream { public:
    void WriteLong(long value);                   // 0x00993EB0
};
class CMemoryDataInputStream;
enum ECrimeSeverity { ECrimeSeverity_Default };

template <class T> struct CPersistTraits {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* value);   // 0x0047926C for this T
};

struct CPersistContext_TransferableObjectBase {
    virtual void TransferIn();
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableObjectBase {   // vtable 0x0126D7EC
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
        PSaveStream->WriteLong(value);
        break;
    case 2:
        CPersistTraits<T>().TransferBinaryIn(PLoadStream, &value);
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

template void CPersistContext::Transfer<ECrimeSeverity>(const char* name, ECrimeSeverity& value, const ECrimeSeverity& defaultValue);

// ---- harness ----
static const char* g_crc;
static const char* g_name;
static int g_which;
static void* g_ptr;
static void* g_stream;
static long g_written;
static long g_seen;

void CDataOutputStream::WriteLong(long value) { g_which = 3; g_stream = this; g_written = value; }
template <class T> void CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStream* stream, T* value) { g_which = 2; g_stream = stream; g_ptr = value; *value = (T)7; }
void CPersistContext_TransferableObjectBase::TransferIn() {}
template <class T> void CPersistContext_TransferableComponent<T>::TransferIn() {}
void CPersistContext::CheckCRC(const char* name) { g_crc = name; }
void CPersistContext::TransferObjectSaveText(const char* name, CPersistContext_TransferableObjectBase& item)
{
    g_which = 1; g_name = name; g_ptr = ((CPersistContext_TransferableComponent<ECrimeSeverity>&)item).Value; g_seen = *(long*)g_ptr;
}
void CPersistContext::TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item)
{
    g_which = 10; g_name = name; g_ptr = ((CPersistContext_TransferableComponent<ECrimeSeverity>&)item).Value; g_seen = *(long*)g_ptr;
}

static int g_fail;
static void check(int cond, const char* what) { if (!cond) { printf("FAIL %s\n", what); g_fail = 1; } }

static void run(CPersistContext& ctx, long mode, bool useDefaults, ECrimeSeverity& v, const ECrimeSeverity& def)
{
    g_crc = 0; g_name = 0; g_which = -1; g_ptr = 0; g_stream = 0; g_written = 0; g_seen = -1;
    ctx.Mode = mode; ctx.UseDefaults = useDefaults;
    ctx.Transfer("LastCrimeSeenSeverity", v, def);
}

int main()
{
    CPersistContext ctx;
    ECrimeSeverity v;
    const ECrimeSeverity def = (ECrimeSeverity)5;
    memset(&ctx, 0, sizeof(ctx));
    ctx.PLoadStream = (CMemoryDataInputStream*)0x1111;
    ctx.PSaveStream = (CDataOutputStream*)0x2222;

    v = (ECrimeSeverity)3; run(ctx, 0, false, v, def);
    check(g_crc != 0 && strcmp(g_crc, "LastCrimeSeenSeverity") == 0, "CheckCRC gets the key");
    check(g_which == 10 && g_ptr == &v && g_seen == 3 && v == 3, "mode 0 text load without defaults");
    v = (ECrimeSeverity)3; run(ctx, 0, true, v, def);
    check(g_which == 10 && g_ptr == &v && g_seen == 5 && v == 5, "mode 0 text load seeds the default first");
    v = (ECrimeSeverity)4; run(ctx, 1, true, v, def);
    check(g_which == 1 && g_ptr == &v && g_seen == 4 && v == 4, "mode 1 text save leaves the value");
    v = (ECrimeSeverity)4; run(ctx, 2, true, v, def);
    check(g_which == 2 && g_stream == (void*)0x1111 && g_ptr == &v && v == 7, "mode 2 binary load");
    v = (ECrimeSeverity)6; run(ctx, 3, false, v, def);
    check(g_which == 3 && g_stream == (void*)0x2222 && g_written == 6, "mode 3 binary save");
    v = (ECrimeSeverity)2; run(ctx, 4, false, v, def);
    check(g_which == -1 && v == 5, "mode 4 resets to the default");
    if (g_fail) return 1;
    printf("007b45a0_TEST PASS\n");
    return 0;
}