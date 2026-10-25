#include <cstdio>
// Behaviour test for 0x0049D8B0 CWorld::StartCountdownTimer: links against the source object.
class CWorld {
public:
    unsigned char _pad00[0xa0];
    long CountdownTimer;
    void StartCountdownTimer(long frames);
};
int main()
{
    if ((char*)&((CWorld*)0)->CountdownTimer - (char*)0 != 0xa0) { std::printf("0049d8b0_TEST FAIL layout\n"); return 1; }
    static CWorld w;
    w._pad00[0x9f] = 0x5a;
    w.StartCountdownTimer(300);
    if (w.CountdownTimer != 300 || w._pad00[0x9f] != 0x5a) { std::printf("0049d8b0_TEST FAIL store\n"); return 1; }
    w.StartCountdownTimer(0);
    if (w.CountdownTimer != 0) { std::printf("0049d8b0_TEST FAIL zero\n"); return 1; }
    std::printf("0049d8b0_TEST PASS\n");
    return 0;
}
