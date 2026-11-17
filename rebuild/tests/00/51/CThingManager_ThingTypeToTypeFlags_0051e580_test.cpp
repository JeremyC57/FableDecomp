// behaviour test for 0x0051E580 CThingManager::ThingTypeToTypeFlags(long) const
// (model: the manager holds a {first, last, end} array of 12-byte type-info records at +0x3C; the getter returns the dword at +4 of
// record `type`). Rewritten by agentNF54 on the new name's behaviour (the old test modelled the CEditWorldMap_GetEditMap stand-in
// through inline assembly); the catalog PassPattern is kept.
#include <stdio.h>
#include <stddef.h>
namespace model {
struct TypeRecord { long DefClassType; unsigned long Flags; long Extra; };
struct Manager {
    char _pad[0x3c];
    TypeRecord* First;
    TypeRecord* Last;
    TypeRecord* End;
    unsigned long TypeFlags(long type) const { return First[type].Flags; }
};
}
int main()
{
    model::TypeRecord records[3] = { { 1, 0x10u, 7 }, { 2, 0x22u, 8 }, { 3, 0x80000001u, 9 } };
    model::Manager manager;
    manager.First = records; manager.Last = records + 3; manager.End = records + 3;
    int ok = offsetof(model::Manager, First) == 0x3c && sizeof(model::TypeRecord) == 12 &&
             manager.TypeFlags(0) == 0x10u && manager.TypeFlags(1) == 0x22u && manager.TypeFlags(2) == 0x80000001u;
    if (!ok) { printf("FAIL_0x0051e580\n"); return 1; }
    printf("OK_0x0051e580\n");
    return 0;
}
