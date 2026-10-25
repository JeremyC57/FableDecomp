// NCameraModes::CStringAndCageBase::UpdateStringLength(C3DVector& pos) const -- keep the string-and-cage camera within its string length of the observed thing (2D): beyond it, pull the go-to position in onto the length and return true; inside it, when the mode drifts to the maximum length, ease the length 1/11 of the way towards the current distance and place the position there (true); otherwise false
// Grade: DIFFER(417v419) -- x87 scheduling only: the 2D length is kept with `fst` where retail stores and reloads it, and the inline operator/ keeps 1/dist on the stack (`fst ; fmul st(2) ; fstp st(1)`) in retail's first path
// Leaf: ?UpdateStringLength@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008722D0, length 419 bytes (__thiscall, ret 4, bool in al). PDB (Ego_r):
// ?UpdateStringLength@CStringAndCageBase@NCameraModes@@ABE_NAAVC3DVector@@@Z (private const). The catalog label
// "ParticleMorph_UpdatePositionWithVelocity" is propagated and wrong. Caller: the landed CStringAndCageBase::Update @ 0x008728C0
// (`if (!UpdateStringLength(goTo)) UpdateCageRadius(goTo);`); sibling: the landed UpdateCageRadius @ 0x00871D50 (same model).
// Callees: NCameraModes::CBase::GetObservedThing() const 0x0084F1B0, CThing::GetPosition() const 0x004C73D0,
//          NCameraModes::CBase::GetCamTools() const 0x0084D080 (result unused, as in Update), virtual slot 31 (+0x7c; 0x0069DDE0
//          `xor al,al ; ret` in CStringAndCageBase) -- named IsDriftToMaxStringLength from the class's Ego_r.pdb protected
//          const bool virtuals (descriptive slot assignment).
// Pools: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 10.0f 0x01237F0C, 1/11 0x01277544 (VC7.1 turns `/ 11.0f` into this multiply).
// Layout (retail = Ego_r - 0x24, as the landed siblings): CurrentStringLength +0xB4.
// Shape: as UpdateCageRadius -- centre copied memberwise, offset (pos - centre in X/Y, Z 0.0f), squared length summed in
// statement order (X first), `offset * length / dist + centre` through the inline operators; the length is read once into a
// local that lives in the dead `pos` argument slot and is reused after the virtual call. `dist > length` gives
// `test ah,41h ; jne`.
// Tried (diagnostic compiles): one-expression sum (Y squared first), `length < dist`, length declared after dist (fixes the
// store/reload but swaps the `offset * length` operands), by-reference float operators, named scaled vector, reciprocal
// form, `f * X` operator, QFE toolset and /Oa (no change).
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator+(const C3DVector& r) const { return C3DVector(X + r.X, Y + r.Y, Z + r.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator/(float s) const { float inv = 1.0f / s; return C3DVector(X * inv, Y * inv, Z * inv); }
};
class CThing {
public:
    const C3DVector& GetPosition() const;                  // 0x004C73D0
};
class CThingGameObject : public CThing { };
namespace NCameraModes {
class CTools { unsigned char _body[0x24]; };
class CBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual bool IsDriftToMaxStringLength() const;         // slot 31 (+0x7c)
    unsigned char _pad04[0xa4 - 0x04];
    const CThingGameObject* GetObservedThing() const;      // 0x0084F1B0
    const CTools& GetCamTools() const;                     // 0x0084D080
};
class CStringAndCageBase : public CBase {
public:
    float DesiredHeightOffset;                             // +0xa4
    float DesiredStringLength;                             // +0xa8
    float DesiredCageRadius;                               // +0xac
    float CurrentHeightOffset;                             // +0xb0
    float CurrentStringLength;                             // +0xb4
    float CurrentCageRadius;                               // +0xb8
    bool UpdateStringLength(C3DVector& pos) const;
};
bool CStringAndCageBase::UpdateStringLength(C3DVector& pos) const
{
    C3DVector centre = GetObservedThing()->GetPosition();
    C3DVector offset(pos.X - centre.X, pos.Y - centre.Y, 0.0f);
    float length = CurrentStringLength;
    float distSq = offset.X * offset.X;
    distSq += offset.Y * offset.Y;
    float dist = (float)sqrt(distSq);
    if (dist > length) {
        pos = offset * length / dist + centre;
        return true;
    }
    if (IsDriftToMaxStringLength()) {
        float drift = (dist * 10.0f + length) / 11.0f;
        GetCamTools();
        pos = offset * drift / dist + centre;
        return true;
    }
    return false;
}
}
