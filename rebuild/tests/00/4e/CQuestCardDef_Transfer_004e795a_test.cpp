// behaviour test for 0x004E795A CQuestCardDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x58, +0x59, +0x5A, +0x5B, +0x5C, +0x68, +0x6C, +0x70),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Blob4 { unsigned char b[4]; };
struct Blob12 { unsigned char b[12]; };
struct Context {
    unsigned char buf[1024]; unsigned int pos; int load; const void* order[64]; unsigned int count;
    Context(int l) : pos(0), load(l), count(0) { }
    template <class T> void Transfer(T& v)
    {
        order[count++] = &v;
        if (load) std::memcpy(&v, buf + pos, sizeof(T)); else std::memcpy(buf + pos, &v, sizeof(T));
        pos += (unsigned int)sizeof(T);
    }
};
struct Def {
    long QuestName;
    long QuestSummary;
    long QuestObjective;
    long SuccessSummary;
    Blob4 RegionName;
    Blob4 TeleporterRegionName;
    long InventoryCategory;
    long RenownReward;
    long GoldReward;
    Blob12 RewardObjects;
    bool IsCore;
    bool IsVignette;
    bool IsExclusive;
    bool MakeVignetteRouteAppearOnMinimap;
    Blob12 Prerequisites;
    long NumBoasts;
    bool CanPlayerCancel;
    unsigned long QuestEpilogue;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.QuestName);
    c.Transfer(d.QuestSummary);
    c.Transfer(d.QuestObjective);
    c.Transfer(d.SuccessSummary);
    c.Transfer(d.RegionName);
    c.Transfer(d.TeleporterRegionName);
    c.Transfer(d.InventoryCategory);
    c.Transfer(d.RenownReward);
    c.Transfer(d.GoldReward);
    c.Transfer(d.RewardObjects);
    c.Transfer(d.IsCore);
    c.Transfer(d.IsVignette);
    c.Transfer(d.IsExclusive);
    c.Transfer(d.MakeVignetteRouteAppearOnMinimap);
    c.Transfer(d.Prerequisites);
    c.Transfer(d.NumBoasts);
    c.Transfer(d.CanPlayerCancel);
    c.Transfer(d.QuestEpilogue);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.IsCore = true;
    d.IsVignette = true;
    d.IsExclusive = true;
    d.MakeVignetteRouteAppearOnMinimap = true;
    d.CanPlayerCancel = true;
    Context save(0); Persist(d, save);
    const void* want[18] = { &d.QuestName, &d.QuestSummary, &d.QuestObjective, &d.SuccessSummary, &d.RegionName, &d.TeleporterRegionName, &d.InventoryCategory, &d.RenownReward, &d.GoldReward, &d.RewardObjects, &d.IsCore, &d.IsVignette, &d.IsExclusive, &d.MakeVignetteRouteAppearOnMinimap, &d.Prerequisites, &d.NumBoasts, &d.CanPlayerCancel, &d.QuestEpilogue };
    if (save.count != 18u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 18u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.QuestName, 0, sizeof(e.QuestName));
    std::memset(&e.QuestSummary, 0, sizeof(e.QuestSummary));
    std::memset(&e.QuestObjective, 0, sizeof(e.QuestObjective));
    std::memset(&e.SuccessSummary, 0, sizeof(e.SuccessSummary));
    std::memset(&e.RegionName, 0, sizeof(e.RegionName));
    std::memset(&e.TeleporterRegionName, 0, sizeof(e.TeleporterRegionName));
    std::memset(&e.InventoryCategory, 0, sizeof(e.InventoryCategory));
    std::memset(&e.RenownReward, 0, sizeof(e.RenownReward));
    std::memset(&e.GoldReward, 0, sizeof(e.GoldReward));
    std::memset(&e.RewardObjects, 0, sizeof(e.RewardObjects));
    std::memset(&e.IsCore, 0, sizeof(e.IsCore));
    std::memset(&e.IsVignette, 0, sizeof(e.IsVignette));
    std::memset(&e.IsExclusive, 0, sizeof(e.IsExclusive));
    std::memset(&e.MakeVignetteRouteAppearOnMinimap, 0, sizeof(e.MakeVignetteRouteAppearOnMinimap));
    std::memset(&e.Prerequisites, 0, sizeof(e.Prerequisites));
    std::memset(&e.NumBoasts, 0, sizeof(e.NumBoasts));
    std::memset(&e.CanPlayerCancel, 0, sizeof(e.CanPlayerCancel));
    std::memset(&e.QuestEpilogue, 0, sizeof(e.QuestEpilogue));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("004e795a_TEST FAIL %d\n", fail); return 1; }
    std::printf("004e795a_TEST PASS\n");
    return 0;
}