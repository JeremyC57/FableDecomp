// Behaviour test for 0x004AF270 CQuestManager::PrepareQuestsWhenFinalQuestIsCompleted(): links the row object and calls it on raw
// CQuestManager / CWorld storage through the shared headers; checks that CWorld::MapTableShowQuestCardsOnUsed (+0xDA) is set and its
// neighbours are untouched. (NF108 2026-11-19: replaces the self-contained copy of the old DontPopulateNextLoadedRegion stand-in.)
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_PREPAREQUESTSWHENFINALQUESTISCOMPLETED
#define FABLE_RETAIL_CWORLD
#include "engine/CWorld.h"
#include "engine/CQuestManager.h"

int main()
{
    static unsigned char manager[0x200];
    static unsigned char world[0x200];
    std::memset(manager, 0, sizeof(manager));
    std::memset(world, 0x11, sizeof(world));
    world[0xda] = 0;
    void* world_pointer = world;
    std::memcpy(manager + 0x7c, &world_pointer, sizeof(world_pointer));
    reinterpret_cast<CQuestManager*>(manager)->PrepareQuestsWhenFinalQuestIsCompleted();
    bool ok = world[0xda] == 1 && world[0xd9] == 0x11 && world[0xdb] == 0x11;
    if (!ok) { std::printf("BAD %d\n", world[0xda]); return 1; }
    std::printf("OK_0x004af270\n");
    return 0;
}
