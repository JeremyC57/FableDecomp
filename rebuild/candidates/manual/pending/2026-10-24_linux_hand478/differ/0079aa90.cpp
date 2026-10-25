// CTCSummonSpell::GetValidTeleportPos(const CThing* target) -- find where to put a summoned target: its own position if valid, else points on rings around the caster (the caster-to-target direction rotated in 1/36-turn steps, the ring radius growing by the original distance each full turn); after ten failures settle for the rotated direction at the original distance
// Grade: DIFFER(557v557) -- x87 stack/slot allocation only: retail stores dir.Z with `fst` and squares it in place (VC7.1 here: `fstp` + `fld`), and keeps the growing ring radius in the dead `target` slot with the fixed distance in a frame slot (here swapped), which also moves the loop-alignment `jmp`; the reciprocal-normalise sequence and everything else match
// Leaf: ?GetValidTeleportPos@CTCSummonSpell@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079AA90, length 557 bytes (thiscall, `ret 8`: hidden result pointer, target).
// Identity: Ego_r.pdb ?GetValidTeleportPos@CTCSummonSpell@@AAE?AVC3DVector@@PBVCThing@@@Z. The catalog label
//   "CTCSummonSpell_GetValidSummonPos" is descriptive.
// Callees: CThing::GetPosition 0x004C73D0, CTCBase::GetWorld 0x00686A70, CWorld vtable slot 13 (the world map),
//   CTCSummonSpell::IsValidTeleportPos(const C3DVector&, const CWorldMap&, const CThing*) 0x0079A890,
//   C3DVector::RotateXY(float) 0x00A14040.
// Globals: pool floats 1.0f 0x0122DED8, 1/36 0x0126C324.
// Shape: `dir = target pos - caster pos; radius = dir.Normalise() (length, reciprocal, X/Y/Z *= inv); distance = radius; angle = 0;
//   pos = target pos; tries = 0; while (!IsValidTeleportPos(pos, map, target)) { angle += 1/36; if (angle >= 1) { angle
//   = 0; radius += distance; } d = dir; d.RotateXY(angle); pos = caster pos + d * radius; if (++tries > 10) { pos =
//   caster pos + d * distance; break; } } return pos;`
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
    const float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    float GetLength() const;
    float Normalise()
    {
        float length = GetLength();
        float inverse = 1.0f / length;
        X *= inverse;
        Y *= inverse;
        Z *= inverse;
        return length;
    }
    void RotateXY(float angle);                         // 0x00A14040
};

extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

inline float C3DVector::GetLength() const { return (float)sqrt(Dot(*this)); }

class CThing {
public:
    const C3DVector& GetPosition() const;               // 0x004C73D0
};

class CWorldMap;

class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual const CWorldMap& GetWorldMap() const;       // slot 13 (descriptive)
};

class CTCSummonSpell {
public:
    virtual void v00();
    CThing* PThing;                                     // +0x04 (CTCBase)
    CWorld* GetWorld() const;                           // 0x00686A70 (CTCBase)
private:
    bool IsValidTeleportPos(const C3DVector& pos, const CWorldMap& map, const CThing* target);   // 0x0079A890
    C3DVector GetValidTeleportPos(const CThing* target);
};

C3DVector CTCSummonSpell::GetValidTeleportPos(const CThing* target)
{
    C3DVector direction = target->GetPosition() - PThing->GetPosition();
    float angle = 0.0f;
    float radius = direction.Normalise();
    float distance = radius;
    const CWorldMap& map = GetWorld()->GetWorldMap();
    C3DVector pos = target->GetPosition();
    long tries = 0;
    while (!IsValidTeleportPos(pos, map, target)) {
        angle += 1.0f / 36.0f;
        if (angle >= 1.0f) {
            angle = 0.0f;
            radius += distance;
        }
        C3DVector rotated = direction;
        rotated.RotateXY(angle);
        pos = PThing->GetPosition() + rotated * radius;
        if (++tries > 10) {
            pos = PThing->GetPosition() + rotated * distance;
            break;
        }
    }
    return pos;
}
