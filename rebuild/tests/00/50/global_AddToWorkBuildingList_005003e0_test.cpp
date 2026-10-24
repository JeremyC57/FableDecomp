// Behaviour test for 0x005003E0 AddToWorkBuildingList(creature, buildings, building): the building's assignee list
// grows by the creature while size + 1 stays below the sim definition's WorkSpaces; when the next one would fill it
// the building is erased from the candidate map instead; a building without a sim definition is left alone. The
// definition reference taken for the test is released again.
#include <cstdio>

struct SimDefModel {
    long RefCount;
    long WorkSpaces;
};

struct BuildingModel {
    SimDefModel* Def;
    SimDefModel* PeekPSimDefinition() const { if (Def) ++Def->RefCount; return Def; }
};

struct EntryModel {
    BuildingModel* Building;
    void* Creatures[8];
    unsigned int Count;
    bool Present;
};

static void model(void* creature, EntryModel& entry)
{
    EntryModel* it = &entry;                       // find(building): assumed present
    SimDefModel* def = it->Building->PeekPSimDefinition();
    if (def) {
        if (it->Count + 1 < (unsigned int)def->WorkSpaces)
            it->Creatures[it->Count++] = creature;
        else
            it->Present = false;                   // buildings.erase(it)
        if (--def->RefCount == 0) def->RefCount = -100;   // the release (slot-1 destroy on zero)
    }
}

int main()
{
    SimDefModel def = { 1, 3 };
    BuildingModel b = { &def };
    EntryModel e = { &b, { 0 }, 0, true };
    int c1, c2, c3;
    model(&c1, e);
    model(&c2, e);
    if (e.Count != 2 || !e.Present || e.Creatures[1] != &c2 || def.RefCount != 1) { std::printf("005003e0_TEST FAIL\n"); return 1; }
    model(&c3, e);                                 // 2 + 1 == WorkSpaces: the building is full
    if (e.Count != 2 || e.Present || def.RefCount != 1) { std::printf("005003e0_TEST FAIL\n"); return 1; }
    BuildingModel none = { 0 };
    EntryModel e2 = { &none, { 0 }, 0, true };
    model(&c1, e2);
    if (e2.Count != 0 || !e2.Present) { std::printf("005003e0_TEST FAIL\n"); return 1; }
    std::printf("005003e0_TEST PASS\n");
    return 0;
}