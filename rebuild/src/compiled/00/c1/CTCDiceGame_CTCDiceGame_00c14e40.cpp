// CTCDiceGame::CTCDiceGame(CThing* parent, const CDiceGameSetup* setup, long seed) -- (labelled Initialize: it is the constructor) builds the component: parent link, two inline-buffered rule holders, the seeded pair, three short quads, then allocates the game state (0xd4 bytes, from the setup's first word) and the 0x64-byte controller
// Leaf: CTCDiceGame
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14E40, 336 bytes (`ret 0Ch`, EH frame).
// Callees: CDiceRuleSet ctor(long) @ 0x00C194F0, CDiceSeedPair ctor(long, long) @ 0x00C2D0C0, CDiceQuad ctor @ 0x00C2C6B0,
// operator new @ 0x00BFEA1A, CDiceGameState ctor(CTCDiceGame*, long) @ 0x00C2A8C0, CDiceController ctor(CTCDiceGame*) @ 0x00C2BD90.
// Vtables: CTCDiceGame 0x012B29F8, the two holders 0x012B29B0 / 0x012B28D8, the rule set 0x012B2F0C, the quad 0x012B4630.
// Layout: retail, local (the Ego_r.pdb CTCDiceGame is 24 bytes, so the CTCDiceGame attribution of this vptr-0x012B29F8 class is
// unverified; names are descriptive): parent +0x04, game state +0x08, controller +0x0c,
// holders +0x10 / +0x40 (vptr, buffer pointer, 0x28-byte rule set), seed pair +0x70, quads +0x78/+0x84/+0x90, +0x9c/+0xa0 zero.
// Shape: the base is an empty polymorphic shell (EH state 0, its vptr store dead) and the parent is the derived class's first
// member: a base that owns the parent hoists the argument load above the `this` spill. Each holder is a polymorphic base taking
// the inline storage's address plus the storage member (EH states 1/3 partial, 2/4 complete); the seed pair has a trivial dtor.
struct CThing;
struct CDiceGameSetup { long Value; };
struct CDiceRuleSet {                                       // 0x28 bytes
    CDiceRuleSet(long n);                                   // 0x00C194F0
    virtual ~CDiceRuleSet();
    unsigned char _pad04[0x24];
};
struct CDiceRuleHolderBase {
    CDiceRuleHolderBase(CDiceRuleSet* rules) : Rules(rules) { }
    virtual ~CDiceRuleHolderBase() { }
    CDiceRuleSet* Rules;
};
struct CDiceRuleHolderA : CDiceRuleHolderBase {
    CDiceRuleHolderA() : CDiceRuleHolderBase(&Storage), Storage(0) { }
    virtual ~CDiceRuleHolderA() { }
    CDiceRuleSet Storage;
};
struct CDiceRuleHolderB : CDiceRuleHolderBase {
    CDiceRuleHolderB() : CDiceRuleHolderBase(&Storage), Storage(0) { }
    virtual ~CDiceRuleHolderB() { }
    CDiceRuleSet Storage;
};
struct CDiceSeedPair {
    CDiceSeedPair(long a, long b);                          // 0x00C2D0C0
    long A, B;
};
struct CDiceQuad {
    CDiceQuad();                                            // 0x00C2C6B0
    virtual ~CDiceQuad();
    short V[4];
};
struct CTCDiceGame;
struct CDiceGameState {
    CDiceGameState(CTCDiceGame* game, long value);          // 0x00C2A8C0
    unsigned char _pad[0xd4];
};
struct CDiceController {
    CDiceController(CTCDiceGame* game);                     // 0x00C2BD90
    unsigned char _pad[0x64];
};
struct CTCBaseShell {
    CTCBaseShell() { }
    virtual ~CTCBaseShell() { }
};
struct CTCDiceGame : CTCBaseShell {
    CThing* ParentThing;
    CTCDiceGame(CThing* parent, const CDiceGameSetup* setup, long seed);
    virtual ~CTCDiceGame();
    CDiceGameState*  State;                                 // +0x08
    CDiceController* Controller;                            // +0x0c
    CDiceRuleHolderA HolderA;                               // +0x10
    CDiceRuleHolderB HolderB;                               // +0x40
    CDiceSeedPair    Seed;                                  // +0x70
    CDiceQuad        QuadA;                                 // +0x78
    CDiceQuad        QuadB;                                 // +0x84
    CDiceQuad        QuadC;                                 // +0x90
    long             Field9C;                               // +0x9c
    long             FieldA0;                               // +0xa0
};

CTCDiceGame::CTCDiceGame(CThing* parent, const CDiceGameSetup* setup, long seed)
    : ParentThing(parent), State(0), Controller(0), Seed(0, seed), Field9C(0), FieldA0(0)
{
    State = new CDiceGameState(this, setup->Value);
    Controller = new CDiceController(this);
}