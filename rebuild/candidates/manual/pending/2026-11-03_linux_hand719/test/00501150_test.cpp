#include <cstdio>
#include <vector>
// Model of CWorldMap::LoadLevelFromFiles: one CLevelFiles entry (id + three files), the object is set loading with the
// map's loadable sections and handed to the level loader; a type-0 (synchronous) load runs the loader until it is idle.
struct Files { long LevelID; int Topology, Navigation, Thing; };
struct Object { std::vector<Files> files; int sections; long region; };
struct Loader {
    std::vector<Object> objects; int pending, runs;
    void Add(const Object& o) { objects.push_back(o); pending += 3; }
    bool IsBusy() const { return pending > 0; }
    void Run() { --pending; ++runs; }
};
static void LoadLevelFromFiles(Loader& loader, long id, int topology, int navigation, int thing, int loadType)
{
    Object object;
    std::vector<Files> levelFiles(1);
    levelFiles[0].LevelID = id;
    levelFiles[0].Navigation = navigation;
    levelFiles[0].Thing = thing;
    levelFiles[0].Topology = topology;
    object.files = levelFiles; object.sections = 7; object.region = 0;
    loader.Add(object);
    while (loadType == 0 && loader.IsBusy())
        loader.Run();
}
int main()
{
    Loader a = { std::vector<Object>(), 0, 0 }, b = { std::vector<Object>(), 0, 0 };
    LoadLevelFromFiles(a, 12, 1, 2, 3, 0);
    LoadLevelFromFiles(b, 13, 4, 5, 6, 1);
    const Files& f = a.objects[0].files[0];
    if (a.runs != 3 || a.IsBusy() || f.LevelID != 12 || f.Topology != 1 || f.Navigation != 2 || f.Thing != 3) { std::printf("00501150_TEST FAIL\n"); return 1; }
    if (b.runs != 0 || !b.IsBusy() || b.objects.size() != 1) { std::printf("00501150_TEST FAIL\n"); return 1; }
    std::printf("00501150_TEST PASS\n"); return 0;
}
