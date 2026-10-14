#include <cstdio>
static const void* g_baseSrc = 0;
struct CDefClassBase { virtual void v00() { } unsigned char _body[0x24]; CDefClassBase() { } CDefClassBase(const CDefClassBase& o) { g_baseSrc = &o; } };
namespace NSpeechGainManager {
struct CDialogueLayerDef : CDefClassBase {
    virtual void v00() { }
    long ResponseTimeSecsAttack, ResponseTimeSecsRelease, GainLevelHigh, GainLevelMid, GainLevelLow, FocusShiftDistanceThreshold;
    CDialogueLayerDef() { }
    CDialogueLayerDef(const CDialogueLayerDef& o) : CDefClassBase(o), ResponseTimeSecsAttack(o.ResponseTimeSecsAttack), ResponseTimeSecsRelease(o.ResponseTimeSecsRelease),
        GainLevelHigh(o.GainLevelHigh), GainLevelMid(o.GainLevelMid), GainLevelLow(o.GainLevelLow), FocusShiftDistanceThreshold(o.FocusShiftDistanceThreshold) { }
}; }
int main()
{
    NSpeechGainManager::CDialogueLayerDef a;
    a.ResponseTimeSecsAttack = 1; a.ResponseTimeSecsRelease = 2; a.GainLevelHigh = 3; a.GainLevelMid = 4; a.GainLevelLow = 5; a.FocusShiftDistanceThreshold = 6;
    NSpeechGainManager::CDialogueLayerDef b(a);
    if (g_baseSrc != &a || b.ResponseTimeSecsAttack != 1 || b.ResponseTimeSecsRelease != 2 || b.GainLevelHigh != 3 || b.GainLevelMid != 4 || b.GainLevelLow != 5
        || b.FocusShiftDistanceThreshold != 6 || (char*)&b.FocusShiftDistanceThreshold - (char*)&b != 0x3c || sizeof(b) != 0x40) { std::printf("004663e7_TEST FAIL\n"); return 1; }
    std::printf("004663e7_TEST PASS\n"); return 0;
}