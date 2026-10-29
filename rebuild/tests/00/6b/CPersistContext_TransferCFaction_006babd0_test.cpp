// Behaviour test for 006babd0 CPersistContext::Transfer<CFaction> (function-scope traits with TransferBinaryOut/In)
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __assume(x)
#endif
static char g_log[512];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static long g_live = 0;
class CFaction { public: long Id; CFaction() : Id(0) { ++g_live; } CFaction(const CFaction& o) : Id(o.Id) { ++g_live; } ~CFaction() { --g_live; } CFaction& operator=(const CFaction& o) { Id = o.Id; return *this; } };
typedef CFaction T;
static T MK(long n) { CFaction f; f.Id = n; return f; }
static long VAL(const T& v) { return v.Id; }
class CMemoryDataInputStream { public: int id;  };
class CDataOutputStream { public: int id; long written;  };
class CDataOutputStream;
template <class U> struct CPersistTraits;
template <> struct CPersistTraits<CFaction> { CFaction DefaultVal; void TransferBinaryOut(CDataOutputStream* s, const CFaction* v); void TransferBinaryIn(CMemoryDataInputStream* s, CFaction* v) { LOG(s->id == 1 && DefaultVal.Id == 0 ? "binin" : "binin?"); v->Id = 7; } };
void CPersistTraits<CFaction>::TransferBinaryOut(CDataOutputStream* s, const CFaction* v) { LOG("write"); s->written = v->Id; }
class CPersistContext_TransferableComponentGenericType { public: virtual long Peek() { return -1; } virtual ~CPersistContext_TransferableComponentGenericType() {} };
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {
    T* Value;
    CPersistContext_TransferableComponent(T& value) : Value(&value) {}
    virtual long Peek() { return VAL(*Value); }
};
class CPersistContext { public:
    bool UseDefaults;
    long Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
    long seen;
    void CheckCRC(const char* name) { LOG(std::strcmp(name, "Key") == 0 ? "crc" : "crc?"); }
    void TransferObjectSaveText(const char*, CPersistContext_TransferableComponentGenericType& item) { LOG("save"); seen = item.Peek(); }
    void TransferObjectLoadText(const char*, CPersistContext_TransferableComponentGenericType& item) { LOG("load"); seen = item.Peek(); }
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
template <class T>
void CPersistContext::Transfer(const char* name, T& value, const T& defaultValue)
{
    CheckCRC(name);
    CPersistTraits<T> traits;
    switch (Mode) {
    case 4: value = defaultValue; break;
    case 3: traits.TransferBinaryOut(PSaveStream, &value); break;
    case 2: { traits.TransferBinaryIn(PLoadStream, &value); break; }
    case 1: { CPersistContext_TransferableComponent<T> item(value); TransferObjectSaveText(name, item); break; }
    case 0: { if (UseDefaults) value = defaultValue; CPersistContext_TransferableComponent<T> item(value); TransferObjectLoadText(name, item); return; }
    default: __assume(0);
    }
}
int main()
{
    static const char* expect[6] = { "crc;load;", "crc;save;", "crc;binin;", "crc;write;", "crc;", "crc;load;" };
    CMemoryDataInputStream in; in.id = 1;
    CDataOutputStream out; out.id = 2; out.written = 0;
    for (long m = 0; m < 6; ++m) {
        CPersistContext c; c.Mode = (m == 5) ? 0 : m; c.UseDefaults = (m == 5); c.PLoadStream = &in; c.PSaveStream = &out; c.seen = -9;
        T value = MK(1); const T def = MK(2);
        g_log[0] = 0;
        c.Transfer("Key", value, def);
        if (std::strcmp(g_log, expect[m]) != 0) { std::printf("006babd0_TEST FAIL\n"); return 1; }
        long want = (m == 2) ? 7 : (m == 4 || m == 5) ? 2 : 1;
        if (VAL(value) != want) { std::printf("006babd0_TEST FAIL\n"); return 1; }
        if ((m == 0 && c.seen != 1) || (m == 5 && c.seen != 2) || (m == 1 && c.seen != 1) || (m == 3 && out.written != 1)) { std::printf("006babd0_TEST FAIL\n"); return 1; }
    }
    std::printf("006babd0_TEST PASS\n");
    return 0;
}