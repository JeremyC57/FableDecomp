#include <cstdio>
#include <cstring>
struct CPersistContext {
    bool saving; bool stored; bool loadedValue; int baseCalls; char lastName[16];
    bool IsSaving() { return saving; }
    void Transfer(const char* name, bool& value, const bool& defaultValue) { std::strcpy(lastName, name); stored = value; (void)defaultValue; }
    void Transfer(const char* name, bool& value) { std::strcpy(lastName, name); value = loadedValue; }
};
struct Base { void OnSerialise(CPersistContext& context, bool) { ++context.baseCalls; } };
struct Clothing : Base {
    bool LoadedFromSaveGame;
    void OnSerialise(CPersistContext& context, bool inGameSave)
    {
        Base::OnSerialise(context, inGameSave);
        if (context.IsSaving())
            context.Transfer("InGameSave", inGameSave, false);
        else
            context.Transfer("InGameSave", LoadedFromSaveGame);
    }
};
int main()
{
    CPersistContext c; c.baseCalls = 0; c.stored = false; c.saving = true; c.loadedValue = false;
    Clothing cl; cl.LoadedFromSaveGame = false;
    cl.OnSerialise(c, true);
    if (!c.stored || c.baseCalls != 1 || std::strcmp(c.lastName, "InGameSave") || cl.LoadedFromSaveGame) { std::printf("005b369a_TEST FAIL\n"); return 1; }
    c.saving = false; c.loadedValue = true; cl.OnSerialise(c, false);
    if (!cl.LoadedFromSaveGame || c.baseCalls != 2) { std::printf("005b369a_TEST FAIL\n"); return 1; }
    std::printf("005b369a_TEST PASS\n");
    return 0;
}
