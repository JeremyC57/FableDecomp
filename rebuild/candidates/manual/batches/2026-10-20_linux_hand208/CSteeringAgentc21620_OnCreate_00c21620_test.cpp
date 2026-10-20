// Behaviour test for the steering-library OnCreate (0x00C21620): attaches when the state query says active or
// finished, registers with the registry (if any) and the listener sets (C only when flagged), resets the state
// member and fields, and adds the agent to the world list -- in that order.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
struct CSteeringAgent_c21620;
struct CSteeringListener_c21620 { const char* Name; virtual void Add(CSteeringAgent_c21620*) { Log(Name); } };
struct CSteeringAgentList_c21620 { void Add(CSteeringAgent_c21620*) { Log("list"); } };
struct CSteeringWorld_c21620 { CSteeringAgentList_c21620 Agents; CSteeringListener_c21620 ListenerA, ListenerB, ListenerC; };
struct CSteeringRegistry_c21620 { void Register(CSteeringWorld_c21620*, CSteeringAgent_c21620*) { Log("registry"); } };
struct CSteeringState_c21620 { void Reset() { Log("reset"); } };
struct CSteeringAgent_c21620 {
    long Index; CSteeringState_c21620 State; long Pending; CSteeringRegistry_c21620* PRegistry; unsigned short StateWord; bool ListensToC; long Event;
    void UpdateState_00c20720(long event)
    {
        switch (StateWord) {
        case 0: Event = event; break; case 1: Event = 1; break; case 2: Event = 2; break; case 3: Event = 3; break;
        case 4: Event = 4; break; case 5: Event = 5; break; case 6: Event = 6; break;
        }
    }
    long IsActive_00c207c0() { UpdateState_00c20720(-1); long state = StateWord; if (state >= 2 && (state <= 4 || state == 6)) return 1; return 0; }
    long IsFinished_00c207a0() { UpdateState_00c20720(-1); return StateWord == 5; }
    void Attach_00c21560(CSteeringWorld_c21620*) { Log("attach"); }
    void OnCreate(CSteeringWorld_c21620* world);
};
void CSteeringAgent_c21620::OnCreate(CSteeringWorld_c21620* world)
{
    UpdateState_00c20720(-1);
    if (IsActive_00c207c0() || IsFinished_00c207a0())
        Attach_00c21560(world);
    UpdateState_00c20720(1);
    if (PRegistry)
        PRegistry->Register(world, this);
    if (ListensToC)
        world->ListenerC.Add(this);
    world->ListenerB.Add(this);
    world->ListenerA.Add(this);
    State.Reset();
    Pending = 0;
    StateWord = 0;
    Index = -1;
    world->Agents.Add(this);
}
int main()
{
    CSteeringWorld_c21620 world; world.ListenerA.Name = "A"; world.ListenerB.Name = "B"; world.ListenerC.Name = "C";
    CSteeringRegistry_c21620 registry;
    CSteeringAgent_c21620 agent; agent.Index = 3; agent.Pending = 7; agent.PRegistry = 0; agent.StateWord = 1; agent.ListensToC = false;
    g_log[0] = 0; agent.OnCreate(&world);
    if (std::strcmp(g_log, "B,A,reset,list,") || agent.Index != -1 || agent.Pending != 0 || agent.StateWord != 0) { std::printf("00c21620_TEST FAIL\n"); return 1; }
    agent.StateWord = 5; agent.PRegistry = &registry; agent.ListensToC = true;
    g_log[0] = 0; agent.OnCreate(&world);
    if (std::strcmp(g_log, "attach,registry,C,B,A,reset,list,")) { std::printf("00c21620_TEST FAIL\n"); return 1; }
    agent.StateWord = 3; agent.PRegistry = 0; agent.ListensToC = false;
    g_log[0] = 0; agent.OnCreate(&world);
    if (std::strcmp(g_log, "attach,B,A,reset,list,")) { std::printf("00c21620_TEST FAIL\n"); return 1; }
    std::printf("00c21620_TEST PASS\n");
    return 0;
}
