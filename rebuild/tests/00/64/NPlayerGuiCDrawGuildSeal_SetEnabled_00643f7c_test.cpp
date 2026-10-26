#include <cstdio>
struct CGuiDef { unsigned char _pad[0x68]; long m_guildSealFlashCycles; }; static CGuiDef g_def; static CGuiDef* PeekGuiDef() { return &g_def; }
namespace NPlayerGui {
class CDrawGuildSeal { public: unsigned char _pad[0x28]; bool Enabled; bool ReadyToLevelUp; unsigned char FlashAlpha; unsigned char _pad2b; long FlashCycles; unsigned char _pad30[0x1c]; float ReminderTime;
    void SetEnabled(bool e) { Enabled = e; FlashAlpha = 0; FlashCycles = PeekGuiDef()->m_guildSealFlashCycles; ReminderTime = 0.0f; } }; }
int main() { g_def.m_guildSealFlashCycles = 6; NPlayerGui::CDrawGuildSeal s; s.FlashAlpha = 9; s.ReminderTime = 3.0f; s.SetEnabled(true);
    if (!s.Enabled || s.FlashAlpha != 0 || s.FlashCycles != 6 || s.ReminderTime != 0.0f) { std::printf("00643f7c_TEST FAIL\n"); return 1; } s.SetEnabled(false); if (s.Enabled) { std::printf("00643f7c_TEST FAIL\n"); return 1; } std::printf("00643f7c_TEST PASS\n"); return 0; }