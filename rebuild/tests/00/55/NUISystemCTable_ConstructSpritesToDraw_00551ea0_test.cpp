// Behaviour test for 0x00551EA0 NUISystem::CTable::ConstructSpritesToDraw: the same algorithm over a plain sprite table
// (index = ETableSprites, `present` = found in the map), recording the corner children and the line calls; checked against
// positions worked out by hand for a stretching table and for a fixed one whose bottom-right corner sits at the origin.
#include <cstdio>
#include <cmath>

enum ETableSprites { TABLE_SPRITES_TOP_LEFT = 0, TABLE_SPRITES_TOP_RIGHT = 1, TABLE_SPRITES_BOTTOM_LEFT = 2, TABLE_SPRITES_BOTTOM_RIGHT = 3,
    TABLE_SPRITES_TOP_MIDDLE = 4, TABLE_SPRITES_BOTTOM_MIDDLE = 5, TABLE_SPRITES_MIDDLE_LEFT = 6, TABLE_SPRITES_MIDDLE_RIGHT = 7,
    TABLE_SPRITES_SEPARATION_BOTTOM = 8, TABLE_SPRITES_SEPARATION_TOP = 9, TABLE_SPRITES_SEPARATION_LEFT = 10,
    TABLE_SPRITES_SEPARATION_RIGHT = 11, TABLE_SPRITES_CROSS = 12, TABLE_SPRITES_AMOUNT = 13 };
enum { TABLE_EXPANSION_HORIZONTAL = 1, TABLE_EXPANSION_VERTICAL = 2 };
static const float DELTA_NOTIONAL_ZERO = 1e-4f;

struct C2DVector {
    float X, Y;
    C2DVector() : X(0.0f), Y(0.0f) { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector& operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
    bool IsNonZero() const { return X * X + Y * Y > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO; }
};
struct SpriteDef { bool present; long defIndex; C2DVector size; };
struct Child { long defIndex; C2DVector position; C2DVector statePosition; bool initialised; };
struct LineCall { char kind; int sprite; int separation; C2DVector position; unsigned long count; unsigned long offset; };

struct CTableModel {
    unsigned long ExpansionType;
    C2DVector Size;
    SpriteDef Sprites[TABLE_SPRITES_AMOUNT];
    unsigned long HorizontalSeparations[4]; unsigned long nbHorizontalSeparations;
    unsigned long VerticalSeparations[4]; unsigned long nbVerticalSeparations;
    Child Children[8]; int nbChildren;
    LineCall Lines[16]; int nbLines;

    int Find(int sprite) const { return Sprites[sprite].present ? sprite : -1; }
    void AddCorner(int sprite, const C2DVector& position)
    {
        Child& c = Children[nbChildren++];
        c.defIndex = Sprites[sprite].defIndex;     // CManager::CreateComponent(GetDefIndex())
        c.initialised = true;                      // Initialise()
        c.position = position;                     // SetPosition()
        c.statePosition = position;                // GetStates()[0].Position
    }
    void Line(char kind, int sprite, int separation, const C2DVector& position, unsigned long count, unsigned long offset)
    {
        LineCall& l = Lines[nbLines++];
        l.kind = kind; l.sprite = sprite; l.separation = separation; l.position = position; l.count = count; l.offset = offset;
    }
    void ConstructSpritesToDraw();
};

void CTableModel::ConstructSpritesToDraw()
{
    C2DVector topLeftPosition(0.0f, 0.0f);
    C2DVector topRightPosition(0.0f, 0.0f);
    C2DVector bottomLeftPosition(0.0f, 0.0f);
    C2DVector bottomRightPosition(0.0f, 0.0f);
    unsigned long nbHorizontal = 0;
    unsigned long nbVertical = 0;
    unsigned long horizontalSize = 0;
    unsigned long verticalSize = 0;
    if (ExpansionType & TABLE_EXPANSION_HORIZONTAL) {
        int it = Find(TABLE_SPRITES_TOP_MIDDLE);
        if (it != -1) {
            horizontalSize = (unsigned long)Sprites[it].size.X;
            nbHorizontal = (unsigned long)(Size.X / horizontalSize);
            for (unsigned long i = 0; i < nbVerticalSeparations; ++i)
                VerticalSeparations[i] /= horizontalSize;
        }
    }
    if (ExpansionType & TABLE_EXPANSION_VERTICAL) {
        int it = Find(TABLE_SPRITES_MIDDLE_LEFT);
        if (it != -1) {
            verticalSize = (unsigned long)Sprites[it].size.Y;
            nbVertical = (unsigned long)(Size.Y / verticalSize);
            for (unsigned long i = 0; i < nbHorizontalSeparations; ++i)
                HorizontalSeparations[i] /= verticalSize;
        }
    }
    topRightPosition.X += horizontalSize * nbHorizontal;
    bottomLeftPosition.Y += verticalSize * nbVertical;
    bottomRightPosition.X = topRightPosition.X;
    bottomRightPosition.Y = bottomLeftPosition.Y;

    int it = Find(TABLE_SPRITES_TOP_LEFT);
    if (it != -1) {
        AddCorner(it, topLeftPosition);
        topLeftPosition.X += Sprites[it].size.X;
        bottomLeftPosition.Y += Sprites[it].size.Y;
        topRightPosition.X += Sprites[it].size.X;
    }
    it = Find(TABLE_SPRITES_TOP_RIGHT);
    if (it != -1 && topRightPosition.IsNonZero()) {
        AddCorner(it, topRightPosition);
        topRightPosition += Sprites[it].size;
        topRightPosition.X -= horizontalSize;
        bottomRightPosition.Y += Sprites[it].size.Y;
    }
    it = Find(TABLE_SPRITES_BOTTOM_LEFT);
    if (it != -1 && bottomLeftPosition.IsNonZero()) {
        AddCorner(it, bottomLeftPosition);
        bottomLeftPosition.Y -= verticalSize * nbVertical;
        bottomRightPosition.X += Sprites[it].size.X;
    }
    it = Find(TABLE_SPRITES_BOTTOM_RIGHT);
    if (it != -1 && bottomRightPosition.IsNonZero()) {
        AddCorner(it, bottomRightPosition);
        bottomRightPosition.Y += Sprites[it].size.Y;
        bottomRightPosition.X -= horizontalSize * nbHorizontal;
        bottomRightPosition.Y -= verticalSize;
    }

    it = Find(TABLE_SPRITES_TOP_MIDDLE);
    if (it != -1) {
        Line('H', it, Find(TABLE_SPRITES_SEPARATION_TOP), topLeftPosition, nbHorizontal, 0);
        for (unsigned long i = 0; i < nbHorizontalSeparations; ++i) {
            C2DVector position = bottomLeftPosition;
            position.Y += HorizontalSeparations[i] * verticalSize;
            position.X = bottomLeftPosition.X + horizontalSize;
            unsigned long count = (unsigned long)((topRightPosition.X - position.X) / horizontalSize);
            unsigned long offset = 0;
            int corner = Find(TABLE_SPRITES_TOP_LEFT);
            if (corner != -1)
                offset = (unsigned long)(Sprites[corner].size.X / horizontalSize);
            Line('H', it, Find(TABLE_SPRITES_CROSS), position, count, offset - 1);
        }
    }
    it = Find(TABLE_SPRITES_BOTTOM_MIDDLE);
    if (it != -1)
        Line('H', it, Find(TABLE_SPRITES_SEPARATION_BOTTOM), bottomRightPosition, nbHorizontal, 0);
    it = Find(TABLE_SPRITES_MIDDLE_LEFT);
    if (it != -1) {
        Line('V', it, Find(TABLE_SPRITES_SEPARATION_LEFT), bottomLeftPosition, nbVertical, 0);
        for (unsigned long i = 0; i < nbVerticalSeparations; ++i) {
            C2DVector position = topLeftPosition;
            position.X += VerticalSeparations[i] * horizontalSize;
            position.Y += verticalSize;
            unsigned long count = (unsigned long)((bottomRightPosition.Y - position.Y) / verticalSize);
            unsigned long offset = 0;
            int corner = Find(TABLE_SPRITES_TOP_LEFT);
            if (corner != -1)
                offset = (unsigned long)(Sprites[corner].size.Y / verticalSize);
            Line('V', it, Find(TABLE_SPRITES_CROSS), position, count, offset - 1);
        }
    }
    it = Find(TABLE_SPRITES_MIDDLE_RIGHT);
    if (it != -1)
        Line('V', it, Find(TABLE_SPRITES_SEPARATION_RIGHT), topRightPosition, nbVertical, 0);
}

static int failures = 0;
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
static void CheckChild(const CTableModel& t, int n, long def, float x, float y)
{
    const Child& c = t.Children[n];
    if (n >= t.nbChildren || c.defIndex != def || !c.initialised || !Near(c.position.X, x) || !Near(c.position.Y, y)
        || !Near(c.statePosition.X, x) || !Near(c.statePosition.Y, y)) {
        std::printf("child %d: def %ld at (%g,%g)\n", n, c.defIndex, c.position.X, c.position.Y); ++failures;
    }
}
static void CheckLine(const CTableModel& t, int n, char kind, int sprite, int separation, float x, float y, unsigned long count,
                      unsigned long offset)
{
    const LineCall& l = t.Lines[n];
    if (n >= t.nbLines || l.kind != kind || l.sprite != sprite || l.separation != separation || !Near(l.position.X, x)
        || !Near(l.position.Y, y) || l.count != count || l.offset != offset) {
        std::printf("line %d: %c %d %d (%g,%g) %lu %lu\n", n, l.kind, l.sprite, l.separation, l.position.X, l.position.Y,
                    l.count, l.offset);
        ++failures;
    }
}
static void Setup(CTableModel& t)
{
    for (int i = 0; i < TABLE_SPRITES_AMOUNT; ++i) {
        t.Sprites[i].present = false; t.Sprites[i].defIndex = 100 + i; t.Sprites[i].size = C2DVector(16.0f, 16.0f);
    }
    t.nbHorizontalSeparations = 0; t.nbVerticalSeparations = 0; t.nbChildren = 0; t.nbLines = 0;
}

int main()
{
    // 1. A table stretching both ways, 100 x 60, with 8-pixel edge tiles and 16-pixel corners; separations at y 24 and x 40, 80.
    CTableModel t;
    Setup(t);
    t.ExpansionType = TABLE_EXPANSION_HORIZONTAL | TABLE_EXPANSION_VERTICAL;
    t.Size = C2DVector(100.0f, 60.0f);
    for (int i = 0; i < TABLE_SPRITES_AMOUNT; ++i) t.Sprites[i].present = true;
    t.Sprites[TABLE_SPRITES_SEPARATION_BOTTOM].present = false;
    t.Sprites[TABLE_SPRITES_SEPARATION_RIGHT].present = false;
    t.Sprites[TABLE_SPRITES_TOP_MIDDLE].size = C2DVector(8.0f, 16.0f);
    t.Sprites[TABLE_SPRITES_BOTTOM_MIDDLE].size = C2DVector(8.0f, 16.0f);
    t.Sprites[TABLE_SPRITES_MIDDLE_LEFT].size = C2DVector(16.0f, 8.0f);
    t.Sprites[TABLE_SPRITES_MIDDLE_RIGHT].size = C2DVector(16.0f, 8.0f);
    t.HorizontalSeparations[0] = 24; t.nbHorizontalSeparations = 1;
    t.VerticalSeparations[0] = 40; t.VerticalSeparations[1] = 80; t.nbVerticalSeparations = 2;
    t.ConstructSpritesToDraw();
    // tiles: 12 across (100 / 8), 7 down (60 / 8); separations rescaled to tiles
    if (t.VerticalSeparations[0] != 5 || t.VerticalSeparations[1] != 10 || t.HorizontalSeparations[0] != 3) ++failures;
    if (t.nbChildren != 4 || t.nbLines != 7) { std::printf("children %d lines %d\n", t.nbChildren, t.nbLines); ++failures; }
    CheckChild(t, 0, 100, 0.0f, 0.0f);      // top left
    CheckChild(t, 1, 101, 112.0f, 0.0f);    // top right: 96 of tiles + 16 of corner
    CheckChild(t, 2, 102, 0.0f, 72.0f);     // bottom left: 56 of tiles + 16 of corner
    CheckChild(t, 3, 103, 112.0f, 72.0f);   // bottom right
    CheckLine(t, 0, 'H', TABLE_SPRITES_TOP_MIDDLE, TABLE_SPRITES_SEPARATION_TOP, 16.0f, 0.0f, 12, 0);
    CheckLine(t, 1, 'H', TABLE_SPRITES_TOP_MIDDLE, TABLE_SPRITES_CROSS, 8.0f, 40.0f, 14, 1);
    CheckLine(t, 2, 'H', TABLE_SPRITES_BOTTOM_MIDDLE, -1, 16.0f, 80.0f, 12, 0);
    CheckLine(t, 3, 'V', TABLE_SPRITES_MIDDLE_LEFT, TABLE_SPRITES_SEPARATION_LEFT, 0.0f, 16.0f, 7, 0);
    CheckLine(t, 4, 'V', TABLE_SPRITES_MIDDLE_LEFT, TABLE_SPRITES_CROSS, 56.0f, 8.0f, 9, 1);
    CheckLine(t, 5, 'V', TABLE_SPRITES_MIDDLE_LEFT, TABLE_SPRITES_CROSS, 96.0f, 8.0f, 9, 1);
    CheckLine(t, 6, 'V', TABLE_SPRITES_MIDDLE_RIGHT, -1, 120.0f, 16.0f, 7, 0);

    // 2. A fixed table with only two corners: the bottom-right position stays at the origin, so IsNonZero skips that corner.
    CTableModel u;
    Setup(u);
    u.ExpansionType = 0;
    u.Size = C2DVector(100.0f, 60.0f);
    u.Sprites[TABLE_SPRITES_TOP_LEFT].present = true;
    u.Sprites[TABLE_SPRITES_BOTTOM_RIGHT].present = true;
    u.ConstructSpritesToDraw();
    if (u.nbChildren != 1 || u.nbLines != 0) { std::printf("fixed: children %d lines %d\n", u.nbChildren, u.nbLines); ++failures; }
    CheckChild(u, 0, 100, 0.0f, 0.0f);

    if (failures) { std::printf("00551ea0_TEST FAIL\n"); return 1; }
    std::printf("00551ea0_TEST PASS\n");
    return 0;
}