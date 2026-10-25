// Behaviour test for 0x006FAA90 NAnimationEvents::InitialiseManagers(const CMeshDataBank&): two fresh managers bound to
// the bank, built from <def dir>game_animation_events.h (embedded events on) and sound_animation_events.h (off), with
// empty extra-event maps and a clean saved-copy flag; Reset releases whatever the counted pointers held before
#include <cstdio>
#include <cstring>

namespace model {

struct CMeshDataBank { int Id; };
int g_live = 0;

struct CManager {
    const CMeshDataBank* MeshBank;
    char  SymbolFile[64];
    void* ExtraFirst;
    void* ExtraLast;
    void* ExtraEnd;
    bool  ExtraDirty;
    bool  DifferentToSavedCopy;
    bool  UseEmbeddedEvents;
    CManager(const CMeshDataBank& bank, const char* fileName, bool useEmbeddedEvents)
        : MeshBank(&bank), ExtraFirst(0), ExtraLast(0), ExtraEnd(0), ExtraDirty(false), DifferentToSavedCopy(false),
          UseEmbeddedEvents(useEmbeddedEvents)
    {
        std::strcpy(SymbolFile, fileName);            // EventSymbolMap.LoadFromFile(fileName)
        ++g_live;
    }
    virtual ~CManager() { --g_live; }
};

struct CCountedPointer {                              // sole owner here: Reset deletes the old object
    CManager* Data;
    void Reset(CManager* p) { if (Data != p) { delete Data; Data = p; } }
};
CCountedPointer GameEventManager = { 0 };
CCountedPointer SoundEventManager = { 0 };

const char* GetDefDir() { return "data/defs/"; }

void InitialiseManagers(const CMeshDataBank& bank)
{
    char path[128];
    std::strcpy(path, GetDefDir());
    std::strcat(path, "game_animation_events.h");
    GameEventManager.Reset(new CManager(bank, path, true));
    std::strcpy(path, GetDefDir());
    std::strcat(path, "sound_animation_events.h");
    SoundEventManager.Reset(new CManager(bank, path, false));
}

}

int main()
{
    using namespace model;
    int bad = 0;
    CMeshDataBank bank1 = { 1 }, bank2 = { 2 };
    InitialiseManagers(bank1);
    CManager* g = GameEventManager.Data;
    CManager* s = SoundEventManager.Data;
    if (!g || !s || g == s || g_live != 2) bad = 1;
    else {
        if (g->MeshBank != &bank1 || s->MeshBank != &bank1) bad = 2;
        if (std::strcmp(g->SymbolFile, "data/defs/game_animation_events.h") != 0) bad = 3;
        if (std::strcmp(s->SymbolFile, "data/defs/sound_animation_events.h") != 0) bad = 4;
        if (!g->UseEmbeddedEvents || s->UseEmbeddedEvents) bad = 5;
        if (g->DifferentToSavedCopy || s->DifferentToSavedCopy || g->ExtraFirst || s->ExtraEnd || g->ExtraDirty) bad = 6;
    }
    InitialiseManagers(bank2);                        // re-initialising replaces both managers
    if (g_live != 2 || GameEventManager.Data->MeshBank != &bank2 || SoundEventManager.Data->MeshBank != &bank2) bad = 7;
    GameEventManager.Reset(0);
    SoundEventManager.Reset(0);
    if (g_live != 0) bad = 8;
    if (bad) { std::printf("006faa90_TEST FAIL\n"); return 1; }
    std::printf("006faa90_TEST PASS\n");
    return 0;
}