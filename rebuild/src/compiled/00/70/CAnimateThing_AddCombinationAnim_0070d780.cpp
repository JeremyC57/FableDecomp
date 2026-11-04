// CAnimateThing::AddCombinationAnim(const CAnimInit& init, long fadeInFrames, long delay, bool force, bool ownedByMainAnim) -- unless the main anim set blocks combinations (Flags bit 0) and the add is not forced, build a CAnimSet from the init (OwnedByMainAnim, LastAlpha, AnimDelay; fading in over fadeInFrames from internal alpha 0, else fully on) and append it to the combination list (or the delayed list when delay != 0)
// Leaf: ?AddCombinationAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ_N1@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070D780, 305 bytes, thiscall `ret 14h`. Ego_r.pdb ?AddCombinationAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ_N1@Z.
// Callees: CAnimSet::Init(CAnimateThing&, const CAnimInit&, float) 0x0070D1A0 (Ego_r.pdb ?Init@CAnimSet@@AAEXAAVCAnimateThing@@ABVCAnimInit@@M@Z,
//   private), CWorld::GetFrame 0x0049D870 (inside the inlined CAnimSet::SetInternalAlpha, out of line at 0x0070C280), malloc
//   0x00BFEA0E (the 0x5C-byte STLport list node), CAnimSet(const CAnimSet&) 0x00710B80 (the implicit copy constructor, emitted out of
//   line), free 0x00BFEA14 (~CAnimSet's two vectors).
// Shape: the Init body is visible in this TU (retail 0x0070D1A0 is compiled just before this function; the landed body's statement
//   order), noinline: VC7.1 then knows the set does not escape and keeps its InternalAlphas start in ebp from right after Init
//   through the SetInternalAlpha stores to the final free. LastAlpha is stored before the OwnedByMainAnim bit (ecx = LastAlpha,
//   dl = the flag, as retail).
// Layout: Ego_r.pdb CAnimSet / CAnimInit / CTransitionBlendAlpha / CCompleteAnimation with STLport 4.6 containers (CAnimSet 0x54,
//   CCompleteAnimation 0x60: MainAnimSet +0, TransitionAnimSets +0x54, CombinationAnims +0x58); CAnimateThing's second CAnimSet
//   list +0x64 is TLC only. No engine header models these classes.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CWorld.h"                             // CWorld::GetFrame 0x0049D870 (static)

enum EAnimUpdateMode { };
class CAnimFrameInfo { public:
    float Current;
    float Last;
};
class CAnimInit { public:                             // Ego_r.pdb, 44 bytes
    EAnimUpdateMode UpdateMode;                       // +0x00
    unsigned char Type;                               // +0x04
    bool          Looping;                            // +0x05
    long          Anim;                               // +0x08
    float         Speed;                              // +0x0c
    float         Alpha;                              // +0x10
    float         LastAlpha;                          // +0x14
    CAnimFrameInfo Frame;                             // +0x18
    unsigned long Flags;                              // +0x20
    unsigned long TypeFlags;                          // +0x24
    long          Priority;                           // +0x28
};
class CTransitionBlendAlpha { public:                 // Ego_r.pdb, 8 bytes
    float CurrentAlpha;
    float LastAlpha;
    CTransitionBlendAlpha(float current, float last) : CurrentAlpha(current), LastAlpha(last) { }
};
class CAnimateThing;
class CAnimSet { public:                              // retail 0x54 bytes (Ego_r.pdb 0x5c)
    EAnimUpdateMode UpdateMode;                       // +0x00
    std::vector<short> Anims;                         // +0x04
    std::vector<CTransitionBlendAlpha> InternalAlphas;   // +0x10
    float CurrentPercentageComplete;                  // +0x1c
    float LastPercentageComplete;                     // +0x20
    float Speed;                                      // +0x24
    unsigned long FrameInternalAlphasUpdated;         // +0x28
    float CurrentAlpha;                               // +0x2c
    float LastAlpha;                                  // +0x30
    float LastUpdateChange;                           // +0x34
    float ChangeAlphaBy;                              // +0x38
    CAnimateThing* PAnimator;                         // +0x3c
    unsigned long Flags;                              // +0x40
    unsigned long TypeFlags;                          // +0x44
    long AnimDelay;                                   // +0x48
    long Priority;                                    // +0x4c
    unsigned char Type;                               // +0x50
    bool Looping : 1;                                 // +0x51 bit 0
    bool Finished : 1;
    bool Forwards : 1;
    bool HadUpdate : 1;
    bool OwnedByMainAnim : 1;                         // bit 4
    CAnimSet() { }
    void SetInternalAlpha(long index, float alpha, bool setLastToo)   // 0x0070C280 (inline here)
    {
        InternalAlphas[index].LastAlpha = InternalAlphas[index].CurrentAlpha;
        InternalAlphas[index].CurrentAlpha = alpha;
        if (setLastToo)
            InternalAlphas[index].LastAlpha = alpha;
        FrameInternalAlphasUpdated = CWorld::GetFrame();
    }
private:
    friend class CAnimateThing;
    __declspec(noinline) void Init(CAnimateThing& animator, const CAnimInit& init, float alpha)   // 0x0070D1A0
    {
        PAnimator = &animator;
        Anims.clear();
        Anims.push_back((short)init.Anim);
        InternalAlphas.clear();
        InternalAlphas.push_back(CTransitionBlendAlpha(init.Alpha, init.Alpha));
        FrameInternalAlphasUpdated = CWorld::GetFrame();
        CurrentPercentageComplete = init.Frame.Current;
        LastPercentageComplete = init.Frame.Last;
        CurrentAlpha = LastAlpha = alpha;
        LastUpdateChange = 0.0f;
        ChangeAlphaBy = 0.0f;
        Type = init.Type;
        UpdateMode = init.UpdateMode;
        Looping = init.Looping;
        Speed = init.Speed;
        Finished = false;
        Forwards = true;
        Flags = init.Flags;
        TypeFlags = init.TypeFlags;
        HadUpdate = false;
        Priority = init.Priority;
    }
};

class CCompleteAnimation { public:
    CAnimSet MainAnimSet;                             // +0x00
    std::list<CAnimSet> TransitionAnimSets;           // +0x54
    std::list<CAnimSet> CombinationAnims;             // +0x58
    bool MainAnimCanTransitionOut;                    // +0x5c
};
class CAnimateThing { public:
    class CDelayedAnimation;                          // Ego_r.pdb CAnimateThing::CDelayedAnimation (not touched here)
    CCompleteAnimation CurrentAnim;                   // +0x00
    std::list<CDelayedAnimation> DelayedAnimQueue;    // +0x60
    std::list<CAnimSet> DelayedCombinationAnims;      // +0x64 (TLC; not in Ego_r.pdb, name descriptive)
    void AddCombinationAnim(const CAnimInit& init, long fadeInFrames, long delay, bool force, bool ownedByMainAnim);
};

void CAnimateThing::AddCombinationAnim(const CAnimInit& init, long fadeInFrames, long delay, bool force, bool ownedByMainAnim)
{
    if (!force && (CurrentAnim.MainAnimSet.Flags & 1))
        return;
    if (!init.Anim)
        return;
    CAnimSet set;
    set.Init(*this, init, init.Alpha);
    set.LastAlpha = init.LastAlpha;
    set.OwnedByMainAnim = ownedByMainAnim;
    set.AnimDelay = delay;
    if (fadeInFrames > 0) {
        set.SetInternalAlpha(0, 0.0f, true);
        set.ChangeAlphaBy = 1.0f / fadeInFrames;
    } else {
        set.CurrentAlpha = 1.0f;
        set.LastAlpha = 1.0f;
        set.ChangeAlphaBy = 0.0f;
    }
    if (delay == 0)
        CurrentAnim.CombinationAnims.push_back(set);
    else
        DelayedCombinationAnims.push_back(set);
}