// CAIStateGroupBase::AIAction_MinionIdle() -- track the nearest interesting thing, then 80% of the time play an idle (a yawn 9% of the time within 100 time units of the creature's two sleep times), else report done
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097FA00, length 468 bytes.
// Callees: IsStopping @ 0x008FD010, CVectorMap::LowerBound @ 0x0040F020 (0x43 CTCLook), CTCLook::TrackNearestInterestingThing
//          @ 0x00753840, GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, CThingAICreature::GetSleepTimes @ 0x00833390,
//          GetPWorld @ 0x008FCFA0 (manifest "CPlayer::GetCurrentMode" is propagated), the world's time object (+0x1c)
//          IsTimeOfDayInRange @ 0x006BB390 (thiscall; manifest "_global::Game_IsTimeOfDayInRange"), CCharString @ 0x0099EBF0 /
//          ~ @ 0x0099EAE0, CThingCreatureBase::GetRandomLoadedAnimFromName @ 0x00662FA0, CCreatureAction_PlayAnimation(creature,
//          CAnimationEntry*, ...) @ 0x00842570, AIAction_PerformAction @ 0x0097D8B0, ~CCreatureActionBase @ 0x00693EF0.
// Shape: `if (rand < 80) { ... return Perform(...); } return 2;` (the done return sits at the end); the time object is
//          reached through an inline GetPTime() accessor, which makes VC evaluate GetPWorld() before the pushed bounds
//          (with `->PTime->` the clamp computations sink into the argument evaluation); the two animation names are
//          implicit CCharString conversion temporaries whose calls tail-merge.
// Layout: the base follows engine/CAIStateGroupBase.h (PBrain +4), modelled locally so the member functions can be declared;
//          CWorld +0x1c time object (descriptive).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CAnimationEntry;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingAICreature : CThing {
    CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool b);   // 0x00662FA0
    void GetSleepTimes(long* wakeTime, long* sleepTime);    // 0x00833390
};
struct CTCLook {                                // interface 0x43
    void TrackNearestInterestingThing(bool b);  // 0x00753840
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingAICreature* creature, CAnimationEntry* animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x00842570
};
struct CGameTime {
    bool IsTimeOfDayInRange(long start, long end);  // 0x006BB390
};
struct CWorld {
    unsigned char _pad[0x1c];
    CGameTime* PTime;                           // +0x1c
    CGameTime* GetPTime() const { return PTime; }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_RUNNING = 4 };
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    bool IsStopping();                          // 0x008FD010
    CWorld* GetPWorld();                        // 0x008FCFA0
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
    EAIActionReturn AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    EAIActionReturn AIAction_MinionIdle();
};

EAIActionReturn CAIStateGroupBase::AIAction_MinionIdle()
{
    if (IsStopping())
        return AI_ACTION_STOPPING;
    CTCLook* look;
    if (GetCreature()->GetInterface(&look, 0x43))
        look->TrackNearestInterestingThing(true);
    if (WorldRandomRange(*GetPWorldSeed(), 100) < 80) {
        long wakeTime, sleepTime;
        GetCreature()->GetSleepTimes(&wakeTime, &sleepTime);
        long beforeWake = wakeTime - 100;
        if (beforeWake < 0)
            beforeWake = 2330;
        long afterSleep = sleepTime + 100;
        if (afterSleep > 2400)
            afterSleep = 30;
        CAnimationEntry* animation;
        if ((GetPWorld()->GetPTime()->IsTimeOfDayInRange(sleepTime, afterSleep) || GetPWorld()->GetPTime()->IsTimeOfDayInRange(beforeWake, wakeTime))
            && WorldRandomRange(*GetPWorldSeed(), 100) < 9)
            animation = GetCreature()->GetRandomLoadedAnimFromName("STANDARD_YAWN", true);
        else
            animation = GetCreature()->GetRandomLoadedAnimFromName("ST_IDLE", true);
        return AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(), false, true, false, 0, 0));
    }
    return AI_ACTION_DONE;
}
