// CTCVolumeContainmentTracker::FrameUpdate() -- while tracking is enabled and the tracked volume is active, re-runs the containment Update every 5 seconds' worth of frames (every 0.8 s when the camera is within 10 units of the thing), staggered by the thing's UID; immediately when the def says so or the tracker is not yet updated
// Grade: DIFFER(ours v retail) -- 237v318, lane gap (asm_bake family): retail rounds the frame intervals with an inline-asm `fld ; fadd 0.5 ; fistp` round-to-nearest helper (frame locals, then `fild ; fsub 1.0` and an integer compare of the float bits), which VC7.1 never emits from plain C; this C model calls __ftol2 instead
// Leaf: ?FrameUpdate@CTCVolumeContainmentTracker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00785B40, 318 bytes (plain `ret`, ebp frame).
// Callees: CTCBase::GetWorld() 0x00686A70; CThing::GetPosition() const 0x004C73D0; the world's slot 8 (+0x20) camera
//          position getter (C3DVector by hidden pointer); GFGetCurrentFrame 0x0049D870; __allrem 0x00BFEC30;
//          CTCVolumeContainmentTracker::Update() 0x00785620.
// Globals: the tracking-enabled byte 0x01375757, g_framesPerSecond 0x01375550; constants 5.0f 0x0126B664, 0.8f 0x0126B660,
//          100.0f 0x01230A08, 0.5f/1.0f as immediates.
// Layout (Ego_r.pdb): CTCVolumeContainmentTracker PreviouslyUpdated +0x18, PDef +0x1c (def +0x25 an always-update flag);
//          CThing UID (unsigned __int64) +0x18.
class CThing;
struct C3DVector { float X, Y, Z; };
extern unsigned char g_bVolumeTrackingEnabled;  // 0x01375757
extern long g_framesPerSecond;                  // 0x01375550
long __fastcall GFGetCurrentFrame();            // 0x0049D870

struct CWorldView {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual C3DVector GetCameraPosition();      // slot 8 (+0x20)
};
class CThing {
public:
    unsigned char    _pad_0x00[0x18];
    unsigned __int64 UID;                       // +0x18
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CVolumeContainmentTrackerDef {
    unsigned char _pad_0x00[0x25];
    bool          AlwaysUpdate;                 // +0x25
};
inline long RoundToFrames(float value)          // retail: an inline-asm fistp round (see the Grade line)
{
    long frames = (long)(value + 0.5f);
    if (value == (float)frames - 1.0f)
        --frames;
    return frames;
}

struct CTCVolumeContainmentTracker {
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x10];
    bool          PreviouslyUpdated;            // +0x18
    CVolumeContainmentTrackerDef* PDef;         // +0x1c
    CWorldView* GetWorld();                     // 0x00686A70 (CTCBase)
    void Update();                              // 0x00785620
    virtual void FrameUpdate();
};

void CTCVolumeContainmentTracker::FrameUpdate()
{
    if (!g_bVolumeTrackingEnabled)
        return;
    if (!PDef->AlwaysUpdate && PreviouslyUpdated) {
        long interval = RoundToFrames(g_framesPerSecond * 5.0f);
        CWorldView* world = GetWorld();
        const C3DVector& pos = ParentThing->GetPosition();
        C3DVector camera = world->GetCameraPosition();
        float dx = camera.X - pos.X;
        float dy = camera.Y - pos.Y;
        float dz = camera.Z - pos.Z;
        if (dz * dz + dy * dy + dx * dx < 100.0f)
            interval = RoundToFrames(g_framesPerSecond * 0.8f);
        if ((ParentThing->UID + GFGetCurrentFrame()) % interval != 0)
            return;
    }
    Update();
}
