// Behaviour test for 007b5470 CPersistContext::TransferVector<CCrimeWaitingForBribe>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __assume(x)
#endif
static char g_log[512];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CCrimeWaitingForBribe { long Data[3]; };
struct CDataOutputStream { int id; long written; };
struct CMemoryDataInputStream { int id; };
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void clear() { LOG("clear"); _M_finish = _M_start; }   // out of line in retail
};
static void GFSerialiseVectorBinaryIn(CMemoryDataInputStream* stream, vector<CCrimeWaitingForBribe>& value) { LOG(stream->id == 1 ? "binin" : "binin?"); value._M_finish = value._M_start + 1; }
static void GFSerialiseVectorBinaryOut(CDataOutputStream* stream, vector<CCrimeWaitingForBribe>& value) { LOG("binout"); stream->written = (long)(value._M_finish - value._M_start); }
struct CPersistContext_TransferableVectorGenericType { virtual long GetCount() { return -1; } virtual ~CPersistContext_TransferableVectorGenericType() {} };
template <class T> struct CPersistContext_TransferableVector : CPersistContext_TransferableVectorGenericType {
    vector<T>* TransVector;
    CPersistContext_TransferableVector(vector<T>& v) : TransVector(&v) {}
    virtual long GetCount() { return (long)(TransVector->_M_finish - TransVector->_M_start); }
};
struct CPersistContext {
    bool UseDefaults;
    long Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
    long seen;
    void CheckCRC(const char* name) { LOG(std::strcmp(name, "Key") == 0 ? "crc" : "crc?"); }
    void TransferVectorSaveText(const char*, CPersistContext_TransferableVectorGenericType& item) { LOG("save"); seen = item.GetCount(); }
    void TransferVectorLoadText(const char*, CPersistContext_TransferableVectorGenericType& item) { LOG("load"); seen = item.GetCount(); }
    template <class T> void TransferVector(const char* name, vector<T>& value);
};
template <class T>
void CPersistContext::TransferVector(const char* name, vector<T>& value)
{
    CheckCRC(name);
    switch (Mode) {
    case 4: value.clear(); break;
    case 3: GFSerialiseVectorBinaryOut(PSaveStream, value); break;
    case 2: GFSerialiseVectorBinaryIn(PLoadStream, value); break;
    case 1: { CPersistContext_TransferableVector<T> item(value); TransferVectorSaveText(name, item); break; }
    case 0: { CPersistContext_TransferableVector<T> item(value); TransferVectorLoadText(name, item); break; }
    default: __assume(0);
    }
}
int main()
{
    static const char* expect[5] = { "crc;load;", "crc;save;", "crc;binin;", "crc;binout;", "crc;clear;" };
    static const long count[5] = { 3, 3, 1, 3, 0 };
    CMemoryDataInputStream in; in.id = 1;
    CDataOutputStream out; out.id = 2; out.written = 0;
    CCrimeWaitingForBribe storage[4];
    for (long m = 0; m < 5; ++m) {
        CPersistContext c; c.Mode = m; c.UseDefaults = false; c.PLoadStream = &in; c.PSaveStream = &out; c.seen = -9;
        vector<CCrimeWaitingForBribe> v; v._M_start = storage; v._M_finish = storage + 3; v._M_end_of_storage = storage + 4;
        g_log[0] = 0;
        c.TransferVector("Key", v);
        if (std::strcmp(g_log, expect[m]) != 0) { std::printf("007b5470_TEST FAIL\n"); return 1; }
        if ((long)(v._M_finish - v._M_start) != count[m] || v._M_start != storage) { std::printf("007b5470_TEST FAIL\n"); return 1; }
        if ((m <= 1 && c.seen != 3) || (m == 3 && out.written != 3)) { std::printf("007b5470_TEST FAIL\n"); return 1; }
    }
    std::printf("007b5470_TEST PASS\n");
    return 0;
}