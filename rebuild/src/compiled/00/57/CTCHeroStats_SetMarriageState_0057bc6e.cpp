// CTCHeroStats::SetMarriageState(CThingAICreature*, EMarriageState) -- updates the wife's marriage record and reports the spouse scores
// Leaf: SetMarriageState
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057BC6E, 515 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`, as the landed AddNewMarriage 0x0057BB8D).
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020,
//          CTCHeroOnlineScoreboard::UpdateScoreForActivity(long, float, bool) @ 0x0055F3B0.
// Shape: walks the Marriages list (retail: a 4-byte head pointer at +0x1D8); a record whose WifeUID matches the
// wife's UID takes the new state. Murdered (7), dead (6) and divorced (5) marriages lose their marital home and
// bump the matching counter, then report to the scoreboard (interface 0x10D, from the unchecked out-parameter
// GetInterface; each arm has its own out word). The three arms share one call tail.
// Layout: retail CTCHeroStats sits below the PDB (engine/CTCHeroStats.h): NumWeddings +0x144 (PDB +0x158),
// NumSpousesMurdered +0x148, NumSpousesDied +0x14C, NumDivorces +0x150, Marriages +0x1D8 (PDB +0x1F0).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature : public CThing { public:
};

enum EMarriageState { MARRIAGE_STATE_NONE };

class CHeroMarriageInfo { public:
    unsigned __int64 WifeUID;                        // +0x00
    unsigned __int64 MaritalHomeUID;                 // +0x08
    EMarriageState   State;                          // +0x10
    bool             GayMarriage;                    // +0x14
};

struct CHeroMarriageNode {
    CHeroMarriageNode* _M_next;                      // +0x00
    CHeroMarriageNode* _M_prev;                      // +0x04
    CHeroMarriageInfo  _M_data;                      // +0x08
};

struct CHeroMarriageList {                           // std::list<CHeroMarriageInfo> (retail: the head pointer only)
    CHeroMarriageNode* _M_node;
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
    long              NumDivorces;                   // +0x150 (PDB +0x164)
    unsigned char     _pad_0x154[0x84];
    CHeroMarriageList Marriages;                     // +0x1D8 (PDB +0x1F0)

    void SetMarriageState(CThingAICreature* wife, EMarriageState state);
};

void CTCHeroStats::SetMarriageState(CThingAICreature* wife, EMarriageState state)
{
    CHeroMarriageNode* last = Marriages._M_node;
    for (CHeroMarriageNode* it = last->_M_next; it != last; it = it->_M_next) {
        if (it->_M_data.WifeUID == wife->GetUniqueID()) {
            it->_M_data.State = state;
            if (state == 5 || state == 6 || state == 7) {
                it->_M_data.MaritalHomeUID = 0;
                if (state == 7) {
                    ++NumSpousesMurdered;
                    CTCHeroOnlineScoreboard* scoreboard;
                    ParentThing->GetTC(TCI_HERO_ONLINE_SCOREBOARD, &scoreboard);
                    scoreboard->UpdateScoreForActivity(14, (float)(NumSpousesDied + NumSpousesMurdered), false);
                    long currentSpouses = NumWeddings - NumSpousesDied - NumSpousesMurdered;
                    scoreboard->UpdateScoreForActivity(9, (float)currentSpouses, false);
                } else if (state == 6) {
                    ++NumSpousesDied;
                    CTCHeroOnlineScoreboard* scoreboard;
                    ParentThing->GetTC(TCI_HERO_ONLINE_SCOREBOARD, &scoreboard);
                    scoreboard->UpdateScoreForActivity(14, (float)(NumSpousesDied + NumSpousesMurdered), false);
                    long currentSpouses = NumWeddings - NumSpousesDied - NumSpousesMurdered;
                    scoreboard->UpdateScoreForActivity(9, (float)currentSpouses, false);
                } else if (state == 5) {
                    ++NumDivorces;
                    CTCHeroOnlineScoreboard* scoreboard;
                    ParentThing->GetTC(TCI_HERO_ONLINE_SCOREBOARD, &scoreboard);
                    scoreboard->UpdateScoreForActivity(13, (float)NumDivorces, false);
                }
            }
        }
    }
}