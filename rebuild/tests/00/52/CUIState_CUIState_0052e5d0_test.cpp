// Behaviour test for NUISystem::CUIState::CUIState (0x0052E5D0).
#include <cstdio>

struct C2DVector { float X, Y; C2DVector() { } };
struct CRGBColour { unsigned char B, G, R, A; CRGBColour() { } };
struct CUIState {
    unsigned long StateChangeFlag;
    long StateChangeType;
    C2DVector Position;
    C2DVector Zoom;
    CRGBColour Colour;
    float UpdateTime;
    bool LinearChange;
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

int main()
{
    CUIState s;
    if (s.StateChangeFlag != 7 || s.StateChangeType != 0 || s.Position.X != 0.0f || s.Position.Y != 0.0f
        || s.Zoom.X != 1.0f || s.Zoom.Y != 1.0f || s.Colour.R != 255 || s.Colour.G != 255 || s.Colour.B != 255
        || s.Colour.A != 255 || s.UpdateTime != -1.0f || s.LinearChange) {
        std::printf("0052e5d0_TEST FAIL\n");
        return 1;
    }
    std::printf("0052e5d0_TEST PASS\n");
    return 0;
}