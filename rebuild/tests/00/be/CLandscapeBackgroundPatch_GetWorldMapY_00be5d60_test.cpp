// Behaviour test for 00be5d60 CLandscapeBackgroundPatch::GetWorldMapY: the patch's map-local Y plus the world position of
// the engine map its parent tree node's landscape map wraps (unsigned shorts widened to long).
#include <cstdio>
struct CEngineMap { unsigned short MapWidth, MapHeight, WorldPosX, WorldPosY; };
struct CEngineLandscapeMap { long PatchGridWidth; CEngineMap* Map; };
struct CLandscapeBackgroundTreeNode { CEngineLandscapeMap* Map; };
struct CLandscapeBackgroundPatch {
    CLandscapeBackgroundTreeNode* Parent;
    unsigned short MapX, MapY;
    long GetWorldMapX() { return Parent->Map->Map->WorldPosX + MapX; }
    long GetWorldMapY() { return Parent->Map->Map->WorldPosY + MapY; }
};
int main()
{
    CEngineMap em; em.MapWidth = 1; em.MapHeight = 2; em.WorldPosX = 60000; em.WorldPosY = 300;
    CEngineLandscapeMap lm; lm.PatchGridWidth = 0; lm.Map = &em;
    CLandscapeBackgroundTreeNode node; node.Map = &lm;
    CLandscapeBackgroundPatch p; p.Parent = &node; p.MapX = 60000; p.MapY = 12;
    if (p.GetWorldMapY() != ('Y' == 'X' ? 120000L : 312L)) { std::printf("00be5d60_TEST FAIL\n"); return 1; }
    std::printf("00be5d60_TEST PASS\n");
    return 0;
}