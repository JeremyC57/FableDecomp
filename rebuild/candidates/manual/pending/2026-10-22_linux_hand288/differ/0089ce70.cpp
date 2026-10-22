// CGameScriptInterface::IsThingAwareOfOtherThingInAnyWay(const CScriptThing& other, const CScriptThing& thing) const -- the second thing (with a perception component, 0x34) is aware of the first when it is its perceived thing (then the component's awareness flag decides), when the first is within the smaller of the two perception ranges, or when it is within the vision distance and inside the field-of-view cone around the physics facing
// Grade: DIFFER(610v616) -- calls, constants and control flow match; residue is x87 scheduling (retail squares the offset Z, X, Y and re-sums the cached squares for the inline fast Normalise; VC7.1 canonicalises the sum and keeps the whole length), the std::min lea order, the two `return true` epilogues (VC7.1 merges ours) and the facing temporary read through eax
// Leaf: IsThingAwareOfOtherThingInAnyWay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089CE70, length 616 bytes (virtual, `ret 8`).
// Callees: CScriptThing slots 11/75, LowerBound 0x0040F020, CTCPerceiveThing 0x007446A0 (GetPerceivedThing) / 0x00643B3B / 0x00744440 /
//   0x00744450 / 0x00744950 / 0x007445D0 (names of the four float getters are guesses), CThing::GetPosition 0x004C73D0, the physics
//   body's slot 70 GetFacing (+0x118, C3DVector by hidden pointer). Globals: the reciprocal-square-root mantissa table 0x013CE558,
//   3.0f 0x0122DED4, 0.5f 0x0122F59C, the double 2*pi (float-rounded) 0x0124F2B8.
// Layout: CThing presence bits +0x20, component map +0x44, physics body +0x60 (as the landed 0x007BB150 / 0x007A6E20).
#include <math.h>
#pragma intrinsic(cos)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    float LengthSquared() const { return X * X + Y * Y + Z * Z; }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCPhysics {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacing() const;        // slot 70 (+0x118)
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysics* Physics;                        // +0x60
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CTCPerceiveThing {
    CThing* GetPerceivedThing() const;          // 0x007446A0
    bool IsAwareOfPerceivedThing() const;       // 0x00643B3B
    float GetSightRange() const;                // 0x00744440
    float GetHearingRange() const;              // 0x00744450
    float GetVisionDistance() const;            // 0x00744950
    float GetFieldOfView() const;               // 0x007445D0
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;           // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;               // slot 75 (+0x12c)
};
struct CWorld;
struct CGameScriptInterface {
    virtual void v000();
    CWorld* World;                              // +0x04
    virtual bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing& other, const CScriptThing& thing) const;
};

bool CGameScriptInterface::IsThingAwareOfOtherThingInAnyWay(const CScriptThing& other, const CScriptThing& thing) const
{
    if (!other.IsValid() || !thing.IsValid())
        return false;
    CThing* perceiver = thing.GetThing();
    CThing* target = other.GetThing();
    if (!perceiver->HasTC(0x34))
        return false;
    CTCPerceiveThing* perception = perceiver->GetTC<CTCPerceiveThing>(0x34);
    CThing* perceived = perception->GetPerceivedThing();
    if (perceived && perceived == target)
        return perception->IsAwareOfPerceivedThing();
    float sight = perception->GetSightRange();
    float hearing = perception->GetHearingRange();
    float range = Min(hearing, sight);
    if (!(range * range < (target->GetPosition() - perceiver->GetPosition()).LengthSquared()))
        return true;
    float distance = perception->GetVisionDistance();
    C3DVector direction = target->GetPosition() - perceiver->GetPosition();
    if (distance * distance < direction.LengthSquared())
        return false;
    direction.Normalise();
    if (cos(perception->GetFieldOfView() * 6.2831854820251465) < Dot(perceiver->Physics->GetFacing(), direction))
        return true;
    return false;
}
