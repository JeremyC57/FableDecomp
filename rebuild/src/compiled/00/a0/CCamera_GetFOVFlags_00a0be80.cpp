// CCamera::GetFOVFlags() const -- returns the camera's FOVFlags
// Leaf: ?GetFOVFlags@CCamera@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0BE80, mov eax,[ecx+28h] ; ret.
// Identity (sweep3; catalog was CEngineCamera::GetFOVFlags (source: free __fastcall DrawGetEnvironment)): Ego_r.pdb and FableWin.pdb define the CCamera getters in the order
//   GetFOVFlags, GetHorizontalFOV, GetVerticalFOV, GetFOV, GetZoom (Ego_r 0xB4C970..0xB4C9B0, FableWin 0x30F5B40..0x30F6070);
//   retail 0x00A0BE80 .. 0x00A0BEC0 is that run, 0x10 apart, reading the Ego_r CCamera fields FOVFlags +0x28,
//   HorizontalFOV +0x2C, VerticalFOV +0x30, (GetFOV: HorizontalFOV +0x2C again), Zoom +0x34. No ICF in retail, so the two
//   byte-identical +0x2C getters are told apart by that order.
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up, Forward; };

class CCamera {                                     // Ego_r.pdb CCamera, 0x38 bytes
public:
    C3DVector       WorldPosition;                  // +0x00
    CRightHandedSet RHSet;                          // +0x0C
    bool            HeightLocked;                   // +0x24
    bool            ViewVecZLocked;                 // +0x25
    unsigned long   FOVFlags;                       // +0x28
    float           HorizontalFOV;                  // +0x2C
    float           VerticalFOV;                    // +0x30
    float           Zoom;                           // +0x34
    unsigned long GetFOVFlags() const;
};

unsigned long CCamera::GetFOVFlags() const
{
    return FOVFlags;
}
