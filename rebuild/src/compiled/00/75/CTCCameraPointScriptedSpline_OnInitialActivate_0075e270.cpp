// CTCCameraPointScriptedSpline::OnInitialActivate() -- move every key camera from thing-relative to world position, rebuild the splines and reset playback to run over all keys
// Grade: RELOCATION_MATCH (172 bytes) -- the inlined InitialiseSpline writes the end node through a named reference (`long& end = NodeToEndOn; end = endNode; if (end == -1) end = NumKeyCameras() - 1;`), which keeps retail's dead `NodeToEndOn = -1` store and the first/last loads after it; the key position sum reads the thing position through a raw float pointer (`const float* pp = &ParentThing->GetPos().X; ... key->Pos.X + pp[0]`, GOTCHAS raw-pointer lever) so the X term loads the key first; the plain member sum and the member store were DIFFER(160v172), 6 masked lines (sec2c)
// Leaf: ?OnInitialActivate@CTCCameraPointScriptedSpline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075E270, length 172 bytes.
// Callees: CTCBase::OnInitialActivate @ 0x00686870 (qualified base call), CThing::GetPos @ 0x004C73D0 (`?GetPos@CThing@@QBEABVC3DVector@@XZ`), the private
// CTCCameraPointScriptedSpline::UpdateSplines @ 0x0075DDF0; InitialiseSpline(0, -1) (out of line at 0x0075D050) is inlined.
// Layout: Ego_r.pdb CTCCameraPointScriptedSpline with the retail container sizes (landed 0x0075D050): KeyCameras +0x68
// (0x38-byte CKeyCamera, Pos at +4), PauseFor +0xA8, NodeToStartOn +0xB4, NodeToEndOn +0xB8, AnimationSpeed +0xBC.
// Shape: an iterator walk with end() re-read per pass; each key position is `Pos = C3DVector(Pos.X + p[0], ...)` from a raw float
// view of the thing position (a C3DVector temporary copied back with integer moves; `Pos + pos` through operator+ flips the X pair);
// InitialiseSpline(0, -1) is inlined with the end node through a named `long&` reference (keeps the dead -1 store retail has).
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/CThing.h"
struct CKeyCamera {                             // CTCCameraPointScriptedSpline::CKeyCamera, 0x38 bytes
    float     DeltaIndex;                       // +0x00
    C3DVector Pos;                              // +0x04
    unsigned char _rest[0x38 - 0x10];
};
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
class CTCCameraPointScriptedSpline : public CTCBase {
public:
    unsigned char _pad0c[0x68 - 0x0c];
    CKeyCamera*   KeyCameras_first;             // +0x68 vector<CKeyCamera>
    CKeyCamera*   KeyCameras_last;              // +0x6c
    CKeyCamera*   KeyCameras_end;               // +0x70
    unsigned char _pad74[0xa8 - 0x74];
    float         PauseFor;                     // +0xa8 (PDB +0xb4)
    float         TimeToPlay;                   // +0xac
    float         EditPreviewIndex;             // +0xb0
    long          NodeToStartOn;                // +0xb4 (PDB +0xc0)
    long          NodeToEndOn;                  // +0xb8 (PDB +0xc4)
    float         AnimationSpeed;               // +0xbc (PDB +0xc8)
    long NumKeyCameras() const { return KeyCameras_last - KeyCameras_first; }
    void InitialiseSpline(long startNode, long endNode)    // out of line at 0x0075D050
    {
        AnimationSpeed = 0.0f;
        PauseFor = 0.0f;
        NodeToStartOn = startNode;
        long& end = NodeToEndOn;
        end = endNode;
        if (end == -1)
            end = NumKeyCameras() - 1;
    }
    void UpdateSplines();                       // 0x0075DDF0
    virtual void OnInitialActivate();
};

void CTCCameraPointScriptedSpline::OnInitialActivate()
{
    CTCBase::OnInitialActivate();
    for (CKeyCamera* key = KeyCameras_first; key != KeyCameras_last; ++key)
    {
        const float* pp = &ParentThing->GetPos().X;
        key->Pos = C3DVector(key->Pos.X + pp[0], key->Pos.Y + pp[1], key->Pos.Z + pp[2]);
    }
    UpdateSplines();
    InitialiseSpline(0, -1);
}