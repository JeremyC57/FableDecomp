// Behaviour test for NUISystem::CComponent::Initialise (0x005339B0): state-0 record seeding, PPositionParentMode-gated
// resets, opaque-white tint, timer clears and the attach-and-initialise pass over parentless children.
#include <cstdio>

struct C2DVector {
    float X, Y;
    C2DVector() {}
    void Clear() { X = 0.0f; Y = 0.0f; }
};
struct CRGBColour { unsigned char B, G, R, A; };
struct SComponentState {
    C2DVector  Position;
    C2DVector  Zoom;
    CRGBColour Colour;
};
struct CStates {
    SComponentState Records[2];
    int Lookups;
    SComponentState& operator[](const unsigned long& key) { ++Lookups; return Records[key]; }
};
struct CComponent;
struct CCountedComponentPointer {
    CComponent* Object;
    void* Info;
    CComponent* operator->() const { return Object; }
};
struct CComponentPointerVector {
    CCountedComponentPointer* _M_start;
    CCountedComponentPointer* _M_finish;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CCountedComponentPointer& operator[](unsigned int i) { return _M_start[i]; }
};
struct CComponent {
    CStates  States;
    float      Time;
    C2DVector  Position, TargetPosition, InitialPosition, ParentPosition;
    C2DVector  Zoom, TargetZoom, InitialZoom;
    float      ScaleY, ScaleX;
    CRGBColour Colour, TargetColour, InitialColour, ParentColour;
    float      PositionTimeElapsed, ZoomTimeElapsed, ColourTimeElapsed;
    CComponentPointerVector Children;
    C2DVector  RelativeParentPosition;
    float      RelativeParentZoomY, RelativeParentZoomX;
    long       PPositionParentMode;
    CComponent* Parent;
    int        InitCalls;
    virtual ~CComponent() {}
    virtual CComponent* GetParent() { return Parent; }
    virtual void SetParent(CComponent* p) { Parent = p; }
    virtual void Initialise();
};

void CComponent::Initialise()
{
    ++InitCalls;
    {
        const C2DVector& position = States[0].Position;
        Position = position;
        TargetPosition = position;
    }
    {
        const C2DVector& zoom = States[0].Zoom;
        Zoom = zoom;
        TargetZoom = zoom;
    }
    TargetColour = Colour = States[0].Colour;
    if (PPositionParentMode == 0) {
        ParentPosition.Clear();
        RelativeParentPosition.Clear();
    }
    ParentColour.R = ParentColour.G = ParentColour.B = ParentColour.A = 0xff;
    ScaleX = ScaleY = 1.0f;
    if (PPositionParentMode == 0) {
        RelativeParentZoomX = RelativeParentZoomY = 1.0f;
    }
    InitialPosition = Position;
    InitialZoom = Zoom;
    InitialColour = Colour;
    ZoomTimeElapsed = 0.0f;
    PositionTimeElapsed = 0.0f;
    ColourTimeElapsed = 0.0f;
    Time = 0;
    for (unsigned int i = 0; i < Children.size(); ++i) {
        if (Children[i]->GetParent() == 0) {
            Children[i]->SetParent(this);
            Children[i]->Initialise();
        }
    }
}

static void Prepare(CComponent& c, long mode)
{
    c.States.Lookups = 0;
    c.States.Records[0].Position.X = 3.0f; c.States.Records[0].Position.Y = 4.0f;
    c.States.Records[0].Zoom.X = 10.0f; c.States.Records[0].Zoom.Y = 20.0f;
    c.States.Records[0].Colour.R = 1; c.States.Records[0].Colour.G = 2;
    c.States.Records[0].Colour.B = 3; c.States.Records[0].Colour.A = 4;
    c.Time = 7; c.ParentPosition.X = 5.0f; c.ParentPosition.Y = 6.0f; c.RelativeParentPosition.X = 1.0f; c.RelativeParentPosition.Y = 2.0f;
    c.RelativeParentZoomX = c.RelativeParentZoomY = 0.5f; c.ScaleX = c.ScaleY = 0.25f;
    c.PositionTimeElapsed = c.ZoomTimeElapsed = c.ColourTimeElapsed = 9.0f;
    c.PPositionParentMode = mode; c.Parent = 0; c.InitCalls = 0;
    c.Children._M_start = c.Children._M_finish = 0;
}

int main()
{
    CComponent root, a, b;
    Prepare(root, 0); Prepare(a, 1); Prepare(b, 0);
    b.Parent = &a;                                   // already attached: skipped
    CCountedComponentPointer kids[2] = { { &a, 0 }, { &b, 0 } };
    root.Children._M_start = kids; root.Children._M_finish = kids + 2;
    root.Initialise();
    bool ok = root.States.Lookups == 3
        && root.Position.X == 3.0f && root.TargetPosition.Y == 4.0f && root.InitialPosition.X == 3.0f
        && root.Zoom.Y == 20.0f && root.TargetZoom.X == 10.0f && root.InitialZoom.Y == 20.0f
        && root.Colour.B == 3 && root.TargetColour.A == 4 && root.InitialColour.R == 1
        && root.ParentPosition.X == 0.0f && root.RelativeParentPosition.Y == 0.0f && root.RelativeParentZoomX == 1.0f
        && root.ScaleX == 1.0f && root.ScaleY == 1.0f
        && root.ParentColour.R == 0xff && root.ParentColour.A == 0xff
        && root.PositionTimeElapsed == 0.0f && root.ColourTimeElapsed == 0.0f && root.Time == 0.0f
        && a.Parent == &root && a.InitCalls == 1 && a.ParentPosition.X == 5.0f && a.RelativeParentZoomY == 0.5f
        && b.Parent == &a && b.InitCalls == 0;
    if (!ok) { std::printf("005339b0_TEST FAIL\n"); return 1; }
    std::printf("005339b0_TEST PASS\n");
    return 0;
}