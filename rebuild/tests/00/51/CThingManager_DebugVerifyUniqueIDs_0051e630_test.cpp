// Behaviour test for CThingManager::DebugVerifyUniqueIDs (0x0051E630): every thing of the input vector with a non-zero unique
// ID is compared against every other managed thing; each other thing carrying the same ID gets a fresh one (CreateUID); the
// def names are built only for the first clash (the debug text is unused).
#include <cstdio>

static unsigned long long g_nextUID = 1000;
static int g_namesBuilt = 0;
class CThing { public:
    unsigned long long UniqueID;
    bool HasDef;
    void CreateUID() { UniqueID = g_nextUID++; }
    void BuildDefName() const { ++g_namesBuilt; }
};
struct CThingListNode { CThingListNode* Next; CThing* Data; };

void DebugVerifyUniqueIDs(CThing** first, CThing** last, CThingListNode* head)
{
    bool reported = false;
    for (CThing** it = first; it != last; ++it) {
        CThing* thing = *it;
        if (thing->UniqueID) {
            for (CThingListNode* node = head->Next; node != head; node = node->Next) {
                CThing* other = node->Data;
                if (thing != other && thing->UniqueID == other->UniqueID && thing->UniqueID) {
                    if (!reported && (thing->HasDef || other->HasDef)) {
                        thing->BuildDefName();
                        other->BuildDefName();
                        reported = true;
                    }
                    other->CreateUID();
                }
            }
        }
    }
}

int main()
{
    CThing a = { 5, true }, b = { 5, false }, c = { 7, true }, d = { 5, true }, e = { 0, true }, f = { 0, false };
    CThing* all[6] = { &a, &b, &c, &d, &e, &f };
    CThingListNode head, n[6];
    head.Next = &n[0];
    for (int i = 0; i < 6; ++i) { n[i].Data = all[i]; n[i].Next = (i == 5) ? &head : &n[i + 1]; }
    CThing* input[3] = { &a, &c, &e };
    DebugVerifyUniqueIDs(input, input + 3, &head);
    // a (5) clashes with b and d: both renumbered; c (7) unique; e has no ID (skipped, f keeps 0).
    bool ok = a.UniqueID == 5 && b.UniqueID == 1000 && d.UniqueID == 1001 && c.UniqueID == 7 && e.UniqueID == 0 &&
              f.UniqueID == 0 && g_namesBuilt == 2;
    if (!ok) { std::printf("0051e630_TEST FAIL\n"); return 1; }
    std::printf("0051e630_TEST PASS\n");
    return 0;
}