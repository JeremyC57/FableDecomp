#include <cstdio>

// Model of the Changed-bit upload @ 0x00707ad0: nothing happens while Changed is clear; when set, the bit is cleared and
// hidden modifiers, hidden materials, graphics are rebuilt in that order, then the owner's appearance is flagged (15).
struct CModifierEntry { long DefIndex; bool Rendered; };
struct CModifierVector {
    CModifierEntry* _Myfirst; CModifierEntry* _Mylast; CModifierEntry* _Myend;
    void clear()
    {
        CModifierEntry* first = _Myfirst; CModifierEntry* last = _Mylast; CModifierEntry* result = first;
        for (long n = _Mylast - last; n > 0; --n) { *result = *last; ++last; ++result; }   /* copy(last, _Mylast, first) */
        _Mylast = result;
    }
};
static long g_frame = 0; static int g_worldCalls = 0;
struct CWorld { static long GetFrame() { return g_frame; } };
static CWorld g_world;
static int g_log[8]; static int g_logCount = 0; static unsigned long g_appearanceFlags = 0;
struct CTCGraphicAppearance { void SetAsChanged(unsigned long flags) { g_appearanceFlags |= flags; g_log[g_logCount++] = 4; } };
struct CThing { CTCGraphicAppearance* PTCGraphicAppearance; };
struct CTCBase {
    CThing* ParentThing;
    CWorld& GetWorld() const { ++g_worldCalls; return g_world; }
    virtual void FrameUpdate() { }
    virtual void Copy(const CTCBase&) { }
    virtual ~CTCBase() { }
};
struct CTCHeroAttachableAppearanceModifiers : CTCBase {
    unsigned long FrameLastChanged;
    bool Changed : 1; bool RecreatePrimitives : 1; bool AppearanceModifiersLoaded : 1;
    CModifierVector* Lists; long ListCount;                     /* AppearanceModifiers (vector of vectors) */
    void SetAsChanged() { Changed = true; FrameLastChanged = GetWorld().GetFrame(); }
    bool IsWearingAppearanceModifier(long defIndex, long type) const
    {
        const CModifierVector& list = Lists[type];
        for (const CModifierEntry* it = list._Myfirst; it != list._Mylast; ++it)
            if (it->DefIndex == defIndex) return true;
        return false;
    }
    void RemoveAllAppearanceModifiers(long type) { Lists[type].clear(); SetAsChanged(); }
    void UpdateHiddenModifiers() { g_log[g_logCount++] = 1; }
    void UpdateHiddenMaterials() { g_log[g_logCount++] = 2; }
    void UpdateGraphics() { g_log[g_logCount++] = 3; }
    void UploadChangesToGraphics()
    {
        if (Changed) { Changed = false; UpdateHiddenModifiers(); UpdateHiddenMaterials(); UpdateGraphics(); ParentThing->PTCGraphicAppearance->SetAsChanged(15); }
    }
    virtual void FrameUpdate()
    {
        if (Changed) { Changed = false; UpdateHiddenModifiers(); UpdateHiddenMaterials(); UpdateGraphics(); ParentThing->PTCGraphicAppearance->SetAsChanged(15); }
    }
};

int main()
{
    CTCGraphicAppearance appearance; CThing thing; thing.PTCGraphicAppearance = &appearance;
    CTCHeroAttachableAppearanceModifiers m; m.ParentThing = &thing; m.Lists = 0; m.ListCount = 0;
    m.Changed = false; m.RecreatePrimitives = true; m.AppearanceModifiersLoaded = true;
    CTCBase& base = m; (void)base;
    base.FrameUpdate();
    if (g_logCount != 0) { std::printf("00707ad0_TEST FAIL\n"); return 1; }
    m.Changed = true;
    base.FrameUpdate();
    if (m.Changed || !m.RecreatePrimitives || !m.AppearanceModifiersLoaded || g_logCount != 4 || g_log[0] != 1 || g_log[1] != 2 ||
        g_log[2] != 3 || g_log[3] != 4 || g_appearanceFlags != 15) { std::printf("00707ad0_TEST FAIL\n"); return 1; }
    base.FrameUpdate();
    if (g_logCount != 4) { std::printf("00707ad0_TEST FAIL\n"); return 1; }
    std::printf("00707ad0_TEST PASS\n");
    return 0;
}