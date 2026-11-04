// Behaviour test for CThingManager::RuntimeSaveEntities 0x005239C0: game-persistent sections are stored as binary (script form cleared)
// when the binary-save switch is on, otherwise as script text (binary form cleared); level-persistent sections are stored as
// script text only; the serialising flag is raised only while saving.
#include <cstdio>
#include <string>
#include <vector>
struct CThing { int id; };
static bool g_bRuntimeSaveBinary = true;
struct CEntityRuntimePersistence {
    std::string gameScript[4], levelScript[4], gameData[4];
    void SetGamePersistentSaveData(long i, const char* data, unsigned long len) { gameData[i] = data ? std::string(data, len) : std::string(); }
    void SetGamePersistentScript(long i, const std::string& s) { gameScript[i] = s; }
    void SetLevelPersistentScript(long i, const std::string& s) { levelScript[i] = s; }
};
struct CThingManager {
    bool SerialisingGameState;
    bool sawFlag;
    CEntityRuntimePersistence EntityRuntimePersistence;
    void SaveEntitiesToScript(std::string& script, long, const std::vector<CThing*>& things, std::vector<std::string>& names)
    { sawFlag = SerialisingGameState; for (size_t i = 0; i < things.size(); ++i) { script += "T"; names.push_back("s"); } }
    void SaveEntitiesBinary(std::string& stream, long, const std::vector<CThing*>& things, std::vector<std::string>& names)
    { sawFlag = SerialisingGameState; for (size_t i = 0; i < things.size(); ++i) { stream += "B"; names.push_back("s"); } }
    void RuntimeSaveEntities(long section, const std::vector<CThing*>& things, bool gamePersistent)
    {
        std::vector<std::string> names;
        SerialisingGameState = true;
        if (gamePersistent) {
            if (g_bRuntimeSaveBinary) {
                std::string stream; SaveEntitiesBinary(stream, 2, things, names);
                EntityRuntimePersistence.SetGamePersistentSaveData(section, stream.data(), (unsigned long)stream.size());
                EntityRuntimePersistence.SetGamePersistentScript(section, "");
            } else {
                std::string script; SaveEntitiesToScript(script, 2, things, names);
                EntityRuntimePersistence.SetGamePersistentScript(section, script);
                EntityRuntimePersistence.SetGamePersistentSaveData(section, 0, 0);
            }
        } else {
            std::string script; SaveEntitiesToScript(script, 2, things, names);
            EntityRuntimePersistence.SetLevelPersistentScript(section, script);
        }
        SerialisingGameState = false;
    }
};
int main()
{
    CThing a = { 1 }, b = { 2 };
    std::vector<CThing*> things; things.push_back(&a); things.push_back(&b);
    CThingManager tm; tm.SerialisingGameState = false; tm.sawFlag = false;
    tm.EntityRuntimePersistence.gameScript[1] = "old";
    tm.RuntimeSaveEntities(1, things, true);
    if (tm.EntityRuntimePersistence.gameData[1] != "BB" || tm.EntityRuntimePersistence.gameScript[1] != "" || !tm.sawFlag || tm.SerialisingGameState) { std::printf("005239c0_TEST FAIL\n"); return 1; }
    g_bRuntimeSaveBinary = false;
    tm.RuntimeSaveEntities(1, things, true);
    if (tm.EntityRuntimePersistence.gameData[1] != "" || tm.EntityRuntimePersistence.gameScript[1] != "TT") { std::printf("005239c0_TEST FAIL\n"); return 1; }
    tm.RuntimeSaveEntities(2, things, false);
    if (tm.EntityRuntimePersistence.levelScript[2] != "TT" || tm.EntityRuntimePersistence.gameScript[2] != "") { std::printf("005239c0_TEST FAIL\n"); return 1; }
    std::printf("005239c0_TEST PASS\n");
    return 0;
}