// Behaviour test for 0x0121F260: CTCDrunkenness::DrunkennessDeltaPerFrameOfDrinking = 0.1f / fps, and its use (frames * delta added to a drunkenness float).
#include <cstdio>
class CMainGameComponent {
protected:
    static long ConstantFPS;
public:
    static long GetConstantFPS() { return ConstantFPS; }
};
long CMainGameComponent::ConstantFPS = 25;
class CTCDrunkenness {
public:
    static const float DrunkennessDeltaPerFrameOfDrinking;
    float Drunkenness;
    void AddDrinkingFrames(long frames) { Drunkenness += frames * DrunkennessDeltaPerFrameOfDrinking; }
};
const float CTCDrunkenness::DrunkennessDeltaPerFrameOfDrinking = 0.1f / CMainGameComponent::GetConstantFPS();
int main()
{
    float d = CTCDrunkenness::DrunkennessDeltaPerFrameOfDrinking - 0.004f;
    CTCDrunkenness t; t.Drunkenness = 0.0f; t.AddDrinkingFrames(250);   // ten seconds of drinking = 0.1 per second
    float e = t.Drunkenness - 1.0f;
    if (d > 0.00001f || d < -0.00001f || e > 0.0001f || e < -0.0001f) { std::printf("0121f260_TEST FAIL\n"); return 1; }
    std::printf("0121f260_TEST PASS\n");
    return 0;
}