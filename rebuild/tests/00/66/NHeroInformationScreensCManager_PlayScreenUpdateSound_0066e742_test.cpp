// Behaviour test for 0x0066E742 NHeroInformationScreens::CManager::PlayScreenUpdateSound (model copy)
#include <cstdio>
struct Bank { int id; };
struct Init { float Volume; Init() : Volume(1.0f) { } };
struct Player {
    int calls; Bank* bank; int sample; bool loop, attached; float vol;
    unsigned int PlaySample(Bank& b, int s, bool l, const Init& i, bool a) { ++calls; bank = &b; sample = s; loop = l; attached = a; vol = i.Volume; return 0; }
};
struct State { Player player; Player* GetSoundPlayer() { return &player; } };
struct Main { Bank* PSampleBank; Bank& GetSampleBank() const { return *PSampleBank; } };
struct GuiDef { int InventoryNoMovementSound; };
static GuiDef g_def = { 77 };
static const GuiDef& PeekGuiDef() { return g_def; }
static State g_state; static State* g_pGameState = &g_state;
static Bank g_bank = { 5 }; static Main g_main = { &g_bank };
enum EScreenSoundTypes { T0, T1, T2 };
static void PlayScreenUpdateSound(EScreenSoundTypes type)
{
    int sample = 0;
    switch (type) { case T2: sample = PeekGuiDef().InventoryNoMovementSound; break; default: break; }
    if (g_pGameState) { Bank& bank = g_main.GetSampleBank(); g_pGameState->GetSoundPlayer()->PlaySample(bank, sample, true, Init(), false); }
}
int main()
{
    Player& p = g_state.player; p.calls = 0;
    PlayScreenUpdateSound(T2);
    bool ok = p.calls == 1 && p.sample == 77 && p.bank == &g_bank && p.loop && !p.attached && p.vol == 1.0f;
    PlayScreenUpdateSound(T1); ok = ok && p.calls == 2 && p.sample == 0;
    g_pGameState = 0; PlayScreenUpdateSound(T2); ok = ok && p.calls == 2;
    std::printf(ok ? "0066e742_TEST PASS\n" : "0066e742_TEST FAIL\n");
    return ok ? 0 : 1;
}