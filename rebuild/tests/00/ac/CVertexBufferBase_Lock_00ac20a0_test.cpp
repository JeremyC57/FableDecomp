// 00ac20a0 test: CVertexBufferBase::Lock on the shared header, linked against the row object (the call reaches DoLock, arguments unchanged).
#include "engine/CVertexBufferBase.h"
#include <stdio.h>
// The out-of-line base members (0x0099A2D0 / 0x0099A2E0 / 0x0099A2F0 / 0x0099A300) are not in this test: inert definitions.
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
// A test double for the private virtuals: records what reached it.
static int g_calls = 0; static unsigned long g_args[3]; static unsigned char g_mapped[4];
class CTestVertexBuffer : public CVertexBufferBase {
public:
    CTestVertexBuffer() { PVertexBuffer = 0; Stride = 0; }
private:
    unsigned char* DoLock(unsigned long a, unsigned long b, unsigned long c) { g_calls += 1; g_args[0] = a; g_args[1] = b; g_args[2] = c; return g_mapped; }
    void DoUnlock() { g_calls += 10; }
    bool DoIsBusy() const { return false; }
    unsigned long DoSizeof() const { return 0; }
};

int main()
{
    CTestVertexBuffer vb;
    unsigned char* p = vb.Lock(4, 8, 640);
    if (p != g_mapped || g_calls != 1 || g_args[0] != 4 || g_args[1] != 8 || g_args[2] != 640) { printf("00ac20a0 FAIL\n"); return 1; }
    printf("PASS_00ac20a0\n"); return 0;
}
