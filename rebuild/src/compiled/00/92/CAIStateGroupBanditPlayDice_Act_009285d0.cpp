// CAIStateGroup_BanditPlayDice::Act() -- play the watching-into animation, then until a random 375..674 frames pass either take a turn at the dice game (pick, throw, maybe win/lose, release the turn) or keep playing, then play the watching-out-of animation
// Retail: Fable.exe (TLC, VC7.1) @ 0x009285D0, 992 bytes (aligned frame, no EH frame).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCreatureAction_PlayAnimation ctor 0x008424F0,
//   AIAction_PerformActionAndWait 0x0097DB70, ~CCreatureActionBase 0x00693EF0, GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890,
//   GetWorldFrame 0x0049D870 (static), GFFloatRandom 0x004978A0 (__fastcall), rand 0x00BFEB16, CBaseIntelligentPointer
//   ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout: engine/CAIStateGroup_BanditPlayDice.h DiceGame +0x18; CTCDiceGame NumberOfPlayers +0xc, CurrentPlayer +0x10
//   (CIntelligentPointer<CThing>, Ego_r.pdb); state group PBrain +4, brain creature +0x18. The generated
//   engine/CAIStateGroup_BanditPlayDice.h is a flat packed struct with no base-class methods, so the members are restated here.
// Shape: the dice-game pointer is named after the creature (CThing* player; CTCDiceGame* game = DiceGame) for both
//   CurrentPlayer assignments, so it is loaded into ebp before the CIntelligentPointer<CThing> temporary is built; without
//   the named pointers VC reloads DiceGame after the temporary and swaps the zero/result registers.
struct CThing { };
unsigned long __fastcall GFROR13(unsigned long value);    // 0x00497890
float __fastcall GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0
extern "C" int __cdecl rand();                           // 0x00BFEB16
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CTCDiceGame {
    unsigned char _pad00[0xc];
    long NumberOfPlayers;                       // +0x0c
    CIntelligentPointerThing CurrentPlayer;     // +0x10
};
struct CThingCreatureBase : CThing { };
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _members[0xb4 - 0xa8];
    CCreatureAction_PlayAnimation(CThingCreatureBase* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool stayOnLastFrame, bool b, bool c, long d, long e);   // 0x008424F0
};
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
    unsigned long Random(unsigned long range) const
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        *seed = GFROR13(*seed);
        return *seed % range;
    }
    EAIActionReturn AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
};
struct CAIStateGroup_BanditPlayDice : CAIStateGroupBase {
    void* DiceGamePosition;                     // +0x14
    CTCDiceGame* DiceGame;                      // +0x18
    long FrameToInitiate;                       // +0x1c
    long FrameToJoin;                           // +0x20
    bool WasCarryingWeapon;                     // +0x24
    EAIActionReturn Act();
};

EAIActionReturn CAIStateGroup_BanditPlayDice::Act()
{
    EAIActionReturn result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(),
        "ST_DICEGAME_WATCHING_INTO", CSoundPair(), false, true, false, 0, 0), false);
    if (result != AI_ACTION_DONE)
        return result;
    unsigned long wait = Random(300);
    long endFrame = CWorld::GetWorldFrame() + wait + 375;
    while (CWorld::GetWorldFrame() < endFrame) {
        if (DiceGame->NumberOfPlayers >= 2 && !DiceGame->CurrentPlayer.GetPItem()
            && GFFloatRandom(1.0f, GetPWorldSeed()) < 0.2f) {
            CThing* player = GetCreature();
            CTCDiceGame* game = DiceGame;
            game->CurrentPlayer = CIntelligentPointerThing(player);
            AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_DICEGAME_PICKING_DICE",
                CSoundPair(), false, true, false, 0, 0), false);
            result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(),
                "ST_DICEGAME_PLAYING_THROW", CSoundPair(), false, true, false, 0, 0), false);
            switch (rand() % 5) {
            case 0:
                result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(),
                    "ST_DICEGAME_PLAYING_WIN", CSoundPair(), false, true, false, 0, 0), false);
                break;
            case 1:
                result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(),
                    "ST_DICEGAME_PLAYING_LOSE", CSoundPair(), false, true, false, 0, 0), false);
                break;
            }
            CTCDiceGame* table = DiceGame;
            table->CurrentPlayer = CIntelligentPointerThing(0);
        } else
            result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_DICEGAME_PLAYING",
                CSoundPair(), false, true, false, 0, 0), false);
        if (result != AI_ACTION_DONE)
            return result;
    }
    return AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_DICEGAME_WATCHING_OUTOF",
        CSoundPair(), false, true, false, 0, 0), false);
}