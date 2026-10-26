#include <cstdio>
// Behaviour test for CThingManager::SetNonScriptedEntitesPaused (0x0051EFC0).
class CThing { public:
    unsigned char ThingType; unsigned long UsingComponents[9]; unsigned char Dead : 1; int kills;
    unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    void Kill(bool) { ++kills; }
};
struct SThingListNode { SThingListNode* PNext; SThingListNode* PPrev; CThing* PThing; };
class CThingManager { public:
    SThingListNode* PHead; bool NonScriptedEntitesPauseMode;
    void SetNonScriptedEntitesPaused(bool paused)
    {
        NonScriptedEntitesPauseMode = paused;
        if (paused)
            for (SThingListNode* it = PHead->PNext; it != PHead; it = it->PNext) {
                CThing* thing = it->PThing;
                if (thing && !thing->Dead && thing->ThingType == 5 && thing->HasTC(119)) thing->Kill(true);
            }
    }
};
int main()
{
    CThing t[4];
    for (int i = 0; i < 4; ++i) { t[i].ThingType = 5; t[i].Dead = 0; t[i].kills = 0; for (int w = 0; w < 9; ++w) t[i].UsingComponents[w] = 0; t[i].UsingComponents[3] = 0x800000UL; }
    t[1].Dead = 1; t[2].ThingType = 4; t[3].UsingComponents[3] = 0x400000UL;
    SThingListNode head, n[5];
    head.PNext = &n[0]; for (int i = 0; i < 4; ++i) { n[i].PNext = &n[i + 1]; n[i].PThing = &t[i]; } n[4].PNext = &head; n[4].PThing = 0;
    CThingManager m; m.PHead = &head; m.NonScriptedEntitesPauseMode = false;
    int fails = 0;
    m.SetNonScriptedEntitesPaused(false);
    if (m.NonScriptedEntitesPauseMode || t[0].kills) ++fails;
    m.SetNonScriptedEntitesPaused(true);
    if (!m.NonScriptedEntitesPauseMode || t[0].kills != 1 || t[1].kills || t[2].kills || t[3].kills) ++fails;
    if (fails) { std::printf("0051efc0_TEST FAIL\n"); return 1; }
    std::printf("0051efc0_TEST PASS\n");
    return 0;
}