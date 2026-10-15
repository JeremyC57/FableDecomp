#include <cstdio>
// Behaviour test for 00651904.

struct CPlayerGui { int id; };
struct CRetailHandleVector { unsigned int reserved; CRetailHandleVector() : reserved(0) { } void Resize(long n) { reserved = (unsigned int)n; } };
struct CRetailEmitterVector { void* m_pFirst; void* m_pLast; void* m_pEnd; unsigned int reserved;
    CRetailEmitterVector() : m_pFirst(0), m_pLast(0), m_pEnd(0), reserved(0) { } void Resize(long n) { reserved = (unsigned int)n; } };
struct CFadeInFadeOutBase { int built; CFadeInFadeOutBase() : built(1) { } };
struct CDrawBase {
    CPlayerGui* PlayerGui; void* DrawList; CRetailHandleVector PrimitiveHandles;
    CDrawBase(CPlayerGui* gui, void* list) : PlayerGui(gui), DrawList(list) { }
    virtual ~CDrawBase() { }
};

// NPlayerGui::CDrawGuildSeal::CDrawGuildSeal: flags/flash cleared, fade built, reminder zeroed, level-up graphic read from the GUI def,
// message frames zeroed, ten primitive handles reserved.
struct CPlayerGuiDef { long GuildSealLevelUpGraphic; };
static CPlayerGuiDef g_def;
static CPlayerGuiDef* PeekGuiDef() { return &g_def; }
struct CMessageQueue { int built; CMessageQueue() : built(1) { } };
struct CDrawGuildSeal : CDrawBase {
    bool Enabled, ReadyToLevelUp; unsigned char FlashAlpha; long FlashCycles; CFadeInFadeOutBase FadeInOut; float ReminderTime;
    void* LevelUpPositions; long LevelUpGraphic, CurrentMessageEndFrame, NextMessageDelayEndFrame; CMessageQueue Messages;
    CDrawGuildSeal(CPlayerGui* gui, void* drawList)
        : CDrawBase(gui, drawList), Enabled(false), ReadyToLevelUp(false), FlashAlpha(0), FlashCycles(0), FadeInOut(),
          ReminderTime(0.0f), LevelUpPositions(0), LevelUpGraphic(PeekGuiDef()->GuildSealLevelUpGraphic), CurrentMessageEndFrame(0),
          NextMessageDelayEndFrame(0), Messages()
    { PrimitiveHandles.Resize(10); }
};
int main()
{
    g_def.GuildSealLevelUpGraphic = 77;
    CPlayerGui gui;
    CDrawGuildSeal s(&gui, 0);
    int ok = !s.Enabled && !s.ReadyToLevelUp && s.FlashAlpha == 0 && s.FlashCycles == 0 && s.FadeInOut.built && s.ReminderTime == 0.0f
          && s.LevelUpGraphic == 77 && s.CurrentMessageEndFrame == 0 && s.Messages.built && s.PrimitiveHandles.reserved == 10;
    if (!ok) { std::printf("00651904_TEST FAIL\n"); return 1; }
    std::printf("00651904_TEST PASS\n");
    return 0;
}