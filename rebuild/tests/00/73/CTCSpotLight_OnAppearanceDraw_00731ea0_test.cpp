// Behaviour test for CTCSpotLight::OnAppearanceDraw 0x00731EA0 (copy of the model's logic with recording stand-ins for the callees).
#include <cstdio>
#include <cmath>
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
class CRGBColour { public: unsigned char B, G, R, A; CRGBColour() {} CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
    CRGBFloatColour(const CRGBColour& c) : R(c.R * (1.0f / 255.0f)), G(c.G * (1.0f / 255.0f)), B(c.B * (1.0f / 255.0f)), A(c.A * (1.0f / 255.0f)) {}
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
    CRGBColour GetRGBColour() const { return CRGBColour((unsigned char)(R * 255.0f), (unsigned char)(G * 255.0f), (unsigned char)(B * 255.0f), (unsigned char)(A * 255.0f)); }
};
class CThing { public: C3DVector Pos; unsigned __int64 UniqueID; const C3DVector& GetPos() const { return Pos; } };
struct CSpotLightInfo { float Angle, Width, InnerRadius, OuterRadius, Flicker; CRGBColour Colour; };
class CSpotLightDef { public: CRGBColour Colour; float InnerRadius, OuterRadius, Width, Flicker, FlickerSpeed, Angle; };
struct CPrimitive { int Type; };
struct CSpot { unsigned __int64 id; C3DVector pos, dir; CRGBColour colour; float radius, spot, cone; int set; unsigned long mask; };
struct CAppearance {
    int adds; CPrimitive slot; CPrimitive* last; CSpot drawn;
    CPrimitive* AddPersistentPrimitive(const CPrimitive& p) { ++adds; slot = p; return &slot; }
    void SetPrimitiveGraphics(CPrimitive* p, const CSpot& s, unsigned short) { last = p; drawn = s; }
};
struct CDrawInfo { float Interpolate; };
class CTCSpotLight { public:
    CThing* ParentThing; CSpotLightInfo* PLightInfo; const CSpotLightDef* PLightDef; bool Overridden; CPrimitive* LightPrimitive;
    float AccumilativeFlickerFactor, OldAccumilativeFlickerFactor;
    CRGBColour GetColour() { if (Overridden) return PLightInfo->Colour; else return PLightDef->Colour; }
    float GetWidth() { if (Overridden) return PLightInfo->Width; else return PLightDef->Width; }
    float GetInnerRadius() { if (Overridden) return PLightInfo->InnerRadius; else return PLightDef->InnerRadius; }
    float GetOuterRadius() { if (Overridden) return PLightInfo->OuterRadius; else return PLightDef->OuterRadius; }
    C3DVector GetDirection() { return C3DVector(0.0f, 0.0f, -1.0f); }
    bool OnAppearanceDraw(CAppearance& tc_appearance, const CDrawInfo& draw_info)
    {
        if (LightPrimitive == 0) { CPrimitive p; p.Type = 3; LightPrimitive = tc_appearance.AddPersistentPrimitive(p); }
        CRGBFloatColour colour(GetColour());
        float flicker = (1.0f - draw_info.Interpolate) * OldAccumilativeFlickerFactor + AccumilativeFlickerFactor * draw_info.Interpolate;
        CRGBColour draw_colour = (colour * flicker).GetRGBColour();
        float width = GetWidth(), inner_radius = GetInnerRadius(), outer_radius = GetOuterRadius();
        const C3DVector& pos = ParentThing->GetPos();
        const CThing& thing = *ParentThing;
        CSpot s; s.id = thing.UniqueID; s.pos = pos; s.dir = GetDirection(); s.colour = draw_colour;
        s.radius = outer_radius; s.spot = inner_radius * flicker; s.cone = width * 0.5f; s.set = 0; s.mask = 0xFFFFFFFF;
        tc_appearance.SetPrimitiveGraphics(LightPrimitive, s, 0);
        return true;
    }
};
static int fail(int line) { std::printf("00731ea0_TEST FAIL\n"); std::printf("line %d\n", line); return 1; }
int main()
{
    CThing thing; thing.Pos = C3DVector(1.0f, 2.0f, 3.0f); thing.UniqueID = 0x123456789ULL;
    CSpotLightDef def; def.Colour = CRGBColour(200, 100, 50, 255); def.InnerRadius = 2.0f; def.OuterRadius = 8.0f; def.Width = 4.0f;
    CSpotLightInfo info; info.Colour = CRGBColour(10, 20, 30, 40); info.Width = 6.0f; info.InnerRadius = 1.0f; info.OuterRadius = 5.0f;
    CTCSpotLight light; light.ParentThing = &thing; light.PLightInfo = &info; light.PLightDef = &def; light.Overridden = false;
    light.LightPrimitive = 0; light.AccumilativeFlickerFactor = 1.0f; light.OldAccumilativeFlickerFactor = 0.5f;
    CAppearance app; app.adds = 0; app.last = 0;
    CDrawInfo di; di.Interpolate = 0.5f;                      // flicker = 0.5*0.5 + 1*0.5 = 0.75
    if (!light.OnAppearanceDraw(app, di)) return fail(__LINE__);
    if (app.adds != 1 || app.slot.Type != 3 || light.LightPrimitive != &app.slot || app.last != &app.slot) return fail(__LINE__);
    if (app.drawn.id != 0x123456789ULL || app.drawn.pos.Y != 2.0f || app.drawn.dir.Z != -1.0f) return fail(__LINE__);
    if (app.drawn.radius != 8.0f || std::fabs(app.drawn.spot - 1.5f) > 1e-5f || app.drawn.cone != 2.0f) return fail(__LINE__);
    if (app.drawn.colour.R != 149 && app.drawn.colour.R != 150) return fail(__LINE__);
    if (app.drawn.colour.A != 191 || app.drawn.colour.B != 37) return fail(__LINE__);
    if (app.drawn.set != 0 || app.drawn.mask != 0xFFFFFFFF) return fail(__LINE__);
    light.Overridden = true; di.Interpolate = 1.0f;            // flicker = AccumilativeFlickerFactor = 1
    light.OnAppearanceDraw(app, di);
    if (app.adds != 1) return fail(__LINE__);                 // slot allocated once
    if (app.drawn.radius != 5.0f || app.drawn.spot != 1.0f || app.drawn.cone != 3.0f) return fail(__LINE__);
    if (app.drawn.colour.R != 10 || app.drawn.colour.G != 20 || app.drawn.colour.B != 30 || app.drawn.colour.A != 40) return fail(__LINE__);
    std::printf("00731ea0_TEST PASS\n");
    return 0;
}