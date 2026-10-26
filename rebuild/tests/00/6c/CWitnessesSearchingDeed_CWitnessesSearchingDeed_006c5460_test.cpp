// behaviour test for 006c5460: CWitnessesSearchingDeed's copy constructor default-constructs the
// recipient pointer and then assigns the whole record (`*this = other`): every member is copied and the
// recipient is re-pointed through SetPItem.
#include <cstdio>

class CThing { public: long m_id; };
class CThingAICreature;
class CBaseObject;
class CFaction { public: long m_id; };

static long g_setCalls = 0;

struct CBaseIntelligentPointer {
    virtual long Kind() const { return 1; }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; ++g_setCalls; }
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual long Kind() const { return 2; }
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};

enum EOpinionDeedType { OPINION_DEED_TYPE_NONE, OPINION_DEED_TYPE_THEFT = 5 };
enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_NONE, OPINION_POST_DEED_TYPE_REPORT = 3 };

class CWitnessesSearchingDeed { public:
    long                                  DeedHandle;
    EOpinionDeedType                      DeedType;
    CIntelligentPointer<CThingAICreature> Recipient;
    EOpinionPostDeedType                  PostType;
    CFaction*                             PFaction;
    bool                                  ReverseFactionSearch;
    CWitnessesSearchingDeed() : DeedHandle(0), DeedType(OPINION_DEED_TYPE_NONE), PostType(OPINION_POST_DEED_TYPE_NONE), PFaction(0), ReverseFactionSearch(false) { }
    CWitnessesSearchingDeed(const CWitnessesSearchingDeed& other) { *this = other; }
};

int main()
{
    CThing creature;
    CFaction faction;
    CWitnessesSearchingDeed source;
    int ok = 1;
    creature.m_id = 77;
    faction.m_id = 3;
    source.DeedHandle = 1234;
    source.DeedType = OPINION_DEED_TYPE_THEFT;
    source.Recipient.SetPItem((CBaseObject*)&creature);
    source.PostType = OPINION_POST_DEED_TYPE_REPORT;
    source.PFaction = &faction;
    source.ReverseFactionSearch = true;
    g_setCalls = 0;
    CWitnessesSearchingDeed copy(source);
    if (copy.DeedHandle != 1234 || copy.DeedType != OPINION_DEED_TYPE_THEFT || copy.PostType != OPINION_POST_DEED_TYPE_REPORT)
        ok = 0;
    if (copy.PFaction != &faction || !copy.ReverseFactionSearch)
        ok = 0;
    if (copy.Recipient.GetPItem() != &creature || copy.Recipient.Kind() != 2 || g_setCalls != 1)
        ok = 0;
    if (!ok) {
        std::printf("006c5460_TEST FAIL\n");
        return 1;
    }
    std::printf("006c5460_TEST PASS\n");
    return 0;
}