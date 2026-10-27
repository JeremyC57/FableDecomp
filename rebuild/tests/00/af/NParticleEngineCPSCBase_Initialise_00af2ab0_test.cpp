// Behaviour test for 0x00AF2AB0 NParticleEngine::CPSCBase::Initialise(...) -- sets the Initialised bit and returns it
#include <cstdio>
class CPSCBase {
public:
    void* ParticleSystem;
    bool UpdateStarted : 1;
    bool Initialised : 1;
    virtual ~CPSCBase() { }
    virtual bool Initialise(void*, void*, const void*, void*) { Initialised = true; return Initialised; }
};
int main()
{
    CPSCBase c;
    c.UpdateStarted = true; c.Initialised = false;
    if (!c.Initialise(0, 0, 0, 0) || !c.Initialised || !c.UpdateStarted) { std::printf("00af2ab0_TEST FAIL\n"); return 1; }
    c.UpdateStarted = false;
    if (!c.Initialise(0, 0, 0, 0) || c.UpdateStarted) { std::printf("00af2ab0_TEST FAIL\n"); return 1; }
    std::printf("00af2ab0_TEST PASS\n");
    return 0;
}