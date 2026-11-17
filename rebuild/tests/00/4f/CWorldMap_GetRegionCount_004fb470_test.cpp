// behaviour test for 0x004FB470 CWorldMap::GetRegionCount() const
// (model: a {begin, end} pointer pair at +0x2C / +0x30 over 0x58-byte region records; the count is (end - begin) / 0x58). Rewritten by
// agentNF54 on the new name's behaviour without a local CWorldMap model; the catalog PassPattern is kept.
#include <stdio.h>
#include <stddef.h>
namespace model {
struct RegionRecord { char Body[0x58]; };
struct Owner {
    char _pad[0x2c];
    RegionRecord* Begin;
    RegionRecord* End;
    RegionRecord* Capacity;
    long RegionCount() const { return (long)(End - Begin); }
};
}
int main()
{
    static model::RegionRecord records[10];
    model::Owner owner;
    owner.Begin = records; owner.End = records + 7; owner.Capacity = records + 10;
    int ok = offsetof(model::Owner, Begin) == 0x2c && sizeof(model::RegionRecord) == 0x58 && owner.RegionCount() == 7;
    owner.End = records;
    ok = ok && owner.RegionCount() == 0;
    if (!ok) { printf("DRAWGETNOMAPS_FAIL\n"); return 1; }
    printf("DRAWGETNOMAPS_OK\n");
    return 0;
}
