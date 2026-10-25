// Behaviour test for CThingManager::RuntimeSaveGameHero (0x00523B40): nothing happens without a living hero; the
// binary form stores the stream bytes in game-persistent slot 1 and clears the slot's script, the script form stores
// the text and clears the binary data; the serialising flag is set only while the hero is saved -- through a copy of
// the body over recording stand-ins.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

struct Thing { bool Dead; int id; };

struct Persistence {
    int dataIndex, scriptIndex;
    std::string data, script;
    bool dataCleared;
    Persistence() : dataIndex(-1), scriptIndex(-1), dataCleared(false) { }
    void SetGamePersistentSaveData(int index, const void* bytes, unsigned int length)
    {
        dataIndex = index;
        dataCleared = (bytes == 0 && length == 0);
        data = bytes ? std::string((const char*)bytes, length) : std::string();
    }
    void SetGamePersistentScript(int index, const std::string& text) { scriptIndex = index; script = text; }
};

struct Manager {
    bool SerialisingGameState;
    bool flagSeenDuringSave;
    Persistence EntityRuntimePersistence;
    Manager() : SerialisingGameState(false), flagSeenDuringSave(false) { }
    void SaveEntitiesBinary(std::string& stream, int mode, const std::vector<Thing*>& things, std::vector<std::string>& names)
    {
        flagSeenDuringSave = SerialisingGameState;
        for (unsigned int i = 0; i < things.size(); ++i) { stream += (char)('0' + things[i]->id); names.push_back("hero"); }
        stream += (char)('0' + mode);
    }
    void SaveEntitiesToScript(std::string& script, int mode, const std::vector<Thing*>& things, std::vector<std::string>& names)
    {
        flagSeenDuringSave = SerialisingGameState;
        for (unsigned int i = 0; i < things.size(); ++i) { script += "Hero;"; names.push_back("hero"); }
        script += (char)('0' + mode);
    }
    void RuntimeSaveGameHero(Thing* hero, bool binary)
    {
        if (hero != 0 && !hero->Dead) {
            std::vector<Thing*> things;
            things.push_back(hero);
            std::vector<std::string> names;
            SerialisingGameState = true;
            if (binary) {
                std::string stream;
                stream.reserve(0x2000);
                SaveEntitiesBinary(stream, 2, things, names);
                EntityRuntimePersistence.SetGamePersistentSaveData(1, stream.data(), (unsigned int)stream.size());
                std::string noScript("");
                EntityRuntimePersistence.SetGamePersistentScript(1, noScript);
            } else {
                std::string script;
                script.reserve(0x8000);
                SaveEntitiesToScript(script, 2, things, names);
                EntityRuntimePersistence.SetGamePersistentScript(1, script);
                EntityRuntimePersistence.SetGamePersistentSaveData(1, 0, 0);
            }
            SerialisingGameState = false;
        }
    }
};

int main()
{
    Manager none;
    none.RuntimeSaveGameHero(0, true);
    Thing dead = { true, 7 };
    none.RuntimeSaveGameHero(&dead, true);
    if (none.EntityRuntimePersistence.dataIndex != -1 || none.EntityRuntimePersistence.scriptIndex != -1) {
        std::printf("00523b40_TEST FAIL\n");
        return 1;
    }
    Thing hero = { false, 7 };
    Manager binary;
    binary.RuntimeSaveGameHero(&hero, true);
    if (binary.EntityRuntimePersistence.dataIndex != 1 || binary.EntityRuntimePersistence.data != "72" ||
        binary.EntityRuntimePersistence.scriptIndex != 1 || !binary.EntityRuntimePersistence.script.empty() ||
        !binary.flagSeenDuringSave || binary.SerialisingGameState) {
        std::printf("00523b40_TEST FAIL\n");
        return 1;
    }
    Manager text;
    text.RuntimeSaveGameHero(&hero, false);
    if (text.EntityRuntimePersistence.script != "Hero;2" || !text.EntityRuntimePersistence.dataCleared ||
        text.EntityRuntimePersistence.dataIndex != 1 || !text.flagSeenDuringSave || text.SerialisingGameState) {
        std::printf("00523b40_TEST FAIL\n");
        return 1;
    }
    std::printf("00523b40_TEST PASS\n");
    return 0;
}