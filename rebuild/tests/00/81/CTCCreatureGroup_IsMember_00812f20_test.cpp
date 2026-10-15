#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct CThing {
    u64 UniqueID;
    unsigned char Dead : 1;
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
};
struct SMemberNode { u64 Key; };
static SMemberNode g_members[2];
static int g_finds = 0;
struct CGroupMemberMap {
    SMemberNode* Header; unsigned long Count;
    SMemberNode* FindNode(const u64& key) const
    {
        ++g_finds;
        for (int i = 0; i < 2; ++i) if (g_members[i].Key == key) return &g_members[i];
        return Header;
    }
};
struct CTCCreatureGroup {
    CGroupMemberMap Members;
    bool IsMember(const CThing* thing) const
    {
        if (thing == 0)
            return false;
        if (thing->Dead)
            return false;
        u64 id = thing->UniqueID;
        return Members.FindNode(id) != Members.Header;
    }
};
static void Init(CThing& t, u64 id) { t.UniqueID = id; t.Dead = 0; t.CreatedByOwnerThingCreation = 0; t.IncludeInThingScans = 1;
    t.Activated = 1; t.GloballyLoaded = 0; t.InLimbo = 0; t.KillLocked = 0; t.CreatedBySerialisation = 0; }
int main()
{
    SMemberNode header = { 0 };
    g_members[0].Key = 0x200000005ULL; g_members[1].Key = 9;
    CTCCreatureGroup g; g.Members.Header = &header; g.Members.Count = 2;
    CThing in, out; Init(in, 0x200000005ULL); Init(out, 5);
    if (g.IsMember(0) || g_finds != 0) { std::printf("00812f20_TEST FAIL\n"); return 1; }
    if (!g.IsMember(&in) || g.IsMember(&out) || g_finds != 2) { std::printf("00812f20_TEST FAIL\n"); return 1; }
    in.Dead = 1;
    if (g.IsMember(&in) || g_finds != 2) { std::printf("00812f20_TEST FAIL\n"); return 1; }
    std::printf("00812f20_TEST PASS\n");
    return 0;
}