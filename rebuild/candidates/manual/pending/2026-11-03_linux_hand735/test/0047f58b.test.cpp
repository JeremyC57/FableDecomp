// Behaviour test for CTCInGameMenu::AddRegionAndTimeInfo (0x0047F58B): a model of the slot reader. The ".hs" companion
// supplies the world frame when it verifies and exists; the save itself then supplies the region names (and the world frame
// only when the companion did not); a missing or unverified file leaves the defaults ("" / "" / 0); one entry is always appended.
#include <cstdio>
#include <cstring>
struct SHeader { bool present; long world_frame; const char* region; const char* minimap; };
struct SSlot { SHeader companion; SHeader save; };
struct SLists { const char* region[4]; const char* minimap[4]; long frame[4]; int count; };
static void ReadHeader(const SHeader& h, long& frame_out, const char*& region_out, const char*& minimap_out, bool want_frame, bool want_regions)
{
    if (want_frame) frame_out = h.world_frame;
    if (want_regions) { region_out = h.region; minimap_out = h.minimap; }
}
static void AddRegionAndTimeInfo(SLists& lists, const SSlot& slot)
{
    const char* region_name = "";
    const char* region_minimap_name = "";
    long world_frame = 0;
    bool got_world_frame = false;
    if (slot.companion.present) {
        ReadHeader(slot.companion, world_frame, region_name, region_minimap_name, true, false);
        got_world_frame = true;
    }
    if (slot.save.present) {
        long unused_long = 0;
        if (!got_world_frame)
            ReadHeader(slot.save, world_frame, region_name, region_minimap_name, true, true);
        else
            ReadHeader(slot.save, unused_long, region_name, region_minimap_name, false, true);
    }
    lists.region[lists.count] = region_name;
    lists.minimap[lists.count] = region_minimap_name;
    lists.frame[lists.count] = world_frame;
    ++lists.count;
}
int main()
{
    SLists lists; lists.count = 0;
    SSlot both = { { true, 111, "X", "Y" }, { true, 222, "OAKVALE", "MAP_OAKVALE" } };
    SSlot save_only = { { false, 0, "", "" }, { true, 333, "BOWERSTONE", "MAP_BOW" } };
    SSlot none = { { false, 0, "", "" }, { false, 0, "", "" } };
    SSlot companion_only = { { true, 444, "Z", "W" }, { false, 0, "", "" } };
    AddRegionAndTimeInfo(lists, both);
    AddRegionAndTimeInfo(lists, save_only);
    AddRegionAndTimeInfo(lists, none);
    AddRegionAndTimeInfo(lists, companion_only);
    bool ok = lists.count == 4
        && lists.frame[0] == 111 && std::strcmp(lists.region[0], "OAKVALE") == 0 && std::strcmp(lists.minimap[0], "MAP_OAKVALE") == 0
        && lists.frame[1] == 333 && std::strcmp(lists.region[1], "BOWERSTONE") == 0
        && lists.frame[2] == 0 && lists.region[2][0] == 0 && lists.minimap[2][0] == 0
        && lists.frame[3] == 444 && lists.region[3][0] == 0;
    if (!ok) { std::printf("0047f58b_TEST FAIL\n"); return 1; }
    std::printf("0047f58b_TEST PASS\n");
    return 0;
}
