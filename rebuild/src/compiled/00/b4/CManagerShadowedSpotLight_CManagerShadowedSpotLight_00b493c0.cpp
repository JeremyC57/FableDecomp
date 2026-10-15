// CManagerShadowedSpotLight::CManagerShadowedSpotLight(const C3DVector& position, const C3DVector& direction, float radius, float spotRadius, float coneRadius, const CRGBFloatColour& colour) -- value constructor of the light manager's shadowed spot-light record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B493C0, length 105 bytes. No callees; vtable 0x012A2120.
// Layout (PDB CManagerShadowedSpotLight, 0x38): vptr, Position +0x04, Direction +0x10, Radius +0x1c, SpotRadius +0x20,
//   ConeRadius +0x24, Colour (CRGBFloatColour) +0x28.
// Shape: `mov eax,ecx` first (this kept in eax), the C3DVectors copy through their inline memberwise copy constructor
//   (direct [eax+N] stores), the colour is a plain POD struct copy (`lea ecx,[eax+28h]` then [ecx+N]).
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRGBFloatColour { float R, G, B, A; };

struct CManagerShadowedSpotLight {
    virtual void Render();                         // the class is polymorphic (vtable 0x012A2120)
    C3DVector       Position;                      // +0x04
    C3DVector       Direction;                     // +0x10
    float           Radius;                        // +0x1c
    float           SpotRadius;                    // +0x20
    float           ConeRadius;                    // +0x24
    CRGBFloatColour Colour;                        // +0x28
    CManagerShadowedSpotLight(const C3DVector& position, const C3DVector& direction, float radius, float spotRadius, float coneRadius, const CRGBFloatColour& colour);
};

CManagerShadowedSpotLight::CManagerShadowedSpotLight(const C3DVector& position, const C3DVector& direction, float radius, float spotRadius, float coneRadius, const CRGBFloatColour& colour)
    : Position(position), Direction(direction), Radius(radius), SpotRadius(spotRadius), ConeRadius(coneRadius), Colour(colour)
{
}