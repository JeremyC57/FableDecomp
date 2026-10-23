// CTCHeroStats::ProcessDrunkenness() -- sobers the hero over game time, drains the beer reservoir into drunkenness, schedules and runs vomiting
// Leaf: ProcessDrunkenness
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057CB7E, 567 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`, as the landed SetAge 0x0057B05F).
// Callees: CTCBase::GetWorld() @ 0x00686A70 / GetWorld() const @ 0x00686A80, CGameScriptInterface::IsInMovieSequence()
//          (slot 395, +0x62C, through CWorld::PGameScriptInterface +0x38), CTCBase::GetDefinitions() @ 0x006869D0,
//          CTCHeroStats::GetMaxDrunkenness() @ 0x0057A89D, CTCBase::GetPRandomSeed() @ 0x00686DA0,
//          GFRandom(range, seed) @ 0x004CDAE0 (__fastcall), GFGetCurrentFrame() @ 0x0049D870, CTCHeroStats::Vomit()
//          @ 0x0057B783, CTCBase::GetPlayerManager() @ 0x00686D50, CPlayerManager::GetMainPlayer() @ 0x00449960,
//          CPlayer::GetPPlayerCharacter() @ 0x00487DC0, std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound
//          @ 0x0040F020, CTCHeroOpinionDeedLog::RemoveDeedStillSearchingForWitnesses(long) @ 0x006C5920.
// Globals: g_DrunkennessSoberRate @ 0x0137CBD8 (a 1.0f data float), g_framesPerSecond @ 0x01375550, 24.0f @ 0x0123822C,
//          0.0f @ 0x0122DEDC.
// Shape: the frames-per-second global is read into a named local ahead of the vomit-frame call chain; the three
// definition reads each call GetDefinitions(); interface 0x51 is the hero's opinion deed log (the handle is pushed
// after it was reset to -1, as retail does). The script interface comes through an inline CWorld getter (`mov ecx,[eax+38h]`),
// and the const world through an inline `GetConstWorld()` (a `const` self pointer copies Drunkenness to the frame first).
// Layout: retail CTCHeroStats sits 0x18 below the PDB here (engine/CTCHeroStats.h): Drunkenness +0x19C (PDB +0x1B4),
// BeerReservoir +0x1A0, FrameToVomit +0x1AC, DeedHandle +0x1B0, FrameEndVomit +0x1B4 (PDB +0x1CC).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

#define V10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CGameScriptInterface {
    V10(0) V10(1) V10(2) V10(3) V10(4) V10(5) V10(6) V10(7) V10(8) V10(9)
    V10(10) V10(11) V10(12) V10(13) V10(14) V10(15) V10(16) V10(17) V10(18) V10(19)
    V10(20) V10(21) V10(22) V10(23) V10(24) V10(25) V10(26) V10(27) V10(28) V10(29)
    V10(30) V10(31) V10(32) V10(33) V10(34) V10(35) V10(36) V10(37) V10(38)
    virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394();
    virtual bool IsInMovieSequence() const;          // slot 395 (+0x62C)
};
#undef V10

struct CGameTimeManager {
    unsigned char _pad_0x00[0x10];
    float         TimeStep;                          // +0x10
};

struct CWorld {
    unsigned char         _pad_0x00[0x1C];
    CGameTimeManager*     PGameTimeManager;          // +0x1C scoped_ptr<CGameTimeManager>
    unsigned char         _pad_0x20[0x18];
    CGameScriptInterface* PGameScriptInterface;      // +0x38 scoped_ptr<CGameScriptInterface>
    CGameScriptInterface* GetGameScriptInterface() const { return PGameScriptInterface; }
};

struct CThingPlayerCreature : CThing {};
struct CPlayer {
    CThingPlayerCreature* GetPPlayerCharacter() const;   // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                  // 0x00449960
};

struct CTCHeroOpinionDeedLog {
    void RemoveDeedStillSearchingForWitnesses(long handle);   // 0x006C5920
};

struct CHeroStatsDef {
    unsigned char _pad_0x00[0x94];
    long          MinSecondsToVomit;                 // +0x94
    long          MaxSecondsToVomit;                 // +0x98
    float         VomitSoberAmount;                  // +0x9C
    unsigned char _pad_0xa0[0x8];
    float         BeerReservoirDrainRate;            // +0xA8
};

struct CDefinitionHolder {
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC CDefPointer<const CHeroStatsDef>::Object
};

unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
long __fastcall GFGetCurrentFrame();                 // 0x0049D870
extern float g_DrunkennessSoberRate;                 // 0x0137CBD8
extern long g_framesPerSecond;                       // 0x01375550

struct CTCHeroStats {
    void*         __vftable;                         // +0x000
    CThing*       ParentThing;                       // +0x004 CTCBase
    unsigned char _pad_0x008[0x194];
    float         Drunkenness;                       // +0x19C (PDB +0x1B4)
    float         BeerReservoir;                     // +0x1A0 (PDB +0x1B8)
    float         DrinkAffectMultiplier;             // +0x1A4
    float         DrinkAffectMultiplierMultiplier;   // +0x1A8
    long          FrameToVomit;                      // +0x1AC (PDB +0x1C4)
    long          DeedHandle;                        // +0x1B0 (PDB +0x1C8)
    long          FrameEndVomit;                     // +0x1B4 (PDB +0x1CC)

    CWorld* GetWorld();                              // 0x00686A70
    const CWorld* GetWorld() const;                  // 0x00686A80
    CDefinitionHolder* GetDefinitions() const;       // 0x006869D0
    unsigned long* GetPRandomSeed();                 // 0x00686DA0
    CPlayerManager* GetPlayerManager() const;        // 0x00686D50
    const CWorld* GetConstWorld() const { return GetWorld(); }
    const CHeroStatsDef* GetHeroStatsDef() const { return GetDefinitions()->PHeroStatsDef; }
    float GetMaxDrunkenness();                       // 0x0057A89D
    void Vomit();                                    // 0x0057B783
    void ProcessDrunkenness();
};

void CTCHeroStats::ProcessDrunkenness()
{
    if (GetWorld()->GetGameScriptInterface()->IsInMovieSequence())
        return;
    Drunkenness -= g_DrunkennessSoberRate * GetConstWorld()->PGameTimeManager->TimeStep * 24.0f / g_framesPerSecond;
    if (BeerReservoir > 0.0f) {
        float drain = GetHeroStatsDef()->BeerReservoirDrainRate / g_framesPerSecond;
        if (drain > BeerReservoir) {
            Drunkenness += BeerReservoir;
            BeerReservoir = 0.0f;
        } else {
            Drunkenness += drain;
            BeerReservoir -= drain;
        }
    }
    if (GetMaxDrunkenness() < Drunkenness && FrameToVomit == -1 && DeedHandle == -1) {
        long fps = g_framesPerSecond;
        FrameToVomit = (GetHeroStatsDef()->MinSecondsToVomit +
                        GFRandom(GetHeroStatsDef()->MaxSecondsToVomit - GetHeroStatsDef()->MinSecondsToVomit + 1,
                                 GetPRandomSeed())) * fps + GFGetCurrentFrame();
    }
    if (FrameToVomit != -1 && FrameToVomit < GFGetCurrentFrame())
        Vomit();
    if (FrameEndVomit != 0)
        Drunkenness -= GetHeroStatsDef()->VomitSoberAmount / g_framesPerSecond;
    if (Drunkenness < 0.0f)
        Drunkenness = 0.0f;
    if (DeedHandle != -1 && GFGetCurrentFrame() > FrameEndVomit) {
        DeedHandle = -1;
        FrameEndVomit = 0;
        CThingPlayerCreature* hero = GetPlayerManager()->GetMainPlayer()->GetPPlayerCharacter();
        if (hero) {
            CTCHeroOpinionDeedLog* log;
            if (hero->GetInterface(&log, 0x51))
                log->RemoveDeedStillSearchingForWitnesses(DeedHandle);
        }
    }
}