// NParticleEngine::CPSCAttractor::InterpolateTo(const CPSCBase& other, float t) -- vtable slot 14: blend towards another attractor (the two flags and the position parameter and its name taken from the target past the halfway point; radius, force and the integer fall-off lerped; every user point lerped towards the target's point of the same index)
// Grade: DIFFER(946v926) retry1 -- retail_asm helper; residue: register model of the whole body
// Leaf: ?InterpolateTo@CPSCAttractor@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B17BD0, 924 bytes (+ int3 padding), `ret 8`. Slot 14 of the CPSCAttractor vtable
//   (the slot of the landed CPSCSpline::InterpolateTo 0x00B1AF50, FableWin.pdb ?InterpolateTo@CPSCSpline@NParticleEngine@@
//   UAEXABVCPSCBase@2@M@Z). No callees.
// Retail-asm candidate: 0x00B17CAA `fstp dword ptr [esp+0Ch] ; fld dword ptr [esp+0Ch] ; fistp dword ptr [esp+10h]` -- the
//   engine's inline-assembly float-to-long helper (as the landed CPSCEmitterGeneric::GetRandomRadius 0x00B102C0: the value
//   homed in the helper's by-value parameter and reloaded for a bare dword fistp). This C++ reference body converts with
//   a cast, which VC7.1 emits as a call to __ftol2.
// Constants: 0.5f @ 0x0122F59C, 1.0f @ 0x0122DED8 (compiler literals).
// Layout = Ego_r.pdb CPSCAttractor (as the landed DatabaseCopy 0x00B18060): AttractorEnabled / AttractorUseParamPosition
//   bits 0/1 of +0x0c, AttractorInfluenceFallOff +0x10, AttractorPositionParam +0x14, AttractorPositionParamName +0x18,
//   AttractorInfluenceRadius +0x1c, AttractorInfluenceForce +0x20, AttractorUserPointsArray +0x24 (STLport vector).
// Shape: each switched member is `t > 0.5f ? other : own` (the compares re-issued per member); `1 - t` is a float local;
//   the lerps are `t * other + s * own`; the point loop runs over a hoisted count (VC7.1 unrolls it four times) and builds
//   each point from inline C3DVector operator* / operator+ temporaries.
// the engine's inline-assembly float-to-long helper: expanded in place, reads the homed value, writes a frame word
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CVector3D {                                 // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    C3DVector& operator[](unsigned int n) { return _M_start[n]; }
    const C3DVector& operator[](unsigned int n) const { return _M_start[n]; }
};

namespace NParticleEngine {

struct CParticleSystem;

struct CPSCBase {
    virtual void v00();
    CParticleSystem* ParticleSystem;               // +0x04
    bool UpdateStarted : 1;                        // +0x08
    bool Initialised : 1;
};

class CPSCAttractor : public CPSCBase {
public:
    bool          AttractorEnabled : 1;            // +0x0c bit 0
    bool          AttractorUseParamPosition : 1;   // +0x0c bit 1
    long          AttractorInfluenceFallOff;       // +0x10
    unsigned long AttractorPositionParam;          // +0x14
    unsigned long AttractorPositionParamName;      // +0x18
    float         AttractorInfluenceRadius;        // +0x1c
    float         AttractorInfluenceForce;         // +0x20
    CVector3D     AttractorUserPointsArray;        // +0x24
    virtual void InterpolateTo(const CPSCBase& other, float t);
};

void CPSCAttractor::InterpolateTo(const CPSCBase& other, float t)
{
    const CPSCAttractor& source = static_cast<const CPSCAttractor&>(other);
    AttractorEnabled = t > 0.5f ? source.AttractorEnabled : AttractorEnabled;
    AttractorPositionParam = t > 0.5f ? source.AttractorPositionParam : AttractorPositionParam;
    AttractorUseParamPosition = t > 0.5f ? source.AttractorUseParamPosition : AttractorUseParamPosition;
    AttractorPositionParamName = t > 0.5f ? source.AttractorPositionParamName : AttractorPositionParamName;
    float s = 1.0f - t;
    AttractorInfluenceRadius = t * source.AttractorInfluenceRadius + s * AttractorInfluenceRadius;
    AttractorInfluenceForce = t * source.AttractorInfluenceForce + s * AttractorInfluenceForce;
    AttractorInfluenceFallOff = FloatToLong(source.AttractorInfluenceFallOff * t + AttractorInfluenceFallOff * s);
    unsigned int count = AttractorUserPointsArray.size();
    for (unsigned int i = 0; i < count; ++i)
        AttractorUserPointsArray[i] = AttractorUserPointsArray[i] * s + source.AttractorUserPointsArray[i] * t;
}

} // namespace NParticleEngine
