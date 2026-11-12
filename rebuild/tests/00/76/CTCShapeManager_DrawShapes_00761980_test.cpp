// Behaviour test for 0x00761980 CTCShapeManager::DrawShapes (model copy; compiled separately from the row source).
// Checks: every persistent shape is drawn in blue; a shape with points gets one red line from its first point lifted by 0.15
// to the owner thing's position (width 0.05, depth test on); a shape without points gets no line; the selected shape is drawn
// again in sand and its points are highlighted in white; no selection means no second draw.
#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
static bool SameColour(const CRGBColour& c, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return c.R == r && c.G == g && c.B == b && c.A == a;
}
class CWorldMap { public: int Id; };
class CEnginePrimitiveLine { public:
    C3DVector Start, End;
    CRGBColour Colour;
    float Width;
    long BlendMode;
    unsigned long BankIndex;
    bool UseZBuffer;
    CEnginePrimitiveLine(unsigned __int64, const C3DVector& start, const C3DVector& end, const CRGBColour& colour, float width,
                         long blendMode, unsigned long bank, bool useZ, long, long)
        : Start(start), End(end), Colour(colour), Width(width), BlendMode(blendMode), BankIndex(bank), UseZBuffer(useZ) {}
};
class CIEngine { public:
    int LineCount;
    C3DVector Starts[8], Ends[8];
    CRGBColour Colours[8];
    float Widths[8];
    bool UseZ[8];
    CIEngine() : LineCount(0) {}
    void AddPrimitive(const CEnginePrimitiveLine& line)
    {
        if (LineCount < 8) {
            Starts[LineCount] = line.Start; Ends[LineCount] = line.End; Colours[LineCount] = line.Colour;
            Widths[LineCount] = line.Width; UseZ[LineCount] = line.UseZBuffer;
        }
        ++LineCount;
    }
};
class CEnginePrimitivePrepareInfo { public: CIEngine* Engine; };
struct CShape {
    const C3DVector* PointsBegin;
    const C3DVector* PointsEnd;
    int DrawCount, HighlightCount;
    CRGBColour DrawColours[4], HighlightColour;
    const CWorldMap* DrawMap;
    const C3DVector* DrawView;
    CShape() : PointsBegin(0), PointsEnd(0), DrawCount(0), HighlightCount(0), DrawMap(0), DrawView(0) {}
    void EditorDrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&, const CWorldMap& map, const C3DVector& view,
                                                 const CRGBColour& colour)
    {
        if (DrawCount < 4) DrawColours[DrawCount] = colour;
        ++DrawCount; DrawMap = &map; DrawView = &view;
    }
    bool GetStartPoint(C3DVector& pos) const
    {
        if (PointsBegin == PointsEnd) return false;
        pos = *PointsBegin;
        return true;
    }
    void EditorDrawHighlightPoints(const CEnginePrimitivePrepareInfo&, const CWorldMap&, const C3DVector&, const CRGBColour& colour)
    {
        HighlightColour = colour; ++HighlightCount;
    }
};
class CThing { public:
    C3DVector Pos;
    const C3DVector& GetPos() const { return Pos; }
};
class CTCShapeManager { public:
    CThing* ParentThing;
    CShape** ShapesBegin;
    CShape** ShapesEnd;
    CShape** SelectedShape;
    CWorldMap* Map;
    CThing& GetParentThing() { return *ParentThing; }
    void DrawShapes(const CEnginePrimitivePrepareInfo& info, const C3DVector& viewPos)
    {
        CRGBColour shapeColour(0x50, 0x50, 0xff, 0xff);
        CRGBColour selectedColour(0xb4, 0xa0, 0x50, 0xff);
        const CWorldMap& map = *Map;
        for (CShape** it = ShapesBegin; it != ShapesEnd; ++it) {
            CShape* shape = *it;
            shape->EditorDrawPreparePrimitivesForRendering(info, map, viewPos, shapeColour);
            C3DVector start;
            if (shape->GetStartPoint(start)) {
                CThing& parent = GetParentThing();
                C3DVector top(start.X, start.Y, start.Z + 0.15f);
                info.Engine->AddPrimitive(CEnginePrimitiveLine(0, top, parent.GetPos(), CRGBColour(0xff, 0, 0, 0xff), 0.05f, 0, 0, true, 0, 0));
            }
        }
        if (SelectedShape != ShapesEnd) {
            CShape* shape = *SelectedShape;
            shape->EditorDrawPreparePrimitivesForRendering(info, map, viewPos, selectedColour);
            shape->EditorDrawHighlightPoints(info, map, viewPos, CRGBColour(0xff, 0xff, 0xff, 0xff));
        }
    }
};
static int Fail() { std::printf("00761980_TEST FAIL\n"); return 1; }
int main()
{
    C3DVector pointsA[2]; pointsA[0] = C3DVector(1.0f, 2.0f, 3.0f); pointsA[1] = C3DVector(9.0f, 9.0f, 9.0f);
    C3DVector pointsC[1]; pointsC[0] = C3DVector(-4.0f, 5.5f, 0.25f);
    CShape a, b, c;
    a.PointsBegin = pointsA; a.PointsEnd = pointsA + 2;      // two points: line from the first
    // b has no points: drawn, no line
    c.PointsBegin = pointsC; c.PointsEnd = pointsC + 1;
    CShape* shapes[3] = { &a, &b, &c };
    CThing parent; parent.Pos = C3DVector(10.0f, 20.0f, 30.0f);
    CWorldMap map; map.Id = 7;
    CIEngine engine;
    CEnginePrimitivePrepareInfo info; info.Engine = &engine;
    C3DVector view(0.5f, 0.5f, 0.5f);
    CTCShapeManager manager;
    manager.ParentThing = &parent; manager.ShapesBegin = shapes; manager.ShapesEnd = shapes + 3; manager.Map = &map;

    // 1: shape c selected.
    manager.SelectedShape = shapes + 2;
    manager.DrawShapes(info, view);
    if (a.DrawCount != 1 || b.DrawCount != 1 || c.DrawCount != 2) return Fail();
    if (!SameColour(a.DrawColours[0], 0x50, 0x50, 0xff, 0xff) || !SameColour(b.DrawColours[0], 0x50, 0x50, 0xff, 0xff)) return Fail();
    if (!SameColour(c.DrawColours[0], 0x50, 0x50, 0xff, 0xff) || !SameColour(c.DrawColours[1], 0xb4, 0xa0, 0x50, 0xff)) return Fail();
    if (a.DrawMap != &map || a.DrawView != &view) return Fail();
    if (a.HighlightCount != 0 || b.HighlightCount != 0 || c.HighlightCount != 1) return Fail();
    if (!SameColour(c.HighlightColour, 0xff, 0xff, 0xff, 0xff)) return Fail();
    if (engine.LineCount != 2) return Fail();                 // a and c have a start point, b has none
    if (engine.Starts[0].X != 1.0f || engine.Starts[0].Y != 2.0f || engine.Starts[0].Z != 3.0f + 0.15f) return Fail();
    if (engine.Starts[1].X != -4.0f || engine.Starts[1].Y != 5.5f || engine.Starts[1].Z != 0.25f + 0.15f) return Fail();
    for (int i = 0; i < 2; ++i) {
        if (engine.Ends[i].X != 10.0f || engine.Ends[i].Y != 20.0f || engine.Ends[i].Z != 30.0f) return Fail();
        if (!SameColour(engine.Colours[i], 0xff, 0, 0, 0xff) || engine.Widths[i] != 0.05f || !engine.UseZ[i]) return Fail();
    }
    if (pointsA[0].Z != 3.0f || pointsC[0].Z != 0.25f) return Fail();   // the shape's own point is not lifted

    // 2: nothing selected (iterator at end): one blue draw each, no highlight; lines again for a and c.
    manager.SelectedShape = shapes + 3;
    manager.DrawShapes(info, view);
    if (a.DrawCount != 2 || b.DrawCount != 2 || c.DrawCount != 3) return Fail();
    if (!SameColour(c.DrawColours[2], 0x50, 0x50, 0xff, 0xff)) return Fail();
    if (a.HighlightCount != 0 || b.HighlightCount != 0 || c.HighlightCount != 1) return Fail();
    if (engine.LineCount != 4) return Fail();

    // 3: no shapes at all, selection == end: nothing happens.
    manager.ShapesEnd = shapes; manager.SelectedShape = shapes;
    manager.DrawShapes(info, view);
    if (engine.LineCount != 4 || a.DrawCount != 2) return Fail();

    // 4: the empty shape b selected: drawn blue then sand, highlighted, still no line for it.
    manager.ShapesBegin = shapes + 1; manager.ShapesEnd = shapes + 2; manager.SelectedShape = shapes + 1;
    manager.DrawShapes(info, view);
    if (b.DrawCount != 4 || !SameColour(b.DrawColours[3], 0xb4, 0xa0, 0x50, 0xff) || b.HighlightCount != 1) return Fail();
    if (engine.LineCount != 4) return Fail();

    std::printf("00761980_TEST PASS\n");
    return 0;
}