// CTCCreatureGenerator::OnCreate() -- fetches the generator def from the parent thing's def class; with a def, the random effect / sound countdowns are set to its intervals in frames; the activation frame starts one self-trigger reset interval in the past; the parent thing becomes level-persistent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749EA0, length 95 bytes.
// Callees: CDefClassBase::GetSubDef<CCreatureGeneratorDef>(CDefPointer<CCreatureGeneratorDef const>&) @
// 0x0074D160 (landed). Global: the frame-rate long @ 0x01375550 (15).
// Layout: engine/CTCCreatureGenerator.h (Ego_r.pdb) is 0x10 bytes high from GenParams and 0x1c from PDef
// on (retail CArray 0xc, STLport list 4, vectors 12), so the model is local: GenParams +0x28
// (SelfTriggerResetInterval at +0xc of it), ActivationFrame +0x58, PDef +0x8c, FramesUntilRandomEffect
// +0x98, FramesUntilRandomSoundEffect +0x9c. CCreatureGeneratorDef: SecondsBetweenEffects +0x38,
// SecondsBetweenSoundEffects +0x3c (Ego_r.pdb less 4: the retail vector before them is 12 bytes). CThing
// (retail, 0x10 below the PDB): the def class +0x70, the second flag bitfield byte +0x92 (PDB +0xa2:
// DeletedOnOwnerLevelOutOfScope, Drawable, Dying, ForceFrameUpdate, LevelPersistent = bit 4, ...).
// Shape: the def pointer's address stays in edi from the call's push to the null test; seconds * rate is
// `mov reg,[def+n] ; imul reg,[rate]`; the negated activation frame is `imul ; neg`; LevelPersistent is a
// one-bit bitfield set (`mov al ; or al,10h ; mov`), the parent thing re-read into esi.
struct CCreatureGeneratorDef {
    unsigned char _pad[0x38];
    long          SecondsBetweenEffects;                    // +0x38
    long          SecondsBetweenSoundEffects;               // +0x3c
};

struct CDefPointer_CreatureGeneratorDef {                   // CDefPointer<CCreatureGeneratorDef const>: one word
    const CCreatureGeneratorDef* Object;
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer_CreatureGeneratorDef& out);  // 0x0074D160
};

struct CThing {
    unsigned char  _pad0[0x70];
    CDefClassBase* DefClass;                                // +0x70
    unsigned char  _pad74[0x1e];
    unsigned char  DeletedOnOwnerLevelOutOfScope : 1;       // +0x92 bit 0
    unsigned char  Drawable                      : 1;
    unsigned char  Dying                         : 1;
    unsigned char  ForceFrameUpdate              : 1;
    unsigned char  LevelPersistent               : 1;       // bit 4
    unsigned char  GameStatePersisted            : 1;
    unsigned char  PersistOnUnloadWhenInLimbo    : 1;
    unsigned char  PausedDuringCutscenes         : 1;
};

extern long g_FramesPerSecond;                              // 0x01375550

struct CTCCreatureGenerator {
    void*                            __vftable;             // +0x00
    CThing*                          ParentThing;           // +0x04 (CTCBase)
    unsigned char                    _pad08[0x2c];          // +0x08
    long                             SelfTriggerResetInterval;   // +0x34 GenParams.SelfTriggerResetInterval
    unsigned char                    _pad38[0x20];          // +0x38
    long                             ActivationFrame;       // +0x58
    unsigned char                    _pad5c[0x30];          // +0x5c
    CDefPointer_CreatureGeneratorDef PDef;                  // +0x8c
    unsigned char                    PRandomParticle[8];    // +0x90 CIntelligentPointer<CThing>
    long                             FramesUntilRandomEffect;        // +0x98
    long                             FramesUntilRandomSoundEffect;   // +0x9c
    void OnCreate();
};

void CTCCreatureGenerator::OnCreate()
{
    ParentThing->DefClass->GetSubDef(PDef);
    if (PDef.Object) {
        FramesUntilRandomEffect = PDef.Object->SecondsBetweenEffects * g_FramesPerSecond;
        FramesUntilRandomSoundEffect = PDef.Object->SecondsBetweenSoundEffects * g_FramesPerSecond;
    }
    ActivationFrame = -(SelfTriggerResetInterval * g_FramesPerSecond);
    ParentThing->LevelPersistent = 1;
}