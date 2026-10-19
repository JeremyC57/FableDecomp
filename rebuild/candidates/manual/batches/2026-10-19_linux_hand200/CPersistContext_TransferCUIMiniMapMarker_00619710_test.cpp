#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __assume(x)
#endif
static char g_log[512];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Elem { long v[3]; };
struct CDataOutputStream { int id; };
struct CMemoryDataInputStream { int id; };
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last) { LOG("erase"); if (first == _M_start && last == _M_finish) _M_finish = _M_start; return first; }
    void clear() { erase(begin(), end()); }
};
static vector<Elem>* g_seen;
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream* s, vector<Elem>& v) { LOG(s->id == 1 ? "binin" : "binin?"); g_seen = &v; }
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream* s, vector<Elem>& v) { LOG(s->id == 2 ? "binout" : "binout?"); g_seen = &v; }
struct CPersistContext_TransferableVectorGenericType { virtual long GetCount() { return -1; } virtual ~CPersistContext_TransferableVectorGenericType() {} };
template <class T> struct CPersistContext_TransferableVector : CPersistContext_TransferableVectorGenericType {
    vector<T>* TransVector;
    CPersistContext_TransferableVector(vector<T>& v) : TransVector(&v) {}
    virtual long GetCount() { return (long)(TransVector->_M_finish - TransVector->_M_start); }
};
struct CPersistContext {
    long Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
    void CheckCRC(const char* name) { LOG(std::strcmp(name, "Key") == 0 ? "crc" : "crc?"); }
    void TransferVectorSaveText(const char* name, CPersistContext_TransferableVectorGenericType& item) { LOG(item.GetCount() == 2 && name[0] == 'K' ? "save" : "save?"); }
    void TransferVectorLoadText(const char* name, CPersistContext_TransferableVectorGenericType& item) { LOG(item.GetCount() == 2 && name[0] == 'K' ? "load" : "load?"); }
    template <class T> void Transfer(const char* name, vector<T>& value);
};
template <class T>
void CPersistContext::Transfer(const char* name, vector<T>& value)
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
    static const char* expect[5] = { "crc;load;", "crc;save;", "crc;binin;", "crc;binout;", "crc;erase;" };
    CMemoryDataInputStream in = { 1 }; CDataOutputStream out = { 2 };
    for (long m = 0; m < 5; ++m) {
        Elem e[2]; std::memset(e, 0, sizeof(e));
        vector<Elem> v; v._M_start = e; v._M_finish = e + 2; v._M_end_of_storage = e + 2;
        CPersistContext c; c.Mode = m; c.PLoadStream = &in; c.PSaveStream = &out;
        g_log[0] = 0; g_seen = 0;
        c.Transfer("Key", v);
        if (std::strcmp(g_log, expect[m]) != 0) { std::printf("00619710_TEST FAIL\n"); return 1; }
        if ((m == 2 || m == 3) && g_seen != &v) { std::printf("00619710_TEST FAIL\n"); return 1; }
        if (m == 4 && v._M_finish != v._M_start) { std::printf("00619710_TEST FAIL\n"); return 1; }
        if (m != 4 && v._M_finish != e + 2) { std::printf("00619710_TEST FAIL\n"); return 1; }
    }
    std::printf("00619710_TEST PASS\n");
    return 0;
}
