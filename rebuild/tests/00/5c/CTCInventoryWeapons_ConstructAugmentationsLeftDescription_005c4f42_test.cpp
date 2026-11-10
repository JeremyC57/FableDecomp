// Behaviour test of CTCInventoryWeapons::ConstructAugmentationsLeftDescription (0x005C4F42): the owned augmentations are summed
// per augmentation type (a def without an augmentation sub-def is skipped); then, in ascending type order, every type with a
// positive count adds "<TEXT_OBJECT_AUGMENTATION_<X>_TITLE> x<count>\n" (types outside the eight known bits add nothing), and
// finally the melee (true) or ranged (false) augment guide text is appended.
#include <cstdio>
#include <map>
#include <string>
#include <utility>
#include <vector>
struct CAugDef { bool IsAugmentation; long Type; };
static std::map<long, CAugDef> g_Defs;                         // def index -> def (stub of GetDef / GetSubDef<CAugmentationDef>)
static std::string IntToString(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static std::string TitleOf(long type)
{
    switch (type) {
    case 0x08: return "LIGHTNING";
    case 0x04: return "FLAME";
    case 0x02: return "SILVER";
    case 0x01: return "SHARPENING";
    case 0x10: return "PIERCING";
    case 0x80: return "EXPERIENCE";
    case 0x40: return "WILL_REGENERATION";
    case 0x20: return "HEALTH_REGENERATION";
    }
    return "";
}
static void ConstructAugmentationsLeftDescription(std::string& text, std::vector<std::pair<long, long> >& augmentations, bool melee)
{
    std::string x = " x";
    std::map<long, long> counts;
    for (unsigned long i = 0; i < augmentations.size(); ++i) {
        const CAugDef& def = g_Defs[augmentations[i].first];
        if (def.IsAugmentation) {
            std::map<long, long>::iterator it = counts.find(def.Type);
            if (it != counts.end())
                it->second += augmentations[i].second;
            else
                counts.insert(std::make_pair(def.Type, augmentations[i].second));
        }
    }
    for (std::map<long, long>::iterator it = counts.begin(); it != counts.end(); ++it) {
        if (it->second > 0) {
            std::string title = TitleOf(it->first);
            if (!title.empty())
                text += "TEXT_OBJECT_AUGMENTATION_" + title + "_TITLE" + x + IntToString(it->second) + "\n";
        }
    }
    text += melee ? "MELEE_AUGMENT" : "RANGED_AUGMENT";
}
int main()
{
    CAugDef flame = { true, 0x04 }, silver = { true, 0x02 }, odd = { true, 0x100 }, sword = { false, 0 }, zero = { true, 0x01 };
    g_Defs[10] = flame; g_Defs[11] = silver; g_Defs[12] = odd; g_Defs[13] = sword; g_Defs[14] = zero;
    std::vector<std::pair<long, long> > items;
    items.push_back(std::make_pair(10L, 2L));
    items.push_back(std::make_pair(11L, 1L));
    items.push_back(std::make_pair(10L, 3L));                  // summed with the first flame entry
    items.push_back(std::make_pair(12L, 4L));                  // unknown type bit: counted, no line
    items.push_back(std::make_pair(13L, 7L));                  // not an augmentation: skipped
    items.push_back(std::make_pair(14L, 0L));                  // zero count: no line
    std::string t;
    ConstructAugmentationsLeftDescription(t, items, true);
    int bad = t != "TEXT_OBJECT_AUGMENTATION_SILVER_TITLE x1\nTEXT_OBJECT_AUGMENTATION_FLAME_TITLE x5\nMELEE_AUGMENT";
    std::string r;
    std::vector<std::pair<long, long> > none;
    ConstructAugmentationsLeftDescription(r, none, false);
    bad |= r != "RANGED_AUGMENT";
    if (bad) { std::printf("005c4f42_TEST FAIL %s|%s\n", t.c_str(), r.c_str()); return 1; }
    std::printf("005c4f42_TEST PASS\n");
    return 0;
}