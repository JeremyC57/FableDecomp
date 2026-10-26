#include <cstdio>
// Model of CAInputProcess::CAInputProcess @ 0x00A0D290: base, null manager, zeroed list position.
static int g_base;
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() {} CBaseClassNonCopyable() { ++g_base; } };
struct CInputProcessOwner : CBaseClassNonCopyable { void* PPlayerInterface; CInputProcessOwner() : CBaseClassNonCopyable(), PPlayerInterface(0) {} };
class CLinkedListPosition { public: void* Data; void* Next; void* Previous; CLinkedListPosition() : Data(0), Next(0), Previous(0) {} };
class CAInputProcess : public CInputProcessOwner { public: CLinkedListPosition InputProcessListPos; CAInputProcess() : CInputProcessOwner(), InputProcessListPos() {} };
int main()
{
    CAInputProcess p;
    if (g_base != 1 || p.PPlayerInterface || p.InputProcessListPos.Data || p.InputProcessListPos.Next || p.InputProcessListPos.Previous) { std::printf("00a0d290_TEST FAIL\n"); return 1; }
    std::printf("00a0d290_TEST PASS\n"); return 0;
}