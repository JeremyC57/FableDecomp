// NUISystem::CComponent::ChangeZoom(const C2DVector& zoom, float time, bool unused) -- set the target zoom; with a positive time start an interpolation from the current zoom, else snap to it
// Leaf: ?ChangeZoom@CComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052EBB0, 98 bytes (112 with padding). No callees.
// Vtable 0x0122F5D4 (NUISystem::CSprite, inherited from CComponent) slot 35 -- the slot the landed
// ChangeZoomDelta 0x0052EC20 calls as ChangeZoom. Same shape as the landed ChangePosition 0x0052E9C0:
// retail view of the Ego_r CComponent layout (the States map is 8 bytes smaller in retail), so Zoom
// sits at +0x5c (PDB +0x64), TargetZoom +0x64, InitialZoom +0x6c, ZoomTimeElapsed +0xa0, ZoomTime +0xa4.
// ZoomTime is stored first as a raw dword; the TargetZoom copy is written in EACH arm so VC hoists the
// common X store above the branch; `fcomp [0.0f] ; test ah,41h ; jne` is `time > 0.0f`. The third
// argument (`ret 0Ch`) is never read.
struct C2DVector {
    float X;
    float Y;
};

struct CComponent {
    unsigned char _pad[0x5c];
    C2DVector Zoom;                         // +0x5c
    C2DVector TargetZoom;                   // +0x64
    C2DVector InitialZoom;                  // +0x6c
    unsigned char _pad74[0xa0 - 0x74];
    float ZoomTimeElapsed;                  // +0xa0
    float ZoomTime;                         // +0xa4
    void ChangeZoom(const C2DVector& zoom, float time, bool unused);
};

void CComponent::ChangeZoom(const C2DVector& zoom, float time, bool unused)
{
    ZoomTime = time;
    if (time > 0.0f) {
        TargetZoom = zoom;
        InitialZoom = Zoom;
        ZoomTimeElapsed = 0.0f;
    } else {
        TargetZoom = zoom;
        InitialZoom = zoom;
        Zoom = zoom;
    }
}