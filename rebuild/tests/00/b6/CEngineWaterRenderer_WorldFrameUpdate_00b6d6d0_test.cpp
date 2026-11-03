// Behaviour test for 0x00B6D6D0 CEngineWaterRenderer::WorldFrameUpdate: the call reaches the embedded settings object (not the
// renderer) with the force flag unchanged, through the virtual slot.
#include <cstdio>
static const void* gSettings = 0;
static int gForce = -1;
class CEngineComponent { public:
    virtual ~CEngineComponent() { }
    virtual void WorldFrameUpdate(bool force) = 0;
    void** GlobalPointerAddress; bool RenderEnabled; unsigned char KeepAlive;
};
class CEngineWaterSettings { public:
    int Dummy;
    void WorldFrameUpdate(bool force) { gSettings = this; gForce = force ? 1 : 0; }
};
class CEngineWaterRenderer : public CEngineComponent { public:
    unsigned char Before[0x274];
    CEngineWaterSettings Settings;
    virtual void WorldFrameUpdate(bool force);
};
void CEngineWaterRenderer::WorldFrameUpdate(bool force)
{
    Settings.WorldFrameUpdate(force);
}
int main()
{
    CEngineWaterRenderer r;
    CEngineComponent* c = &r;
    c->WorldFrameUpdate(true);
    if (gSettings != &r.Settings || gForce != 1) { std::printf("00b6d6d0_TEST FAIL\n"); return 1; }
    c->WorldFrameUpdate(false);
    if (gForce != 0) { std::printf("00b6d6d0_TEST FAIL\n"); return 1; }
    std::printf("00b6d6d0_TEST PASS\n");
    return 0;
}