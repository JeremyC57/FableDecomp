// Behaviour test for 0x004174F1 CMainGameComponent::InitParticleEngine: engine initialised, interaction holding the world installed, PARTICLE_MAIN opened.
#include <cstdio>
#include <cstring>
struct World { int id; };
struct Interaction { World* world; Interaction(World* w) : world(w) { } };
static int g_inited = 0; static Interaction* g_interaction = 0; static char g_bank[32];
struct CMainGameComponent {
    World* m_pWorld;
    void InitParticleEngine()
    {
        ++g_inited;
        Interaction* interaction = new Interaction(m_pWorld);
        delete g_interaction; g_interaction = interaction;
        const char* bankName = "PARTICLE_MAIN";
        std::strcpy(g_bank, bankName);
    }
};
int main()
{
    World w = { 3 };
    CMainGameComponent c; c.m_pWorld = &w;
    c.InitParticleEngine();
    bool ok = g_inited == 1 && g_interaction && g_interaction->world == &w && std::strcmp(g_bank, "PARTICLE_MAIN") == 0;
    delete g_interaction;
    if (!ok) { std::printf("004174f1_TEST FAIL\n"); return 1; }
    std::printf("004174f1_TEST PASS\n");
    return 0;
}
