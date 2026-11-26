// Behaviour test for 0x00925F80 CAIStateGroup_WifeGiveYouGift::InitStateGroup(const CWorld&, CThingAICreature&): clears the three gift members.
#include <cstdio>
struct CWorldModel { }; struct CCreatureModel { };
struct CWifeModel {
    long FrameToSpeakNext; long GiftToGiveDef; bool DeliveredGiveSpeech;
    void InitStateGroup(const CWorldModel&, CCreatureModel&) { FrameToSpeakNext = 0; GiftToGiveDef = 0; DeliveredGiveSpeech = false; }
};
int main()
{
    CWorldModel w; CCreatureModel c; CWifeModel o; o.FrameToSpeakNext = 9; o.GiftToGiveDef = 9; o.DeliveredGiveSpeech = true; o.InitStateGroup(w, c);
    if (o.FrameToSpeakNext || o.GiftToGiveDef || o.DeliveredGiveSpeech) { std::printf("00925f80_TEST FAIL\n"); return 1; }
    std::printf("00925f80_TEST PASS\n"); return 0;
}
