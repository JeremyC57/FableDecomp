#include <cstdio>
#include <cstring>
// Behaviour model of CSkeletalMorphResourceManager::PreloadBoneConfigs (0x006C37D0): every *.bncfg file the
// finder returns is looked up in the morph map; only names that are not yet present are inserted and loaded.
struct Entry { char key[64]; int loads; };
struct Manager {
    Entry entries[8]; int count;
    Entry* find(const char* key) { for (int i = 0; i < count; ++i) if (std::strcmp(entries[i].key, key) == 0) return &entries[i]; return 0; }
    void PreloadBoneConfigs(const char* const* files, int fileCount)
    {
        int index = 0;
        if (index < fileCount) {
            do {
                const char* name = files[index];
                Entry* node = find(name);
                if (node == 0) {
                    std::strcpy(entries[count].key, name); entries[count].loads = 0; ++count;
                    node = find(name);
                    ++node->loads;
                }
                ++index;
            } while (index < fileCount);
        }
    }
};
int main()
{
    Manager m; m.count = 0;
    std::strcpy(m.entries[0].key, "hero.bncfg"); m.entries[0].loads = 7; m.count = 1;
    const char* files[3] = { "hero.bncfg", "troll.bncfg", "balverine.bncfg" };
    m.PreloadBoneConfigs(files, 3);
    bool ok = m.count == 3 && m.entries[0].loads == 7 && m.entries[1].loads == 1 && m.entries[2].loads == 1
        && std::strcmp(m.entries[2].key, "balverine.bncfg") == 0;
    m.PreloadBoneConfigs(files, 0);
    ok = ok && m.count == 3;
    m.PreloadBoneConfigs(files, 3);
    ok = ok && m.count == 3 && m.entries[1].loads == 1;
    if (!ok) { std::printf("006c37d0_TEST FAIL\n"); return 1; }
    std::printf("006c37d0_TEST PASS\n");
    return 0;
}
