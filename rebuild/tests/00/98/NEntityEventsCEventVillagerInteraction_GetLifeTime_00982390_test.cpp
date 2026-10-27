// Behaviour test for 0x00982390 NEntityEvents::CEventVillagerInteraction::GetLifeTime() const:
// StillWaitingFrame - WorldFrame + 60.
#include <cstdio>

class EventBase { public: long WorldFrame; virtual ~EventBase() { } virtual long GetLifeTime() const { return 0; } };
class VillagerInteraction : public EventBase {
public:
    long StillWaitingFrame;
    virtual long GetLifeTime() const { return StillWaitingFrame - WorldFrame + 60; }
};

int main()
{
    VillagerInteraction e; e.WorldFrame = 100; e.StillWaitingFrame = 100;
    const EventBase& b = e;
    if (b.GetLifeTime() != 60) { std::printf("00982390_TEST FAIL\n"); return 1; }
    e.StillWaitingFrame = 145;
    if (b.GetLifeTime() != 105) { std::printf("00982390_TEST FAIL\n"); return 1; }
    std::printf("00982390_TEST PASS\n");
    return 0;
}