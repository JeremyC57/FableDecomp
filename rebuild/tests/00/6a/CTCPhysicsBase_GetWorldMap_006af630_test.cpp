// Behaviour test for 0x006AF630 CTCPhysicsBase::GetWorldMap() const (re-identified; was _unattributed::DisplayEnginePredicate13):
// the const world's virtual PeekWorldMap (CWorld slot 13) through CTCBase::PeekWorld().
#include <cstdio>
class CWorldMap { public: int Id; };
static CWorldMap g_map = { 9 };
static int g_peekWorldCalls = 0;
static int g_peekWorldMapCalls = 0;
class CWorld {
public:
    virtual ~CWorld() { }
    virtual const CWorldMap& PeekWorldMap() const { ++g_peekWorldMapCalls; return g_map; }
};
static CWorld g_world;
class CTCBase {
public:
    const CWorld& PeekWorld() const { ++g_peekWorldCalls; return g_world; }
};
class CTCPhysicsBase : public CTCBase {
public:
    const CWorldMap& GetWorldMap() const { return PeekWorld().PeekWorldMap(); }
};
int main()
{
    const CTCPhysicsBase physics = CTCPhysicsBase();
    const CWorldMap& map = physics.GetWorldMap();
    if (&map == &g_map && map.Id == 9 && g_peekWorldCalls == 1 && g_peekWorldMapCalls == 1) {
        std::printf("006af630_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
