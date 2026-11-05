// CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea>::operator()(const CThingBuilding& thing) -- both filters: in the active quest (or none) and overlapping the area
// Leaf: ??$?RVCThingBuilding@@@?$CFilter_Bind2@VCThingFilter_IsInQuestOrNullQuest@@VCThingFilter_OverlappingArea@@@@QBE_NABVCThingBuilding@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00511710, 252 bytes, thiscall, `ret 4`, ebp frame.
// Identity (probable): FableWin.pdb ??$?RVCThingBuilding@@@?$CFilter_Bind2@VCThingFilter_IsInQuestOrNullQuest@@VCThingFilter_OverlappingArea@@@@QBE_NABVCThingBuilding@@@Z;
//   the four CFilter_Bind2<IsInQuestOrNullQuest, OverlappingArea> instantiations (Ego_r.pdb / FableWin.pdb: CThingBuilding,
//   CThingObject, CThingVillage, CThingAICreature, in that FableWin.pdb order) are the unreferenced consecutive bodies
//   0x00511710 / 0x00511810 / 0x00511910 / 0x00511A10; this one is matched by position (bytes are type-independent).
// Callees: CThingFilter_IsInQuestOrNullQuest::operator()(const CThing&) const 0x006A7C60, CTCPhysicsBase::GetBoundingInfo
//   (C3DVector&, float&) const 0x006AF640, __ftol2 0x00BFEA70 (six times).
// Shape (family M104, representative 0x00511710): `return (*F1)(thing) && (*F2)(thing);` with CThingFilter_OverlappingArea's
//   body inline (the landed 0x0050EF30 model: the radius homed in the dead argument slot, rounded up in place, the box grown
//   by four `(long)` casts, the centre's two casts tested against the box).
// Retail-asm: 0x0051175D..0x00511777 `fld [ebp-10h] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-14h] ; mov eax,[ebp-10h] ; cmp eax,[ebp-14h] ; jne ; dec dword ptr [ebp-4]` -- the engine's inline-assembly ceiling helper expanded in place (a bare dword fistp, the bit compare and the memory decrement in one 10-instruction block; VC7.1 emits no bare dword fistp from C++), as in the landed 0x0050EF30.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongCeil.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
#pragma optimize("y", off)
#include "engine/C3DVector.h"
class C2DBoxI {
public:
    long X0, Y0, X1, Y1;
    C2DBoxI() { }
    C2DBoxI(const C2DBoxI& o) : X0(o.X0), Y0(o.Y0), X1(o.X1), Y1(o.Y1) { }
    void Grow(float amount)
    {
        X1 = (long)(X1 + amount);
        X0 = (long)(X0 - amount);
        Y0 = (long)(Y0 - amount);
        Y1 = (long)(Y1 + amount);
    }
};
class CTCPhysicsBase {
public:
    void GetBoundingInfo(C3DVector& centre, float& radius) const;   // 0x006AF640
};
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
class CThingBuilding : public CThing { };
#include "engine/GFFloatToLongCeil.h"

class CThingFilter_IsInQuestOrNullQuest {
public:
    bool operator()(const CThing& thing) const;           // 0x006A7C60
    CCharString QuestSectionName;
};
class CThingFilter_OverlappingArea {
public:
    __forceinline bool operator()(const CThing& thing) const
    {
        if (thing.GetPhysicsTC()) {
            float radius;
            C3DVector centre;
            thing.GetPhysicsTC()->GetBoundingInfo(centre, radius);
            radius = (float)GFFloatToLongCeil(radius);
            C2DBoxI box = *Area;
            box.Grow(radius);
            long x = (long)centre.X;
            long y = (long)centre.Y;
            return x >= box.X0 && x < box.X1 && y >= box.Y0 && y < box.Y1;
        }
        return false;
    }
    C2DBoxI* Area;                                        // +0x0
};
template <class TF1, class TF2> class CFilter_Bind2 {     // Ego_r.pdb: F1, F2 (pointers)
public:
    template <class T> bool operator()(const T& thing) const { return (*F1)(thing) && (*F2)(thing); }
    TF1* F1;                                              // +0x0
    TF2* F2;                                              // +0x4
};
template bool CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea>::operator()<CThingBuilding>(const CThingBuilding& thing) const;