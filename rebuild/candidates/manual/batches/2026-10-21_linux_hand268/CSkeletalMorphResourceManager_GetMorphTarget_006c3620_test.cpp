#include <cstdio>
#include <cstring>
// Behaviour model of CSkeletalMorphResourceManager::GetMorphTarget (0x006C3620): the key is the full path
// "Data\Bones\<name>"; the first request inserts an empty entry and creates the target, loading it when the file
// exists; later requests return the same entry without reloading.
struct Target { int loads; };
struct Entry { char key[64]; Target* target; };
struct Manager {
    Entry entries[8]; int count;
    Target pool[8]; int created;
    const char* existing;
    Entry* find(const char* key) { for (int i = 0; i < count; ++i) if (std::strcmp(entries[i].key, key) == 0) return &entries[i]; return 0; }
    Target*& GetMorphTarget(const char* name)
    {
        char path[64];
        std::strcpy(path, "Data\\Bones\\");
        std::strcat(path, name);
        Entry* e = find(path);
        if (!e) {
            std::strcpy(entries[count].key, path); entries[count].target = 0; ++count;
            e = find(path);
        }
        Target*& target = e->target;
        if (target == 0) {
            target = &pool[created++]; target->loads = 0;
            if (existing && std::strcmp(existing, path) == 0)
                ++target->loads;
        }
        return target;
    }
};
int main()
{
    Manager m; m.count = 0; m.created = 0; m.existing = "Data\\Bones\\hero_fat";
    Target*& a = m.GetMorphTarget("hero_fat");
    bool ok = a != 0 && a->loads == 1 && m.count == 1 && std::strcmp(m.entries[0].key, "Data\\Bones\\hero_fat") == 0;
    Target*& b = m.GetMorphTarget("hero_fat");
    ok = ok && &a == &b && b->loads == 1 && m.created == 1;
    Target*& c = m.GetMorphTarget("hero_thin");
    ok = ok && c != a && c->loads == 0 && m.count == 2 && m.created == 2;
    if (!ok) { std::printf("006c3620_TEST FAIL\n"); return 1; }
    std::printf("006c3620_TEST PASS\n");
    return 0;
}
