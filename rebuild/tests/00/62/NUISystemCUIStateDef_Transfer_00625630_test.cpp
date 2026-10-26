#include <cstdio>
// Behaviour test: CUIStateDef::Transfer visits its members in retail order through the typed CPersistContext transfers.
class C2DVector { public: float X, Y; };
class CRGBFloatColour { public: float R, G, B, A; };
struct CULongVector { unsigned long* First; unsigned long* Last; unsigned long* End; };
namespace NUISystem { enum EStateChangeType { STATE_CHANGE_NONE, STATE_CHANGE_FADE }; }
class CPersistContext { public:
    const void* Seen[20];
    char Kind[20];
    int Count;
    CPersistContext() : Count(0) {}
    void Record(const void* p, char k) { Seen[Count] = p; Kind[Count] = k; ++Count; }
    void Transfer(unsigned long& value) { Record(&value, 'u'); }
    void Transfer(float& value) { Record(&value, 'f'); }
    void Transfer(NUISystem::EStateChangeType& value) { Record(&value, 'e'); }
    void Transfer(bool& value) { Record(&value, 'b'); }
    void Transfer(CULongVector& value) { Record(&value, 'v'); }
};
namespace NUISystem {
class CUIStateDef { public:
    unsigned long    GraphicIndex;
    unsigned long    StateChangeFlag;
    EStateChangeType StateChangeType;
    C2DVector        Position;
    C2DVector        Zoom;
    CRGBFloatColour  Colour;
    float            UpdateTime;
    CULongVector     ChildrenNotAffected;
    bool             LinearChange;
    void Transfer(CPersistContext& context)
    {
        context.Transfer(GraphicIndex);
        context.Transfer(Position.X);
        context.Transfer(Position.Y);
        context.Transfer(Zoom.X);
        context.Transfer(Zoom.Y);
        context.Transfer(Colour.R);
        context.Transfer(Colour.G);
        context.Transfer(Colour.B);
        context.Transfer(Colour.A);
        context.Transfer(UpdateTime);
        context.Transfer(StateChangeType);
        context.Transfer(LinearChange);
        context.Transfer(StateChangeFlag);
        context.Transfer(ChildrenNotAffected);
    }
};
}
int main()
{
    NUISystem::CUIStateDef d;
    CPersistContext c;
    d.Transfer(c);
    const void* want[14] = { &d.GraphicIndex, &d.Position.X, &d.Position.Y, &d.Zoom.X, &d.Zoom.Y, &d.Colour.R, &d.Colour.G,
                             &d.Colour.B, &d.Colour.A, &d.UpdateTime, &d.StateChangeType, &d.LinearChange, &d.StateChangeFlag,
                             &d.ChildrenNotAffected };
    const char* kinds = "ufffffffffebuv";
    if (c.Count != 14) { std::printf("00625630_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 14; ++i)
        if (c.Seen[i] != want[i] || c.Kind[i] != kinds[i]) { std::printf("00625630_TEST FAIL\n"); return 1; }
    std::printf("00625630_TEST PASS\n");
    return 0;
}