// CAIStateGroup_HoverHornetQueen::IsRouteToPosBlockedByTarget(long circlePoint) -- true when the target and the circle marker are alive, the direction to the marker is within 60 degrees of the direction to the target (dot > 0.5) and the target is within 3 units in XY
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095A490, length 558 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPos @ 0x004C73D0.
// Globals: inverse-square-root mantissa table @ 0x013CE558; 3.0f @ 0x0122DED4, 0.5f @ 0x0122F59C.
// Shape: both directions use the inline table-driven Normalise written as `*this *= FastInvSqrt(lenSq)` with
// `operator*=(const float&)`: the const-reference binding to the inline temporary spills the reciprocal
// (`fstp [r] ; fxch ; fmul [r]` x3) as retail does (chunk 278's lesson; a plain `X *= r` keeps it on the x87 stack);
// the XY distance shares the X*X/Y*Y products with the normalise length (VC CSE). The thing pointers are named
// before their GetPos calls.
// Layout: Ego_r.pdb CAIStateGroup_HoverMovementBase target pointer +0x14; HoverHornetQueen CircleMarkers +0x50 in
// retail (PDB +0x54: the STLport vector Things at +0x40 is 12 bytes, not 16).
#include <math.h>
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetXYMagnitude() const { return (float)sqrt(X * X + Y * Y); }
    __forceinline C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline void Normalise() { *this *= FastInvSqrt(X * X + Y * Y + Z * Z); }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
inline bool IsAlive(const CIntelligentPointer<CThing>& p) { return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1); }
struct CIntelligentPointerVector {              // STLport vector<CIntelligentPointer<CThing> > (12 bytes)
    CIntelligentPointer<CThing>* _M_start;
    CIntelligentPointer<CThing>* _M_finish;
    CIntelligentPointer<CThing>* _M_end_of_storage;
    CIntelligentPointer<CThing>* begin() const { return _M_start; }
    CIntelligentPointer<CThing>& operator[](long n) const { return *(begin() + n); }
};

class CAIBrain { public:
    virtual void v00();
    unsigned char _pad[0x14];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    CThing* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_HoverMovementBase : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTarget;            // +0x14
};
struct CAIStateGroup_HoverHornetQueen : CAIStateGroup_HoverMovementBase {
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
    bool bOneTime;                              // +0x34
    CThing* PEmitter;                           // +0x38
    CThing* PEmitter2;                          // +0x3c
    CIntelligentPointerVector Things;           // +0x40
    long CurrentCirclePoint;                    // +0x4c
    CIntelligentPointerVector CircleMarkers;    // +0x50
    long EnemyKnockDownFrame;                   // +0x5c
    bool TimingCritical;                        // +0x60
    long RouteDirection;                        // +0x64
    bool IsRouteToPosBlockedByTarget(long circlePoint);
};

bool CAIStateGroup_HoverHornetQueen::IsRouteToPosBlockedByTarget(long circlePoint)
{
    if (IsAlive(PTarget) && IsAlive(CircleMarkers[circlePoint])) {
        const CThing* creature = GetCreature();
        const CThing* target = PTarget.GetPItem();
        C3DVector toTarget = target->GetPos() - creature->GetPos();
        float distanceXY = toTarget.GetXYMagnitude();
        toTarget.Normalise();
        const CThing* me = GetCreature();
        const CThing* marker = CircleMarkers[circlePoint].GetPItem();
        C3DVector toMarker = marker->GetPos() - me->GetPos();
        toMarker.Normalise();
        if (Dot(toMarker, toTarget) > 0.5f && distanceXY < 3.0f)
            return true;
    }
    return false;
}