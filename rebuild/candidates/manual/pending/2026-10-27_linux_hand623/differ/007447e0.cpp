// CTCPerceiveThing::UpdateDistanceFromPerceivedThing() -- DistanceFromPerceivedThingSquared = |owner position - perceived thing position|^2
// Grade: DIFFER(77v77) -- epilogue interleave only: retail `pop esi ; fstp st(0) ; pop ebx ; fstp st(0) ; fstp st(0)`, ours pops esi and ebx before the three dead x87 pops
// Wave-25 chunk 623 retry (tried, inert): PDB spellings (private UpdateDistanceFromPerceivedThing, CThing::GetPos, CBaseIntelligentPointer::GetPItem returning CBaseObject*, inline GetPerceivedThing) -- same residue
// Leaf: ?UpdateDistanceFromPerceivedThing@CTCPerceiveThing@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007447E0, 77 bytes, thiscall, `ret`.
// Identity: Ego_r.pdb ?UpdateDistanceFromPerceivedThing@CTCPerceiveThing@@AAEXXZ (private).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PDB QBEPAVCBaseObject@@XZ, on PerceivedThing +0x0c),
//   CThing::GetPos 0x004C73D0 (PDB QBEABVC3DVector@@XZ: `mov eax,[ecx+60h] ; add eax,0Ch`), twice.
// Layout: engine/CTCPerceiveThing.h (PerceivedThing +0x0c, DistanceFromPerceivedThingSquared +0x14); its +0x04 "field4"
//   is CTCBase::ParentThing (Ego_r) -- header debt, read through GetThing().
// Shape: the inline C3DVector difference (owner first) and an inline length-squared; the perceived thing and the owner
//   are named locals in that order; the right operand's GetPos is called first.
#define CTCPerceiveThing CTCPerceiveThingLayout
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

class CBaseObject { };
class CThing : public CBaseObject {
public:
    const C3DVector& GetPos() const;                    // 0x004C73D0
};

class CBaseIntelligentPointer {
public:
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};

class CTCPerceiveThing : public CTCPerceiveThingLayout {
public:
    CThing* GetThing() const { return reinterpret_cast<CThing*>(field4); }     // CTCBase::ParentThing +0x04
    const CThing* GetPerceivedThing() const
    {
        return static_cast<const CThing*>(reinterpret_cast<const CBaseIntelligentPointer*>(PerceivedThing)->GetPItem());
    }
private:
    void UpdateDistanceFromPerceivedThing();
};

void CTCPerceiveThing::UpdateDistanceFromPerceivedThing()
{
    const CThing* perceived = GetPerceivedThing();
    CThing* owner = GetThing();
    DistanceFromPerceivedThingSquared = (owner->GetPos() - perceived->GetPos()).GetLengthSquared();
}
