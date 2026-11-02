// Behaviour test for CMap::Update (0x00820B50): height map updated, both changed-area counts zeroed, brushes advanced last.
#include <cstdio>
static int order[8]; static int n = 0;
class CHeightMap {
    friend class CMap;
    void Update() { order[n++] = 1; }
};
class CMap {
public:
    CHeightMap* HeightMap;
    long NoChangedHeightAreas;
    long NoChangedGameAreas;
    virtual ~CMap() { }
    void UpdateScriptedMapBrushes() { order[n++] = 2; seenH = NoChangedHeightAreas; seenG = NoChangedGameAreas; }
    long seenH, seenG;
    virtual void Update();
};
void CMap::Update()
{
    HeightMap->Update();
    NoChangedHeightAreas = 0;
    NoChangedGameAreas = 0;
    UpdateScriptedMapBrushes();
}
int main()
{
    CHeightMap h;
    CMap m;
    m.HeightMap = &h;
    m.NoChangedHeightAreas = 7;
    m.NoChangedGameAreas = 3;
    m.Update();
    if (n != 2 || order[0] != 1 || order[1] != 2 || m.seenH != 0 || m.seenG != 0 || m.NoChangedHeightAreas != 0) {
        std::printf("00820b50_TEST FAIL\n"); return 1;
    }
    std::printf("00820b50_TEST PASS\n");
    return 0;
}