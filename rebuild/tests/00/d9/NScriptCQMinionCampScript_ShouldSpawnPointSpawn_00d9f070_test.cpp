// Behaviour test for CQ_MinionCampScript::ShouldSpawnPointSpawn (0x00D9F070): true only while spawning is on, the
// spawn point is the chosen one, and the script thread is not terminating.
#include <cstdio>

class CScriptThing {
public:
    int id;
    bool operator==(const CScriptThing& other) const { return id == other.id; }
};
struct CQ_MinionCampScript {
    bool terminating;
    bool ShouldSpawn;
    CScriptThing SpawnPointToSpawn;
    bool IsActiveThreadTerminating() { return terminating; }
    bool ShouldSpawnPointSpawn(const CScriptThing& spawnPoint)
    {
        if (ShouldSpawn) {
            if (spawnPoint == SpawnPointToSpawn) {
                if (IsActiveThreadTerminating())
                    return false;
                return true;
            }
        }
        return false;
    }
};

int main()
{
    CQ_MinionCampScript s;
    s.SpawnPointToSpawn.id = 5;
    CScriptThing chosen; chosen.id = 5;
    CScriptThing other; other.id = 6;
    int bad = 0;
    s.ShouldSpawn = true; s.terminating = false;
    if (!s.ShouldSpawnPointSpawn(chosen)) ++bad;
    if (s.ShouldSpawnPointSpawn(other)) ++bad;
    s.terminating = true;
    if (s.ShouldSpawnPointSpawn(chosen)) ++bad;
    s.ShouldSpawn = false; s.terminating = false;
    if (s.ShouldSpawnPointSpawn(chosen)) ++bad;
    if (bad) {
        std::printf("00d9f070_TEST FAIL\n");
        return 1;
    }
    std::printf("00d9f070_TEST PASS\n");
    return 0;
}