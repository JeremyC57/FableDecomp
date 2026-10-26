// Test for 00681ae0 CTCCreatureOpinionOfHero::KnowsOfDeed: pending list (optional) then the per-type known list.
#include <cstdio>
struct SLongNode { SLongNode* Next; SLongNode* Prev; long Data; };
struct CLongList { SLongNode* Header; };
class CTCCreatureOpinionOfHero { public:
    CLongList PendingDeeds;
    CLongList KnownDeeds[4];
    bool KnowsOfDeed(long deedId, long deedType, bool includePending) const
    {
        if (includePending) {
            SLongNode* end = PendingDeeds.Header;
            for (SLongNode* it = end->Next; it != end; it = it->Next)
                if (it->Data == deedId)
                    return true;
        }
        SLongNode* end = KnownDeeds[deedType].Header;
        for (SLongNode* it = end->Next; it != end; it = it->Next)
            if (it->Data == deedId)
                return true;
        return false;
    }
};
static void Link(SLongNode* header, SLongNode* nodes, int n)
{
    SLongNode* prev = header;
    for (int i = 0; i < n; ++i) { prev->Next = &nodes[i]; nodes[i].Prev = prev; prev = &nodes[i]; }
    prev->Next = header; header->Prev = prev;
}
int main()
{
    SLongNode hp, hk[4], pn[2], kn[3];
    pn[0].Data = 10; pn[1].Data = 11; kn[0].Data = 20; kn[1].Data = 21; kn[2].Data = 22;
    Link(&hp, pn, 2);
    for (int i = 0; i < 4; ++i) Link(&hk[i], 0, 0);
    Link(&hk[2], kn, 3);
    CTCCreatureOpinionOfHero o;
    o.PendingDeeds.Header = &hp;
    for (int i = 0; i < 4; ++i) o.KnownDeeds[i].Header = &hk[i];
    if (!o.KnowsOfDeed(11, 0, true) || o.KnowsOfDeed(11, 2, false) || !o.KnowsOfDeed(22, 2, false)
        || !o.KnowsOfDeed(20, 2, true) || o.KnowsOfDeed(20, 1, true) || o.KnowsOfDeed(99, 2, true)) {
        std::printf("00681ae0_TEST FAIL\n"); return 1;
    }
    std::printf("00681ae0_TEST PASS\n");
    return 0;
}