// Behaviour test for CEngineScene::GetMapAt (0x00B94BF0), a portable model of the row. The cell is the floor of the world position
// times the inverse map size. The first call for a cell creates one map over the box (cell * MapSize, extents MapSize, MapSize) with the
// scene's level limit; later calls for the same cell return that same map.
#include <stdio.h>
#include <map>
#include <math.h>
struct SceneMap { float TLX, TLY, BRX, BRY; unsigned long MaxLevels; };
struct Scene {
    float MapSize, OneOverMapSize; unsigned long MaxMapLevels; int created;
    std::map<long, std::map<long, SceneMap*> > MapList;
    SceneMap& GetMapAt(float x, float y)
    {
        long cx = (long)floor(x * OneOverMapSize), cy = (long)floor(y * OneOverMapSize);
        SceneMap*& map_ptr = MapList[cy][cx];
        if (!map_ptr)
        {
            SceneMap* m = new SceneMap;
            m->TLX = cx * MapSize; m->TLY = cy * MapSize; m->BRX = m->TLX + MapSize; m->BRY = m->TLY + MapSize;
            m->MaxLevels = MaxMapLevels; map_ptr = m; ++created;
        }
        return *map_ptr;
    }
};
int main()
{
    int bad = 0;
    Scene s; s.MapSize = 64.0f; s.OneOverMapSize = 1.0f / 64.0f; s.MaxMapLevels = 5; s.created = 0;
    SceneMap& a = s.GetMapAt(10.0f, 70.0f);
    if (a.TLX != 0.0f || a.TLY != 64.0f || a.BRX != 64.0f || a.BRY != 128.0f || a.MaxLevels != 5) ++bad;
    SceneMap& b = s.GetMapAt(63.5f, 127.0f);                               // same cell: same map, nothing created
    if (&a != &b || s.created != 1) ++bad;
    SceneMap& c = s.GetMapAt(-1.0f, 0.0f);                                 // floor of a negative coordinate
    if (c.TLX != -64.0f || c.TLY != 0.0f || s.created != 2) ++bad;
    printf(bad ? "00b94bf0_TEST FAIL\n" : "00b94bf0_TEST PASS\n");
    return bad ? 1 : 0;
}
