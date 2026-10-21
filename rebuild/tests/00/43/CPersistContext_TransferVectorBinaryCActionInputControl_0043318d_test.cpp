// test for 0043318d TransferVectorBinary<CActionInputControl>
#include <stdio.h>
class CMemoryDataInputStream;
class CDataOutputStream;
class CActionInputControl;
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
template <class T> void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<T>& v);
template <class T> void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream& stream, std::vector<T>& v);
class CPersistContext {
public:
    void*                   PDefinitionManager;          // +0x00
    void*                   PSymbolMap;                  // +0x04
    void*                   PParser;                     // +0x08
    void*                   ErrorCallback;               // +0x0c
    void*                   PStringToClean;              // +0x10
    bool                    UseDefaults;                 // +0x14
    bool                    UseCRCCheck;                 // +0x15
    bool                    SingleTransfer;              // +0x16
    bool                    Finished;                    // +0x17
    long                    Mode;                        // +0x18 EMode: 2 binary load
    bool                    Binary;                      // +0x1c
    void*                   PStringToSaveTo;             // +0x20
    CMemoryDataInputStream* PLoadStream;                 // +0x24
    CDataOutputStream*      PSaveStream;                 // +0x28
    void CheckCRC(const char* name);                     // 0x00404500
    template <class T> void TransferVectorBinary(std::vector<T>& v)
    {
        CheckCRC("");
        switch (Mode) {
        case 2:
            GFSerialiseVectorBinaryIn(*PLoadStream, v);
            break;
        default:
            GFSerialiseVectorBinaryOut(*PSaveStream, v);
            break;
        }
    }
};
template void CPersistContext::TransferVectorBinary<CActionInputControl>(std::vector<CActionInputControl>&);

class CActionInputControl { public: long X; };
static int g_in, g_out, g_crc; static void* g_stream; static void* g_vec;
template <class T> void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& s, std::vector<T>& v) { ++g_in; g_stream = &s; g_vec = &v; }
template <class T> void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream& s, std::vector<T>& v) { ++g_out; g_stream = &s; g_vec = &v; }
void CPersistContext::CheckCRC(const char* name) { if (name && !*name) ++g_crc; }
int main()
{
    CPersistContext c; std::vector<CActionInputControl> v; int ld = 1, sv = 2;
    c.PLoadStream = (CMemoryDataInputStream*)&ld; c.PSaveStream = (CDataOutputStream*)&sv;
    c.Mode = 2; c.TransferVectorBinary(v);
    int ok = g_in == 1 && g_out == 0 && g_stream == &ld && g_vec == &v && g_crc == 1;
    c.Mode = 3; c.TransferVectorBinary(v);
    ok = ok && g_in == 1 && g_out == 1 && g_stream == &sv && g_crc == 2;
    printf(ok ? "0043318d_TEST PASS\n" : "0043318d_TEST FAIL\n");
    return 0;
}