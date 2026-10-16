// CTCHeroPodium::OnInitialActivate() -- derive the podium's crowd axes from the parent's physics facing: E_Y = the reversed facing flattened to the ground plane and normalised, E_X = its perpendicular (Y, -X, 0)
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771BD0, length 151 bytes (+9 int3 padding). Vtable 0x0126A89C slot 6.
// Callees: the physics component's virtual slot 70 (+0x118, a C3DVector getter by hidden pointer), C3DVector::Normalise
// @ 0x00A14440 (manifest "GetScaled", propagated).
// Shape: `E_Y = facing * -1.0f` (a literal: the float pool entry 0x0122DEE0 = -1.0; an inline by-value operator* whose 3-float result is
// copied through integer registers), `E_Y.Z = 0.0f` as an immediate, then `E_X = E_Y.GetPerpendicularXY()`: an inline const
// member returning C3DVector(Y, -X, 0) reads both components through the Normalise `this` register (edi); the expression
// written in the caller reads Y through esi+0x2c.
// Layout: retail NumberOfCrowdOccupants +0x0c, CrowdOccupation +0x10 (12-byte vector), E_X +0x1c, E_Y +0x28; the parent's
// physics component CThing +0x60.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& Normalise();                     // 0x00A14440
    C3DVector GetPerpendicularXY() const { return C3DVector(Y, -X, 0.0f); }
};
class CTCPhysicsBase {
public:
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59)
    V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69)
#undef V
    virtual C3DVector GetFacing() const;        // slot 70 (+0x118)
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
template <class T> struct CRetailVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCHeroPodium : public CTCBase {
public:
    long                NumberOfCrowdOccupants; // +0x0c
    CRetailVector<long> CrowdOccupation;        // +0x10
    C3DVector           E_X;                    // +0x1c
    C3DVector           E_Y;                    // +0x28
    virtual void OnInitialActivate();
};

void CTCHeroPodium::OnInitialActivate()
{
    E_Y = ParentThing->PhysicsTC->GetFacing() * -1.0f;
    E_Y.Z = 0.0f;
    E_Y.Normalise();
    E_X = E_Y.GetPerpendicularXY();
}
