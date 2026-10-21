// CTCGraphicAppearance::GetCameraFocalPos(float time) const -- the camera-focal helper point, else the focal helper point (flags 4), else GetFocalPos(time)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C12D0, length 116 bytes, `ret 8` (hidden result + float).
// FableWin.pdb: ?GetCameraFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@M@Z
// Callees: CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector&, float) const @ 0x004C0560
// (helper-name globals 0x013BAC4C then 0x013BAC48, flags 4), CTCGraphicAppearance::GetFocalPos(float) const @ 0x004C1060
// (the hidden result pointer is passed straight through, so the fallback is `return GetFocalPos(time);`).
// Shape: the found arm returns the local through C3DVector's user memberwise copy ctor (`fld/fstp` X, integer Y/Z); each
// lookup has its own `return pos;` (VC cross-jumps the two copies into one block) -- a `found` flag or `||` gives the
// second out-pointer lea in ecx and swaps the Y/Z copy registers.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CCharString { public: void* PStringData; };
class CThing;
extern const CCharString g_helperCameraFocal_013bac4c;   // 0x013BAC4C
extern const CCharString g_helperFocal_013bac48;         // 0x013BAC48

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& out, float time) const;   // 0x004C0560
    C3DVector GetFocalPos(float time) const;                // 0x004C1060
    C3DVector GetCameraFocalPos(float time) const;
};

C3DVector CTCGraphicAppearance::GetCameraFocalPos(float time) const
{
    C3DVector pos;
    if (GetHelperPoint(g_helperCameraFocal_013bac4c, 0, 4, pos, time))
        return pos;
    if (GetHelperPoint(g_helperFocal_013bac48, 0, 4, pos, time))
        return pos;
    return GetFocalPos(time);
}
}

#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
const CCharString g_helperCameraFocal_013bac4c = { 0 };
const CCharString g_helperFocal_013bac48 = { 0 };
static bool g_camOk = false, g_focOk = false; static int g_calls = 0, g_fallback = 0; static unsigned long g_flags = 0;
namespace NGraphicAppearance {
void CTCGraphicAppearance::v00() { }
bool CTCGraphicAppearance::GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& out, float time) const
{
    ++g_calls; g_flags = flags;
    bool ok = (&name == &g_helperCameraFocal_013bac4c) ? g_camOk : g_focOk;
    if (ok) { out.X = time; out.Y = (&name == &g_helperCameraFocal_013bac4c) ? 1.0f : 2.0f; out.Z = 3.0f; }
    return ok;
}
C3DVector CTCGraphicAppearance::GetFocalPos(float time) const
{
    ++g_fallback; C3DVector r; r.X = -time; r.Y = -1.0f; r.Z = -2.0f; return r;
}
}
using namespace NGraphicAppearance;
int main()
{
    static unsigned char mem[sizeof(CTCGraphicAppearance)];
    const CTCGraphicAppearance* a = (const CTCGraphicAppearance*)mem;
    g_camOk = true;
    C3DVector v = a->GetCameraFocalPos(5.0f);
    CHECK(g_calls == 1 && g_flags == 4 && v.X == 5.0f && v.Y == 1.0f && v.Z == 3.0f && g_fallback == 0);
    g_camOk = false; g_focOk = true;
    v = a->GetCameraFocalPos(6.0f);
    CHECK(g_calls == 3 && v.X == 6.0f && v.Y == 2.0f && g_fallback == 0);
    g_focOk = false;
    v = a->GetCameraFocalPos(7.0f);
    CHECK(g_calls == 5 && g_fallback == 1 && v.X == -7.0f && v.Y == -1.0f && v.Z == -2.0f);
    if (fails == 0) printf("004c12d0_TEST PASS\n");
    return fails;
}