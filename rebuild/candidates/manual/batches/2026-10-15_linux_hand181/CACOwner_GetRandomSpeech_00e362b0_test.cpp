// behaviour test for CAC_Owner::GetRandomSpeech (0x00e362b0): 10 lines, never repeat either of the last two picks
#include <cstdio>
#include <cstdlib>
static const int N = 10;
struct Owner {
    const char* Lines[N]; long LastSpeech, PreviousSpeech; int framesLeft; int yields;
    bool IsDone() { return framesLeft <= 0; }
    const char* GetRandomSpeech()
    {
        long index;
        for (;;) {
            if (IsDone()) return "";
            index = std::rand() % N;
            if (index != LastSpeech && index != PreviousSpeech) break;
            ++yields; --framesLeft;
        }
        if (IsDone()) return "";
        PreviousSpeech = LastSpeech; LastSpeech = index;
        if (IsDone()) return "";
        return Lines[index];
    }
};
int main()
{
    Owner o; for (int i = 0; i < N; ++i) o.Lines[i] = "line";
    o.LastSpeech = 0; o.PreviousSpeech = 1; o.framesLeft = 1000; o.yields = 0;
    bool ok = true;
    for (int k = 0; k < 50; ++k) {
        long last = o.LastSpeech, prev = o.PreviousSpeech;
        const char* s = o.GetRandomSpeech();
        if (s[0] == 0 || o.LastSpeech == last || o.LastSpeech == prev || o.PreviousSpeech != last) ok = false;
    }
    o.framesLeft = 0; if (o.GetRandomSpeech()[0] != 0) ok = false;
    if (!ok) { std::printf("00e362b0_TEST FAIL\n"); return 1; }
    std::printf("00e362b0_TEST PASS\n");
    return 0;
}
