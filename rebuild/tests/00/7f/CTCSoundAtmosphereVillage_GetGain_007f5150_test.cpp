// Behaviour test for CTCSoundAtmosphereVillage::GetGain (0x007F5150): a copy of the row's logic over a small stand-alone model
// (layer name test, no-living-spaces exit, timed recount of villagers not asleep in bed, the [0, 1] clamp).
#include <cstdio>
#include <cstring>

enum { MODE_SLEEP_BED = 13, MODE_OTHER = 2 };

struct Mode { unsigned long type; unsigned long GetType() const { return type; } };
struct ModeManager { Mode current; Mode& GetCurrentMode() { return current; } };
struct Creature { ModeManager* modes; bool GetTC(ModeManager** out) { if (modes) { *out = modes; return true; } return false; } };
struct Node { Node* next; Creature* value; };
struct Village { Node head; Village() { head.next = &head; head.value = 0; }
    void Add(Node* n, Creature* c) { n->value = c; n->next = head.next; head.next = n; } };
struct Thing { Village* village; bool GetTC(Village** out) { if (village) { *out = village; return true; } return false; } };

struct Name { const char* text;   // null = the empty CCharString
    bool operator==(const char* other) const { if (!text) { if (std::strcmp(other, "") == 0) return true; return false; } return std::strcmp(text, other) == 0; } };

struct SoundAtmosphereVillage {
    Thing* ParentThing;
    float TimeUntilNextUpdate;
    long NumLivingSpaces;
    long NumVillageMembers;
    float GetGain(const Name& name)
    {
        if (name == "STREETLIFE") {
            if (NumLivingSpaces == 0)
                return 0.0f;
            if (TimeUntilNextUpdate <= 0.0f) {
                Village* village;
                ParentThing->GetTC(&village);
                NumVillageMembers = 0;
                ModeManager* modes;
                for (Node* it = village->head.next; it != &village->head; it = it->next) {
                    it->value->GetTC(&modes);
                    if (modes->GetCurrentMode().GetType() != MODE_SLEEP_BED)
                        ++NumVillageMembers;
                }
                TimeUntilNextUpdate = 5.0f;
            }
            float gain = (float)NumVillageMembers / NumLivingSpaces;
            if (gain < 0.0f)
                gain = 0.0f;
            else if (gain > 1.0f)
                gain = 1.0f;
            return gain;
        }
        return -1.0f;
    }
};

int main()
{
    ModeManager awake1 = { { MODE_OTHER } }, awake2 = { { MODE_OTHER } }, awake3 = { { 7 } }, asleep = { { MODE_SLEEP_BED } };
    Creature c1 = { &awake1 }, c2 = { &asleep }, c3 = { &awake2 }, c4 = { &awake3 };
    Node n1, n2, n3, n4;
    Village village;
    village.Add(&n1, &c1); village.Add(&n2, &c2); village.Add(&n3, &c3);
    Thing thing = { &village };
    SoundAtmosphereVillage tc = { &thing, 0.0f, 4, 99 };
    Name street = { "STREETLIFE" }, other = { "WIND" }, empty = { 0 };
    bool ok = true;
    // other layers and the empty name: -1, nothing touched
    if (tc.GetGain(other) != -1.0f || tc.GetGain(empty) != -1.0f || tc.NumVillageMembers != 99 || tc.TimeUntilNextUpdate != 0.0f) ok = false;
    // recount: 2 of 3 villagers awake, 4 living spaces -> 0.5, timer re-armed to 5
    if (tc.GetGain(street) != 0.5f || tc.NumVillageMembers != 2 || tc.TimeUntilNextUpdate != 5.0f) ok = false;
    // timer still running: no recount even though the sleeper woke up
    asleep.current.type = MODE_OTHER;
    if (tc.GetGain(street) != 0.5f || tc.NumVillageMembers != 2) ok = false;
    // timer ran out: recount sees 3 awake -> 0.75
    tc.TimeUntilNextUpdate = -0.25f;
    if (tc.GetGain(street) != 0.75f || tc.NumVillageMembers != 3 || tc.TimeUntilNextUpdate != 5.0f) ok = false;
    // more awake villagers than living spaces: clamped to 1
    village.Add(&n4, &c4);
    tc.NumLivingSpaces = 2; tc.TimeUntilNextUpdate = 0.0f;
    if (tc.GetGain(street) != 1.0f || tc.NumVillageMembers != 4) ok = false;
    // a negative ratio is clamped to 0 (timer running, so the stored count is used as is)
    tc.NumVillageMembers = -3;
    if (tc.GetGain(street) != 0.0f) ok = false;
    // no living spaces: 0 before any recount
    tc.NumLivingSpaces = 0; tc.TimeUntilNextUpdate = 0.0f; tc.NumVillageMembers = 7;
    if (tc.GetGain(street) != 0.0f || tc.NumVillageMembers != 7 || tc.TimeUntilNextUpdate != 0.0f) ok = false;
    // an empty village counts nobody
    Village nobody; thing.village = &nobody; tc.NumLivingSpaces = 3;
    if (tc.GetGain(street) != 0.0f || tc.NumVillageMembers != 0 || tc.TimeUntilNextUpdate != 5.0f) ok = false;
    if (!ok) { std::printf("007f5150_TEST FAIL\n"); return 1; }
    std::printf("007f5150_TEST PASS\n");
    return 0;
}