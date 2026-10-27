// CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea>::operator()(const CThingVillage& thing) -- both filters: in the active quest (or none) and overlapping the area
// Leaf: ??$?RVCThingVillage@@@?$CFilter_Bind2@VCThingFilter_IsInQuestOrNullQuest@@VCThingFilter_OverlappingArea@@@@QBE_NABVCThingVillage@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00511910, 252 bytes, thiscall, `ret 4`, ebp frame.
// Identity (probable): FableWin.pdb ??$?RVCThingVillage@@@?$CFilter_Bind2@VCThingFilter_IsInQuestOrNullQuest@@VCThingFilter_OverlappingArea@@@@QBE_NABVCThingVillage@@@Z;
//   the four CFilter_Bind2<IsInQuestOrNullQuest, OverlappingArea> instantiations (Ego_r.pdb / FableWin.pdb: CThingBuilding,
//   CThingObject, CThingVillage, CThingAICreature, in that FableWin.pdb order) are the unreferenced consecutive bodies
//   0x00511710 / 0x00511810 / 0x00511910 / 0x00511A10; this one is matched by position (bytes are type-independent).
// Callees: CThingFilter_IsInQuestOrNullQuest::operator()(const CThing&) const 0x006A7C60, CTCPhysicsBase::GetBoundingInfo
//   (C3DVector&, float&) const 0x006AF640, __ftol2 0x00BFEA70 (six times).
// Shape (family M104, representative 0x00511710): `return (*F1)(thing) && (*F2)(thing);` with CThingFilter_OverlappingArea's
//   body inline (the landed 0x0050EF30 model: the radius homed in the dead argument slot, rounded up in place, the box grown
//   by four `(long)` casts, the centre's two casts tested against the box).
// Retail-asm: 0x0051195D..0x00511977 `fld [ebp-10h] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-14h] ; mov eax,[ebp-10h] ; cmp eax,[ebp-14h] ; jne ; dec dword ptr [ebp-4]` -- the engine's inline-assembly ceiling helper expanded in place (a bare dword fistp, the bit compare and the memory decrement in one 10-instruction block; VC7.1 emits no bare dword fistp from C++), as in the landed 0x0050EF30.
#pragma optimize("y", off)
class C3DVector { public: float X, Y, Z; };
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
class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                            // +0x60
};
class CThingVillage : public CThing { };
__forceinline long CeilToLong(float value)
{
    long result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}
class CCharString { public: void* PStringData; };
class CThingFilter_IsInQuestOrNullQuest {
public:
    bool operator()(const CThing& thing) const;           // 0x006A7C60
    CCharString QuestSectionName;
};
class CThingFilter_OverlappingArea {
public:
    __forceinline bool operator()(const CThing& thing) const
    {
        if (thing.PhysicsTC) {
            float radius;
            C3DVector centre;
            thing.PhysicsTC->GetBoundingInfo(centre, radius);
            radius = (float)CeilToLong(radius);
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
template bool CFilter_Bind2<CThingFilter_IsInQuestOrNullQuest, CThingFilter_OverlappingArea>::operator()<CThingVillage>(const CThingVillage& thing) const;