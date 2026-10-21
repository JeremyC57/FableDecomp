// Behaviour test for NCreatureMode::CModeBriarRoseTeleportIntangible::OnActivate @ 0x008E7B90.
#include <cstdio>

static int g_released = 0, g_transitions = 0, g_lastCount = 0; static float g_lastScale = 0;
struct CDefPointeeBase { long m_refCount; void Delete() { ++g_released; } };
template <class T> struct CDefPointer { T* Object; CDefPointer() : Object(0) {} };
template <class T> struct CDefHolder {
    T* Object; CDefHolder(T* o) : Object(o) {}
    ~CDefHolder() { if (Object && --Object->m_refCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
};
template <class T> struct CDefCopy {
    T* Object; CDefCopy(T* o) : Object(o) { if (Object) ++Object->m_refCount; }
    ~CDefCopy() { if (Object && --Object->m_refCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
};
struct CBriarRoseTeleportDef : CDefPointeeBase { float Duration; long ThemeChangeCount; float ThemeScale; };
struct CDefClassBase : CDefPointeeBase {
    CBriarRoseTeleportDef* sub;
    bool GetSubDef(CDefPointer<CBriarRoseTeleportDef>* out) { if (!sub) return false; ++sub->m_refCount; out->Object = sub; return true; }
};
struct CModeBase;
struct CTCFlagged { unsigned char m_flag : 1; };
struct CTCType9 { bool m_active; };
struct CThing {
    unsigned char m_type; CDefClassBase* PDef; CTCFlagged* flagged; CTCType9* type9; int stops; int suspends;
    template <class T> bool GetInterface(T** out, long type)
    {
        if (type == 0x3a && flagged) { *out = (T*)flagged; return true; }
        if (type == 9 && type9) { *out = (T*)type9; return true; }
        return false;
    }
    void SetSuspendAnimation(bool s, CModeBase*) { if (s) ++suspends; }
    void StopMoving(bool) { ++stops; }
};
struct CEnvironment { void TransitionToTheme(long, long mode, long count, float scale) { if (mode == 2) { ++g_transitions; g_lastCount = count; g_lastScale = scale; } } };
static CEnvironment g_env;
struct CGlobalComponents { CEnvironment* Environment; CEnvironment* GetEnvironment() const { return Environment; } };
static CGlobalComponents g_globals = { &g_env };
static long g_GameFramesPerSecond = 30;
struct CModeBase { CThing* ParentThing; CGlobalComponents* GetGlobalComponents() const { return &g_globals; } };
struct CModeBriarRoseTeleportIntangible : CModeBase {
    long FrameCount; int baseActivations;
    void BaseOnActivate() { ++baseActivations; }
    void OnActivate();
};
void CModeBriarRoseTeleportIntangible::OnActivate()
{
    BaseOnActivate();
    CThing* thing = ParentThing;
    float duration = 6.0f; long themeChanges = 0; float scale = 1.0f;
    CDefPointer<CBriarRoseTeleportDef> out;
    bool found;
    { CDefCopy<CDefClassBase> def(thing->PDef); found = def->GetSubDef(&out); }
    CDefHolder<CBriarRoseTeleportDef> teleport(out.Object);
    if (found) { themeChanges = teleport->ThemeChangeCount; duration = teleport->Duration; scale = teleport->ThemeScale; }
    FrameCount = (long)(g_GameFramesPerSecond * duration);
    if (ParentThing->m_type == 1) ParentThing->SetSuspendAnimation(true, this);
    CTCFlagged* flagged;
    if (ParentThing->GetInterface(&flagged, 0x3a)) flagged->m_flag = 0;
    CTCType9* type9;
    if (ParentThing->GetInterface(&type9, 9)) type9->m_active = false;
    ParentThing->StopMoving(false);
    if (themeChanges > 0)
        for (long channel = 0; channel < 6; ++channel)
            GetGlobalComponents()->GetEnvironment()->TransitionToTheme(channel, 2, themeChanges, scale);
}
int main()
{
    CBriarRoseTeleportDef sub; sub.m_refCount = 1; sub.Duration = 2.0f; sub.ThemeChangeCount = 3; sub.ThemeScale = 0.5f;
    CDefClassBase def; def.m_refCount = 1; def.sub = &sub;
    CTCFlagged fl; fl.m_flag = 1; CTCType9 t9; t9.m_active = true;
    CThing thing; thing.m_type = 1; thing.PDef = &def; thing.flagged = &fl; thing.type9 = &t9; thing.stops = 0; thing.suspends = 0;
    CModeBriarRoseTeleportIntangible m; m.ParentThing = &thing; m.FrameCount = 0; m.baseActivations = 0;
    m.OnActivate();
    int ok = m.baseActivations == 1 && m.FrameCount == 60 && thing.suspends == 1 && fl.m_flag == 0 && !t9.m_active && thing.stops == 1
          && g_transitions == 6 && g_lastCount == 3 && g_lastScale == 0.5f && sub.m_refCount == 1 && def.m_refCount == 1 && g_released == 0;
    def.sub = 0; thing.m_type = 0;
    m.OnActivate();
    ok = ok && m.FrameCount == 180 && g_transitions == 6 && thing.suspends == 1;
    if (!ok) { std::printf("008e7b90_TEST FAIL\n"); return 1; }
    std::printf("008e7b90_TEST PASS\n");
    return 0;
}
