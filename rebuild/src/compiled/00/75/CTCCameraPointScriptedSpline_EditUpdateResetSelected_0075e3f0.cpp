// CTCCameraPointScriptedSpline::EditUpdateResetSelected(CCamera* camera) -- copy the editor camera's position, view direction and FOV into the selected key camera, then rebuild the splines
//
// Retail 0x0075E3F0, 109 bytes (`ret 4`). Callees: CCamera::GetHorizontalFOV() const @ 0x00A0BEB0
// (catalog label "CMap::DrawGetSeaLevel" is propagated; the body is `fld [ecx+2Ch]`, CCamera's
// HorizontalFOV), CTCCameraPointScriptedSpline::UpdateSplines @ 0x0075DDF0.
// Layout: KeyCameras {first +0x68, last +0x6c, end +0x70} and SelectedKeyCamera +0x74 (STLport
// vector, as the landed InitialiseSpline 0x0075D050); CKeyCamera (PDB, 0x38): Pos +0x04,
// LookDirection +0x10, FOV +0x1c; CCamera (PDB): WorldPosition +0x00, RHSet {Up +0x0c, Forward +0x18}.
// Shape: the selected iterator is a named local (esi); C3DVector has a user copy ctor and the view
// direction comes from an inline by-value getter, so it is copied into a frame
// temporary first and then into the key camera; the position is a direct member copy.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

class CCamera { public:
    C3DVector       WorldPosition;                  // +0x00
    CRightHandedSet RHSet;                          // +0x0c
    bool            HeightLocked;                   // +0x24
    bool            ViewVecZLocked;                 // +0x25
    unsigned long   FOVFlags;                       // +0x28
    float           HorizontalFOV;                  // +0x2c
    float           VerticalFOV;                    // +0x30
    float           Zoom;                           // +0x34
    C3DVector GetViewVector() const { return RHSet.Forward; }
    float GetHorizontalFOV() const;                 // 0x00A0BEB0
};

struct CKeyCamera {                                 // CTCCameraPointScriptedSpline::CKeyCamera (0x38)
    float         DeltaIndex;                       // +0x00
    C3DVector     Pos;                              // +0x04
    C3DVector     LookDirection;                    // +0x10
    float         FOV;                              // +0x1c
    unsigned char _rest[0x38 - 0x20];               // +0x20 Roll .. SpeedFraction
};

class CTCCameraPointScriptedSpline { public:
    unsigned char _pad00[0x68];
    CKeyCamera*   KeyCameras_first;                 // +0x68 vector<CKeyCamera>
    CKeyCamera*   KeyCameras_last;                  // +0x6c
    CKeyCamera*   KeyCameras_end;                   // +0x70
    CKeyCamera*   SelectedKeyCamera;                // +0x74 vector iterator
    void UpdateSplines();                           // 0x0075DDF0
    void EditUpdateResetSelected(CCamera* camera);
};

void CTCCameraPointScriptedSpline::EditUpdateResetSelected(CCamera* camera)
{
    CKeyCamera* key = SelectedKeyCamera;
    if (key != KeyCameras_last)
    {
        key->Pos = camera->WorldPosition;
        key->LookDirection = camera->GetViewVector();
        key->FOV = camera->GetHorizontalFOV();
    }
    UpdateSplines();
}