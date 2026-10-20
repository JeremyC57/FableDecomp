// CTCCameraPointScriptedSpline::OnInitialActivate() -- move every key camera from thing-relative to world position, rebuild the splines and reset playback to run over all keys
// Grade: DIFFER(ours v retail) -- two known lane gaps: the X term of Pos + position loads its operands the other way (camera-module sum gap, 0x852220), and the inlined InitialiseSpline(0, -1) dead NodeToEndOn = -1 store retail keeps is dropped (0x47D21C gap)
// Leaf: ?OnInitialActivate@CTCCameraPointScriptedSpline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075E270, length 172 bytes.
// Callees: CTCBase::OnInitialActivate @ 0x00686870 (qualified base call), CThing::GetPosition @ 0x004C73D0, the private
// CTCCameraPointScriptedSpline::UpdateSplines @ 0x0075DDF0; InitialiseSpline(0, -1) (out of line at 0x0075D050) is inlined.
// Layout: Ego_r.pdb CTCCameraPointScriptedSpline with the retail container sizes (landed 0x0075D050): KeyCameras +0x68
// (0x38-byte CKeyCamera, Pos at +4), PauseFor +0xA8, NodeToStartOn +0xB4, NodeToEndOn +0xB8, AnimationSpeed +0xBC.
// Shape: an iterator walk with end() re-read per pass; each key position is `Pos = Pos + thing position` through a C3DVector
// temporary (the sum built by `return C3DVector(...)`, copied back with integer moves).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& b) const { return C3DVector(X + b.X, Y + b.Y, Z + b.Z); }
};
struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CKeyCamera {                             // CTCCameraPointScriptedSpline::CKeyCamera, 0x38 bytes
    float     DeltaIndex;                       // +0x00
    C3DVector Pos;                              // +0x04
    unsigned char _rest[0x38 - 0x10];
};
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    void OnInitialActivate();                   // 0x00686870
};
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
        NodeToEndOn = endNode;
        if (endNode == -1)
            NodeToEndOn = NumKeyCameras() - 1;
    }
    void UpdateSplines();                       // 0x0075DDF0
    void OnInitialActivate();
};

void CTCCameraPointScriptedSpline::OnInitialActivate()
{
    CTCBase::OnInitialActivate();
    for (CKeyCamera* key = KeyCameras_first; key != KeyCameras_last; ++key)
        key->Pos = key->Pos + ParentThing->GetPosition();
    UpdateSplines();
    InitialiseSpline(0, -1);
}
