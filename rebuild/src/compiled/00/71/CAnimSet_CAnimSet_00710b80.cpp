// CAnimSet::CAnimSet(const CAnimSet&) -- memberwise copy constructor of an animation set: update mode, the two vectors (out-of-line copy ctors), the timing/alpha/priority words, the type byte and the five flag bits
// Leaf: ??0CAnimSet@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00710B80, length 196 bytes (+12 int3), ret 4, returns `this`.
// Callees: vector<short>::vector(const vector&) @ 0x00710700 (catalog "CAnimSet::CopyConstruct_Shorts"),
// vector<CTransitionBlendAlpha>::vector(const vector&) @ 0x007101A0 (catalog "CAnimSet::CopyConstruct_Pairs").
// Layout: Ego_r.pdb CAnimSet (PDB 0x5C with 16-byte vectors; retail 0x54, STLport vectors 12 bytes): UpdateMode +0,
// Anims +4, InternalAlphas +0x10, CurrentPercentageComplete +0x1c .. Priority +0x4c, Type +0x50, bitfield byte +0x51
// (Looping, Finished, Forwards, HadUpdate, OwnedByMainAnim).
// Shape: the compiler-style copy: each one-bit field is merged into the destination byte with `xor ; and mask ; xor`,
// the byte re-read from the source for every bit.
struct CVectorShort {                              // vector<short> (STLport, 12 bytes)
    short* _M_start; short* _M_finish; short* _M_end_of_storage;
    CVectorShort(const CVectorShort& other);       // 0x00710700
};
class CTransitionBlendAlpha;
struct CVectorTransitionBlendAlpha {               // vector<CTransitionBlendAlpha> (STLport, 12 bytes)
    CTransitionBlendAlpha* _M_start; CTransitionBlendAlpha* _M_finish; CTransitionBlendAlpha* _M_end_of_storage;
    CVectorTransitionBlendAlpha(const CVectorTransitionBlendAlpha& other);   // 0x007101A0
};
class CAnimateThing;

class CAnimSet {
public:
    long UpdateMode;                               // +0x00 EAnimUpdateMode
    CVectorShort Anims;                            // +0x04
    CVectorTransitionBlendAlpha InternalAlphas;    // +0x10
    float CurrentPercentageComplete;               // +0x1c
    float LastPercentageComplete;                  // +0x20
    float Speed;                                   // +0x24
    unsigned long FrameInternalAlphasUpdated;      // +0x28
    float CurrentAlpha;                            // +0x2c
    float LastAlpha;                               // +0x30
    float LastUpdateChange;                        // +0x34
    float ChangeAlphaBy;                           // +0x38
    CAnimateThing* PAnimator;                      // +0x3c
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
    CAnimSet(const CAnimSet& other);
};

CAnimSet::CAnimSet(const CAnimSet& other)
    : UpdateMode(other.UpdateMode), Anims(other.Anims), InternalAlphas(other.InternalAlphas),
      CurrentPercentageComplete(other.CurrentPercentageComplete), LastPercentageComplete(other.LastPercentageComplete),
      Speed(other.Speed), FrameInternalAlphasUpdated(other.FrameInternalAlphasUpdated), CurrentAlpha(other.CurrentAlpha),
      LastAlpha(other.LastAlpha), LastUpdateChange(other.LastUpdateChange), ChangeAlphaBy(other.ChangeAlphaBy),
      PAnimator(other.PAnimator), Flags(other.Flags), TypeFlags(other.TypeFlags), AnimDelay(other.AnimDelay),
      Priority(other.Priority), Type(other.Type)
{
    Looping = other.Looping;
    Finished = other.Finished;
    Forwards = other.Forwards;
    HadUpdate = other.HadUpdate;
    OwnedByMainAnim = other.OwnedByMainAnim;
}