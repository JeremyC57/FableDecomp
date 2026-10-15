// CUIState::CUIState() -- NUISystem::CUIState default constructor: zoom (1,1), position (0,0), white opaque colour, update time -1, type 0, change flag 7, no linear change
// Retail: Fable.exe @ 0x0052E5D0, 55 bytes, no callees.
// Layout from Ego_r.pdb (NUISystem::CUIState, 0x24 bytes): StateChangeFlag +0, StateChangeType +4, Position (C2DVector) +8,
// Zoom (C2DVector) +0x10, Colour (CRGBColour, bytes B,G,R,A) +0x18, UpdateTime +0x1c, LinearChange +0x20.
// Shape: every member is written in the constructor BODY (the vectors and the colour have empty default
// constructors), in the order zoom, position, colour R/G/B/A, update time, type, flag, linear -- that is the
// retail store order (+0x10/+0x14, +8/+0xc, +0x1a/+0x19/+0x18/+0x1b, +0x1c, +4, +0, +0x20). An init list (or
// (x, y) constructors) puts Position before Zoom and the flag/type first.
namespace NUISystem {

struct C2DVector {
    float X, Y;
    C2DVector() { }
};

struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() { }
};

struct CUIState {
    unsigned long StateChangeFlag;  // +0x00
    long StateChangeType;           // +0x04 (EStateChangeType)
    C2DVector Position;             // +0x08
    C2DVector Zoom;                 // +0x10
    CRGBColour Colour;              // +0x18
    float UpdateTime;               // +0x1c
    bool LinearChange;              // +0x20
    CUIState();
};

CUIState::CUIState()
{
    Zoom.X = 1.0f;
    Zoom.Y = 1.0f;
    Position.X = 0.0f;
    Position.Y = 0.0f;
    Colour.R = 255;
    Colour.G = 255;
    Colour.B = 255;
    Colour.A = 255;
    UpdateTime = -1.0f;
    StateChangeType = 0;
    StateChangeFlag = 7;
    LinearChange = false;
}

}
