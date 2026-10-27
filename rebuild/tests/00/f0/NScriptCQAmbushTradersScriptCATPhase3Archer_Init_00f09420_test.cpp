#include <cstdio>
#include <cstring>
#include <string>
// Behaviour model: the spawned thread is named "ParentClass.<F>", bound to the parent and carries a copy of the thing.
struct Thing { int* refs; Thing(int* r) : refs(r) { ++*refs; } Thing(const Thing& o) : refs(o.refs) { ++*refs; } ~Thing() { --*refs; } };
struct Spawned { std::string name; void* parent; Thing par1; Spawned(const std::string& n, void* p, Thing t) : name(n), parent(p), par1(t) { } };
int main()
{
    int refs = 0; Thing me(&refs); int parent = 0;
    Spawned* s = new Spawned(std::string("ParentClass.") + std::string("WatchForEnemyDeath"), &parent, me);
    if (s->name != "ParentClass.WatchForEnemyDeath" || s->parent != &parent || refs != 2) { std::printf("00f09420_TEST FAIL\n"); return 1; }
    delete s;
    if (refs != 1) { std::printf("00f09420_TEST FAIL\n"); return 1; }
    std::printf("00f09420_TEST PASS\n");
    return 0;
}