// CAnimateThing::SetAsFinished() -- mark the main animation set of the current animation as finished
// Leaf: ?SetAsFinished@CAnimateThing@@QAEXXZ
// Retail: Fable.exe @ 0x0070BE80, 5 bytes: `or byte ptr [ecx+51h],2 ; ret`.
// Identity (proven): Ego_r.pdb names ?SetAsFinished@CAnimateThing@@QAEXXZ; the five direct callers (0x00694931, 0x00835F62,
// 0x0086269F, 0x008626B3, 0x008EAD0C) are creature-mode / AI code that ends a playing animation; the landed sibling
// CAnimateThing::IsAnimationFinished 0x0070C0E0 reads the same bit (+0x51 bit 1).
// Layout: CAnimateThing::CurrentAnim (CCompleteAnimation) +0x00 -> MainAnimSet (CAnimSet) +0x00; CAnimSet's flag byte
// (Looping, Finished, Forwards, HadUpdate, OwnedByMainAnim bit fields) sits at retail +0x51 (Ego_r +0x59: the PDB's two
// 16-byte vectors are 12-byte STLport vectors in retail).
class CAnimateThing;
class CAnimSet { public:
    long           UpdateMode;                  // +0x00 EAnimUpdateMode
    unsigned char  Anims[0xC];                  // +0x04 std::vector<short>
    unsigned char  InternalAlphas[0xC];         // +0x10 std::vector<CTransitionBlendAlpha>
    float          CurrentPercentageComplete;   // +0x1c
    float          LastPercentageComplete;      // +0x20
    float          Speed;                       // +0x24
    unsigned long  FrameInternalAlphasUpdated;  // +0x28
    float          CurrentAlpha;                // +0x2c
    float          LastAlpha;                   // +0x30
    float          LastUpdateChange;            // +0x34
    float          ChangeAlphaBy;               // +0x38
    CAnimateThing* PAnimator;                   // +0x3c
    unsigned long  Flags;                       // +0x40
    unsigned long  TypeFlags;                   // +0x44
    long           AnimDelay;                   // +0x48
    long           Priority;                    // +0x4c
    unsigned char  Type;                        // +0x50
    bool           Looping : 1;                 // +0x51 bit 0
    bool           Finished : 1;                // +0x51 bit 1
    bool           Forwards : 1;                // +0x51 bit 2
    bool           HadUpdate : 1;               // +0x51 bit 3
    bool           OwnedByMainAnim : 1;         // +0x51 bit 4
};
class CCompleteAnimation { public:
    CAnimSet MainAnimSet;                       // +0x00
};
class CAnimateThing { public:
    CCompleteAnimation CurrentAnim;             // +0x00
    void SetAsFinished();
};

void CAnimateThing::SetAsFinished()
{
    CurrentAnim.MainAnimSet.Finished = true;
}