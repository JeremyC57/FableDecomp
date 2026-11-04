// Behaviour test for 0x005EBFA0 CTCTavernGame::CameraUseCameraPoint(const CCharString&): a model; checks nothing happens without a live scripted camera point of that script name, and the camera manager is given a tavern-game module from the point start position to its look position otherwise.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct Point { std::string name; bool alive, isCamera; int type; float start, look; };
static int g_set; static float g_from, g_to;
static void Use(const std::vector<Point>& world, const std::string& name)
{
    const Point* p = 0; for (size_t i = 0; i < world.size(); ++i) if (world[i].name == name) { p = &world[i]; break; }
    if (!p || !p->alive || !p->isCamera || p->type != 6) return;
    ++g_set; g_from = p->start; g_to = p->look;
}
int main()
{
    int ok = 1; std::vector<Point> w; Point a = { "CAM", true, true, 6, 1, 2 }, b = { "OTHER", true, true, 5, 3, 4 };
    w.push_back(a); w.push_back(b);
    Use(w, "NONE"); Use(w, "OTHER"); ok &= g_set == 0;
    Use(w, "CAM"); ok &= g_set == 1 && g_from == 1 && g_to == 2;
    std::printf(ok ? "005EBFA0_TEST PASS\n" : "005EBFA0_TEST FAIL\n"); return ok ? 0 : 1;
}
