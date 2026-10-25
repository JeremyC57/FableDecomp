// Behaviour test for CWitnessesSearchingDeed::CWitnessesSearchingDeed (0x006C5410): every field stored, the recipient
// intelligent pointer built empty and then pointed at the recipient through SetPItem.
#include <cstdio>

class CThing { public: virtual ~CThing() {} };
class CThingAICreature : public CThing { };
class CFaction { public: int id; };
enum EOpinionDeedType { OPINION_DEED_TYPE_NONE, OPINION_DEED_TYPE_7 = 7 };
enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_NONE, OPINION_POST_DEED_TYPE_3 = 3 };

static int g_setCalls = 0; static CThing* g_itemBeforeSet = (CThing*)1;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    void SetPItem(CThing* item) { ++g_setCalls; g_itemBeforeSet = PItem; PItem = item; }
    CThing* PItem;
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
};

class CWitnessesSearchingDeed {
public:
    CWitnessesSearchingDeed(long handle, EOpinionDeedType type, CThingAICreature* recipient,
                            EOpinionPostDeedType postType, CFaction* faction, bool reverseFactionSearch);
    long                                  DeedHandle;
    EOpinionDeedType                      DeedType;
    CIntelligentPointer<CThingAICreature> Recipient;
    EOpinionPostDeedType                  PostType;
    CFaction*                             PFaction;
    bool                                  ReverseFactionSearch;
};

CWitnessesSearchingDeed::CWitnessesSearchingDeed(long handle, EOpinionDeedType type, CThingAICreature* recipient,
                                                 EOpinionPostDeedType postType, CFaction* faction,
                                                 bool reverseFactionSearch)
    : DeedHandle(handle), DeedType(type), Recipient(recipient), PostType(postType), PFaction(faction),
      ReverseFactionSearch(reverseFactionSearch)
{
}

int main()
{
    CThingAICreature creature;
    CFaction faction;
    CWitnessesSearchingDeed deed(42, OPINION_DEED_TYPE_7, &creature, OPINION_POST_DEED_TYPE_3, &faction, true);
    if (deed.DeedHandle != 42 || deed.DeedType != OPINION_DEED_TYPE_7 || deed.Recipient.PItem != &creature
        || deed.PostType != OPINION_POST_DEED_TYPE_3 || deed.PFaction != &faction || !deed.ReverseFactionSearch
        || g_setCalls != 1 || g_itemBeforeSet != 0)
        { std::printf("006c5410_TEST FAIL\n"); return 1; }
    std::printf("006c5410_TEST PASS\n");
    return 0;
}