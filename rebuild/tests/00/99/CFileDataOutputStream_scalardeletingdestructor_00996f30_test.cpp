#include <cstdio>
// Behaviour test for CFileDataOutputStream scalar deleting destructor (0x00996F30): the file-name string,
// then the CDataStream layer, then the CBase root.
static int g_order[8]; static int g_n = 0;
struct CCharString { void* m_pData; ~CCharString() { g_order[g_n++] = 3; } };
namespace N_0099a300 { struct CBase { virtual ~CBase() { g_order[g_n++] = 1; } }; } using N_0099a300::CBase;
struct CDataStream : CBase { virtual ~CDataStream() { g_order[g_n++] = 2; } };
struct CFileDataOutputStream : CDataStream { CCharString m_fileName; void* File; virtual ~CFileDataOutputStream() { } };
int main()
{
    CBase* p = new CFileDataOutputStream;
    delete p;
    if (g_n != 3 || g_order[0] != 3 || g_order[1] != 2 || g_order[2] != 1) { std::printf("00996f30_TEST FAIL\n"); return 1; }
    std::printf("00996f30_TEST PASS\n");
    return 0;
}