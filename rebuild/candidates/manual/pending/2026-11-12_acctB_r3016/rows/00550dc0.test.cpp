// Behaviour test for 0x00550DC0 NUISystem::CTable::Draw: a self-contained host model of the row's logic. A bastard-child table takes
// its parent's render position / relative render position (plus its own position), render zoom / relative render zoom (times its own
// zoom) and render colour (times its own colour, channel by channel through float colours), and pushes them to every child sprite
// (colour product -- the table's colour variable is reused for each child's product, as in the row --, position offsets, the relative position scaled by the UI scale, zoom products, the graphic mapped to the parent's
// state); then it draws as a component.
#include <cstdio>
#include <cmath>

struct V2 { float X, Y; };
struct RGB { unsigned char B, G, R, A; };
struct FColour {
    float R, G, B, A;
    explicit FColour(const RGB& c) { R = c.R * (1.0f / 255.0f); G = c.G * (1.0f / 255.0f); B = c.B * (1.0f / 255.0f); A = c.A * (1.0f / 255.0f); }
    FColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    RGB GetRGBColour() const { RGB c; c.R = (unsigned char)(R * 255.0f); c.G = (unsigned char)(G * 255.0f); c.B = (unsigned char)(B * 255.0f); c.A = (unsigned char)(A * 255.0f); return c; }
};
static FColour Mul(const FColour& a, const FColour& b) { return FColour(a.R * b.R, a.G * b.G, a.B * b.B, a.A * b.A); }

struct Sprite {
    RGB Colour; V2 Position, Zoom;
    RGB RenderColour; V2 RenderPosition, RelativeRenderPosition, RenderZoom, RelativeRenderZoom;
    unsigned long Graphic; unsigned long Indices[4];
};
struct Parent { V2 RenderPosition, RelativeRenderPosition, RenderZoom, RelativeRenderZoom; RGB RenderColour; unsigned long State; };

struct Table {
    bool BastardChild;
    V2 Position, Zoom; RGB Colour;
    V2 RenderPosition, RelativeRenderPosition, RenderZoom, RelativeRenderZoom; RGB RenderColour;
    Sprite* Children[4]; unsigned long ChildCount;
    int BaseDraws;
    void Draw(Parent* parent, V2 uiScale) {
        if (BastardChild && parent) {
            RenderPosition.X = parent->RenderPosition.X + Position.X; RenderPosition.Y = parent->RenderPosition.Y + Position.Y;
            RelativeRenderPosition.X = parent->RelativeRenderPosition.X + Position.X; RelativeRenderPosition.Y = parent->RelativeRenderPosition.Y + Position.Y;
            RenderZoom.X = parent->RenderZoom.X * Zoom.X; RenderZoom.Y = parent->RenderZoom.Y * Zoom.Y;
            RelativeRenderZoom.X = parent->RelativeRenderZoom.X * Zoom.X; RelativeRenderZoom.Y = parent->RelativeRenderZoom.Y * Zoom.Y;
            FColour parent_colour(parent->RenderColour);
            FColour colour(Colour);
            FColour render_colour = Mul(colour, parent_colour);
            RenderColour = render_colour.GetRGBColour();
            for (unsigned long i = 0; i < ChildCount; ++i) {
                Sprite* s = Children[i];
                colour = Mul(FColour(s->Colour), render_colour);
                RGB child_render_colour = colour.GetRGBColour();
                s->RenderColour = child_render_colour;
                s->RenderPosition.X = RenderPosition.X + s->Position.X; s->RenderPosition.Y = RenderPosition.Y + s->Position.Y;
                V2 rel = s->Position; rel.X *= uiScale.X; rel.Y *= uiScale.Y;
                s->RelativeRenderPosition.X = rel.X + RelativeRenderPosition.X; s->RelativeRenderPosition.Y = rel.Y + RelativeRenderPosition.Y;
                s->RenderZoom.X = s->Zoom.X * RenderZoom.X; s->RenderZoom.Y = s->Zoom.Y * RenderZoom.Y;
                s->RelativeRenderZoom.X = s->Zoom.X * RelativeRenderZoom.X; s->RelativeRenderZoom.Y = s->Zoom.Y * RelativeRenderZoom.Y;
                s->Graphic = s->Indices[parent->State];
            }
        }
        ++BaseDraws;
    }
};

static int g_fail;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("check failed: %s\n", what); g_fail = 1; } }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static RGB Make(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { RGB c; c.R = r; c.G = g; c.B = b; c.A = a; return c; }

int main()
{
    Sprite s0 = { Make(255, 255, 255, 255), { 1.0f, 2.0f }, { 2.0f, 0.5f } };
    Sprite s1 = { Make(255, 0, 255, 255), { -3.0f, 4.0f }, { 1.0f, 1.0f } };
    s0.Indices[0] = 10; s0.Indices[1] = 11; s0.Indices[2] = 12; s0.Indices[3] = 13;
    s1.Indices[0] = 20; s1.Indices[1] = 21; s1.Indices[2] = 22; s1.Indices[3] = 23;
    Parent p = { { 100.0f, 50.0f }, { 10.0f, 5.0f }, { 2.0f, 3.0f }, { 0.5f, 0.25f }, Make(255, 255, 255, 255), 2 };
    Table t;
    t.BastardChild = true; t.Position.X = 4.0f; t.Position.Y = 6.0f; t.Zoom.X = 1.5f; t.Zoom.Y = 2.0f; t.Colour = Make(255, 255, 0, 255);
    t.Children[0] = &s0; t.Children[1] = &s1; t.ChildCount = 2; t.BaseDraws = 0;
    V2 ui = { 2.0f, 0.5f };
    t.Draw(&p, ui);
    Check(Near(t.RenderPosition.X, 104.0f) && Near(t.RenderPosition.Y, 56.0f), "table render position");
    Check(Near(t.RelativeRenderPosition.X, 14.0f) && Near(t.RelativeRenderPosition.Y, 11.0f), "table relative position");
    Check(Near(t.RenderZoom.X, 3.0f) && Near(t.RenderZoom.Y, 6.0f) && Near(t.RelativeRenderZoom.X, 0.75f) && Near(t.RelativeRenderZoom.Y, 0.5f), "table zooms");
    Check(t.RenderColour.R == 255 && t.RenderColour.G == 255 && t.RenderColour.B == 0 && t.RenderColour.A == 255, "table colour");
    Check(s0.RenderColour.R == 255 && s0.RenderColour.G == 255 && s0.RenderColour.B == 0, "child 0 colour");
    Check(s1.RenderColour.R == 255 && s1.RenderColour.G == 0 && s1.RenderColour.B == 0 && s1.RenderColour.A == 255, "child 1 colour");
    Check(Near(s0.RenderPosition.X, 105.0f) && Near(s0.RenderPosition.Y, 58.0f), "child position");
    Check(Near(s1.RelativeRenderPosition.X, 8.0f) && Near(s1.RelativeRenderPosition.Y, 13.0f), "child relative position");
    Check(Near(s0.RenderZoom.X, 6.0f) && Near(s0.RenderZoom.Y, 3.0f) && Near(s0.RelativeRenderZoom.X, 1.5f) && Near(s0.RelativeRenderZoom.Y, 0.25f), "child zooms");
    Check(s0.Graphic == 12 && s1.Graphic == 22 && t.BaseDraws == 1, "graphic by parent state, base draw");
    // Half colours: 128/255 * 128/255 * 255 truncates.
    p.RenderColour = Make(128, 128, 128, 128); t.Colour = Make(128, 255, 255, 64);
    t.Draw(&p, ui);
    Check(t.RenderColour.R == 64 && t.RenderColour.G == 128 && t.RenderColour.B == 128 && t.RenderColour.A == 32, "colour product truncation");
    // Not a bastard child, or no parent: only the component draw.
    s0.Graphic = 0; t.BastardChild = false; t.Draw(&p, ui);
    Check(s0.Graphic == 0 && t.BaseDraws == 3, "not bastard child");
    t.BastardChild = true; t.Draw(0, ui);
    Check(s0.Graphic == 0 && t.BaseDraws == 4, "no parent");
    if (g_fail) { std::printf("00550dc0_TEST FAIL\n"); return 1; }
    std::printf("00550dc0_TEST PASS\n");
    return 0;
}
