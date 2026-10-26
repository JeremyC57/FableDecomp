// Behaviour test for CTCDParticleEmitter::AttachToCamera(long, float) @ 0x006E0DB0: attachments cleared first, the
// camera bit set (other bits kept), flags and offset stored, then the camera attachment updated with true.
#include <cstdio>
static int g_order = 0, g_clearAt = 0, g_updateAt = 0;
static bool g_updateArg = false;
static bool g_cameraBitAtUpdate = false;
class CTCDParticleEmitter { public:
    virtual void v00() { }
    unsigned char _base[0x8]; unsigned char _body0[0x24 - 0x0c];
    long AttachmentFlags; void* AttachmentPosName; long AttachmentPosIndex; float FloatOffset;
    long DieOffTimer; long FramesLeftToWaitForParticleToCreate; void* PCreationParameters[2];
    bool WaitingForParticleToLoad : 1; bool ForceParticleToLoad : 1; bool IndependantObject : 1; bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1; bool ResetEmitterOrientation : 1; bool AttachedToCamera : 1; bool Moving : 1;
    void ClearAttachments() { g_clearAt = ++g_order; AttachedToCamera = false; }
    void UpdateEmitterCameraAttachment(bool force) { g_updateAt = ++g_order; g_updateArg = force; g_cameraBitAtUpdate = AttachedToCamera; }
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
int main()
{
    CTCDParticleEmitter c; c.WaitingForParticleToLoad = true; c.Moving = true; c.AttachedToCamera = false;
    c.AttachmentFlags = 0; c.FloatOffset = 0.0f;
    c.AttachToCamera(5, 2.5f);
    if (g_clearAt != 1 || g_updateAt != 2 || !g_updateArg || !g_cameraBitAtUpdate || !c.AttachedToCamera || !c.WaitingForParticleToLoad
        || !c.Moving || c.AttachmentFlags != 5 || c.FloatOffset != 2.5f) { std::printf("006e0db0_TEST FAIL\n"); return 1; }
    std::printf("006e0db0_TEST PASS\n");
    return 0;
}