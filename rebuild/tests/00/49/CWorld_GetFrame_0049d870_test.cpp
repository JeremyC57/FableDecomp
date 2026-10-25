#include <cstdio>
// Behaviour test for 0x0049D870 CWorld::GetFrame: links against the source object and defines the static counter.
class CWorld {
public:
    static long __fastcall GetFrame();
    static void SetFrameForTest(long frame) { Frame = frame; }
private:
    static long Frame;
};
long CWorld::Frame = 0;
int main()
{
    CWorld::SetFrameForTest(12345);
    if (CWorld::GetFrame() != 12345) { std::printf("0049d870_TEST FAIL first\n"); return 1; }
    CWorld::SetFrameForTest(-7);
    if (CWorld::GetFrame() != -7) { std::printf("0049d870_TEST FAIL second\n"); return 1; }
    std::printf("0049d870_TEST PASS\n");
    return 0;
}
