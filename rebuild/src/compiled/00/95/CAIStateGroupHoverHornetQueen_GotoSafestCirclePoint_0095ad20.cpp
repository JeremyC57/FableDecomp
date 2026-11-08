// CAIStateGroup_HoverHornetQueen::GotoSafestCirclePoint(long movementType) -- with no circle markers return 3; otherwise compare the direction to the target with the directions to the next and previous circle markers and fly to the one facing away from the target
// Leaf: ?GotoSafestCirclePoint@CAIStateGroup_HoverHornetQueen@@QAEJJ@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local Normalise replaced by the header Normalise (same computation); local free operator- removed (the header member operator- is the same inline member-wise computation).
// Naming wave 18: g_InvSqrtMantissaTable is GFastOneOverSqrtTable (?GFastOneOverSqrtTable@@3PAKA); ChatGPT pass 10 audit, raw Ego_r public.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095AD20, 754 bytes, `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPos @ 0x004C73D0, GotoCirclePoint @ 0x0095A6C0;
//          GFastOneOverSqrtTable @ 0x013CE558 (the inline fast inverse square root's mantissa table).
// Layout: retail STLport vectors are 12 bytes, so CurrentCirclePoint is +0x4c and CircleMarkers +0x50 (the PDB header says +0x50/+0x54).
// Shape: three inline fast normalises (`*this *= FastInvSqrt(len)` through a const float& operator*=); the circle-marker
//        element is read through the vector's inline `*(begin() + i)` accessor (a plain `_M_start[i]` colours the begin
//        reload and the next normalise's bit temporaries edx/ecx and hoists the index load above the creature load).
#include "fable_compiler.h"
extern unsigned long GFastOneOverSqrtTable[128];   // 0x013CE558
FABLE_FORCEINLINE float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
class CThing;
FABLE_FORCEINLINE C3DVector Direction(const CThing* to, const CThing* from);
FABLE_FORCEINLINE float Dot(const C3DVector& a, const C3DVector& b) { return a.Z * b.Z + a.Y * b.Y + a.X * b.X; }
#include "engine/CThing.h"
class CThingCreatureBase : public CThing { public: };
FABLE_FORCEINLINE C3DVector Direction(const CThing* to, const CThing* from) { return to->GetPos() - from->GetPos(); }
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

template <class T> struct CVec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T& operator[](long i) { return *(begin() + i); }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_HoverMovementBase : CAIStateGroupBase { };
struct CAIStateGroup_HoverHornetQueen : CAIStateGroup_HoverMovementBase {
    CIntelligentPointer<CThing> PTarget;            // +0x14
    unsigned char _pad1c[0x40 - 0x1c];
    CVec<CIntelligentPointer<CThing> > Things;       // +0x40
    long CurrentCirclePoint;                    // +0x4c
    CVec<CIntelligentPointer<CThing> > CircleMarkers;   // +0x50
    long GetNextCirclePoint() { long i = CurrentCirclePoint + 1; if (i == CircleMarkers.size()) i = 0; return i; }
    long GetPrevCirclePoint() { long i = CurrentCirclePoint - 1; if (i == -1) i = CircleMarkers.size() - 1; return i; }
    long GotoCirclePoint(long index, long movementType);   // 0x0095A6C0
    long GotoSafestCirclePoint(long movementType);
};

long CAIStateGroup_HoverHornetQueen::GotoSafestCirclePoint(long movementType)
{
    if (CircleMarkers.size() == 0)
        return 3;
    C3DVector toTarget = Direction(PTarget.Get(), GetCreature());
    toTarget.Normalise();
    C3DVector toNext = Direction(CircleMarkers[GetNextCirclePoint()].Get(), GetCreature());
    toNext.Normalise();
    C3DVector toPrev = Direction(CircleMarkers[GetPrevCirclePoint()].Get(), GetCreature());
    toPrev.Normalise();
    if (Dot(toNext, toTarget) > Dot(toPrev, toTarget))
        return GotoCirclePoint(GetPrevCirclePoint(), movementType);
    return GotoCirclePoint(GetNextCirclePoint(), movementType);
}