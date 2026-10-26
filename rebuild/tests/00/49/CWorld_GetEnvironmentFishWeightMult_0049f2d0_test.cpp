// Behaviour test for 0049f2d0: src/0049f2d0.cpp copied verbatim; GetEnvironmentFishWeightMult sums, over the cell's three
// atmos themes (theme 0 = the region theme, else mapped to a global index), blend/255 * the day-set def's
// FishWeightMult for every theme that has a def, adds the unblended remainder at 1.0, and returns 1.0 off the map.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CWorld::GetEnvironmentFishWeightMult(const C3DVector&) -- the fishing weight multiplier at a position: the blend-weighted sum of the cell's three day-set themes' FishWeightMult, any unblended remainder counting 1.0; 1.0 off the game map
// Leaf: GetEnvironmentFishWeightMult
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049F2D0, 283 bytes, __fastcall `ret 4`.
// Callees: CWorld::PeekWorldMap (slot 13), CWorldMap::GetPGameMapCellAt @ 0x0063D8A0, CEnvironment::
// GetGlobalEnvironmentThemeIndexFromLocal @ 0x006B2C60, CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDef<
// CEnvironmentThemeDaySetDef> @ 0x004ABA00, CGameMapCell::GetAtmosBlend @ 0x006B74E0, the def's virtual slot 1.
// Shape: GetDef is a same-TU noinline stand-in body that fills the raw, null-initialised def pointer through `T*&`
// (writes only the out slot), so the def stays in edi and the dead out slot's word is reused for the fild temporaries
// (0x14 frame); the def is released by hand at the end of each iteration.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct CDefBase : CDefVirtual {
    long m_count;                                  // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
class CEnvironmentThemeDaySetDef : public CDefBase { public:
    unsigned char _pad08[0x68 - 0x08];
    float FishWeightMult;                          // +0x68 (Ego_r.pdb +0x70)
};
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, T*& out);   // 0x004ABA00 (CEnvironmentThemeDaySetDef instantiation)
};
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index);   // stand-in callee
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, T*& out)
{
    void* def = Lookup_CDefinitionManager_GetDef(this, index);
    if (!def)
        return false;
    out = (T*)def;
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
class CGameMapCell { public:
    unsigned char AtmosThemes[3];                  // +0x00
    unsigned char AtmosBlends[2];                  // +0x03
    signed char   SoundTheme;                      // +0x05
    unsigned char Music;                           // +0x06
    unsigned char GetAtmosBlend(long index) const; // 0x006B74E0
};
class CWorldMap { public:
    const CGameMapCell* GetPGameMapCellAt(const C3DVector& pos) const;   // 0x0063D8A0
};
class CEnvironment { public:
    void* __vftable;
    void* DefinitionManager;                       // +0x04
    unsigned char _pad08[8];
    long  RegionTheme;                             // +0x10
    long GetGlobalEnvironmentThemeIndexFromLocal(long localIndex);   // 0x006B2C60
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual const CWorldMap* PeekWorldMap() const;   // slot 13 (+0x34)
    unsigned char _pad04[0x14];
    CEnvironment* PEnvironment;                    // +0x18 (scoped_ptr)
    float GetEnvironmentFishWeightMult(const C3DVector& pos);
};
float CWorld::GetEnvironmentFishWeightMult(const C3DVector& pos)
{
    const CWorldMap* map = PeekWorldMap();
    const CGameMapCell* cell = map->GetPGameMapCellAt(C3DVector(pos.X, pos.Y, 0.0f));
    if (!cell)
        return 1.0f;
    float mult = 0.0f;
    long totalBlend = 0;
    for (long i = 0; i != 3; ++i) {
        long theme = cell->AtmosThemes[i];
        long globalTheme = theme == 0 ? PEnvironment->RegionTheme : PEnvironment->GetGlobalEnvironmentThemeIndexFromLocal(theme);
        CEnvironmentThemeDaySetDef* def = 0;
        if (CGameDefinitionManager::Get().GetDef(globalTheme, def)) {
            totalBlend += cell->GetAtmosBlend(i);
            mult += (float)cell->GetAtmosBlend(i) * def->FishWeightMult * (1.0f / 255.0f);
        }
        if (def)
            def->Release();
    }
    if (totalBlend < 255)
        mult += (float)(255 - totalBlend) * (1.0f / 255.0f);
    return mult;
}
// ---- harness ----
static int g_destroyed = 0, g_lookups = 0;
static CEnvironmentThemeDaySetDef* g_defs[8];
static long g_lookupIndex[3]; static C3DVector g_cellPos(0, 0, 0); static const CGameMapCell* g_cell = 0;
static CGameDefinitionManager g_manager; static const CWorldMap* g_map = 0;
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() { ++g_destroyed; }
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index)
{ CHECK(manager == &g_manager); if (g_lookups < 3) g_lookupIndex[g_lookups] = index; ++g_lookups;
  CEnvironmentThemeDaySetDef* d = (index >= 0 && index < 8) ? g_defs[index] : 0; if (d) ++d->m_count; return d; }
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_manager; }
const CGameMapCell* CWorldMap::GetPGameMapCellAt(const C3DVector& pos) const { g_cellPos = pos; return g_cell; }
unsigned char CGameMapCell::GetAtmosBlend(long index) const
{ if (index < 2) return AtmosBlends[index]; return (unsigned char)(255 - AtmosBlends[0] - AtmosBlends[1]); }
long CEnvironment::GetGlobalEnvironmentThemeIndexFromLocal(long localIndex) { return localIndex + 3; }
void CWorld::v00() { } void CWorld::v01() { } void CWorld::v02() { } void CWorld::v03() { } void CWorld::v04() { }
void CWorld::v05() { } void CWorld::v06() { } void CWorld::v07() { } void CWorld::v08() { } void CWorld::v09() { }
void CWorld::v10() { } void CWorld::v11() { } void CWorld::v12() { }
const CWorldMap* CWorld::PeekWorldMap() const { return g_map; }
static bool near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    static unsigned char mapBuf[4]; g_map = (const CWorldMap*)mapBuf;
    static CEnvironmentThemeDaySetDef d1, d4, d5;
    d1.m_count = 1; d1.FishWeightMult = 2.0f; d4.m_count = 1; d4.FishWeightMult = 0.5f; d5.m_count = 1; d5.FishWeightMult = 4.0f;
    g_defs[1] = &d1; g_defs[4] = &d4; g_defs[5] = &d5;
    CEnvironment env; memset(&env, 0, sizeof env); env.RegionTheme = 1;
    CWorld world; world.PEnvironment = &env;
    CGameMapCell cell; memset(&cell, 0, sizeof cell);
    cell.AtmosThemes[0] = 0; cell.AtmosThemes[1] = 1; cell.AtmosThemes[2] = 2;   // -> 1, 4, 5
    cell.AtmosBlends[0] = 100; cell.AtmosBlends[1] = 55;                          // third 100
    g_cell = &cell;
    C3DVector pos(7.0f, 8.0f, 9.0f);

    float r = world.GetEnvironmentFishWeightMult(pos);
    CHECK(g_cellPos.X == 7.0f && g_cellPos.Y == 8.0f && g_cellPos.Z == 0.0f);
    CHECK(g_lookups == 3 && g_lookupIndex[0] == 1 && g_lookupIndex[1] == 4 && g_lookupIndex[2] == 5);
    CHECK(near(r, (100 * 2.0f + 55 * 0.5f + 100 * 4.0f) / 255.0f));
    CHECK(d1.m_count == 1 && d4.m_count == 1 && d5.m_count == 1 && g_destroyed == 0);

    // a theme without a def: its blend counts at 1.0 (remainder)
    g_lookups = 0; g_defs[4] = 0;
    r = world.GetEnvironmentFishWeightMult(pos);
    CHECK(near(r, (100 * 2.0f + 100 * 4.0f) / 255.0f + 55 / 255.0f));
    g_defs[4] = &d4;

    // last reference: released
    g_lookups = 0; d5.m_count = 0;
    r = world.GetEnvironmentFishWeightMult(pos);
    CHECK(d5.m_count == 0 && g_destroyed == 1);

    // off the map
    g_cell = 0;
    CHECK(world.GetEnvironmentFishWeightMult(pos) == 1.0f);

    if (fails == 0) printf("0049f2d0_TEST PASS\n");
    return fails;
}