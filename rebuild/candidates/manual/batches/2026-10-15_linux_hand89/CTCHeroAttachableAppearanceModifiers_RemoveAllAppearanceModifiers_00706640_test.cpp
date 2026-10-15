#include <cstdio>

// Model of CTCHeroAttachableAppearanceModifiers::RemoveAllAppearanceModifiers @ 0x00706640: the list of the type is
// emptied (last = first), the other list is untouched, Changed is set and the frame stamped.
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
    CModifierEntry a[2] = { { 1, false }, { 2, false } }; CModifierEntry b[1] = { { 3, true } };
    CModifierVector lists[2];
    lists[0]._Myfirst = a; lists[0]._Mylast = a + 2; lists[0]._Myend = a + 2;
    lists[1]._Myfirst = b; lists[1]._Mylast = b + 1; lists[1]._Myend = b + 1;
    CTCHeroAttachableAppearanceModifiers m; m.Lists = lists; m.ListCount = 2; m.Changed = false; m.FrameLastChanged = 0;
    g_frame = 1234;
    m.RemoveAllAppearanceModifiers(0);
    if (lists[0]._Mylast != a || lists[0]._Myend != a + 2 || lists[1]._Mylast != b + 1 || !m.Changed ||
        m.FrameLastChanged != 1234 || g_worldCalls != 1 || m.IsWearingAppearanceModifier(1, 0) || !m.IsWearingAppearanceModifier(3, 1))
        { std::printf("00706640_TEST FAIL\n"); return 1; }
    std::printf("00706640_TEST PASS\n");
    return 0;
}
