// CAIStateGroup_GazeAtHome::CAIStateGroup_GazeAtHome() -- CAIStateGroup_Gaze(), this vtable, then the base's
// ChanceOfRunning = 0.05f and MinJourneyForRunning = 10.0f.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009182B0, length 32 bytes.
// Callees: CAIStateGroup_Gaze::CAIStateGroup_Gaze @ 0x00918190. Vtable 0x0128FE7C = CAIStateGroup_GazeAtHome
// (rtti-vtables.tsv); the manifest label "CCarriedReadableDef" is propagated and rejected.
// PDB CAIStateGroup_Gaze (sizeof 0x30): +0x14 GazeMarker, +0x1C float ChanceOfRunning, +0x20 float
// MinJourneyForRunning, +0x24 MaxDistanceForCreaturesToLookAt, +0x28/+0x2C marker defs. The two floats are
// base members assigned in the derived body (immediate dword stores after the vtable store).
struct CAIStateGroup_Gaze {
    virtual void v00();
    unsigned char _pad_0x04[0x10];               // +0x04
    unsigned char GazeMarker[0x08];              // +0x14 CIntelligentPointer<CThingGameObject>
    float         ChanceOfRunning;               // +0x1C
    float         MinJourneyForRunning;          // +0x20
    float         MaxDistanceForCreaturesToLookAt; // +0x24
    long          PrimaryMarkerDef;              // +0x28
    long          SecondaryMarkerDef;            // +0x2C
    CAIStateGroup_Gaze();                        // 0x00918190
};

struct CAIStateGroup_GazeAtHome : CAIStateGroup_Gaze {
    virtual void v00();                          // own vtable (0x0128FE7C)
    CAIStateGroup_GazeAtHome();
};

CAIStateGroup_GazeAtHome::CAIStateGroup_GazeAtHome()
    : CAIStateGroup_Gaze()
{
    ChanceOfRunning = 0.05f;
    MinJourneyForRunning = 10.0f;
}
