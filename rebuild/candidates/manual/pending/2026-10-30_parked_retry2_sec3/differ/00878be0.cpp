// Grade: DIFFER(232v244) -- x87 tail: retail spills the position copy's X (Z copied as an integer before Y is loaded) and sums position + offset from memory into two temporaries; ours keeps position.X on the FPU and commutes the first add. Lerp form 1 + (1 - 1) * t and the named GetRotatedPosition local match retail
// NCameraModes::CProjectileWeapon::GetPosFromParams(float angle, float extension) -- camera position for the projectile-weapon mode: the observed thing's position plus the rotated offset for the angle (scaled) plus the mode's extension direction times the remapped extension
// Leaf: GetPosFromParams
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00878BE0, length 244 bytes, `ret 0xc` (C3DVector by hidden pointer).
// Callees: GetRotatedPosition(float) 0x00878A80 (same class, propagated label "CThing::GetRotatedPosition"),
// GetObservedThing 0x0084F1B0 (propagated label "_Cons_val<...>"). Floats 0.5f 0x012777F8, 0.0f 0x0122DEDC,
// 1.0f 0x0122DED8.
// Layout: packed camera-mode members (the landed CProjectileWeapon ctor 0x00879620): extension direction +0xc1;
// the observed thing's +0x60 object holds the position at +0xc.
#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CPlacement {
    unsigned char _pad00[0xc];
    C3DVector     Position;                            // +0x0c
};
struct CThingGameObject {
    unsigned char _pad00[0x60];
    CPlacement*   Placement;                           // +0x60
};
namespace NCameraModes {
struct CProjectileWeapon {
    unsigned char _pad00[0xc1];
    C3DVector     ExtensionDirection;                  // +0xc1
    C3DVector GetRotatedPosition(float angle);         // 0x00878A80
    const CThingGameObject* GetObservedThing() const;  // 0x0084F1B0
    C3DVector GetPosFromParams(float angle, float extension);
};
}
#pragma pack(pop)

C3DVector NCameraModes::CProjectileWeapon::GetPosFromParams(float angle, float extension)
{
    extension = extension + extension + 0.5f;
    float t = (extension - 0.5f) * 0.5f;
    float scale = 1.0f + (1.0f - 1.0f) * t;
    C3DVector rotated = GetRotatedPosition(angle);
    C3DVector offset = rotated * scale;
    C3DVector position = GetObservedThing()->Placement->Position;
    C3DVector reach = ExtensionDirection * extension;
    return position + offset + reach;
}
