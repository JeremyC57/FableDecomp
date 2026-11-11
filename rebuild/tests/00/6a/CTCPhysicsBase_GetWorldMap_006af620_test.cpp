// Behaviour test for 0x006AF620 CTCPhysicsBase::GetWorldMap() (re-identified; was CThingTrackNode::DrawIsDrawable): the world's
// virtual GetWorldMap (CWorld slot 12) through CTCBase::GetWorld().
#include <cstdio>
class CWorldMap { public: int Id; };
static CWorldMap g_map = { 7 };
static int g_getWorldCalls = 0;
static int g_getWorldMapCalls = 0;
class CWorld {
public:
    virtual ~CWorld() { }
    virtual CWorldMap& GetWorldMap() { ++g_getWorldMapCalls; return g_map; }
};
static CWorld g_world;
class CTCBase {
public:
    CWorld& GetWorld() const { ++g_getWorldCalls; return g_world; }
};
class CTCPhysicsBase : public CTCBase {
public:
    CWorldMap& GetWorldMap() { return GetWorld().GetWorldMap(); }
};
int main()
{
    CTCPhysicsBase physics;
    CWorldMap& map = physics.GetWorldMap();
    if (&map == &g_map && map.Id == 7 && g_getWorldCalls == 1 && g_getWorldMapCalls == 1) {
        std::printf("DRAWISDRAWABLE_FORWARD_OK\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
