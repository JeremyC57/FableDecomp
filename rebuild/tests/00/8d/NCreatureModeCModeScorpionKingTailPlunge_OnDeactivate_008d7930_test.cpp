// behaviour test for 0x008D7930 NCreatureMode::CModeScorpionKingTailPlunge::OnDeactivate()
#include <cstdio>
class CThing { public: bool dead; int kills; bool lastFlag; void Kill(bool f) { ++kills; lastFlag = f; } };
struct CIntelligentPointer { CThing* item; CThing* GetPItem() const { return item; } };
struct CMode {
    bool Active; CIntelligentPointer PTailEmitter, PDustTrailEmitter;
    void OnDeactivate()
    {
        if (PTailEmitter.GetPItem() && !PTailEmitter.GetPItem()->dead)
            PTailEmitter.GetPItem()->Kill(true);
        if (PDustTrailEmitter.GetPItem() && !PDustTrailEmitter.GetPItem()->dead)
            PDustTrailEmitter.GetPItem()->Kill(true);
        Active = false;
    }
};
int main()
{
    CThing a = { false, 0, false }, b = { true, 0, false };
    CMode m1 = { true, { &a }, { &b } };
    m1.OnDeactivate();
    if (m1.Active || a.kills != 1 || !a.lastFlag || b.kills != 0) { std::printf("008d7930_TEST FAIL\n"); return 1; }
    CMode m2 = { true, { 0 }, { &a } };
    m2.OnDeactivate();
    if (m2.Active || a.kills != 2) { std::printf("008d7930_TEST FAIL\n"); return 1; }
    std::printf("008d7930_TEST PASS\n");
    return 0;
}