// CTCTavernGame::OnSerialise(CPersistContext& context, bool) -- persists TargetTime (default 0), TargetRound (default 0) and HasPrizeBeenGiven (default false)
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA8E0, length 108 bytes.
// Label: the catalog says CQR_EscortTrader_ManagerScript::OnPersist (propagated); the body is slot 1 (the TC persist
// hook, as the landed CTCTrophy::OnSerialise 0x006DDBF0) of the CTCTavernGame vtable 0x012560BC (rtti-vtables.tsv) and
// of four derived tavern-game vtables (0x0126F224/2F4/49C/814); the members it persists fit CTCTavernGame, not the
// 0x6c-byte quest manager script.
// Callees: CPersistContext::Transfer<float> @ 0x00410620, Transfer<long> @ 0x00410BE0, Transfer<bool> @ 0x004045C0
// (each `(name, value&, const default&)`). Keys 0x01256030 / 0x01256024 / 0x01256010 are string literals.
// Layout: Ego_r.pdb CTCTavernGame with retail 12-byte vectors (six vectors before these members: -0x1c):
// TargetTime +0x98, TargetRound +0x9c, HasPrizeBeenGiven +0xe9.
// Shape: the float default is a named local in the `push ecx` frame word; the long and bool defaults are literals
// bound to the const references, homed in the dead context-argument slot; the last member address is formed by
// advancing `this` (`add esi,0E9h`). The bool parameter is unused.
struct CPersistContext {
    void Transfer(const char* name, float& value, const float& defaultValue);   // 0x00410620
    void Transfer(const char* name, long& value, const long& defaultValue);     // 0x00410BE0
    void Transfer(const char* name, bool& value, const bool& defaultValue);     // 0x004045C0
};

struct CTCTavernGame {
    virtual void v00();
    virtual void OnSerialise(CPersistContext& context, bool flag);
    unsigned char _pad04[0x98 - 0x04];      // +0x04 CTCBase, UIComponents, LastRenderTime, game state, handle vectors, fonts
    float         TargetTime;               // +0x98
    long          TargetRound;              // +0x9c
    unsigned char _padA0[0xe9 - 0xa0];      // +0xa0 NewBestScore .. GamePlayed, bet timing, Mult, BetIncreasing
    bool          HasPrizeBeenGiven;        // +0xe9
};

void CTCTavernGame::OnSerialise(CPersistContext& context, bool flag)
{
    float defaultTime = 0.0f;
    context.Transfer("TargetTime", TargetTime, defaultTime);
    context.Transfer("TargetRound", TargetRound, 0);
    context.Transfer("HasPrizeBeenGiven", HasPrizeBeenGiven, false);
}