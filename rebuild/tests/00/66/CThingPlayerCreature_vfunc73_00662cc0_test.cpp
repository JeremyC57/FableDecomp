// behaviour test for CThingPlayerCreature::vfunc_73 (0x00662CC0): mode-filtered lookup, optional any-group fallback, def released on every exit
#include <cstdio>
struct CCharString { int m_id; };
struct CAnimFilterBase { int m_kind; CAnimFilterBase(int k) : m_kind(k) {} };
struct CAnimFilterNoGroup : CAnimFilterBase { CAnimFilterNoGroup() : CAnimFilterBase(0) {} };
static int g_modeHas = -1, g_anyHas = -1, g_lookups = 0;
struct CAnimationSet { bool HasAnimation_005dc2e0(const CCharString& n, const CAnimFilterBase& f) const
    { ++g_lookups; return f.m_kind == 0 ? n.m_id == g_anyHas : n.m_id == g_modeHas; } };
static int g_destroyed = 0;
struct CDefPointeeBase { long RefCount; virtual void Destroy() { ++g_destroyed; } virtual ~CDefPointeeBase() {} };
template <class T> struct CDefPointer { T* Object; CDefPointer() : Object(0) {} };
template <class T> struct CHeldDef { T* Object; CHeldDef(T* o) : Object(o) {}
    ~CHeldDef() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } } T* operator->() const { return Object; } };
struct CAppearanceDef : CDefPointeeBase { CAnimationSet Animation; };
static CAppearanceDef g_def;
struct CThingBaseDef { bool GetSubDef(CDefPointer<CAppearanceDef>* out) const { ++g_def.RefCount; out->Object = &g_def; return true; } };
struct CCreatureModeBase { CAnimFilterBase m_filter; CCreatureModeBase() : m_filter(1) {} const CAnimFilterBase& GetAnimFilter_slot4() { return m_filter; } };
struct CTCCreatureModeManager { CCreatureModeBase m_mode; CCreatureModeBase* GetCurrentMode() { return &m_mode; } };
class CThingPlayerCreature { public:
    CThingBaseDef* PBaseDef; CTCCreatureModeManager* PTCModeManager;
    bool vfunc_73(const CCharString& name, bool useModeFilterOnly)
    {
        CDefPointer<CAppearanceDef> out;
        PBaseDef->GetSubDef(&out);
        CCreatureModeBase* mode = PTCModeManager->GetCurrentMode();
        CHeldDef<CAppearanceDef> def(out.Object);
        CAnimationSet& animations = def->Animation;
        if (animations.HasAnimation_005dc2e0(name, mode->GetAnimFilter_slot4())) return true;
        if (!useModeFilterOnly) return animations.HasAnimation_005dc2e0(name, CAnimFilterNoGroup());
        return false;
    } };
int main()
{
    CThingBaseDef base; CTCCreatureModeManager modes; g_def.RefCount = 0;
    CThingPlayerCreature p; p.PBaseDef = &base; p.PTCModeManager = &modes;
    CCharString a = { 1 }, b = { 2 }, c = { 3 };
    g_modeHas = 1; g_anyHas = 2;
    int bad = 0;
    if (!p.vfunc_73(a, true) || g_lookups != 1) bad = 1;           // mode filter hit
    if (p.vfunc_73(b, true) || g_lookups != 2) bad = 1;            // restricted: no fallback
    if (!p.vfunc_73(b, false) || g_lookups != 4) bad = 1;          // fallback hit
    if (p.vfunc_73(c, false)) bad = 1;
    if (g_destroyed != 4 || g_def.RefCount != 0) bad = 1;          // released on every exit
    if (bad) { std::printf("00662cc0_TEST FAIL\n"); return 1; }
    std::printf("00662cc0_TEST PASS\n");
    return 0;
}