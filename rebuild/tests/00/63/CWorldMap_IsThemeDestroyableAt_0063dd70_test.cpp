// 0063dd70 behaviour test for CWorldMap::IsThemeDestroyableAt(const C3DVector&) const
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
class C3DVector { public: float X, Y, Z; };
static int g_destroyed = 0;
class CDefPointeeBase {
public:
    long m_refCount;
    CDefPointeeBase() : m_refCount(1) { }
    virtual ~CDefPointeeBase() { }
    virtual void Destroy() { ++g_destroyed; }
};
class CEngineThemeDef : public CDefPointeeBase { public: long DestructionThemeDef; };
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
static CEngineThemeDef g_defs[3];
class CDefinitionManager {
public:
    bool GetDef(long index, CDefPointer<CEngineThemeDef>& out) const
    {
        if (index <= 0 || index > 2) return false;
        out.Object = &g_defs[index];
        ++out.Object->m_refCount;
        return true;
    }
};
static CDefinitionManager g_dm;
class CWorldMap {
public:
    long MapNumber; bool Loaded; long Theme; int ThemeQueries;
    long GetMapNumberAt(const C3DVector& pos) const { (void)pos; return MapNumber; }
    bool IsMapLoaded(long map) const { return Loaded && map == MapNumber; }
    long GetStrongestEngineThemeAt(const C3DVector& pos, long* layer) const { (void)pos; *layer = 0; ++((CWorldMap*)this)->ThemeQueries; return Theme; }
    const CDefinitionManager& PeekDefinitionManager() const { return g_dm; }
    bool IsThemeDestroyableAt(const C3DVector& pos) const;
};
bool CWorldMap::IsThemeDestroyableAt(const C3DVector& pos) const
{
    long map = GetMapNumberAt(pos);
    if (map != 0 && IsMapLoaded(map)) {
        long layer;
        long theme = GetStrongestEngineThemeAt(pos, &layer);
        CDefPointer<CEngineThemeDef> def;
        PeekDefinitionManager().GetDef(theme, def);
        return def->DestructionThemeDef != 0;
    }
    return false;
}
static void run()
{
    g_defs[1].DestructionThemeDef = 0; g_defs[2].DestructionThemeDef = 17;
    C3DVector pos = { 1.0f, 2.0f, 0.0f };
    CWorldMap wm; wm.MapNumber = 0; wm.Loaded = true; wm.Theme = 2; wm.ThemeQueries = 0;
    CHECK(!wm.IsThemeDestroyableAt(pos));                       // no map under the position
    wm.MapNumber = 3; wm.Loaded = false;
    CHECK(!wm.IsThemeDestroyableAt(pos));                       // map not loaded
    CHECK(wm.ThemeQueries == 0);
    wm.Loaded = true;
    CHECK(wm.IsThemeDestroyableAt(pos));                        // theme 2 has a destruction theme
    CHECK(g_defs[2].m_refCount == 1);                           // the temporary reference was released
    wm.Theme = 1;
    CHECK(!wm.IsThemeDestroyableAt(pos));
    CHECK(g_defs[1].m_refCount == 1 && g_destroyed == 0);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("0063dd70_TEST FAIL\n");
        return 1;
    }
    std::printf("0063dd70_TEST PASS\n");
    return 0;
}