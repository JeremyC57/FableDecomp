// CModeStandBase::SetUpMovementAnim(CAnimateThing& animate, EExistingAnimStatus status, float speed, const CMovementSetup& from, const CMovementSetup& to) const -- blend between the two movement anims by where the speed falls between their maximum speeds: when the pair is already playing (and new anims are not forced) re-weight both layers; otherwise start `from` as the main anim (keeping the current frame unless the existing anim is the same pair on a non-finished state) and add `to` as the weighted blend
// Leaf: SetUpMovementAnim
// Retail: Fable.exe (TLC, VC7.1) @ 0x00861A80, 0x1E6 bytes (padded to 496), `ret 14h`. PDB name
//   ?SetUpMovementAnim@CModeStandBase@@ABEXAAVCAnimateThing@@W4EExistingAnimStatus@1@MABVCMovementSetup@1@2@Z (private; the
//   enum and CMovementSetup are nested in CModeStandBase; public here so the behaviour test can call it).
// Callees: CAnimationEntry 0x005DC1F0 (delay), 0x005DC390 GetAnimateThingFlags, 0x005DC230 GetTimeToBlendIn;
//   CAnimEntryLite::GetTimeToBlendIn 0x0073A690; CAnimInit::Set 0x0070C050; CAnimateThing::SetInternalAlpha 0x0070C280,
//   SetMainAnim 0x0070D580 and the blend-anim add 0x0070D2F0. Data: epsilon 0x0129BA3C, 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout: engine/CModeStandBase.h (included below as CModeStandBasePdb) agrees on ForceAddNewAnims +0x24 but declares no
//   methods and pads +0x04 (the flattened base's owner pointer), so the method's class is declared here with the same
//   offsets. CMovementSetup and CAnimEntryLite per the PDB (MaxMovementSpeed +0, Type +4, PAnimEntry +8, PAnimEntryLite
//   +0xC; BankIndex, DelayTime, TransitionTime, AnimFlags); CAnimInit 0x2c bytes as the landed CTCBed model; CAnimateThing
//   state at +0 and current frame info at +0x1C.
// Shape: the lite branches go through the PDB's inline CAnimEntryLite accessors (a direct DelayTime read lets VC7.1 assign
//   the to-delay to ebx inside the ternary arms); the delays meet in Max's const-reference temporaries (dead argument
//   slots); CAnimInit is built by an inline constructor that calls Set (the frame-info zero stores land after the argument
//   pushes) and Set's first parameter is an unsigned char (`xor eax,eax ; mov al`); CAnimFrameInfo has a memberwise copy
//   constructor (the 8-byte frame temporary at [esp+10h]); a status other than none/replace sets the short blend before the
//   finished-state test, which shares the single frame-copy block.
#define CModeStandBase CModeStandBasePdb
#include "engine/CModeStandBase.h"
#undef CModeStandBase
#include <math.h>
struct CThing;
extern float g_epsilon_0129ba3c;                    // 0x0129BA3C
struct CAnimationEntry {
    long BankIndex;                                 // +0x00
    long GetDelayTime() const;                      // 0x005DC1F0
    unsigned long GetAnimateThingFlags() const;     // 0x005DC390
    long GetTimeToBlendIn(CThing* thing) const;     // 0x005DC230
};
struct CAnimEntryLite {
    long BankIndex;                                 // +0x00
    long DelayTime;                                 // +0x04
    long TransitionTime;                            // +0x08
    long AnimFlags;                                 // +0x0C
    long GetTimeToBlendIn(CThing* thing) const;     // 0x0073A690
    long GetDelayTime() const { return DelayTime; }
    long GetAnimateThingFlags() const { return AnimFlags; }
};
struct CAnimFrameInfo {
    float A, B;
    CAnimFrameInfo() : A(0.0f), B(0.0f) {}
    CAnimFrameInfo(const CAnimFrameInfo& o) : A(o.A), B(o.B) {}
};
struct CAnimInit {
    long           UpdateMode;                      // +0x00
    unsigned char  Type;                            // +0x04
    bool           Looping;                         // +0x05
    long           Anim;                            // +0x08
    float          Speed;                           // +0x0C
    float          Alpha;                           // +0x10
    float          LastAlpha;                       // +0x14
    CAnimFrameInfo Frame;                           // +0x18
    unsigned long  Flags;                           // +0x20
    unsigned long  TypeFlags;                       // +0x24
    long           Priority;                        // +0x28
    void Set(unsigned char type, long mode, long anim, long a, bool b, unsigned long flags, long priority);   // 0x0070C050
    CAnimInit(unsigned char type, long mode, long anim, long a, bool b, unsigned long flags, long priority)
    {
        Set(type, mode, anim, a, b, flags, priority);
    }
};
struct CAnimateThing {
    long           State;                           // +0x00
    unsigned char  _pad04[0x18];
    CAnimFrameInfo Frame;                           // +0x1C
    CAnimFrameInfo GetFrame() const { return Frame; }
    void SetInternalAlpha(long layer, float alpha, bool immediate);  // 0x0070C280
    void SetMainAnim(CAnimInit* init, long blendTime, long delay);   // 0x0070D580
    void AddBlendAnim(long anim, float alpha);                       // 0x0070D2F0
};
template<class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
class CModeStandBase {
public:
    enum EExistingAnimStatus { STATUS_NONE, STATUS_SAME, STATUS_REPLACE };
    struct CMovementSetup {
        float            MaxMovementSpeed;              // +0x00
        unsigned char    Type;                          // +0x04
        CAnimationEntry* PAnimEntry;                    // +0x08
        CAnimEntryLite*  PAnimEntryLite;                // +0x0C
        long GetDelayTime() const { return PAnimEntry ? PAnimEntry->GetDelayTime() : PAnimEntryLite->GetDelayTime(); }
        unsigned long GetAnimFlags() const { return PAnimEntry ? PAnimEntry->GetAnimateThingFlags() : PAnimEntryLite->GetAnimateThingFlags(); }
        long GetBankIndex() const { return PAnimEntry ? PAnimEntry->BankIndex : PAnimEntryLite->BankIndex; }
        long GetTimeToBlendIn(CThing* thing) const
        {
            return PAnimEntry ? PAnimEntry->GetTimeToBlendIn(thing) : PAnimEntryLite->GetTimeToBlendIn(thing);
        }
    };
    void*         _vptr;                            // +0x00
    CThing*       Owner;                            // +0x04
    unsigned char _pad08[0x1c];
    bool          ForceAddNewAnims;                 // +0x24
    void SetUpMovementAnim(CAnimateThing& animate, EExistingAnimStatus status, float speed, const CMovementSetup& from,
                           const CMovementSetup& to) const;
};
void CModeStandBase::SetUpMovementAnim(CAnimateThing& animate, EExistingAnimStatus status, float speed,
                                       const CMovementSetup& from, const CMovementSetup& to) const
{
    long delay = Max(from.GetDelayTime(), to.GetDelayTime());
    float alpha = 1.0f;
    float range = to.MaxMovementSpeed - from.MaxMovementSpeed;
    if (fabs(range) > g_epsilon_0129ba3c) {
        alpha = (speed - from.MaxMovementSpeed) / range;
        if (alpha < 0.0f)
            alpha = 0.0f;
        else if (alpha > 1.0f)
            alpha = 1.0f;
    }
    if (status == STATUS_SAME && !ForceAddNewAnims) {
        animate.SetInternalAlpha(0, 1.0f - alpha, false);
        animate.SetInternalAlpha(status, alpha, false);
        return;
    }
    CAnimInit init(to.Type, 0, from.GetBankIndex(), 2, true, from.GetAnimFlags(), 100);
    long blendTime = to.GetTimeToBlendIn(Owner);
    if (status == STATUS_REPLACE || status == STATUS_NONE || (blendTime = 3, animate.State == 2)) {
        init.Frame = animate.GetFrame();
        animate.State = 4;
    }
    animate.SetMainAnim(&init, blendTime, delay);
    animate.SetInternalAlpha(0, 1.0f - alpha, false);
    animate.AddBlendAnim(to.GetBankIndex(), alpha);
}