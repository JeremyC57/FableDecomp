// CAIStateGroup_GoToPub::Drink() -- drink from the held tankard: with no live tankard fail; an empty tankard (with no owner on it and the shout timer due) has the creature shout for beer (speech then the shout animation; idle while the speech plays), otherwise idle; a full one is drunk from (2..10 seconds of sipping, idling until the time is up or the table component says stop), lowering the tankard by 0.1 and raising drunkenness by 0.1 per second, then the tankard is put down; a failed drink drops the drinking mode
// Grade: DIFFER(1185v1206) nd=63 (was 1194v1206 nd=113) -- lever: the shout tail returned directly as !(result == 1 || PerformActionAndWait(...) != 2) (frame slots now line up with retail through the speech loop); tried: ?true:false on the flag (113), a function-scope failed flag (113); remaining: retail still stores the shout flag byte 0/1 and re-tests it (test al,al ; sete al: a named bool in a frame byte), the action temporary 8 bytes higher in ours, eax/edx picks
// Retail: Fable.exe (TLC, VC7.1) @ 0x00948880, 1206 bytes, plain ret.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / ~ 0x00A01C10, interface LowerBound 0x0040F020 (types 0xba drinkable,
//   0x0f talk, 0xaa pub owner, 0x31 mode manager, 0xcf table seat, 0xc3 drunk), GetPWorld 0x008FCFC0, GFGetCurrentFrame
//   0x0049D870, CTCTalk::PlayAISpeech 0x006E3B60, AIAction_DoNothing 0x0097E200, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   CCreatureAction_PlayAnimation 0x008424F0 / ~CCreatureActionBase 0x00693EF0, AIAction_PerformActionAndWait 0x0097DB70,
//   CCreatureAction_RemoveFoodFromTable 0x00947B70, CCreatureAction_PutDownTankard 0x00947BE0, CTCCreatureModeManager::
//   HasMode 0x0073A8A0 / RemoveMode 0x0073ABA0, GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890.
// Globals: g_FramesPerSecond 0x01375550, the shout interval seconds 0x0138CEC4; 0.1f @ 0x0122EE58 / 0x0126DEAC.
// Layout: no engine header for CAIStateGroup_GoToPub (landed siblings use a local model): table seat IP +0x24,
//   tankard IP +0x34, next shout frame +0xa4.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCDrinkable {
    unsigned char _pad[0x0c];
    float Amount;                               // +0x0c
    unsigned char _pad10[0x34 - 0x10];
    CBaseIntelligentPointer POwner;             // +0x34
};
struct CTCTalk {
    long PlayAISpeech(const CCharString& name, bool force, long layer, bool b1, bool b2);   // 0x006E3B60
};
struct CTCPubOwner {
    unsigned char _pad[0x34];
    const CCharString* PShoutText;              // +0x34
};
struct CTCCreatureModeManager {
    bool HasMode(unsigned long mode);           // 0x0073A8A0
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CTCTableSeat {
    unsigned char _pad[0x20];
    bool StopDrinking;                          // +0x20
};
struct CTCDrunk {
    unsigned char _pad[0x10];
    float Drunkness;                            // +0x10
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCreatureActionBase {
    void* __vftable;
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThing* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);   // 0x008424F0
};
struct CCreatureAction_RemoveFoodFromTable : CCreatureActionBase {
    CBaseIntelligentPointer PFood;              // +0xa8
    CCreatureAction_RemoveFoodFromTable(CThing* creature, CThing* food);   // 0x00947B70
};
struct CCreatureAction_PutDownTankard : CCreatureActionBase {
    CCreatureAction_PutDownTankard(CThing* creature);   // 0x00947BE0
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
long GFGetCurrentFrame();                       // 0x0049D870
extern long g_FramesPerSecond;                  // 0x01375550
extern long g_ShoutForBeerSeconds;              // 0x0138CEC4

struct CAIStateGroup_GoToPub {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x24 - 0x08];
    CBaseIntelligentPointer PTableSeat;         // +0x24
    unsigned char _pad2c[0x34 - 0x2c];
    CBaseIntelligentPointer PTankard;           // +0x34
    unsigned char _pad3c[0xa4 - 0x3c];
    long NextShoutFrame;                        // +0xa4
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    __forceinline unsigned long Random()
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        return *seed = GFROR13(*seed);
    }
    bool Drink();
};

bool CAIStateGroup_GoToPub::Drink()
{
    if (!IS_THING_ALIVE(PTankard))
        return false;
    CThing* tankard = PTankard.GetPItem();
    CTCDrinkable* drinkable;
    if (tankard->HasTC(0xba))
        drinkable = (CTCDrinkable*)tankard->GetTC(0xba);
    if (drinkable->Amount == 0.0f) {
        if (drinkable->POwner.GetPItem() || NextShoutFrame > GetPWorld()->GetWorldFrame())
            return AIAction_DoNothing() != 1;
        long fps = g_FramesPerSecond;
        NextShoutFrame = GetPWorld()->GetWorldFrame() + g_ShoutForBeerSeconds * fps;
        CThing* creature = GetCreature();
        CTCTalk* talk;
        if (creature->HasTC(0xf))
            talk = (CTCTalk*)creature->GetTC(0xf);
        CTCPubOwner* owner;
        if (creature->HasTC(0xaa))
            owner = (CTCPubOwner*)creature->GetTC(0xaa);
        long result;
        for (;;) {
            result = talk->PlayAISpeech(*owner->PShoutText, false, 2, false, true);
            if (result != 2)
                break;
            if (AIAction_DoNothing() == 4)
                continue;
            return false;
        }
        return !(result == 1 || AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(),
            "SHOUT_FOR_BEER", CSoundPair(), false, true, false, 0, 0), false) != 2);
    }
    bool failed = AIAction_PerformActionAndWait(&CCreatureAction_RemoveFoodFromTable(GetCreature(), PTankard.GetPItem()), false) != 2;
    if (failed) {
        CThing* me = GetCreature();
        CTCCreatureModeManager* modes;
        if (me->HasTC(0x31))
            modes = (CTCCreatureModeManager*)me->GetTC(0x31);
        if (modes->HasMode(0x27))
            modes->RemoveMode(0x27);
        return false;
    }
    long seconds = Random() % 9 + 2;
    long fps = g_FramesPerSecond;
    long endFrame = GFGetCurrentFrame() + fps * seconds;
    CTCTableSeat* seat = 0;
    CThing* table = PTableSeat.GetPItem();
    if (table->HasTC(0xcf))
        seat = (CTCTableSeat*)table->GetTC(0xcf);
    do {
        if (AIAction_DoNothing() != 4) {
            CThing* me = GetCreature();
            CTCCreatureModeManager* modes;
            if (me->HasTC(0x31))
                modes = (CTCCreatureModeManager*)me->GetTC(0x31);
            if (modes->HasMode(0x27))
                modes->RemoveMode(0x27);
            return false;
        }
    } while (endFrame > GFGetCurrentFrame() && (!seat || !seat->StopDrinking));
    float drunk = (float)seconds;
    drinkable->Amount = drinkable->Amount - drunk * 0.1f;
    CThing* drinker = GetCreature();
    CTCDrunk* drunkness;
    if (drinker->HasTC(0xc3))
        drunkness = (CTCDrunk*)drinker->GetTC(0xc3);
    drunkness->Drunkness += drunk * 0.1f;
    if (drinkable->Amount < 0.0f)
        drinkable->Amount = 0.0f;
    failed = AIAction_PerformActionAndWait(&CCreatureAction_PutDownTankard(GetCreature()), false) != 2;
    if (failed) {
        CThing* me = GetCreature();
        CTCCreatureModeManager* modes;
        if (me->HasTC(0x31))
            modes = (CTCCreatureModeManager*)me->GetTC(0x31);
        if (modes->HasMode(0x27))
            modes->RemoveMode(0x27);
        return false;
    }
    return true;
}
