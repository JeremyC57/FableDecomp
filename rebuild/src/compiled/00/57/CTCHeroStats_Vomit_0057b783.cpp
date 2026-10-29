// CTCHeroStats::Vomit() -- makes the hero throw up: plays the puke into/loop/out-of action for a drunkenness-scaled random time, counts it, posts the deed and empties the beer reservoir
// Leaf: Vomit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057B783, 596 bytes; size TU with the biased `lea ebp,[esp-78h]` frame:
// `#pragma optimize("s", on)` alone (adding `optimize("y", off)` as the smaller CTCHeroStats rows gives `mov ebp,esp`).
// Callees: CTCBase::PeekPlayerManager() @ 0x00686D50, CPlayerManager::GetMainPlayer() @ 0x00449960,
//          CPlayer::GetPControlledCreature() @ 0x00487DC0, CTCBase::GetDefinitionManager() @ 0x006869D0, CTCBase::GetPWorldSeed()
//          @ 0x00686DA0, _ftol @ 0x00BFEA70, GFRandom(range, seed) @ 0x004CDAE0 (__fastcall), CCharString @ 0x0099EBF0 /
//          ~CCharString @ 0x0099EAE0 ("ST_PUKE_INTO" 0x0124F37C, "ST_PUKE_LOOP" 0x0124F38C, "ST_PUKE_OUTOF" 0x0124F39C),
//          CCreatureAction_PlayIntoLoopOutOfAnimation ctor @ 0x008427E0 and its implicit dtor @ 0x0057A545 (emitted in
//          this TU), std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020,
//          CTCHeroOnlineScoreboard::UpdateScoreForActivity @ 0x0055F3B0, CThingCreatureBase::IsFreeToPerformAction
//          @ 0x00662210 / SetCurrentAction @ 0x006644F0, CThing::GetPos() @ 0x004C73D0,
//          CTCHeroOpinionDeedLog::PostDeed @ 0x006C6990, CWorld::GetFrame() @ 0x0049D870.
// Globals: 3.0f @ 0x0122DED4, 1.0f @ 0x0122DED8, 2^32 @ 0x0122DCB4, g_framesPerSecond @ 0x01375550.
// Shape: the loop time is `(Drunkenness - 3) / def` capped at the def's maximum, then a random whole number of seconds
// from the def's minimum (unsigned GFRandom result, hence the 2^32 fix-up); the animation names are implicit
// const char* -> const CCharString& conversions (all built before the first push). The scoreboard (interface 0x10D)
// comes from the unchecked out-parameter GetInterface written as the one-ternary entry pick (component straight into
// ecx; the miss path reads the never-written out word); interface 0x51 is the opinion deed log (if-form pick).
// Layout: retail CTCHeroStats sits 0x18 below the PDB here (engine/CTCHeroStats.h): NumberOfVomits +0xEC (PDB +0xF4),
// Drunkenness +0x19C (PDB +0x1B4), BeerReservoir +0x1A0, FrameToVomit +0x1AC, DeedHandle +0x1B0, FrameEndVomit +0x1B4.
#pragma optimize("s", on)

class CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class C3DVector { public: float X, Y, Z; };

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    const C3DVector& GetPos() const;            // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || wanted < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfacePick(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *out = (T*)((it == map->last || wanted < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xA4];
    ~CCreatureActionBase();                          // 0x00693EF0
};
class CThingCreatureBase;
class CCreatureAction_PlayIntoLoopOutOfAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x14];
    CCharString   LoopAnimName;                      // +0xBC
    CCharString   OutOfAnimName;                     // +0xC0
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* creature, const CCharString& into,
        const CCharString& loop, const CCharString& outOf, float loopTime, bool b, long c, long d);   // 0x008427E0
};

class CThingCreatureBase : public CThing { public:
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
    bool SetCurrentAction(const CCreatureActionBase& action);        // 0x006644F0
};
class CThingPlayerCreature : public CThingCreatureBase { public:};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;                  // 0x00449960
};

class CTCHeroOnlineScoreboard { public:
    void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter);  // 0x0055F3B0
};
class CThingAICreature;
class CFaction;
class CTCHeroOpinionDeedLog { public:
    long PostDeed(long type, const C3DVector& position, const CThing* victim, CThingAICreature* witness, long postType,
                  bool a, bool b, const CFaction* faction, bool c, long extra);   // 0x006C6990
};

class CHeroStatsDef { public:
    unsigned char _pad_0x00[0x8C];
    long          MinVomitSeconds;                   // +0x8C
    long          MaxVomitSeconds;                   // +0x90
    unsigned char _pad_0x94[0x8];
    float         DrunkennessPerVomitSecond;         // +0x9C
};

class CGameDefinitionManager { public:
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC CDefPointer<const CHeroStatsDef>::Object
};

unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
class CWorld { public: static long __fastcall GetFrame(); };                 // 0x0049D870
extern long g_framesPerSecond;                       // 0x01375550

class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
    unsigned long* GetPWorldSeed() const;   // 0x00686DA0
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};

class CTCHeroStats : public CTCBase { public:
    void*         __vftable;                         // +0x000
    CThing*       ParentThing;                       // +0x004 CTCBase
    unsigned char _pad_0x008[0xE4];
    long          NumberOfVomits;                    // +0x0EC (PDB +0xF4)
    unsigned char _pad_0x0f0[0xAC];
    float         Drunkenness;                       // +0x19C (PDB +0x1B4)
    float         BeerReservoir;                     // +0x1A0 (PDB +0x1B8)
    float         DrinkAffectMultiplier;             // +0x1A4
    float         DrinkAffectMultiplierMultiplier;   // +0x1A8
    long          FrameToVomit;                      // +0x1AC (PDB +0x1C4)
    long          DeedHandle;                        // +0x1B0 (PDB +0x1C8)
    long          FrameEndVomit;                     // +0x1B4 (PDB +0x1CC)

    const CHeroStatsDef* GetHeroStatsDef() const { return GetDefinitionManager().PHeroStatsDef; }
    void Vomit();
};

void CTCHeroStats::Vomit()
{
    CThingPlayerCreature* hero = PeekPlayerManager().GetMainPlayer()->GetPControlledCreature();
    float seconds = (Drunkenness - 3.0f) / GetHeroStatsDef()->DrunkennessPerVomitSecond;
    if (GetHeroStatsDef()->MaxVomitSeconds < seconds)
        seconds = (float)GetHeroStatsDef()->MaxVomitSeconds;
    float loopTime = (float)(GetHeroStatsDef()->MinVomitSeconds +
                             GFRandom((long)(seconds + 1.0f - GetHeroStatsDef()->MinVomitSeconds), GetPWorldSeed()));
    CCreatureAction_PlayIntoLoopOutOfAnimation action(hero, "ST_PUKE_INTO", "ST_PUKE_LOOP", "ST_PUKE_OUTOF", loopTime,
                                                      false, 0, 1);
    ++NumberOfVomits;
    CTCHeroOnlineScoreboard* scoreboard;
    ParentThing->GetInterfacePick(&scoreboard, 0x10D);
    scoreboard->UpdateScoreForActivity(15, (float)NumberOfVomits, false);
    if (hero->IsFreeToPerformAction(&action)) {
        FrameToVomit = -1;
        if (hero) {
            CTCHeroOpinionDeedLog* log;
            if (hero->GetInterface(&log, 0x51))
                DeedHandle = log->PostDeed(0x15, hero->GetPos(), 0, 0, 7, 0, 1, 0, 0, 0);
            hero->SetCurrentAction(action);
        }
        long fps = g_framesPerSecond;
        FrameEndVomit = (long)(CWorld::GetFrame() + fps * loopTime);
        BeerReservoir = 0.0f;
    }
}