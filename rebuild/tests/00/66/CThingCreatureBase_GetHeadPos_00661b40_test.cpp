#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingCreatureBase::GetHeadPos(bool recalculate) const -- the creature's head position: the cached offset from its position when valid (and no recalculation is asked for), otherwise the graphic's EYE_SET_L dummy, the focus_eye_01 or TARGET_CURSOR_POINT helper, or 1.5 above the position; the offset from the position is cached and marked valid
// Leaf: ?GetHeadPos@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00661B40, length 335 bytes (`ret 8`: hidden result + bool).
//   Signature from Ego_r.pdb `?GetHeadPos@CThingCreatureBase@@QBE?AVC3DVector@@_N@Z`.
// Callees: CThing::GetPosition() const @ 0x004C73D0, CTCGraphicAppearance::GetDummyObject(const CCharString&, long,
//   unsigned long, C3DVector*, CRightHandedSet*, float) @ 0x004C08C0, CTCGraphicAppearance::GetHelperPoint(const
//   CCharString&, long, unsigned long, C3DVector*, float) @ 0x004C0560; the names are the static CCharStrings
//   @ 0x013BAC54 "EYE_SET_L", 0x013BAC48 "focus_eye_01", 0x013BAC50 "TARGET_CURSOR_POINT"; 1.5f @ 0x0125AADC.
// Layout (retail): CThing::PTCGraphicAppearance +0x64 (Ego_r.pdb +0x74); CThingCreatureBase::HeadPosOffset +0x118
//   (PDB +0x13c: retail STLport vectors/lists are smaller) and its valid flag is bit 7 of the byte at +0x15c (mutable).
// Shape: the graphic is reached through an inline accessor (retail loads it into ecx before the pushes), the flag
//   default is `0` then `if (!recalculate) flags = 0x21`, and C3DVector has a declared memberwise copy ctor (the
//   returned `pos` is copied X through the FPU, Y/Z as dwords).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
__forceinline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CCharString { void* PStringData; };
extern CCharString g_CCharString_013bac54;              // "EYE_SET_L"
extern CCharString g_CCharString_013bac48;              // "focus_eye_01"
extern CCharString g_CCharString_013bac50;              // "TARGET_CURSOR_POINT"

struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* pos, float scale);   // 0x004C0560
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* set, float scale);   // 0x004C08C0
};

struct CThing {
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CTCGraphicAppearance* GetGraphicAppearance() const { return PTCGraphicAppearance; }
};

struct CThingCreatureBase : CThing {
    unsigned char _pad68[0x118 - 0x68];
    mutable C3DVector HeadPosOffset;                    // +0x118
    unsigned char _pad124[0x15c - 0x124];
    unsigned char _bits15c : 7;
    mutable unsigned char HeadPosOffsetValid : 1;       // +0x15c bit 7
    C3DVector GetHeadPos(bool recalculate) const;
};

C3DVector CThingCreatureBase::GetHeadPos(bool recalculate) const
{
    if (!recalculate && HeadPosOffsetValid)
        return GetPosition() + HeadPosOffset;
    unsigned long flags = 0;
    if (!recalculate)
        flags = 0x21;
    C3DVector pos;
    CRightHandedSet set;
    if (!GetGraphicAppearance()->GetDummyObject(g_CCharString_013bac54, 0, flags, &pos, &set, 1.0f)
        && !GetGraphicAppearance()->GetHelperPoint(g_CCharString_013bac48, 0, flags, &pos, 1.0f)
        && !GetGraphicAppearance()->GetHelperPoint(g_CCharString_013bac50, 0, flags, &pos, 1.0f)) {
        pos = GetPosition();
        pos.Z += 1.5f;
    }
    HeadPosOffset = pos - GetPosition();
    HeadPosOffsetValid = true;
    return pos;
}


CCharString g_CCharString_013bac54, g_CCharString_013bac48, g_CCharString_013bac50;
static C3DVector g_pos(10.0f, 20.0f, 30.0f);
const C3DVector& CThing::GetPosition() const { return g_pos; }
static int g_have = 0; static unsigned long g_flags = 99; static int g_calls = 0;
bool CTCGraphicAppearance::GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* set, float scale)
{ ++g_calls; g_flags = flags; if (&name == &g_CCharString_013bac54 && g_have == 1 && index == 0 && scale == 1.0f) { *pos = C3DVector(11.0f, 21.0f, 32.0f); return true; } return false; }
bool CTCGraphicAppearance::GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* pos, float scale)
{ ++g_calls; if (g_flags != flags) g_flags = 77; if (index != 0 || scale != 1.0f) return false;
  if (&name == &g_CCharString_013bac48 && g_have == 2) { *pos = C3DVector(12.0f, 22.0f, 33.0f); return true; }
  if (&name == &g_CCharString_013bac50 && g_have == 3) { *pos = C3DVector(13.0f, 23.0f, 34.0f); return true; }
  return false; }
#define CHECK(c) if (!(c)) { std::printf("00661b40_TEST FAIL line %d\n", __LINE__); return 1; }

int main()
{
    static CTCGraphicAppearance gfx; static CThingCreatureBase c; c.PTCGraphicAppearance = &gfx; c.HeadPosOffsetValid = 0; c._bits15c = 5;
    g_have = 1; C3DVector h = c.GetHeadPos(false);
    CHECK(h.X == 11.0f && h.Y == 21.0f && h.Z == 32.0f && g_flags == 0x21 && g_calls == 1 && c.HeadPosOffsetValid && c._bits15c == 5);
    CHECK(c.HeadPosOffset.X == 1.0f && c.HeadPosOffset.Y == 1.0f && c.HeadPosOffset.Z == 2.0f);
    g_pos = C3DVector(100.0f, 200.0f, 300.0f); g_calls = 0;
    h = c.GetHeadPos(false);                                              // cached offset
    CHECK(h.X == 101.0f && h.Y == 201.0f && h.Z == 302.0f && g_calls == 0);
    g_have = 2; h = c.GetHeadPos(true);                                   // forced: flags 0, helper eye
    CHECK(h.X == 12.0f && h.Y == 22.0f && h.Z == 33.0f && g_flags == 0 && g_calls == 2);
    CHECK(c.HeadPosOffset.X == -88.0f && c.HeadPosOffset.Z == -267.0f);
    g_have = 3; g_calls = 0; h = c.GetHeadPos(true);
    CHECK(h.X == 13.0f && h.Y == 23.0f && h.Z == 34.0f && g_calls == 3);
    g_have = 0; c.HeadPosOffsetValid = 0; h = c.GetHeadPos(false);          // nothing found: 1.5 above the position
    CHECK(h.X == 100.0f && h.Y == 200.0f && h.Z == 301.5f && c.HeadPosOffset.Z == 1.5f && c.HeadPosOffset.X == 0.0f && c.HeadPosOffsetValid);
    std::printf("00661b40_TEST PASS\n");
    return 0;
}