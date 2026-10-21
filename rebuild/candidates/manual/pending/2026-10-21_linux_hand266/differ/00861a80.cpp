// Grade: DIFFER(473v486) -- register assignment and one temporary: retail holds this in ebp, to in edi, from in esi and the to-delay in ebx (the lane: this ebx, to esi, from edi, delay ebp), and copies the animate's frame info through an 8-byte frame temporary into init.Frame (frame 0x34 v 0x2c); a named frame local and a user copy ctor on CAnimFrameInfo both still elide it. Delay max (both delays in the dead argument slots), alpha clamp, the same-pair re-weight and the main/blend set-up are in retail order.
// CModeStandBase::SetUpMovementAnim(CAnimateThing& animate, EExistingAnimStatus status, float speed, const CMovementSetup&
// from, const CMovementSetup& to) const -- blend between the two movement anims by where the speed falls between their
// maximum speeds; when the pair is already playing just re-weight both layers, otherwise start the `from` anim as the
// main anim (keeping the current frame when the old anim is being replaced) and add `to` as the weighted blend
// Leaf: SetUpMovementAnim
// Retail: Fable.exe (TLC, VC7.1) @ 0x00861A80, 0x1E6 bytes (padded to 496), `ret 14h`.
// Callees: CAnimationEntry 0x005DC1F0 (delay), 0x005DC390 GetAnimateThingFlags, 0x005DC230 GetTimeToBlendIn;
//   CAnimEntryLite::GetTimeToBlendIn 0x0073A690; CAnimInit::Set 0x0070C050; CAnimateThing::SetInternalAlpha 0x0070C280,
//   SetMainAnim 0x0070D580 and the blend-anim add 0x0070D2F0. Data: epsilon 0x0129BA3C, 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout: local model. CMovementSetup per the PDB (MaxMovementSpeed +0, Type +4, PAnimEntry +8, PAnimEntryLite +0xC);
//   CModeStandBase owner at +4, ForceAddNewAnims +0x24; CAnimateThing state at +0 and current frame info at +0x1C.
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
};
struct CMovementSetup {
    float            MaxMovementSpeed;              // +0x00
    unsigned char    Type;                          // +0x04
    CAnimationEntry* PAnimEntry;                    // +0x08
    CAnimEntryLite*  PAnimEntryLite;                // +0x0C
    long GetDelayTime() const { return PAnimEntry ? PAnimEntry->GetDelayTime() : PAnimEntryLite->DelayTime; }
    unsigned long GetAnimFlags() const { return PAnimEntry ? PAnimEntry->GetAnimateThingFlags() : PAnimEntryLite->AnimFlags; }
    long GetBankIndex() const { return PAnimEntry ? PAnimEntry->BankIndex : PAnimEntryLite->BankIndex; }
    long GetTimeToBlendIn(CThing* thing) const
    {
        return PAnimEntry ? PAnimEntry->GetTimeToBlendIn(thing) : PAnimEntryLite->GetTimeToBlendIn(thing);
    }
};
struct CAnimFrameInfo {
    long A, B;
    CAnimFrameInfo() : A(0), B(0) {}
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
    void Set(long type, long mode, long anim, long a, long b, unsigned long flags, long priority);   // 0x0070C050
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
    CAnimInit init;
    init.Set(to.Type, 0, from.GetBankIndex(), 2, 1, from.GetAnimFlags(), 100);
    long blendTime = to.GetTimeToBlendIn(Owner);
    if (status == STATUS_REPLACE || status == STATUS_NONE || animate.State == 2) {
        init.Frame = animate.GetFrame();
        animate.State = 4;
    } else
        blendTime = 3;
    animate.SetMainAnim(&init, blendTime, delay);
    animate.SetInternalAlpha(0, 1.0f - alpha, false);
    animate.AddBlendAnim(to.GetBankIndex(), alpha);
}
