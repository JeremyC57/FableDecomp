#include <cstdio>
// Behaviour model of CNamedDataStream::~CNamedDataStream @ 0x00996D80: the name member is destroyed before the base.
static int g_order[4]; static int g_n = 0;
class CBaseClassNonCopyable {
public:   virtual ~CBaseClassNonCopyable() { g_order[g_n++] = 2; } };
class CCharString { public: void* m_pData; ~CCharString() { g_order[g_n++] = 1; } };
struct CDataStream : CBaseClassNonCopyable { ~CDataStream() {} };
struct CNamedDataStream : CDataStream { CCharString m_name; ~CNamedDataStream() {} };
int main()
{
    { CNamedDataStream s; s.m_name.m_pData = 0; }
    bool ok = g_n == 2 && g_order[0] == 1 && g_order[1] == 2;
    if (!ok) { std::printf("00996d80_TEST FAIL\n"); return 1; }
    std::printf("00996d80_TEST PASS\n");
    return 0;
}