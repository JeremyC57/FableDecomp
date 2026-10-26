// CTCDParticleEmitter::AttachToCamera(long attachmentFlags, float offset) -- drop any thing attachment, mark the emitter camera-attached and re-place it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0DB0, length 44 bytes.
// Callees: CTCDParticleEmitter::ClearAttachments() @ 0x006E0AF0; CTCDParticleEmitter::UpdateEmitterCameraAttachment(bool)
// @ 0x006E0230 (manifest names, both CTCDParticleEmitter-unique).
// PDB CTCDParticleEmitter: AttachmentFlags +0x24, FloatOffset +0x30, bitfield byte +0x44 (bit 6 AttachedToCamera).
// Signature from ghidra_out/egor_signatures.tsv (`?AttachToCamera@CTCDParticleEmitter@@QAEXJM@Z`).
// Shape: `ret 8`; the bool:1 set is `or cl,40h`; the float argument is copied with integer moves; the three
// stores are interleaved by the scheduler around the argument loads.
class CTCDParticleEmitter { public:
    virtual void v00();
    unsigned char _base[0x8];                               // CTCBase: ParentThing +4, flags +8/+9
    unsigned char _body0[0x24 - 0x0c];                      // EmitterName, ParticleTypeID, PEmitter, PAttachedToThing
    long          AttachmentFlags;                          // +0x24 (PDB ulong; the setter takes a long)
    void*         AttachmentPosName;                        // +0x28 CCharString
    long          AttachmentPosIndex;                       // +0x2c
    float         FloatOffset;                              // +0x30
    long          DieOffTimer;                              // +0x34
    long          FramesLeftToWaitForParticleToCreate;      // +0x38
    void*         PCreationParameters[2];                   // +0x3c CCountedPointer<CParticleCreationParameters>
    bool WaitingForParticleToLoad : 1;                      // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;                              // bit 6
    bool Moving : 1;
    void ClearAttachments();                                // 0x006E0AF0
    void UpdateEmitterCameraAttachment(bool force);         // 0x006E0230
    void AttachToCamera(long attachmentFlags, float offset);
};

void CTCDParticleEmitter::AttachToCamera(long attachmentFlags, float offset)
{
    ClearAttachments();
    AttachedToCamera = true;
    AttachmentFlags = attachmentFlags;
    FloatOffset = offset;
    UpdateEmitterCameraAttachment(true);
}