// Behaviour test for CTCInventoryExperience::ConstructCategoryStrengthPC (0x005759BF): a model of the PC category build. As on
// console, three branches strength / skill / will are appended with their buyable counts; on PC every title, ring picture,
// subtitle and description is also placed in the list's PC_UI_FRAME (16 placements: strength 4, skill 4, will list title, the
// three spell titles, the magic-power title, ring, description and subtitle), and the frame is looked up once.
#include <cstdio>
#include <string>
#include <vector>
struct SFrame { int lookups; std::vector<std::string> placed; };
struct SList { SFrame frame; std::vector<std::string> entries; };
static SFrame* FindFrame(SList& list) { ++list.frame.lookups; return &list.frame; }
static void Construct(SList& list)
{
    SFrame* frame = FindFrame(list);
    frame->placed.push_back("STRENGTH_TITLE");
    frame->placed.push_back("STRENGTH_RING");
    frame->placed.push_back("STRENGTH_SUBTITLE");
    frame->placed.push_back("STRENGTH_DESC");
    list.entries.push_back("STRENGTH");
    frame->placed.push_back("SKILL_TITLE");
    frame->placed.push_back("SKILL_RING");
    frame->placed.push_back("SKILL_DESC");
    frame->placed.push_back("SKILL_SUBTITLE");
    list.entries.push_back("SKILL");
    const char* will[] = { "WILL_TITLE", "ATTACK_TITLE", "SURROUND_TITLE", "PHYSICAL_TITLE", "MAGIC_POWER_TITLE",
                           "WILL_RING", "WILL_DESC", "WILL_SUBTITLE" };
    for (int i = 0; i < 8; ++i) frame->placed.push_back(will[i]);
    list.entries.push_back("WILL");
}
int main()
{
    int fails = 0;
    SList list; list.frame.lookups = 0;
    Construct(list);
    if (list.frame.lookups != 1 || list.frame.placed.size() != 16) ++fails;
    if (list.entries.size() != 3 || list.entries[0] != "STRENGTH" || list.entries[2] != "WILL") ++fails;
    if (list.frame.placed.front() != "STRENGTH_TITLE" || list.frame.placed.back() != "WILL_SUBTITLE") ++fails;
    std::printf(fails ? "005759bf_TEST FAIL\n" : "005759bf_TEST PASS\n");
    return fails;
}