#pragma optimize("s", on)
// NPlayerGui::CDrawMiniMap::ToggleCentering() -- while the mini-map is active, either start the screen-position fade (after re-applying the hero's current region) or, when that fade is fully open, start closing it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064E019, length 136 bytes.
// Callees: CDrawMiniMap::IsActive() @ 0x00643E09, the world's virtual slot 13 (`call [eax+34h]`, the world map, twice),
// CWorldMap::GetCurrentRegionIndex() @ 0x004FB150 (`mov eax,[ecx+9Ch]`; manifest "CWorld::DrawGetEnvironment" is an ICF twin),
// CWorldMap::GetRegion(long) @ 0x004FC180 (0x58-byte regions), CDrawMiniMap::ApplyRegion(CRegion*) @ 0x0064DD9A (as the landed
// SetRegion 0x0064DFDE calls it), CFadeInFadeOutBase::Start(float, float, CRGBColour) @ 0x00434840 (manifest "CEngine::StartNewScene"
// is propagated), CFadeInFadeOutBase::IsOpen() @ 0x0048DA30. Constant 0.5f @ 0x0122F59C.
// Layout: engine/CDrawMiniMap.h is the PDB layout and does not fit the shipped build (see the constructor 0x0064E83F): CDrawBase
// World +0xc; ScreenPosFadeInOut +0x138 (CFadeInFadeOutBase: Active +0, Closing +0xc, Opening +0xd) -- local model.
// Shape: size TU (`fld ; push ecx ; fstp [esp]` float arguments); bl is the shared zero (the Active test, the colour bytes and
// the Opening store); the World member is written twice (no local): VC reads it once straight into ecx for the inner call
// and keeps the copy in edi for the outer one (a named `world` local recolours this/fade/world), the region index evaluated
// first as the inner argument; the colour is built in place in the argument area by a CRGBColour(r, g, b, a) constructor over
// the D3DCOLOR byte order (B, G, R, A), assigning R, G, B, A -- the user copy constructor makes VC build the by-value argument
// in its own stack slot (`push ecx ; mov eax,esp`) instead of a frame temporary pushed as a dword; the fade is reached through a
// pointer local (ebp) in both arms.
struct CRegion;

struct CRGBColour {                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) { }
};

struct CFadeInFadeOutBase {                        // PDB 0x1c bytes
    bool          Active;                          // +0x00
    float         FadeInTime;                      // +0x04
    float         FadeOutTime;                     // +0x08
    bool          Closing;                         // +0x0c
    bool          Opening;                         // +0x0d
    float         OpenTimer;                       // +0x10
    float         CloseTimer;                      // +0x14
    unsigned long ToColour;                        // +0x18
    void Start(float fadeInTime, float fadeOutTime, CRGBColour colour);   // 0x00434840
    bool IsOpen();                                 // 0x0048DA30
};

struct CWorldMap {
    long GetCurrentRegionIndex();                  // 0x004FB150
    CRegion& GetRegion(long index);                // 0x004FC180
};

struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* GetWorldMap();              // slot 13 (+0x34)
};

namespace NPlayerGui {

struct CDrawMiniMap {
    void*              _vtable;                    // +0x00
    void*              PlayerGui;                  // +0x04
    void*              PlayerManager;              // +0x08
    CWorld*            World;                      // +0x0c
    unsigned char      _body10[0x138 - 0x10];
    CFadeInFadeOutBase ScreenPosFadeInOut;         // +0x138
    bool IsActive();                               // 0x00643E09
    void ApplyRegion(CRegion* region);             // 0x0064DD9A
    void ToggleCentering();
};

void CDrawMiniMap::ToggleCentering()
{
    if (IsActive()) {
        CFadeInFadeOutBase* fade = &ScreenPosFadeInOut;
        if (!fade->Active) {
            ApplyRegion(&World->GetWorldMap()->GetRegion(World->GetWorldMap()->GetCurrentRegionIndex()));
            fade->Start(0.5f, 0.5f, CRGBColour(0, 0, 0, 0xff));
        }
        else if (fade->IsOpen()) {
            fade->Opening = false;
            fade->Closing = true;
        }
    }
}

}

#pragma optimize("", on)
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
void CWorld::v00() { }
void CWorld::v01() { }
void CWorld::v02() { }
void CWorld::v03() { }
void CWorld::v04() { }
void CWorld::v05() { }
void CWorld::v06() { }
void CWorld::v07() { }
void CWorld::v08() { }
void CWorld::v09() { }
void CWorld::v10() { }
void CWorld::v11() { }
void CWorld::v12() { }

static CWorldMap g_map;
static int g_getMap = 0, g_idx = 0, g_region = 0, g_apply = 0, g_start = 0, g_isOpen = 0;
static bool g_active = true, g_open = false;
static long g_regionIndex = -1; static CRegion* g_applied = 0;
static float g_in = 0, g_out = 0; static unsigned char g_col[4];
static unsigned char g_regions[4][0x58];
CWorldMap* CWorld::GetWorldMap() { ++g_getMap; return &g_map; }
long CWorldMap::GetCurrentRegionIndex() { ++g_idx; return 2; }
CRegion& CWorldMap::GetRegion(long index) { ++g_region; g_regionIndex = index; return *(CRegion*)g_regions[index]; }
void CFadeInFadeOutBase::Start(float fadeInTime, float fadeOutTime, CRGBColour colour)
{ ++g_start; g_in = fadeInTime; g_out = fadeOutTime; g_col[0] = colour.B; g_col[1] = colour.G; g_col[2] = colour.R; g_col[3] = colour.A; Active = true; }
bool CFadeInFadeOutBase::IsOpen() { ++g_isOpen; return g_open; }
namespace NPlayerGui {
bool CDrawMiniMap::IsActive() { return g_active; }
void CDrawMiniMap::ApplyRegion(CRegion* region) { ++g_apply; g_applied = region; }
}
using namespace NPlayerGui;
int main()
{
    static CWorld world;
    static unsigned char mem[sizeof(CDrawMiniMap)];
    CDrawMiniMap* m = (CDrawMiniMap*)mem;
    m->World = &world;
    m->ScreenPosFadeInOut.Active = false; m->ScreenPosFadeInOut.Opening = true; m->ScreenPosFadeInOut.Closing = false;
    m->ToggleCentering();
    CHECK(g_getMap == 2 && g_idx == 1 && g_region == 1 && g_regionIndex == 2);
    CHECK(g_apply == 1 && g_applied == (CRegion*)g_regions[2]);
    CHECK(g_start == 1 && g_in == 0.5f && g_out == 0.5f && g_col[0] == 0 && g_col[1] == 0 && g_col[2] == 0 && g_col[3] == 0xff);
    // now active, not open: nothing changes
    m->ToggleCentering();
    CHECK(g_isOpen == 1 && m->ScreenPosFadeInOut.Opening == true && m->ScreenPosFadeInOut.Closing == false && g_start == 1);
    g_open = true;
    m->ToggleCentering();
    CHECK(g_isOpen == 2 && m->ScreenPosFadeInOut.Opening == false && m->ScreenPosFadeInOut.Closing == true);
    g_active = false;
    m->ScreenPosFadeInOut.Active = false;
    m->ToggleCentering();
    CHECK(g_apply == 1 && g_start == 1 && g_isOpen == 2);
    if (fails == 0) printf("0064e019_TEST PASS\n");
    return fails;
}