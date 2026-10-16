// CV_SwordInTheStoneScript::CV_SwordInTheStoneScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, data/interface pair, own vtable, two empty CScriptThing members (CrowdMemberPullingCurrent/Null) and the CrowdSwordTimer registered with the global script interface
// Leaf: ??0CV_SwordInTheStoneScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED4AA0, length 101 bytes (padded to 112), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CBase::CBase @ 0x0099A2D0 (each CScriptThing's base, x2),
// g_pGameScriptInterface (0x0143E8F8) slot 87 (+0x15c, RegisterTimer) for the CTimer member.
// Vtable 0x012F301C. Layout (retail = PDB - 0x14): SwordRemovalAttemptCount +0x48, the bools +0x4c..+0x53,
// HeroStrength* +0x54..+0x5c, StatsInitialised +0x60, AttackedByHero +0x61, CrowdMemberPullingCurrent +0x64,
// CrowdMemberPullingNull +0x70 (CScriptThing, vtable 0x01238C8C, 12 bytes), CrowdSwordTimer +0x7c.
// Shape (landed 0x00E1A800 model): argument words before the vtable store from the inline `__declspec(novtable)`
// intermediate base; the CScriptThing pimpl words are zeroed from one `xor ebx,ebx` by an embedded member object.
#define GSI_V10(p) virtual void v##p##0(); virtual void v##p##1(); virtual void v##p##2(); virtual void v##p##3(); virtual void v##p##4(); virtual void v##p##5(); virtual void v##p##6(); virtual void v##p##7(); virtual void v##p##8(); virtual void v##p##9();
struct CGameScriptInterface {                      // retail vtable @ 0x01260F0C
    GSI_V10(0) GSI_V10(1) GSI_V10(2) GSI_V10(3) GSI_V10(4) GSI_V10(5) GSI_V10(6) GSI_V10(7)
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();                  // slot 87 (+0x15c)
};
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
struct CTimer {
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
struct CScriptBase {                               // 0x40 bytes (vtable 0x012C1648)
    virtual void v00();
    unsigned char _body[0x3c];
    CScriptBase();                                 // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
struct CBaseClass {                                // vtable 0x01231710
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
};
struct CScriptThing : CBaseClass {                 // vtable 0x01238C8C, 12 bytes
    CCountedPointerWords PImp;
    CScriptThing() : CBaseClass(), PImp() { }
    virtual void v00();
};
struct CV_SwordInTheStoneScript : CQuestScriptObject {   // vtable 0x012F301C
    long         SwordRemovalAttemptCount;         // +0x48
    bool         SwordCutscene;                    // +0x4c
    bool         CrowdAttemptingToPullSword;       // +0x4d
    bool         CrowdSwordPullActive;             // +0x4e
    bool         HeroHasBeenToldSwordLegend;       // +0x4f
    bool         HeroCongratulated;                // +0x50
    bool         CrowdDisperse;                    // +0x51
    bool         HeroHasTakenSword;                // +0x52
    bool         SwordInTheStoneIntroComplete;     // +0x53
    long         HeroStrengthPhysique;             // +0x54
    long         HeroStrengthToughness;            // +0x58
    long         HeroStrengthHealth;               // +0x5c
    bool         StatsInitialised;                 // +0x60
    bool         AttackedByHero;                   // +0x61
    CScriptThing CrowdMemberPullingCurrent;        // +0x64
    CScriptThing CrowdMemberPullingNull;           // +0x70
    CTimer       CrowdSwordTimer;                  // +0x7c
    CV_SwordInTheStoneScript(void* data, CGameScriptInterface* game);
};
CV_SwordInTheStoneScript::CV_SwordInTheStoneScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}
