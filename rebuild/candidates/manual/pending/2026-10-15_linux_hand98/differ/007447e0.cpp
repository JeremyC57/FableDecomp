// CTCPerceiveThing::UpdateDistanceFromPerceivedThing() -- DistanceFromPerceivedThingSquared = |owner position - perceived thing position|^2
// Grade: DIFFER(ours v retail) -- 77v77: epilogue interleave only -- retail 'pop esi ; fstp st(0) ; pop ebx ; fstp st(0) ; fstp st(0)', the lane pops esi and ebx before the three x87 pops
// Retail: Fable.exe (TLC, VC7.1) @ 0x007447E0, 77 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (on PerceivedThing +0x0c), CThing::GetPosition @ 0x004C73D0
//   (manifest "CVertexBufferWin32::DoSizeof", propagated: returns &(thing->+0x60)->+0x0c, a const C3DVector&), twice.
// Layout: engine/CTCPerceiveThing.h (field4 +0x04 = the owner thing, PerceivedThing +0x0c, DistanceFromPerceivedThingSquared +0x14).
// Shape: the inline C3DVector difference (owner first) and an inline length-squared. The perceived thing and the owner are
//   named locals in that order (the owner load sits after the GetPItem call; a direct `GetThing()->` in the expression is
//   hoisted to the prologue); the right operand's GetPosition is called first.
// Leaf: UpdateDistanceFromPerceivedThing
#include "engine/CTCPerceiveThing.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

struct CThing {
    const C3DVector& GetPosition() const;   // 0x004C73D0
};

struct CBaseIntelligentPointer {
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CTCPerceiveThing_Methods : CTCPerceiveThing {
    CThing* GetThing() const { return reinterpret_cast<CThing*>(field4); }
    const CBaseIntelligentPointer& GetPerceivedThing() const { return *reinterpret_cast<const CBaseIntelligentPointer*>(PerceivedThing); }
    void UpdateDistanceFromPerceivedThing();
};

void CTCPerceiveThing_Methods::UpdateDistanceFromPerceivedThing()
{
    CThing* perceived = GetPerceivedThing().GetPItem();
    CThing* owner = GetThing();
    DistanceFromPerceivedThingSquared = (owner->GetPosition() - perceived->GetPosition()).GetLengthSquared();
}
