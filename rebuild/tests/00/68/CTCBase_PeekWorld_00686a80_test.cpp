#include <cstdio>

class CWorld { public: int Id; };

class CThingManager {
public:
    unsigned char _unknown[0x24];
    CWorld* World;
};

CThingManager* g_pThingManager;

class CTCBase {
protected:
    const CWorld& PeekWorld() const;
public:
    const CWorld& Probe() const { return PeekWorld(); }
};

const CWorld& CTCBase::PeekWorld() const
{
    return *g_pThingManager->World;
}

int main()
{
    static CWorld world;
    static CThingManager mgr;
    world.Id = 77;
    mgr.World = &world;
    g_pThingManager = &mgr;
    CTCBase tc;
    const CWorld& w = tc.Probe();
    if (&w == &world && w.Id == 77 && sizeof(CThingManager) == 0x28) {
        std::printf("00686a80_TEST PASS\n");
        return 0;
    }
    std::printf("00686a80_TEST FAIL\n");
    return 1;
}
