// Behaviour test for 0x00C21750 CSoundEvent::AssignPhysicalPlayer: check state 3, leave the waiting list
// (getting the channel back), then play through the player on that channel -- in that order
#include <stdio.h>
struct CSoundEventManager { int id; };
struct CSoundChannel { int id; };
struct CPhysicalPlayer { int id; };
static int gStep = 0;
static int gCheckStep = 0, gStopStep = 0, gPlayStep = 0;
static long gExpected = -1;
static CSoundEventManager* gStopManager = 0;
static CSoundEventManager* gPlayManager = 0;
static CSoundChannel* gPlayChannel = 0;
static CPhysicalPlayer* gPlayPlayer = 0;
static CSoundChannel gChannel;
struct CSoundEvent {
    unsigned short m_state;
    CSoundChannel* m_pChannel;
    void CheckState(long expected);
    CSoundChannel* StopWaitingForPhysicalPlayer(CSoundEventManager* manager);
    void PlayPhysical(CSoundEventManager* manager, CSoundChannel* channel, CPhysicalPlayer* player);
    void AssignPhysicalPlayer(CSoundEventManager* manager, CPhysicalPlayer* player);
};
void CSoundEvent::CheckState(long expected) { gExpected = expected; gCheckStep = ++gStep; }
CSoundChannel* CSoundEvent::StopWaitingForPhysicalPlayer(CSoundEventManager* manager)
{
    gStopManager = manager; gStopStep = ++gStep;
    CSoundChannel* c = m_pChannel;
    m_pChannel = 0;
    return c;
}
void CSoundEvent::PlayPhysical(CSoundEventManager* manager, CSoundChannel* channel, CPhysicalPlayer* player)
{
    gPlayManager = manager; gPlayChannel = channel; gPlayPlayer = player; gPlayStep = ++gStep;
    m_state = 6;
}
void CSoundEvent::AssignPhysicalPlayer(CSoundEventManager* manager, CPhysicalPlayer* player)
{
    CheckState(3);
    CSoundChannel* channel = StopWaitingForPhysicalPlayer(manager);
    PlayPhysical(manager, channel, player);
}
int main()
{
    CSoundEventManager mgr; mgr.id = 1;
    CPhysicalPlayer player; player.id = 2;
    CSoundEvent ev;
    ev.m_state = 3;
    ev.m_pChannel = &gChannel;
    ev.AssignPhysicalPlayer(&mgr, &player);
    bool ok = true;
    ok = ok && gExpected == 3;
    ok = ok && gCheckStep == 1 && gStopStep == 2 && gPlayStep == 3;
    ok = ok && gStopManager == &mgr && gPlayManager == &mgr;
    ok = ok && gPlayChannel == &gChannel && gPlayPlayer == &player;
    ok = ok && ev.m_state == 6 && ev.m_pChannel == 0;
    if (ok) { printf("00c21750_TEST PASS\n"); return 0; }
    printf("00c21750_TEST FAIL\n");
    return 1;
}