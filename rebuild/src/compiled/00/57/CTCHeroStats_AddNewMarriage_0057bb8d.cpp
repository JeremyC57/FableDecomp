// CTCHeroStats::AddNewMarriage(CThingCreatureBase*, CThing*, EMarriageState) -- records a marriage, counts the wedding and reports both wedding scores
// Leaf: AddNewMarriage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057BB8D, 225 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: CThingCreatureBase::GetSex() @ 0x00661FF0 (label "CPixelShader::CountReferences" is propagated),
//          CHeroMarriageInfo::CHeroMarriageInfo(ulong64, ulong64, EMarriageState, bool) @ 0x0057A376,
//          std::list<CHeroMarriageInfo>::push_back(const CHeroMarriageInfo&) @ 0x0058056F (label
//          "ListPushFrontWrapper" is generic), std::bitset::test @ 0x00410DE0, CVectorMap<ETCInterfaceType,
//          CTCBase*>::LowerBound @ 0x0040F020, CTCHeroOnlineScoreboard::UpdateScoreForActivity(long, float, bool)
//          @ 0x0055F3B0 (x2).
// Shape: the marital home's UID (0 without a home) is a 64-bit named local built first in the frame, the spouse's
// UID a named local held in edi:ebx; GayMarriage is `GetSex() == 1` (the 8-bit `dec ; neg ; sbb ; inc` tail).
// The record is a 24-byte temporary pushed through push_back. The scoreboard (interface 0x10D) comes from the
// unchecked out-parameter GetInterface (key homed in the dead home-pointer slot); activity 12 gets the wedding
// count, activity 9 the weddings minus the dead and murdered spouses (an int spilled into the same dead slot
// for the `fild`).
// Layout: retail CTCHeroStats sits below the PDB (engine/CTCHeroStats.h): NumWeddings +0x144 (PDB +0x158),
// NumSpousesMurdered +0x148, NumSpousesDied +0x14C, Marriages (a 4-byte list head) +0x1D8 (PDB +0x1F0).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

class CThing { public:
    unsigned char    _pad_0x00[0x18];
    unsigned __int64 UID;                            // +0x18
    CPresenceBits    m_present;                      // +0x20
    unsigned char    _pad_0x21[0x23];
    CInterfaceMap    m_interfaces;                   // +0x44
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

class CThingCreatureBase : public CThing { public:
    long GetSex();                                   // 0x00661FF0
};

enum EMarriageState { MARRIAGE_STATE_NONE };

class CHeroMarriageInfo { public:
    unsigned __int64 WifeUID;                        // +0x00
    unsigned __int64 MaritalHomeUID;                 // +0x08
    EMarriageState   State;                          // +0x10
    bool             GayMarriage;                    // +0x14
    CHeroMarriageInfo(unsigned __int64 wife, unsigned __int64 home, EMarriageState state, bool gay);  // 0x0057A376
};

struct CHeroMarriageList {                           // std::list<CHeroMarriageInfo> (retail: the head pointer only)
    void* _M_node;
    void push_back(const CHeroMarriageInfo& info);   // 0x0058056F
};

class CTCHeroOnlineScoreboard { public:
    void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter);  // 0x0055F3B0
};

class CTCHeroStats { public:
    void*             __vftable;                     // +0x000
    CThing*           ParentThing;                   // +0x004 CTCBase
    unsigned char     _pad_0x008[0x13C];
    long              NumWeddings;                   // +0x144 (PDB +0x158)
    long              NumSpousesMurdered;            // +0x148 (PDB +0x15C)
    long              NumSpousesDied;                // +0x14C (PDB +0x160)
    unsigned char     _pad_0x150[0x88];
    CHeroMarriageList Marriages;                     // +0x1D8 (PDB +0x1F0)

    void AddNewMarriage(CThingCreatureBase* wife, CThing* home, EMarriageState state);
};

void CTCHeroStats::AddNewMarriage(CThingCreatureBase* wife, CThing* home, EMarriageState state)
{
    unsigned __int64 homeUID;
    if (home)
        homeUID = home->UID;
    else
        homeUID = 0;
    unsigned __int64 wifeUID = wife->UID;
    Marriages.push_back(CHeroMarriageInfo(wifeUID, homeUID, state, wife->GetSex() == 1));
    ++NumWeddings;
    CTCHeroOnlineScoreboard* scoreboard;
    ParentThing->GetInterface(&scoreboard, 0x10D);
    scoreboard->UpdateScoreForActivity(12, (float)NumWeddings, false);
    long currentSpouses = NumWeddings - NumSpousesDied - NumSpousesMurdered;
    scoreboard->UpdateScoreForActivity(9, (float)currentSpouses, false);
}