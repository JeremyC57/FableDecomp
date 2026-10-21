#include <stdio.h>
#include <string.h>
// CMap::InitPassabilityFromThemesIfNecessary(const C2DBoxI&) -- when the map still needs it, mark each height-field cell of the area passable unless its theme forbids it or the ground there is steeper than a 0.7 normal
// Leaf: InitPassabilityFromThemesIfNecessary
// Shape: same-TU out-only stand-ins for GetDef<CThemeDef> (0x0063F3A0) and CMap::GetThemeIndexAt (0x0081F910) keep the
//   cell position, the weight and the def from escaping (x in edi, one shared out/def word, the def released without a
//   re-read); the theme clears passability when it lacks flag 0 or has a positive Passability (`if (...) passable =
//   false;` -- retail keeps the cell passable for Passability <= 0); the cell pointer steps before x.
// Callees: CMap theme lookup 0x0081F910 (labelled CopyPos), CGameDefinitionManager::Get 0x0044C6B0 (__fastcall), CDefinitionManager::GetDef<T> 0x0063F3A0, CMap::GetGroundFaceNormalAt 0x0081F6C0; constants 0.0f @ 0x0122DEDC, 0.7f @ 0x01255318.
// Layout: no CMap engine header; local model (height field +0x1A7C: cells +0x10 of 12 bytes, width +0x204, height +0x208; passability flag +0x1D38; theme def flags +0xA0 bit 0, passability +0x80).
struct C2DCoordI {
    long X, Y;
};
struct C2DBoxI {
    C2DCoordI Min;
    C2DCoordI Max;
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CHeightFieldCell {
    unsigned char _pad00[9];
    unsigned char Reserved : 1;
    unsigned char Passable : 1;                 // +0x09 bit 1
    unsigned char _rest : 6;
    unsigned char _pad0a[2];
};
struct CHeightField {
    unsigned char     _pad00[0x10];
    CHeightFieldCell* Cells;                    // +0x10
    unsigned char     _pad14[0x1f0];
    long              Width;                    // +0x204
    long              Height;                   // +0x208
    CHeightFieldCell* GetCell(long x, long y) { return &Cells[(Height - y) * (Width + 1) + x]; }
};
struct CThemeDef {
    virtual void v00();
    virtual void Delete();                      // slot 1
    long          m_refCount;                   // +0x04
    unsigned char _pad08[0x78];
    float         Passability;                  // +0x80
    unsigned char _pad84[0x1c];
    unsigned char Flags;                        // +0xa0 bit 0: passable
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (--Object->m_refCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThemeDef>* out);   // 0x0063F3A0
};
void* FindDef_CDefinitionManager_GetDef(CDefinitionManager* manager, long index);   // stand-in callee
// Same-TU stand-in for the GetDef<CThemeDef> instantiation (a template member compiled in the caller's TU): it writes the
// out slot only, so VC7.1 knows the def pointer's address does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CThemeDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef(this, index);
    if (!def)
        return false;
    out->Object = (CThemeDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CGameDefinitionManager {
    static CDefinitionManager* __fastcall Get();            // 0x0044C6B0
};
struct CMap {
    virtual void v00();
    unsigned char _pad04[0x1a78];
    CHeightField* HeightField;                  // +0x1a7c
    unsigned char _pad1a80[0x2b8];
    bool          PassabilityNeedsInit;         // +0x1d38
    long GetThemeIndexAt(const C2DCoordI& pos, long* weight);         // 0x0081F910
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos);            // 0x0081F6C0
    void InitPassabilityFromThemesIfNecessary(const C2DBoxI& area);
};
long LookUpThemeIndex_CMap(CMap* map, long x, long y, long& weight);   // stand-in callee
// Same-TU stand-in for CMap::GetThemeIndexAt (0x0081F910, compiled in this TU in retail): it reads the cell coordinates
// by value and writes the out weight only, so neither the position nor the weight escapes.
#pragma auto_inline(off)
__declspec(noinline) long CMap::GetThemeIndexAt(const C2DCoordI& pos, long* weight)
{
    long w;
    long index = LookUpThemeIndex_CMap(this, pos.X, pos.Y, w);
    *weight = w;
    return index;
}
#pragma auto_inline(on)

void CMap::InitPassabilityFromThemesIfNecessary(const C2DBoxI& area)
{
    if (!PassabilityNeedsInit)
        return;
    CHeightField* heightField = HeightField;
    C2DCoordI pos;
    for (pos.Y = area.Min.Y; pos.Y < area.Max.Y; ++pos.Y) {
        CHeightFieldCell* cell = heightField->GetCell(area.Min.X, pos.Y);
        for (pos.X = area.Min.X; pos.X < area.Max.X; ++cell, ++pos.X) {
            bool passable = true;
            long theme;
            {
                long weight;
                theme = GetThemeIndexAt(pos, &weight);
            }
            if (theme > 0) {
                CDefPointer<CThemeDef> def;
                CGameDefinitionManager::Get()->GetDef(theme, &def);
                if (!(def->Flags & 1) || def->Passability > 0.0f)
                    passable = false;
            }
            if (passable) {
                C3DVector normal = GetGroundFaceNormalAt(C3DVector((float)pos.X, (float)pos.Y, 0.0f));
                if (normal.Z < 0.7f)
                    passable = false;
            }
            cell->Passable = passable;
        }
    }
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
void CThemeDef::v00() {}
static int g_deletes;
void CThemeDef::Delete() { g_deletes++; }
static CThemeDef* g_defs[8];
static CDefinitionManager g_mgr;
static long g_getDefs;
void* FindDef_CDefinitionManager_GetDef(CDefinitionManager* manager, long index) { g_getDefs++; return manager == &g_mgr ? g_defs[index] : 0; }
CDefinitionManager* __fastcall CGameDefinitionManager::Get() { return &g_mgr; }
static long g_themes[4][4]; static long g_lookups; static CMap* g_lookupMap;
long LookUpThemeIndex_CMap(CMap* map, long x, long y, long& weight) { g_lookupMap = map; g_lookups++; weight = 3; return g_themes[y][x]; }
void CMap::v00() {}
static long g_normals;
C3DVector CMap::GetGroundFaceNormalAt(const C3DVector& pos)
{
    g_normals++;
    return C3DVector(0.0f, 0.0f, (pos.X == 1.0f && pos.Y == 1.0f && pos.Z == 0.0f) ? 0.5f : 0.9f);
}
static unsigned char g_mapBuf[0x2000], g_hfBuf[0x300], g_defBuf[3][0x100];
static CHeightFieldCell g_cells[16];
static int Passable(long x, long y) { return (((unsigned char*)&g_cells[(3 - y) * 4 + x])[9] >> 1) & 1; }
int main()
{
    CMap* map = (CMap*)g_mapBuf;
    CHeightField* hf = (CHeightField*)g_hfBuf;
    hf->Cells = g_cells; hf->Width = 3; hf->Height = 3;
    map->HeightField = hf;
    for (int k = 0; k < 3; ++k) {
        CThemeDef* d = (CThemeDef*)g_defBuf[k]; d->m_refCount = 100; g_defs[5 + k] = d;
    }
    g_defs[5]->Flags = 1; g_defs[5]->Passability = 0.0f;    // passable theme
    g_defs[6]->Flags = 0; g_defs[6]->Passability = 0.0f;    // flag clear: impassable
    g_defs[7]->Flags = 1; g_defs[7]->Passability = 0.5f;    // positive passability: impassable
    g_themes[1][0] = 0; g_themes[1][1] = 0; g_themes[1][2] = 5;
    g_themes[2][0] = 6; g_themes[2][1] = 7; g_themes[2][2] = 5;
    C2DBoxI area; area.Min.X = 0; area.Min.Y = 1; area.Max.X = 3; area.Max.Y = 3;
    // not needed: nothing touched
    memset(g_cells, 0xff, sizeof(g_cells)); map->PassabilityNeedsInit = false;
    map->InitPassabilityFromThemesIfNecessary(area);
    CHECK(g_lookups == 0 && Passable(0, 1) == 1);
    // needed: cells of rows 1..2, columns 0..2
    memset(g_cells, 0xff, sizeof(g_cells)); map->PassabilityNeedsInit = true;
    map->InitPassabilityFromThemesIfNecessary(area);
    CHECK(g_lookups == 6 && g_lookupMap == map && g_getDefs == 4);
    CHECK(Passable(0, 1) == 1 && Passable(1, 1) == 0 && Passable(2, 1) == 1);
    CHECK(Passable(0, 2) == 0 && Passable(1, 2) == 0 && Passable(2, 2) == 1);
    CHECK(g_normals == 4);                                  // only cells still passable after the theme test
    CHECK(Passable(3, 1) == 1 && Passable(0, 0) == 1 && Passable(0, 3) == 1);   // outside the area untouched
    CHECK(((unsigned char*)&g_cells[(3 - 1) * 4 + 1])[9] == 0xfd);              // only the passable bit cleared
    CHECK(g_defs[5]->m_refCount == 98 && g_defs[6]->m_refCount == 99 && g_defs[7]->m_refCount == 99 && g_deletes == 0);
    // passable bits set again from cleared cells
    memset(g_cells, 0, sizeof(g_cells));
    map->InitPassabilityFromThemesIfNecessary(area);
    CHECK(Passable(0, 1) == 1 && Passable(2, 2) == 1 && Passable(1, 2) == 0 && ((unsigned char*)&g_cells[(3 - 1) * 4 + 0])[9] == 2);
    if (fails == 0) printf("008201e0_TEST PASS\n"); else printf("008201e0_TEST FAIL\n");
    return fails;
}