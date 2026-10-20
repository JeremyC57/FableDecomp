// Behaviour test for CTCInventoryTrade::OpenInventoryPC (0x005AE86B): the call sequence and the state it leaves.
#include <cstdio>
#include <cstring>

static char g_log[32]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct CComponent { int Resets; void Reset() { ++Resets; Log('R'); } };
static CComponent g_pointer = { 0 };
struct CFrontEndManager {
    void* LastScreen; void* LastThing;
    void OnInventoryScreenOpened(void* s) { LastScreen = s; Log('S'); }
    void OnThingInventoryOpened(void* t) { LastThing = t; Log('T'); }
    CComponent* CreateComponent(const char* def, bool) { Log(std::strcmp(def, "UI_MOUSE_POINTER") == 0 ? 'C' : '?'); return &g_pointer; }
};
static CFrontEndManager g_fe;
struct CSound { int Cats; void RefreshCategory(long c, float a, float b) { if (a == 0.0f && b == 500.0f) Cats |= c; Log('s'); } };
static CSound g_sound = { 0 };
static CSound* g_pSoundSystem = &g_sound;
struct CHud { bool On; void SetEnabled(bool e) { On = e; Log('H'); } };
static CHud g_hud = { true };
struct CTCInventoryTrade {
    void* ParentThing; CComponent* Components[4]; int Count;
    bool InventoryTradeOpen; CComponent* PMouse_Pointer; float AccumFadeTime;
    void PrepareScreen() { Log('P'); }
    void ConstructTradeScreenPC() { Log('L'); }
    void OpenInventory() { Log('O'); }
    void OpenInventoryPC()
    {
        PrepareScreen();
        g_hud.SetEnabled(false);
        AccumFadeTime = 0.0f;
        if (g_pSoundSystem) {
            g_pSoundSystem->RefreshCategory(0x10, 0.0f, 500.0f);
            g_pSoundSystem->RefreshCategory(0x40, 0.0f, 500.0f);
            g_pSoundSystem->RefreshCategory(0x80, 0.0f, 500.0f);
        }
        CFrontEndManager* frontEnd = &g_fe;
        frontEnd->OnInventoryScreenOpened(this);
        frontEnd->OnThingInventoryOpened(ParentThing);
        ConstructTradeScreenPC();
        PMouse_Pointer = frontEnd->CreateComponent("UI_MOUSE_POINTER", false);
        PMouse_Pointer->Reset();
        Components[Count++] = PMouse_Pointer;
        OpenInventory();
        InventoryTradeOpen = true;
    }
};

int main()
{
    int owner = 0;
    CTCInventoryTrade t; std::memset(&t, 0, sizeof(t)); t.ParentThing = &owner; t.AccumFadeTime = 3.0f;
    t.OpenInventoryPC();
    bool ok = std::strcmp(g_log, "PHsssSTLCRO") == 0 && !g_hud.On && g_sound.Cats == 0xd0 && t.AccumFadeTime == 0.0f
        && t.PMouse_Pointer == &g_pointer && t.Count == 1 && t.Components[0] == &g_pointer && t.InventoryTradeOpen
        && g_fe.LastScreen == &t && g_fe.LastThing == &owner;
    if (!ok) { std::printf("005ae86b_TEST FAIL\n"); return 1; }
    std::printf("005ae86b_TEST PASS\n");
    return 0;
}