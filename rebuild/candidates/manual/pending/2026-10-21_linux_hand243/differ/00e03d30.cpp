// CDarkwoodRockTroll::Main() -- the Darkwood rock troll quest script: register an alive-filter over the script's own thing, wait while the troll's health is above zero (one game update per frame, bailing out when the script is being terminated), then post the "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03" objective text for BarrowFields and clear the master-data byte at +0x55
// Grade: DIFFER(335v335) -- same instructions, different order: retail fetches the outer AddQuestObjective vtable (edi) first and the inner GetQuestText vtable after two argument pushes, ours fetches both before the pushes and uses edx for every `lea`; everything else matches | tried: implicit string temporaries, named strings declared last-argument first, `!(health <= 0)` compare
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E03D30, length 335 bytes (vtable slot 1 = Main per the entity-script contract).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (the filter's member at +4; filter vtable 0x012C32F0),
// the filter registration @ 0x00F35B10 (on `this`; manifest label "CWorldMap::AllocateMapThingUID" is propagated), CScriptThing::
// ~CScriptThing @ 0x004AA840, IsDone @ 0x00F35B30 (manifest "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated),
// CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0, and the game interface's virtual slots 7 (+0x1c Update), 70 (+0x118, returns a
// long), 451 (+0x70c, (const CScriptThing&, long)), 264 (+0x420, float health of a script thing), 655 (+0xa3c, CCharString by hidden
// pointer from three strings) and 296 (+0x4a0, one const CCharString&).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14 (its byte +0x55 is cleared), PMasterData +0x18.
// Shape: the landed CHTE_Prisoner 0x00D69F50 model: the filter is a scoped 16-byte frame object, &Me stays in edi; the wait is a
// `while (!(health <= 0)) { Update(); if (IsDone()) return; }` (`test ah,41h ; jnp` / `jp`); the three strings are named locals
// declared last-argument first (so `lea ; push` addresses them), the returned CCharString lives in its own frame word and is destroyed first.
struct CScriptThing {                          // 12 bytes
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
    ~CScriptThing();                          // 0x004AA840
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                           // 0x0099EAE0
};
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
#define VF100(p) VF10(p##0) VF10(p##1) VF10(p##2) VF10(p##3) VF10(p##4) VF10(p##5) VF10(p##6) VF10(p##7) VF10(p##8) VF10(p##9)
struct CGameScriptInterface {
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6)
    virtual void Update();                                             // slot 7 (+0x1c)
    VF(8) VF(9) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual long GetScriptFrame();                                     // slot 70 (+0x118)
    VF(71) VF(72) VF(73) VF(74) VF(75) VF(76) VF(77) VF(78) VF(79)
    VF10(8) VF10(9) VF100(1) VF10(20) VF10(21) VF10(22) VF10(23) VF10(24) VF10(25)
    VF(260) VF(261) VF(262) VF(263)
    virtual float GetHealth(const CScriptThing& thing);                // slot 264 (+0x420)
    VF(265) VF(266) VF(267) VF(268) VF(269) VF10(27) VF10(28) VF(290) VF(291) VF(292) VF(293) VF(294) VF(295)
    virtual void AddQuestObjective(const CCharString& text);           // slot 296 (+0x4a0)
    VF(297) VF(298) VF(299) VF100(3) VF10(40) VF10(41) VF10(42) VF10(43) VF10(44)
    VF(450)
    virtual void SetThingFrameValue(const CScriptThing& thing, long value);   // slot 451 (+0x70c)
    VF(452) VF(453) VF(454) VF(455) VF(456) VF(457) VF(458) VF(459)
    VF10(46) VF10(47) VF10(48) VF10(49) VF100(5) VF10(60) VF10(61) VF10(62) VF10(63) VF10(64)
    VF(650) VF(651) VF(652) VF(653) VF(654)
    virtual CCharString GetQuestText(const CCharString& textId, const CCharString& region, const CCharString& region2);   // slot 655 (+0xa3c)
};
struct CIsThingAlive {                         // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                     // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
struct CQuestScriptData {
    unsigned char _pad[0x55];
    bool TrollAlive;                          // +0x55 (name unverified)
};
class CDarkwoodRockTroll {
public:
    void*                 __vftable;          // +0x00
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    CQuestScriptData*     ParentClass;        // +0x14
    void*                 PMasterData;        // +0x18
    void RegisterFilter(CIsThingAlive& filter);   // 0x00F35B10
    bool IsDone();                            // 0x00F35B30
    void Main();
};

void CDarkwoodRockTroll::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->Update();
    if (IsDone())
        return;
    Game->SetThingFrameValue(Me, Game->GetScriptFrame());
    while (!(Game->GetHealth(Me) <= 0.0f)) {
        Game->Update();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    {
        CCharString region("BarrowFields");
        CCharString region2("BarrowFields");
        CCharString textId("TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03");
        Game->AddQuestObjective(Game->GetQuestText(textId, region2, region));
    }
    ParentClass->TrollAlive = false;
}
