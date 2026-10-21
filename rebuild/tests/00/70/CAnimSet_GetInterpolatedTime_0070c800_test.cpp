// CAnimSet::GetInterpolatedTime(float alpha, long animIndex) const -- the time into one of the set's animations between the last and current completion fractions: looping sets wrap through the interpolation helper, others lerp linearly, both scaled by the animation's duration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C800, length 168 bytes, ret 8, returns float
// (?GetInterpolatedTime@CAnimSet@@ABEMMJ@Z: private const).
// Callees: AnimationInterpolation_CalcValue(bool forwards, float last, float current, float alpha, float range) @ 0x0070BD80
// (static __fastcall: the bool in cl, four floats on the stack, ret 0x10); CMeshDataBank::PeekAnimation(unsigned long) const
// @ 0x00A242C0 (catalog "CTCInventoryAbilities::GetAbilityLevel", propagated; body `return Animations[index]` at +0x380, Ego_r
// 0x00B60570); C3DAnimationInfo::GetNonLoopingDuration() const @ 0x00A23CE0 (catalog "CMap::DrawGetSeaLevel", propagated;
// body `fld [ecx+14h]`). Global: g_pMainGameComponent @ 0x013B86A0 (+0x24 World -> +0x3c PMainMeshBank).
// Shape: Ego_r has the private inline `CAnimSet::PeekAnimInfo(short) const` returning `const C3DAnimationInfo&`; with the bank
// in a named local it keeps retail's split index load (`mov ax,[..]` early, `movsx eax,ax` after the World/bank chain).
// Last/Current are copied into two float locals first (Last loaded first); the Looping bit (bit 0 of +0x51) picks the
// branch; the looping branch reads the Forwards bit (bit 2) into a bool local after PeekAnimInfo (that schedules the
// `mov cl,[esi+51h]` before the argument pushes) and multiplies the helper's result by info.Duration (+0x10) after the
// call; the linear branch multiplies GetNonLoopingDuration() by `(current - last) * alpha + last`.
// Layout: retail CAnimSet (see the copy ctor 0x00710B80): Anims +4 (vector<short>), CurrentPercentageComplete +0x1c,
// LastPercentageComplete +0x20, flag byte +0x51.
#define C3DAnimationInfo C3DAnimationInfo_PDB
#include "engine/C3DAnimationInfo.h"
#undef C3DAnimationInfo
struct C3DAnimationInfo : C3DAnimationInfo_PDB {
    float GetNonLoopingDuration() const;           // 0x00A23CE0
};
struct CMeshDataBank {
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};
struct CWorld {
    unsigned char _pad[0x3c];
    CMeshDataBank* PMainMeshBank;                  // +0x3c
};
struct CMainGameComponent {
    unsigned char _pad[0x24];
    CWorld* World;                                 // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

float __fastcall AnimationInterpolation_CalcValue(bool forwards, float last, float current, float alpha, float range);   // 0x0070BD80

struct CVectorShort {
    short* _M_start; short* _M_finish; short* _M_end_of_storage;
    const short& operator[](long i) const { return _M_start[i]; }
};

class CAnimSet {
public:
    long UpdateMode;                               // +0x00
    CVectorShort Anims;                            // +0x04
    unsigned char InternalAlphas[0xc];             // +0x10 vector<CTransitionBlendAlpha>
    float CurrentPercentageComplete;               // +0x1c
    float LastPercentageComplete;                  // +0x20
    unsigned char _pad24[0x2c];                    // +0x24 .. +0x4f
    unsigned char Type;                            // +0x50
    bool Looping : 1;                              // +0x51 bit 0
    bool Finished : 1;                             // bit 1
    bool Forwards : 1;                             // bit 2
    bool HadUpdate : 1;                            // bit 3
    bool OwnedByMainAnim : 1;                      // bit 4
private:
    const C3DAnimationInfo& PeekAnimInfo(short anim) const
    {
        CMeshDataBank* bank = g_pMainGameComponent->World->PMainMeshBank;
        return *bank->PeekAnimation(anim);
    }
public:
    float GetInterpolatedTime(float alpha, long animIndex) const;
};

float CAnimSet::GetInterpolatedTime(float alpha, long animIndex) const
{
    float last = LastPercentageComplete;
    float current = CurrentPercentageComplete;
    if (Looping) {
        const C3DAnimationInfo& info = PeekAnimInfo(Anims[animIndex]);
        bool forwards = Forwards;
        return AnimationInterpolation_CalcValue(forwards, last, current, alpha, 1.0f) * info.Duration;
    }
    return PeekAnimInfo(Anims[animIndex]).GetNonLoopingDuration() * ((current - last) * alpha + last);
}

// ---- test harness ----
#include <stdio.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

static C3DAnimationInfo g_infos[4];
static CMeshDataBank g_bank;
static CWorld g_world;
static CMainGameComponent g_mgc;
CMainGameComponent* g_pMainGameComponent = &g_mgc;
static unsigned long g_lastIndex;
const C3DAnimationInfo* CMeshDataBank::PeekAnimation(unsigned long index) const { g_lastIndex = index; return &g_infos[index]; }
float C3DAnimationInfo::GetNonLoopingDuration() const { return NonLoopingDuration; }
static bool g_fw; static float g_l, g_c, g_a, g_r;
float __fastcall AnimationInterpolation_CalcValue(bool forwards, float last, float current, float alpha, float range)
{
    g_fw = forwards; g_l = last; g_c = current; g_a = alpha; g_r = range;
    return forwards ? 0.75f : 0.25f;
}

int main()
{
    g_world.PMainMeshBank = &g_bank;
    g_mgc.World = &g_world;
    for (int k = 0; k < 4; ++k) { g_infos[k].Duration = 2.0f * (k + 1); g_infos[k].NonLoopingDuration = 4.0f * (k + 1); }
    static short anims[3] = { 2, 0, 3 };
    static unsigned char raw[0x5c];
    CAnimSet* set = (CAnimSet*)raw;
    set->Anims._M_start = anims; set->Anims._M_finish = anims + 3; set->Anims._M_end_of_storage = anims + 3;
    set->LastPercentageComplete = 0.25f;
    set->CurrentPercentageComplete = 0.75f;
    raw[0x51] = 0;                                  // not looping
    CHECK(set->GetInterpolatedTime(0.5f, 0) == 12.0f * ((0.75f - 0.25f) * 0.5f + 0.25f));
    CHECK(g_lastIndex == 2);
    CHECK(set->GetInterpolatedTime(1.0f, 1) == 4.0f * 0.75f);
    CHECK(g_lastIndex == 0);
    raw[0x51] = 1 | 4;                              // looping, forwards
    CHECK(set->GetInterpolatedTime(0.5f, 2) == 0.75f * 8.0f);
    CHECK(g_lastIndex == 3 && g_fw && g_l == 0.25f && g_c == 0.75f && g_a == 0.5f && g_r == 1.0f);
    raw[0x51] = 1;                                  // looping, backwards
    CHECK(set->GetInterpolatedTime(0.125f, 1) == 0.25f * 2.0f);
    CHECK(g_lastIndex == 0 && !g_fw && g_a == 0.125f);
    if (fails == 0) printf("0070c800_TEST PASS\n");
    return fails;
}