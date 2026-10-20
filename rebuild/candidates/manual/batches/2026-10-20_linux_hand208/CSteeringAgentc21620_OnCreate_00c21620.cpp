// CSteeringAgent_c21620::OnCreate(CSteeringWorld_c21620*) -- AI-steering library object joining a world: state resets, optional attach, registration with the world's three listener sets and its object list
// Leaf: ?OnCreate@CSteeringAgent_c21620
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21620, 167 bytes (+ int3 padding), `ret 4`. Label correction: the
// catalog files this row as "CTCInventory::OnCreate" (propagated); it is library code in the 0x00C1xxxx-0x00C2xxxx AI
// steering module (eleven callers there, e.g. 0x00C112AC), so the classes are named descriptively (unproven).
// Callees: the agent's state update(long) @ 0x00C20720 (a jump-table switch on the state word +0xAC) and its two
// state queries @ 0x00C207C0 / 0x00C207A0, Attach(world) @ 0x00C21560, the registry's Register(world, agent)
// @ 0x00C29EE0 (on +0x30), the world listener sub-objects' virtual slot 8 (+0x20) at +0x1F4 (only when the agent's
// +0xB0 flag is set), +0x1C4 and +0x104, the +0x18 member's Reset() @ 0x00C25CB0, the world list's Add(agent)
// @ 0x00C21F30 (on +0x2C).
// Shape: `this` stays in ecx across the state helpers because their bodies (here stand-ins, compiled above with
// auto_inline off) are known not to clobber it; the queries return int (`test eax,eax`) -- only `mov esi,ecx` is kept for the later calls; the three member
// resets sit between the last call's push and the call.
struct CSteeringWorld_c21620;
struct CSteeringAgent_c21620;

struct CSteeringListener_c21620 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Add(CSteeringAgent_c21620* agent); // slot 8 (+0x20)
};
struct CSteeringAgentList_c21620 {
    void Add(CSteeringAgent_c21620* agent);          // 0x00C21F30
};
struct CSteeringWorld_c21620 {
    unsigned char             _pad_0x000[0x2C];
    CSteeringAgentList_c21620 Agents;                // +0x2C
    unsigned char             _pad_0x030[0xD4];
    CSteeringListener_c21620  ListenerA;             // +0x104
    unsigned char             _pad_0x108[0xBC];
    CSteeringListener_c21620  ListenerB;             // +0x1C4
    unsigned char             _pad_0x1c8[0x2C];
    CSteeringListener_c21620  ListenerC;             // +0x1F4
};
struct CSteeringRegistry_c21620 {
    void Register(CSteeringWorld_c21620* world, CSteeringAgent_c21620* agent);   // 0x00C29EE0
};
struct CSteeringState_c21620 {
    void Reset();                                    // 0x00C25CB0
};

struct CSteeringAgent_c21620 {
    void*                     __vftable;             // +0x00
    unsigned char             _pad_0x04[0x08];
    long                      Index;                 // +0x0C
    unsigned char             _pad_0x10[0x08];
    CSteeringState_c21620     State;                 // +0x18
    unsigned char             _pad_0x19[0x0F];
    long                      Pending;               // +0x28
    unsigned char             _pad_0x2c[0x04];
    CSteeringRegistry_c21620* PRegistry;             // +0x30
    unsigned char             _pad_0x34[0x78];
    unsigned short            StateWord;             // +0xAC
    unsigned char             _pad_0xae[0x02];
    bool                      ListensToC;            // +0xB0
    long                      Event;                 // +0xB4 (stand-in only)

    void UpdateState_00c20720(long event);           // 0x00C20720
    long IsActive_00c207c0();                        // 0x00C207C0 (int result)
    long IsFinished_00c207a0();                      // 0x00C207A0 (int result)
    void Attach_00c21560(CSteeringWorld_c21620* world);   // 0x00C21560
    void OnCreate(CSteeringWorld_c21620* world);
};

// Stand-in bodies of the three state helpers (same TU in retail, compiled before OnCreate): they keep `this` in ecx,
// which is what lets the caller skip reloading it. Only OnCreate is graded from this file.
#pragma auto_inline(off)
void CSteeringAgent_c21620::UpdateState_00c20720(long event)
{
    switch (StateWord) {
    case 0: Event = event; break;
    case 1: Event = 1; break;
    case 2: Event = 2; break;
    case 3: Event = 3; break;
    case 4: Event = 4; break;
    case 5: Event = 5; break;
    case 6: Event = 6; break;
    }
}
long CSteeringAgent_c21620::IsActive_00c207c0()
{
    UpdateState_00c20720(-1);
    long state = StateWord;
    if (state >= 2 && (state <= 4 || state == 6))
        return 1;
    return 0;
}
long CSteeringAgent_c21620::IsFinished_00c207a0()
{
    UpdateState_00c20720(-1);
    return StateWord == 5;
}
#pragma auto_inline(on)

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
