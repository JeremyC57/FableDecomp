// Behaviour test for CMessageEventFilter_CanBeHeard::operator() 0x006AF270: the filter asks its creature whether the message
// event is within hearing range (`mov ecx,[ecx] ; jmp IsMessageEventWithinHearingRange`) and returns that answer.
#include <cstdio>
struct CMessageEvent { float Distance; };
struct CThingAICreature { float HearingRange; bool IsMessageEventWithinHearingRange(const CMessageEvent& e) const { return e.Distance <= HearingRange; } };
struct CMessageEventFilter_CanBeHeard { CThingAICreature* Creature; bool operator()(const CMessageEvent& e) const { return Creature->IsMessageEventWithinHearingRange(e); } };
int main()
{
    CThingAICreature c = { 10.0f };
    CMessageEventFilter_CanBeHeard f = { &c };
    CMessageEvent near_ = { 4.0f }, far_ = { 40.0f };
    if (!f(near_) || f(far_)) { std::printf("006af270_TEST FAIL\n"); return 1; }
    std::printf("006af270_TEST PASS\n");
    return 0;
}