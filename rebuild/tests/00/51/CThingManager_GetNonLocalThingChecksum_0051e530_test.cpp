#include <cstdio>
// Behaviour test for CThingManager::GetNonLocalThingChecksum (0x0051E530): sum over non-Dead things.
struct CThing {
    unsigned long checksum; unsigned char Dead : 1;
    virtual unsigned long GetChecksum() { return checksum; }
    virtual ~CThing() {}
};
struct SThingListNode { SThingListNode* PNext; SThingListNode* PPrev; CThing* PThing; };
struct CThingList {
    SThingListNode* PHead;
    SThingListNode* begin() const { return PHead->PNext; }
    SThingListNode* end() const { return PHead; }
};
struct CThingManager {
    CThingList Things;
    unsigned long GetNonLocalThingChecksum()
    {
        unsigned long checksum = 0;
        for (SThingListNode* it = Things.begin(); it != Things.end(); it = it->PNext) {
            CThing* thing = it->PThing;
            if (!thing->Dead) checksum += thing->GetChecksum();
        }
        return checksum;
    }
};
int main()
{
    CThing t[3]; t[0].checksum = 10; t[0].Dead = 0; t[1].checksum = 200; t[1].Dead = 1; t[2].checksum = 3000; t[2].Dead = 0;
    SThingListNode head, n[3];
    head.PNext = &n[0]; n[0].PNext = &n[1]; n[1].PNext = &n[2]; n[2].PNext = &head;
    for (int i = 0; i < 3; ++i) n[i].PThing = &t[i];
    CThingManager m; m.Things.PHead = &head;
    int fails = 0;
    if (m.GetNonLocalThingChecksum() != 3010) ++fails;
    head.PNext = &head;
    if (m.GetNonLocalThingChecksum() != 0) ++fails;
    if (fails) { std::printf("0051e530_TEST FAIL\n"); return 1; }
    std::printf("0051e530_TEST PASS\n");
    return 0;
}