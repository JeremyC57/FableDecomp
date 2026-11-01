// Behaviour test for CPersistContext::TransferVector<bool> 0x00CDCF80: the five persist modes on a byte-per-bool vector member
#include <cstdio>
static int g_mode = -1, g_crc = 0;
struct BoolVector {
    bool* _M_start; bool* _M_finish; bool* _M_end_of_storage;
    bool* begin() { return _M_start; }
    bool* end() { return _M_finish; }
    bool* erase(bool* first, bool* last) { bool* o = first; for (bool* i = last; i != _M_finish; ++i, ++o) *o = *i; _M_finish = o; return first; }
    void clear() { erase(begin(), end()); }
};
class CDataOutputStream { public: int writes; };
class CMemoryDataInputStream { public: int reads; };
static void GFSerialiseVectorBinaryOut(CDataOutputStream& s, BoolVector&) { ++s.writes; g_mode = 3; }
static void GFSerialiseVectorBinaryIn(CMemoryDataInputStream& s, BoolVector&) { ++s.reads; g_mode = 2; }
struct Wrapper { BoolVector* TransVector; Wrapper(BoolVector& v) : TransVector(&v) {} };
class CPersistContext { public:
    long Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
    void CheckCRC(const char*) { ++g_crc; }
    void TransferVectorSaveText(const char*, Wrapper& w) { g_mode = 1; (void)w; }
    void TransferVectorLoadText(const char*, Wrapper& w) { g_mode = 0; (void)w; }
    void TransferVector(const char* name, BoolVector& value)
    {
        CheckCRC(name);
        switch (Mode) {
        case 4: value.clear(); break;
        case 3: GFSerialiseVectorBinaryOut(*PSaveStream, value); break;
        case 2: GFSerialiseVectorBinaryIn(*PLoadStream, value); break;
        case 1: { Wrapper item(value); TransferVectorSaveText(name, item); break; }
        case 0: { Wrapper item(value); TransferVectorLoadText(name, item); break; }
        }
    }
};
int main()
{
    bool store[4] = { true, false, true, true };
    BoolVector v; v._M_start = store; v._M_finish = store + 4; v._M_end_of_storage = store + 4;
    CDataOutputStream out = { 0 }; CMemoryDataInputStream in = { 0 };
    CPersistContext c; c.PLoadStream = &in; c.PSaveStream = &out;
    for (long m = 0; m < 4; ++m) {
        c.Mode = m; c.TransferVector("PadActive", v);
        if (g_mode != m) { std::printf("00cdcf80_TEST FAIL\n"); return 1; }
    }
    if (out.writes != 1 || in.reads != 1 || v._M_finish != store + 4) { std::printf("00cdcf80_TEST FAIL\n"); return 1; }
    c.Mode = 4; c.TransferVector("PadActive", v);
    if (v._M_finish != v._M_start || g_crc != 5) { std::printf("00cdcf80_TEST FAIL\n"); return 1; }
    std::printf("00cdcf80_TEST PASS\n");
    return 0;
}