// Behaviour test for 00685f80 CPersistContext::Transfer<COpinionTransientOffset> (raw 0x18-byte virtual Write, member TransferBinaryIn)
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __assume(x)
#endif
static char g_log[512];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CMemoryDataInputStream;
class COpinionTransientOffset { public: long Data[6]; void TransferBinaryIn(CMemoryDataInputStream* stream); };
typedef COpinionTransientOffset T;
static T MK(long n) { T t; for (int i = 0; i < 6; ++i) t.Data[i] = n * 10 + i; t.Data[0] = n; return t; }
static long VAL(const T& v) { return v.Data[0]; }
class CMemoryDataInputStream { public: int id;  };
class CDataOutputStream { public: int id; long written; virtual void Write(const void* data, long size) { LOG(size == (long)sizeof(COpinionTransientOffset) ? "write" : "write?"); written = static_cast<const COpinionTransientOffset*>(data)->Data[0]; } virtual ~CDataOutputStream() {} };
void COpinionTransientOffset::TransferBinaryIn(CMemoryDataInputStream* s) { LOG(s->id == 1 ? "binin" : "binin?"); *this = MK(7); }
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
    switch (Mode) {
    case 4: value = defaultValue; break;
    case 3: PSaveStream->Write(&value, sizeof(T)); break;
    case 2: { value.TransferBinaryIn(PLoadStream); break; }
    case 1: { CPersistContext_TransferableComponent<T> item(value); TransferObjectSaveText(name, item); break; }
    case 0: { if (UseDefaults) value = defaultValue; CPersistContext_TransferableComponent<T> item(value); TransferObjectLoadText(name, item); break; }
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
        if (std::strcmp(g_log, expect[m]) != 0) { std::printf("00685f80_TEST FAIL\n"); return 1; }
        long want = (m == 2) ? 7 : (m == 4 || m == 5) ? 2 : 1;
        if (VAL(value) != want) { std::printf("00685f80_TEST FAIL\n"); return 1; }
        if ((m == 0 && c.seen != 1) || (m == 5 && c.seen != 2) || (m == 1 && c.seen != 1) || (m == 3 && out.written != 1)) { std::printf("00685f80_TEST FAIL\n"); return 1; }
    }
    std::printf("00685f80_TEST PASS\n");
    return 0;
}