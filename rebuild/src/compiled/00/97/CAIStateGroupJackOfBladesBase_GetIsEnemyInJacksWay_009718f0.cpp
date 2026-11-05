// CAIStateGroup_JackOfBladesBase::GetIsEnemyInJacksWay(const C3DVector& from, const C3DVector& to) -- true when the live target thing is nearer (in XY) to the destination than the start is, and lies within 15 degrees of the start-to-destination direction as seen from the destination
// Leaf: ?GetIsEnemyInJacksWay@CAIStateGroup_JackOfBladesBase@@IAE_NABVC3DVector@@0@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Naming wave 18: g_InvSqrtMantissaTable is GFastOneOverSqrtTable (?GFastOneOverSqrtTable@@3PAKA); ChatGPT pass 10 audit, raw Ego_r public.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009718F0, length 510 bytes (+ int3 padding), `ret 8`.
// Prototype: FableWin.pdb ?GetIsEnemyInJacksWay@CAIStateGroup_JackOfBladesBase@@IAE_NABVC3DVector@@0@Z (protected).
// Callees (PDB spellings): CBaseIntelligentPointer::GetPItem 0x00A01B50 (?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ,
//   three times on PTargetThing), CThing::GetPos 0x004C73D0 (?GetPos@CThing@@QBEABVC3DVector@@XZ, the bound spelling).
// Globals: cos(15 degrees) 0.965923f 0x012975F4, 3.0f 0x0122DED4, 0.5f 0x0122F59C, the reciprocal-square-root mantissa table
//   0x013CE558 (C3DVector's inline fast Normalise, the landed 0x007A6E20 model).
// Layout: engine/CAIStateGroup_JackOfBladesBase.h (PTargetThing +0x24, an opaque CIntelligentPointer<CThingCreatureBase>
//   viewed through the local intelligent-pointer model); CThing derives from engine/CBaseObject.h (FABLE_RETAIL_BASE_OBJECT);
//   CThing Dead = bit 0 of +0x91 (retail offset).
// C3DVector inlines (Ego_r/FableWin names): copy ctor ??0C3DVector@@QAE@ABV0@@Z, ??GC3DVector@@QBE?BV0@ABV0@@Z,
//   ?GetSquaredMagnitude2D@C3DVector@@QBEMXZ, ?Dot@C3DVector@@QBE?BMABV1@@Z, ?Normalise@C3DVector@@QAEAAV1@XZ.
// Shape (chunk 616): retail CSEs the XY squares of both GetSquaredMagnitude2D calls into the two Normalise sums. The squares
//   order, which spill slot each lands in and the x87 add order follow two compound forms: GetSquaredMagnitude2D as
//   `squared = Y * Y; squared += X * X;` and the Normalise sum as `z * z; += x * x; += y * y`. Each alone fixed one half
//   (the path squares / the enemy spill slots); the earlier `y, z, x` Normalise and `X * X + Y * Y` forms swapped them.
extern unsigned long GFastOneOverSqrtTable[128];   // 0x013CE558

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetSquaredMagnitude2D() const
    {
        float squared = Y * Y;
        squared += X * X;
        return squared;
    }
    const float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    __forceinline C3DVector& Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = z * z;
        lengthSquared += x * x;
        lengthSquared += y * y;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
        return *this;
    }
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
class CThing : public CBaseObject {
public:
    unsigned char _pad08[0x91 - 8];
    unsigned char Dead : 1;                     // +0x91 bit 0
    const C3DVector& GetPos() const;            // 0x004C73D0
};
class CThingCreatureBase : public CThing { };
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

#define CAIStateGroup_JackOfBladesBase CAIStateGroup_JackOfBladesBaseLayout
#include "engine/CAIStateGroup_JackOfBladesBase.h"
#undef CAIStateGroup_JackOfBladesBase
struct CAIStateGroup_JackOfBladesBase : public CAIStateGroup_JackOfBladesBaseLayout {
protected:
    bool GetIsEnemyInJacksWay(const C3DVector& from, const C3DVector& to);
};

bool CAIStateGroup_JackOfBladesBase::GetIsEnemyInJacksWay(const C3DVector& from, const C3DVector& to)
{
    const CIntelligentPointer<CThingCreatureBase>& target = *(const CIntelligentPointer<CThingCreatureBase>*)PTargetThing;
    if (!(target.Get() != 0 && !target->Dead))
        return false;
    C3DVector path = to - from;
    C3DVector fromEnemy = to - target->GetPos();
    float enemyDistanceSquared = fromEnemy.GetSquaredMagnitude2D();
    float pathDistanceSquared = path.GetSquaredMagnitude2D();
    path.Normalise();
    fromEnemy.Normalise();
    if (enemyDistanceSquared < pathDistanceSquared && fromEnemy.Dot(path) > 0.965923f)
        return true;
    return false;
}