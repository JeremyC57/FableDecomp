// Behaviour test for CSoundManagerLH::CreateSoundBank (0x00A3E430): allocate a sound bank owned by this manager, load the
// named file into it (virtual Load, slot 1) and return it. Host model.
#include <cstdio>
#include <string>
struct Manager;
struct Bank { Manager* Owner; std::string Loaded; explicit Bank(Manager* owner) : Owner(owner) {} void Load(const std::string& f) { Loaded = f; } };
struct Manager { Bank* CreateSoundBank(const std::string& fileName) { Bank* result = new Bank(this); result->Load(fileName); return result; } };
int main()
{
    Manager m; Bank* b = m.CreateSoundBank("sounds.lug");
    bool ok = b != 0 && b->Owner == &m && b->Loaded == "sounds.lug";
    delete b;
    if (!ok) { std::printf("00a3e430_TEST FAIL\n"); return 1; }
    std::printf("00a3e430_TEST PASS\n");
    return 0;
}
