// NThingShot::CTCShotDefinitionMultiArrow::CTCShotDefinitionMultiArrow(CThing*) -- CTCShotDefinitionBase(parent), zero acceleration, not a straight arrow, and the base's multi-arrow shot-type flag set
// Leaf: ??0CTCShotDefinitionMultiArrow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BBC65, length 58 bytes.
// Callee: NThingShot::CTCShotDefinitionBase::CTCShotDefinitionBase(CThing*) @ 0x007BF460. Vtable 0x0126DF34 =
// NThingShot::CTCShotDefinitionMultiArrow (rtti-vtables.tsv).
// PDB CTCShotDefinitionMultiArrow (0xb8): Acceleration C3DVector +0xa0, MaxCosAngleToTurn +0xac, MaxAngleToTurn +0xb0,
// StraightArrow +0xb4; base CTCShotDefinitionBase.MultiArrowShotType +0x9c. The retail base is 0x9c bytes (12-byte
// HitThingsList vector, as in the landed DrainLife constructor 0x004DB15E), so everything sits 4 lower: +0x9c, +0xb0, +0x98.
// Shape: size TU (`push dword ptr [esp+8]`) = `#pragma optimize("s", on)`; the C3DVector member has an inline
// (x, y, z) constructor (`lea eax,[esi+9Ch]` before the vtable store, three `fldz ; fstp`); StraightArrow is an
// initialiser; the base's MultiArrowShotType is a body assignment, stored last.
#pragma optimize("s", on)
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

namespace NThingShot {

class CTCShotDefinitionBase { public:
    virtual void v00();
    unsigned char _body[0x98 - 4];           // CTCBase + the shot fields (retail 0x9c bytes)
    bool          MultiArrowShotType;        // +0x98 (PDB +0x9c)
    unsigned char _pad99[3];
    CTCShotDefinitionBase(CThing* parent);   // 0x007BF460
};

class CTCShotDefinitionMultiArrow : public CTCShotDefinitionBase { public:
    C3DVector     Acceleration;              // +0x9c
    float         MaxCosAngleToTurn;         // +0xa8
    float         MaxAngleToTurn;            // +0xac
    bool          StraightArrow;             // +0xb0
    virtual void v00();
    CTCShotDefinitionMultiArrow(CThing* parent);
};

CTCShotDefinitionMultiArrow::CTCShotDefinitionMultiArrow(CThing* parent)
    : CTCShotDefinitionBase(parent)
    , Acceleration(0.0f, 0.0f, 0.0f)
    , StraightArrow(false)
{
    MultiArrowShotType = true;
}

}