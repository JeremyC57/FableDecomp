#include <cstdio>
// Behaviour test for CDrawMapInfo::CDrawMapInfo(const CDrawMapInfo&) (0x0050B490): every member copied,
// the name through its copy constructor.
static int g_copies = 0;
class CCharString { public: const char* p; CCharString(const char* s) : p(s) { } CCharString(const CCharString& o) : p(o.p) { ++g_copies; } };
class CDrawMapInfo { public:
    bool Valid, Loadable, GameLoadable; void* PMap; CCharString StaticMeshName;
    long PosX, PosY; unsigned long UID; long ExtentsX, ExtentsY;
    CDrawMapInfo(const char* n) : StaticMeshName(n) { }
    CDrawMapInfo(const CDrawMapInfo& o)
        : Valid(o.Valid), Loadable(o.Loadable), GameLoadable(o.GameLoadable), PMap(o.PMap), StaticMeshName(o.StaticMeshName),
          PosX(o.PosX), PosY(o.PosY), UID(o.UID), ExtentsX(o.ExtentsX), ExtentsY(o.ExtentsY) { }
};
int main()
{
    int map;
    CDrawMapInfo a("mesh");
    a.Valid = true; a.Loadable = false; a.GameLoadable = true; a.PMap = &map;
    a.PosX = 3; a.PosY = -4; a.UID = 99; a.ExtentsX = 10; a.ExtentsY = 20;
    CDrawMapInfo b(a);
    if (!b.Valid || b.Loadable || !b.GameLoadable || b.PMap != &map || b.StaticMeshName.p != a.StaticMeshName.p || g_copies != 1 ||
        b.PosX != 3 || b.PosY != -4 || b.UID != 99 || b.ExtentsX != 10 || b.ExtentsY != 20) { std::printf("0050b490_TEST FAIL\n"); return 1; }
    std::printf("0050b490_TEST PASS\n");
    return 0;
}