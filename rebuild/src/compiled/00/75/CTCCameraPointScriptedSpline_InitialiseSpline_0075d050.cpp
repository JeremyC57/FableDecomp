// CTCCameraPointScriptedSpline::InitialiseSpline(long startNode, long endNode) -- resets two spline timers, records the start/end key nodes, and for an end of -1 uses the last key camera
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075D050, length 79 bytes.
// Callers: 0x00854136. No callees.
// Layout: Ego_r.pdb CTCCameraPointScriptedSpline with the retail container sizes: KeyCameras (vector of 0x38-byte
// CKeyCamera, cf. the landed push_back 0x0075FAA0) at +0x68 {first, last}; the retail vector is 4 bytes and the
// STLport list MyMessages 8 bytes narrower than the PDB's, so the members after the list sit 0xc lower:
// PauseFor +0xa8, NodeToStartOn +0xb4, NodeToEndOn +0xb8, AnimationSpeed +0xbc (ValidAnimList +0xc0 matches
// 0x0075D8B0). The two zeroed floats are named by that mapping.
// Shape: float zeros assigned in the body share the integer zero register (`xor eax,eax`); the end node
// is stored before the -1 test and overwritten with `size() - 1`, size being the signed byte distance
// divided by 56 (`imul 92492493h ; sar edx,5`).
struct CKeyCamera { unsigned char _body[0x38]; };

class CTCCameraPointScriptedSpline { public:
    unsigned char _pad00[0x68];
    CKeyCamera*   KeyCameras_first;                 // +0x68 vector<CKeyCamera>
    CKeyCamera*   KeyCameras_last;                  // +0x6c
    CKeyCamera*   KeyCameras_end;                   // +0x70
    unsigned char _pad74[0xa8 - 0x74];
    float         PauseFor;                         // +0xa8 (PDB +0xb4)
    float         TimeToPlay;                       // +0xac
    float         EditPreviewIndex;                 // +0xb0
    long          NodeToStartOn;                    // +0xb4 (PDB +0xc0)
    long          NodeToEndOn;                      // +0xb8 (PDB +0xc4)
    float         AnimationSpeed;                   // +0xbc (PDB +0xc8)
    long NumKeyCameras() const { return KeyCameras_last - KeyCameras_first; }
    void InitialiseSpline(long startNode, long endNode);
};

void CTCCameraPointScriptedSpline::InitialiseSpline(long startNode, long endNode)
{
    AnimationSpeed = 0.0f;
    PauseFor = 0.0f;
    NodeToStartOn = startNode;
    NodeToEndOn = endNode;
    if (endNode == -1)
        NodeToEndOn = NumKeyCameras() - 1;
}