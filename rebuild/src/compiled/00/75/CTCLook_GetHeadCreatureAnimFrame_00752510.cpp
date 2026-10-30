// CTCLook::GetHeadCreatureAnimFrame(ECreatureAnim anim) const -- the head-turn animation frame (0..1, 0.5 = centred) for the horizontal (anim 11, XY) or vertical (anim 12, YZ) head-turn animation, from the head angle over the look def's turn range
// Grade: RELOCATION_MATCH (retry wave 2 sec2: GetHeadTurnRangeXY is the half range `0.5f * (range * 1/360)`, the same getter body that matches ChangeHeadAngleXY 0x00752120, and keeps the 1/360 and 0.5 multiplies apart; FableWin/Ego_r spell the getters and the leaf private ABE). Was DIFFER(159v165): retail multiplies the def range by 1/360 and then by 0.5 (`fmul [0x1238E00] ; fmul [0x122F59C]`), VC7.1 here folds (range * k) * 0.5 into one `fmul` by the pre-multiplied constant (6 bytes short; every other byte, the YZ arm and the clamp tail, matches). Tried: getter `range * k`, getter `k * range`, `range / 2.0f`, `0.5f * range`.
// Leaf: ?GetHeadCreatureAnimFrame@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00752510, 165 bytes (+11 int3), __fastcall `ret 4`, float in st(0).
// Name: FableWin.pdb `?GetHeadCreatureAnimFrame@CTCLook@@ABEMW4ECreatureAnim@@@Z` (name set); the only caller is
//   CTCLook::UpdateHeadCreatureAnim 0x00752F30 (call at 0x00752FE3), which ImplementHeadSettings 0x00753BA0 drives with
//   anim types 12 (vertical) and 11 (horizontal).
// Label correction: the manifest's "CTCLook_ComputeAnimationPlaybackRatio" is descriptive.
// Constants: 0.5f 0x0122F59C, 1/360 0x01238E00 (the def stores degrees), 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout: retail CTCLook PDef (CDefPointer<CLookDef const>) +0x14, HeadAngleRelToBodyXY +0x2C, HeadAngleRelToBodyYZ +0x30;
//   CLookDef (Ego_r.pdb) HeadTurnRangeXY +0x30, HeadTurnRangeYZUp +0x34, HeadTurnRangeYZDown +0x38 (degrees).
// Shape: XY arm `(angleXY * 0.5) / (0.5 * (rangeXY * k))`; YZ arm picks Up when angleYZ > 0 (else Down; separate def
//   reloads into edx / eax, a shared `fmul k`), then `angleYZ / range * 0.5`; both arms meet at `0.5 - frame`, then a
//   clamp to [0, 1] written as an in-range early return (`0 <= v && v <= 1`, constant loaded first) followed by the
//   `v < lo` / `v > hi` tests.
#include "engine/CLookDef.h"

enum ECreatureAnim { HEAD_ANIM_HORIZONTAL = 11, HEAD_ANIM_VERTICAL = 12 };

struct CLookDefPointer {
    const CLookDef* m_pDef;
    const CLookDef* operator->() const { return m_pDef; }
};

template <class T> inline T Clamp(T v, T lo, T hi)
{
    if (lo <= v && v <= hi)
        return v;
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

class CTCLook { public:
    unsigned char   _pad00[0x14];
    CLookDefPointer PDef;                   // +0x14
    unsigned char   _pad18[0x2c - 0x18];
    float           HeadAngleRelToBodyXY;   // +0x2c
    float           HeadAngleRelToBodyYZ;   // +0x30
private:
    float GetHeadTurnRangeXY() const { return 0.5f * (PDef->HeadTurnRangeXY * 0.0027777778f); }
    float GetHeadTurnRangeYZUp() const { return PDef->HeadTurnRangeYZUp * 0.0027777778f; }
    float GetHeadTurnRangeYZDown() const { return PDef->HeadTurnRangeYZDown * 0.0027777778f; }
    float GetHeadCreatureAnimFrame(ECreatureAnim anim) const;
};

float CTCLook::GetHeadCreatureAnimFrame(ECreatureAnim anim) const
{
    float frame;
    if (anim == HEAD_ANIM_HORIZONTAL) {
        frame = HeadAngleRelToBodyXY * 0.5f / GetHeadTurnRangeXY();
    }
    else
        frame = HeadAngleRelToBodyYZ / (HeadAngleRelToBodyYZ > 0.0f ? GetHeadTurnRangeYZUp() : GetHeadTurnRangeYZDown()) * 0.5f;
    frame = 0.5f - frame;
    return Clamp(frame, 0.0f, 1.0f);
}