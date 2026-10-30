// CFilter_Bind3<CThingFilter_OverlappingArea,CThingFilter_HasNavigationInformation,CThingFilter_IsInQuestOrNullQuest>::operator()(const CThing& thing) const -- the combined filter: the thing's ceil-rounded bounding circle overlaps the area, it has navigation information (virtual slot 55, reached through either arm of the physics slot-42 test) and it is in the quest or no quest
// Grade: RELOCATION_MATCH retry1 (retail_asm) -- the Bind3 filter out of line (by-reference filters as GetAllThings 0x00516200 inlines them), landed engine CeilToLong asm helper, area grown in place
// Leaf: ??$?RVCThing@@
// Retail-asm: 0x00511620 `fadd [ebp-10h]` + 0x00511623 `fistp dword ptr [ebp-8]` -- the engine inline-assembly CeilToLong (landed 0x00772720 / 0x006A68F0); VC7.1 C++ (long)x calls _ftol.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005115E0, length 299 bytes (+ int3 padding), `ret 4`, ebp frame. No code or data
//   references (an unreferenced out-of-line instantiation); the parked label is global::parked_005115e0.
// Identity: the out-of-line copy of the Bind3 filter that GetAllThings 0x00516200 inlines (same three filters in the same order,
//   held by reference: this+0 -> the area filter, this+8 -> the quest filter); CFilter_Bind3 is the Ego_r.pdb template.
// Callees: CTCPhysicsBase::GetBoundingInfo(C3DVector&, float&) const @ 0x006AF640, CThingFilter_IsInQuestOrNullQuest::operator()
//   @ 0x006A7C60, __ftol2 @ 0x00BFEA70 (six times).
class CThing {
public:
    unsigned char _pad_0x00[0x60];
    class CTCPhysicsBase* PhysicsTC;                  // +0x60
};

// ---- the overlapping-area filter (inline in retail) ----------------------------------------------------------------
class C3DVector { public: float X, Y, Z; };
class C2DBoxI { public:
    long X0, Y0, X1, Y1;
    C2DBoxI() {}
    C2DBoxI(const C2DBoxI& o) : X0(o.X0), Y0(o.Y0), X1(o.X1), Y1(o.Y1) {}
};
#define VSLOTS10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CTCPhysicsBase {
public:
    VSLOTS10(0) VSLOTS10(1) VSLOTS10(2) VSLOTS10(3)
    virtual void v40(); virtual void v41();
    virtual bool IsNavigationObstacle() const;        // slot 42 (+0xa8)
    void GetBoundingInfo(C3DVector& centre, float& radius) const;   // 0x006AF640 (Ego_r.pdb: const, by reference)
};
// the engine's inline-assembly ceil helper (the landed CTCShop::GetMaxStock 0x00772720 / CThingPhysical::ModifyHealth 0x006A68F0 CeilToLong, verbatim)
__forceinline long CeilToLong(float value)
{
    long  result;
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
        radius = (float)CeilToLong(radius);
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
// The thing's navigation query (virtual slot 55, +0xdc) reached through two inline paths (physics obstacle or not)
// that both end in the same virtual call, as in retail.
struct CThingVirtuals {
    VSLOTS10(0) VSLOTS10(1) VSLOTS10(2) VSLOTS10(3) VSLOTS10(4)
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual bool HasNavigationInformation() const;    // slot 55 (+0xdc)
};
class CThingFilter_HasNavigationInformation {
public:
    __forceinline bool operator()(const CThing& thing) const
    {
        const CTCPhysicsBase* physics = thing.PhysicsTC;
        const CThingVirtuals& v = *(const CThingVirtuals*)&thing;
        if (physics && physics->IsNavigationObstacle())
            return v.HasNavigationInformation();
        return v.HasNavigationInformation();
    }
};
class CThingFilter_IsInQuestOrNullQuest {
public:
    const void* QuestSectionName;
    bool operator()(const CThing& thing) const;       // 0x006A7C60
};
template <class A, class B, class C> class CFilter_Bind3 { public:   // the three filters, held by reference
    const A& First;
    const B& Second;
    const C& Third;
    template <class T> bool operator()(const T& thing) const { return First(thing) && Second(thing) && Third(thing); }
};


typedef CFilter_Bind3<CThingFilter_OverlappingArea, CThingFilter_HasNavigationInformation, CThingFilter_IsInQuestOrNullQuest> CNavFilter;
template bool CNavFilter::operator()<CThing>(const CThing&) const;