#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __assume(x)
#endif
static char g_log[512];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CDataOutputStream { int id; long written; void WriteLong(long v) { LOG("write"); written = v; } };
struct CMemoryDataInputStream { int id; };
enum EArcheryState { EArcheryState_A, EArcheryState_B, EArcheryState_C, EArcheryState_Seven = 7 };
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() {}
    void TransferBinaryIn(CMemoryDataInputStream* s, T* value) { LOG(DefaultVal == 0 && s->id == 1 ? "binin" : "binin?"); *value = (T)7; }
};
struct CPersistContext_TransferableObjectBase { virtual long Peek() { return -1; } virtual ~CPersistContext_TransferableObjectBase() {} };
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableObjectBase {
    T* Value;
    CPersistContext_TransferableComponent(T& value) : Value(&value) {}
    virtual long Peek() { return (long)*Value; }
};
struct CPersistContext {
    bool UseDefaults;
    long Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
    long seen;
    void CheckCRC(const char* name) { LOG(std::strcmp(name, "Key") == 0 ? "crc" : "crc?"); }
    void TransferObjectSaveText(const char*, CPersistContext_TransferableObjectBase& item) { LOG("save"); seen = item.Peek(); }
    void TransferObjectLoadText(const char*, CPersistContext_TransferableObjectBase& item) { LOG("load"); seen = item.Peek(); }
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
template <class T>
void CPersistContext::Transfer(const char* name, T& value, const T& defaultValue)
{
    CheckCRC(name);
    CPersistTraits<T> traits;
    switch (Mode) {
    case 4: value = defaultValue; break;
    case 3: PSaveStream->WriteLong(value); break;
    case 2: traits.TransferBinaryIn(PLoadStream, &value); break;
    case 1: { CPersistContext_TransferableComponent<T> item(value); TransferObjectSaveText(name, item); break; }
    case 0: { if (UseDefaults) value = defaultValue; CPersistContext_TransferableComponent<T> item(value); TransferObjectLoadText(name, item); break; }
    default: __assume(0);
    }
}
int main()
{
    static const char* expect[6] = { "crc;load;", "crc;save;", "crc;binin;", "crc;write;", "crc;", "crc;load;" };
    CMemoryDataInputStream in = { 1 }; CDataOutputStream out = { 2, 0 };
    for (long m = 0; m < 6; ++m) {
        CPersistContext c; c.Mode = (m == 5) ? 0 : m; c.UseDefaults = (m == 5); c.PLoadStream = &in; c.PSaveStream = &out; c.seen = -9;
        EArcheryState value = EArcheryState_B; const EArcheryState def = EArcheryState_C;
        g_log[0] = 0;
        c.Transfer("Key", value, def);
        if (std::strcmp(g_log, expect[m]) != 0) { std::printf("00cdcc20_TEST FAIL\n"); return 1; }
        long want = (m == 2) ? 7 : (m == 4 || m == 5) ? 2 : 1;
        if ((long)value != want) { std::printf("00cdcc20_TEST FAIL\n"); return 1; }
        if ((m == 0 && c.seen != 1) || (m == 5 && c.seen != 2) || (m == 1 && c.seen != 1) || (m == 3 && out.written != 1)) { std::printf("00cdcc20_TEST FAIL\n"); return 1; }
    }
    std::printf("00cdcc20_TEST PASS\n");
    return 0;
}