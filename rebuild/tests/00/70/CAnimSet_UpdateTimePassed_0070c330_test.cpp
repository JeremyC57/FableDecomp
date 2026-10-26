// Behaviour test for 0070c330: final.cpp copied verbatim (the engine header modelled locally), the declared callees
// stubbed below so calls and arguments are observable. CAnimSet::UpdateTimePassed(float)
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static char g_log[1024];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CAnimSet::UpdateTimePassed(float time) -- advance the set's completion fraction by time * Speed over its first animation's duration (looping sets wrap both fractions past 1 or below 0, others clamp to [0,1] and finish at 1), recording the signed change; a zero-length animation finishes at once
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C330, length 333 bytes, ret 4.
// Callees: CMeshDataBank::PeekAnimation(unsigned long) const @ 0x00A242C0 (catalog "CTCInventoryAbilities::GetAbilityLevel",
// propagated), C3DAnimationInfo::GetNonLoopingDuration() const @ 0x00A23CE0 (catalog "CMap::DrawGetSeaLevel", propagated).
// Globals: g_pMainGameComponent @ 0x013B86A0 (+0x24 World -> +0x3c PMainMeshBank), EPSILON 0.0001f @ 0x0129BA3C,
// 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC.
// Layout: retail CAnimSet = PDB - 8 (12-byte vectors): Anims +4, CurrentPercentageComplete +0x1c, LastPercentageComplete
// +0x20, Speed +0x24, LastUpdateChange +0x34, flag bits +0x51 (Looping 1, Finished 2, Forwards 4).
// Shape: `fabsf(time) < EPSILON` early out (Last = Current as an integer copy); `time *= Speed` is written back into the
// argument slot; the first animation's info comes through the landed inline PeekAnimInfo (bank in a named local: split
// `mov ax` / `movsx` index load); `Looping ? info.Duration : info.GetNonLoopingDuration()`; the main body sits inside
// `if (duration > EPSILON)` with the zero-length finish (`Current = 1.0f; Finished = true;`, the store scheduled between
// the flag load and store) as the fallthrough tail; the Forwards/Looping tests share one flag-byte load (al), and the
// non-looping clamp goes through `float& current = CurrentPercentageComplete;` -- a store through the reference stops
// VC7.1 reusing that byte for the final `Finished = true`, which retail does as `or byte ptr [esi+51h],2`.
// Leaf: UpdateTimePassed
struct C3DAnimationInfo_PDB {                      // test model of engine/C3DAnimationInfo.h: Duration +0x10, NonLoopingDuration +0x14
    unsigned char _pad00[0x10];
    float Duration;
    float NonLoopingDuration;
    unsigned char _pad18[0x28 - 0x18];
};
class C3DAnimationInfo : public C3DAnimationInfo_PDB { public:
    float GetNonLoopingDuration() const;           // 0x00A23CE0
};
class CMeshDataBank { public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};
class CWorld { public:
    unsigned char _pad[0x3c];
    CMeshDataBank* PMainMeshBank;                  // +0x3c
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld* World;                                 // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern const float EPSILON;                        // 0x0129BA3C
#include <math.h>

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
    float Speed;                                   // +0x24
    unsigned long FrameInternalAlphasUpdated;      // +0x28
    float CurrentAlpha;                            // +0x2c
    float LastAlpha;                               // +0x30
    float LastUpdateChange;                        // +0x34
    float ChangeAlphaBy;                           // +0x38
    void* PAnimator;                               // +0x3c
    unsigned long Flags;                           // +0x40
    unsigned long TypeFlags;                       // +0x44
    long AnimDelay;                                // +0x48
    long Priority;                                 // +0x4c
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
    void UpdateTimePassed(float time);
};

void CAnimSet::UpdateTimePassed(float time)
{
    if (fabsf(time) < EPSILON) {
        LastPercentageComplete = CurrentPercentageComplete;
        return;
    }
    LastPercentageComplete = CurrentPercentageComplete;
    time *= Speed;
    const C3DAnimationInfo& info = PeekAnimInfo(Anims[0]);
    float duration = Looping ? info.Duration : info.GetNonLoopingDuration();
    if (duration > EPSILON) {
        float delta = time / duration;
        if (Forwards)
            CurrentPercentageComplete += delta;
        else {
            CurrentPercentageComplete -= delta;
            delta = -delta;
        }
        LastUpdateChange = delta;
        if (Looping) {
            if (LastPercentageComplete > 1.0f) {
                LastPercentageComplete -= 1.0f;
                CurrentPercentageComplete -= 1.0f;
            } else if (LastPercentageComplete < 0.0f) {
                LastPercentageComplete += 1.0f;
                CurrentPercentageComplete += 1.0f;
            }
        } else {
            float& current = CurrentPercentageComplete;
            if (current < 0.0f)
                current = 0.0f;
            else if (current > 1.0f)
                current = 1.0f;
            if (1.0f - EPSILON <= current)
                Finished = true;
        }
        return;
    }
    CurrentPercentageComplete = 1.0f;
    Finished = true;
}

// ---- stubs ----
const float EPSILON = 0.0001f;
static C3DAnimationInfo g_info;
static CMeshDataBank g_bank;
static CWorld g_world;
static CMainGameComponent g_component;
CMainGameComponent* g_pMainGameComponent = &g_component;
const C3DAnimationInfo* CMeshDataBank::PeekAnimation(unsigned long index) const
{
    char b[64]; _snprintf(b, sizeof b, "peek%lu", index); note(b); CHECK(this == &g_bank); return &g_info;
}
float C3DAnimationInfo::GetNonLoopingDuration() const { note("nonloop"); CHECK(this == &g_info); return NonLoopingDuration; }

static short g_anims[2] = { 7, 9 };
static CAnimSet make(float current, float last, float speed, bool looping, bool forwards)
{
    CAnimSet a; memset(&a, 0, sizeof a);
    a.Anims._M_start = g_anims; a.Anims._M_finish = g_anims + 2; a.Anims._M_end_of_storage = g_anims + 2;
    a.CurrentPercentageComplete = current; a.LastPercentageComplete = last; a.Speed = speed;
    a.Looping = looping; a.Forwards = forwards; a.LastUpdateChange = -99.0f;
    return a;
}
static bool near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-5f; }

int main()
{
    g_world.PMainMeshBank = &g_bank; g_component.World = &g_world;
    g_info.Duration = 2.0f; g_info.NonLoopingDuration = 4.0f;
    // 1. no time passed (either sign): only Last = Current
    { CAnimSet a = make(0.3f, 0.1f, 1.0f, true, true); g_log[0] = 0; a.UpdateTimePassed(0.00005f);
      CHECK(near(a.LastPercentageComplete, 0.3f) && near(a.CurrentPercentageComplete, 0.3f)); CHECK(g_log[0] == 0); CHECK(a.LastUpdateChange == -99.0f);
      a.UpdateTimePassed(-0.00005f); CHECK(g_log[0] == 0); }
    // 2. zero-length animation (looping duration): finished at once, Current = 1
    { CAnimSet a = make(0.3f, 0.1f, 1.0f, true, true); g_info.Duration = 0.0f; g_log[0] = 0; a.UpdateTimePassed(0.5f);
      CHECK(strcmp(g_log, "peek7;") == 0); CHECK(a.Finished && a.CurrentPercentageComplete == 1.0f && near(a.LastPercentageComplete, 0.3f)); CHECK(a.LastUpdateChange == -99.0f);
      g_info.Duration = 2.0f; }
    // 3. looping, forwards, no wrap: Current += time*Speed/Duration
    { CAnimSet a = make(0.25f, 0.0f, 2.0f, true, true); g_log[0] = 0; a.UpdateTimePassed(0.5f);
      CHECK(strcmp(g_log, "peek7;") == 0); CHECK(near(a.CurrentPercentageComplete, 0.75f) && near(a.LastPercentageComplete, 0.25f)); CHECK(near(a.LastUpdateChange, 0.5f)); CHECK(!a.Finished); }
    // 4. looping, backwards, Last > 1 after the copy: both wrap down, change negative
    { CAnimSet a = make(1.5f, 0.0f, 1.0f, true, false); g_log[0] = 0; a.UpdateTimePassed(0.5f);
      CHECK(near(a.CurrentPercentageComplete, 1.5f - 0.25f - 1.0f) && near(a.LastPercentageComplete, 0.5f)); CHECK(near(a.LastUpdateChange, -0.25f)); }
    // 5. looping, Last < 0: both wrap up
    { CAnimSet a = make(-0.5f, 0.0f, 1.0f, true, true); g_log[0] = 0; a.UpdateTimePassed(1.0f);
      CHECK(near(a.CurrentPercentageComplete, -0.5f + 0.5f + 1.0f) && near(a.LastPercentageComplete, 0.5f)); CHECK(near(a.LastUpdateChange, 0.5f)); }
    // 6. non-looping (NonLoopingDuration), forwards, no clamp, not finished
    { CAnimSet a = make(0.1f, 0.0f, 1.0f, false, true); g_log[0] = 0; a.UpdateTimePassed(1.0f);
      CHECK(strcmp(g_log, "peek7;nonloop;") == 0); CHECK(near(a.CurrentPercentageComplete, 0.35f)); CHECK(!a.Finished); CHECK(near(a.LastUpdateChange, 0.25f)); }
    // 7. non-looping, backwards below zero: clamped to 0, not finished
    { CAnimSet a = make(0.1f, 0.0f, 1.0f, false, false); g_log[0] = 0; a.UpdateTimePassed(1.0f);
      CHECK(a.CurrentPercentageComplete == 0.0f); CHECK(!a.Finished); CHECK(near(a.LastUpdateChange, -0.25f)); }
    // 8. non-looping over one: clamped to 1 and finished
    { CAnimSet a = make(0.9f, 0.0f, 1.0f, false, true); g_log[0] = 0; a.UpdateTimePassed(1.0f);
      CHECK(a.CurrentPercentageComplete == 1.0f); CHECK(a.Finished); }
    // 9. non-looping landing within EPSILON of one without clamping: finished
    { CAnimSet a = make(0.5f, 0.0f, 1.0f, false, true); g_log[0] = 0; a.UpdateTimePassed(1.99995f);
      CHECK(a.CurrentPercentageComplete < 1.0f && a.CurrentPercentageComplete >= 1.0f - EPSILON); CHECK(a.Finished); }
    // 10. zero-length non-looping animation
    { CAnimSet a = make(0.5f, 0.0f, 1.0f, false, true); g_info.NonLoopingDuration = 0.00005f; g_log[0] = 0; a.UpdateTimePassed(1.0f);
      CHECK(strcmp(g_log, "peek7;nonloop;") == 0); CHECK(a.Finished && a.CurrentPercentageComplete == 1.0f); g_info.NonLoopingDuration = 4.0f; }

    if (fails == 0) printf("0070c330_TEST PASS\n"); else printf("0070c330_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}