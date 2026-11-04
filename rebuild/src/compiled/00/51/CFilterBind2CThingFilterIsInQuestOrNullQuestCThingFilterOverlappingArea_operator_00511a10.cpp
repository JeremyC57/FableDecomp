// CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest,CThingFilter_OverlappingArea>::operator()(const CThing& thing) const -- the combined filter: the thing is in the quest or no quest and its ceil-rounded bounding circle overlaps the area
// Grade: RELOCATION_MATCH retry1 (retail_asm) -- the Bind2 filter out of line (by-reference filters as GetAllLeafThings 0x00516830 inlines them), landed engine CeilToLong asm helper, area grown in place
// Leaf: ??$?RVCThing@@
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail-asm: 0x00511A60 `fadd [ebp-0Ch]` + 0x00511A63 `fistp dword ptr [ebp-4]` -- the engine inline-assembly GFFloatToLongCeil(landed 0x00772720 / 0x006A68F0); VC7.1 C++ (long)x calls _ftol.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongCeil.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00511A10, length 250 bytes, `ret 4`, ebp frame. No code or data references (an
//   unreferenced out-of-line instantiation); the parked label is global::parked_00511a10.
// Identity: the out-of-line copy of the Bind2 filter that GetAllLeafThings 0x00516830 inlines (held by reference: this+0 -> the
//   quest filter, this+4 -> the area filter); CFilter_Bind2 is the Ego_r.pdb template.
// Callees: CThingFilter_IsInQuestOrNullQuest::operator() @ 0x006A7C60, CTCPhysicsBase::GetBoundingInfo(C3DVector&, float&) const
//   @ 0x006AF640, __ftol2 @ 0x00BFEA70 (six times).
class CThing {
public:
    unsigned char _pad_0x00[0x60];
    class CTCPhysicsBase* PhysicsTC;                  // +0x60
};

// ---- the overlapping-area filter (inline in retail) ----------------------------------------------------------------
#include "engine/C3DVector.h"
class C2DBoxI { public:
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(const C2DBoxI& o) : X0(o.X0), Y0(o.Y0), X1(o.X1), Y1(o.Y1) {}
};
class CTCPhysicsBase {
public:
    void GetBoundingInfo(C3DVector& centre, float& radius) const;   // 0x006AF640 (Ego_r.pdb: const, by reference)
};
// the engine's inline-assembly ceil helper (the landed CTCShop::GetMaxStock 0x00772720 / CThingPhysical::ModifyHealth 0x006A68F0 CeilToLong, verbatim)
#include "engine/GFFloatToLongCeil.h"
class CThingFilter_OverlappingArea {
public:
    const C2DBoxI* Area;                              // +0x00
    __forceinline bool operator()(const CThing& thing) const
    {
        const CTCPhysicsBase* physics = thing.PhysicsTC;
        if (!physics)
            return false;
        C3DVector centre;
        float radius;
        physics->GetBoundingInfo(centre, radius);
        radius = (float)GFFloatToLongCeil(radius);
        C2DBoxI area(*Area);
        area.X1 = (long)((float)area.X1 + radius);
        area.X0 = (long)((float)area.X0 - radius);
        area.Y0 = (long)((float)area.Y0 - radius);
        area.Y1 = (long)((float)area.Y1 + radius);
        long x = (long)centre.X;
        long y = (long)centre.Y;
        return x >= area.X0 && x < area.X1 && y >= area.Y0 && y < area.Y1;
    }
};

class CThingFilter_IsInQuestOrNullQuest {
public:
    const void* QuestSectionName;
    bool operator()(const CThing& thing) const;       // 0x006A7C60
};
template <class A, class B> class CFilter_Bind2 { public:     // both filters, held by reference
    const A& First;
    const B& Second;
    template <class T> bool operator()(const T& thing) const { return First(thing) && Second(thing); }
};


typedef CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea> CLeafFilter;
template bool CLeafFilter::operator()<CThing>(const CThing&) const;