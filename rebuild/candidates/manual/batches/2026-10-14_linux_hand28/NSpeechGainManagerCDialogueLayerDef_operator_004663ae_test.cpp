#include <cstdio>
static const void* g_baseSrc = 0;
struct CDefClassBase { virtual void v00() { } unsigned char _body[0x24]; CDefClassBase& operator=(const CDefClassBase& o) { g_baseSrc = &o; return *this; } };
namespace NSpeechGainManager {
struct CDialogueLayerDef : CDefClassBase {
    virtual void v00() { }
    long ResponseTimeSecsAttack, ResponseTimeSecsRelease, GainLevelHigh, GainLevelMid, GainLevelLow, FocusShiftDistanceThreshold;
    CDialogueLayerDef& operator=(const CDialogueLayerDef& o) {
        CDefClassBase::operator=(o);
        ResponseTimeSecsAttack = o.ResponseTimeSecsAttack; ResponseTimeSecsRelease = o.ResponseTimeSecsRelease; GainLevelHigh = o.GainLevelHigh;
        GainLevelMid = o.GainLevelMid; GainLevelLow = o.GainLevelLow; FocusShiftDistanceThreshold = o.FocusShiftDistanceThreshold; return *this; }
}; }
int main()
{
    NSpeechGainManager::CDialogueLayerDef a, b;
    a.ResponseTimeSecsAttack = 1; a.ResponseTimeSecsRelease = 2; a.GainLevelHigh = 3; a.GainLevelMid = 4; a.GainLevelLow = 5; a.FocusShiftDistanceThreshold = 6;
    NSpeechGainManager::CDialogueLayerDef* r = &(b = a);
    if (r != &b || g_baseSrc != &a || b.ResponseTimeSecsAttack != 1 || b.ResponseTimeSecsRelease != 2 || b.GainLevelHigh != 3 || b.GainLevelMid != 4 || b.GainLevelLow != 5
        || b.FocusShiftDistanceThreshold != 6 || (char*)&b.ResponseTimeSecsAttack - (char*)&b != 0x28 || sizeof(b) != 0x40) { std::printf("004663ae_TEST FAIL\n"); return 1; }
    std::printf("004663ae_TEST PASS\n"); return 0;
}
