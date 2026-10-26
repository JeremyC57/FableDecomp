// Behaviour test for CTCBase::GetWorld() const (0x00686A70): returns the thing manager's world (+0x24) by mutable reference.
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
    CWorld& GetWorld() const;
public:
    CWorld& Probe() const { return GetWorld(); }
};

CWorld& CTCBase::GetWorld() const
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
    const CTCBase tc = CTCBase();
    CWorld& w = tc.Probe();
    w.Id = 78;
    if (&w == &world && world.Id == 78 && sizeof(CThingManager) == 0x28) {
        std::printf("PASS_00686a70\n");
        return 0;
    }
    std::printf("FAIL_00686a70\n");
    return 1;
}
