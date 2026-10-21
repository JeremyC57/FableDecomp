// Grade: DIFFER(303v299) -- lane gap: the radius is rounded up by a bare dword fistp ceil helper in an ebp frame (inline asm in the original, asm_bake family); plain C++ goes through ceil/__ftol2
// CThingFilter_OverlappingArea::operator()(CThing* thing) const -- true when the thing's bounding centre lies in the filter area grown by the thing's radius (rounded up)
// Leaf: operator()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005115E0, length 299 bytes (+ int3 padding), `ret 4`, ebp frame.
// (A combined-filter instantiation: the area test, then the thing's slot-55 predicate (reached through either arm of the
// physics slot-42 test) and the second filter @ 0x006A7C60 on this+8.)
// Callees: CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*) @ 0x006AF640, __ftol2 @ 0x00BFEA70 (six times).
// Shape: the radius is rounded up by a bare `fistp` helper (fld r ; fadd 0.5 ; fistp ; fild ; fsub 1.0 ; compare the
// float bits ; dec) -- an inline-asm CeilToInt in the original (asm_bake family, like 0x0052F440): the ebp frame and the
// dword fistp cannot come from plain C++. This source states the logic with a C ceil.
#include <math.h>
struct C3DVector { float X, Y, Z; };
struct C2DBoxI { long X0, Y0, X1, Y1; };
class CTCPhysicsBase {
public:
    virtual void p00(); virtual void p01(); virtual void p02(); virtual void p03(); virtual void p04(); virtual void p05();
    virtual void p06(); virtual void p07(); virtual void p08(); virtual void p09(); virtual void p10(); virtual void p11();
    virtual void p12(); virtual void p13(); virtual void p14(); virtual void p15(); virtual void p16(); virtual void p17();
    virtual void p18(); virtual void p19(); virtual void p20(); virtual void p21(); virtual void p22(); virtual void p23();
    virtual void p24(); virtual void p25(); virtual void p26(); virtual void p27(); virtual void p28(); virtual void p29();
    virtual void p30(); virtual void p31(); virtual void p32(); virtual void p33(); virtual void p34(); virtual void p35();
    virtual void p36(); virtual void p37(); virtual void p38(); virtual void p39(); virtual void p40(); virtual void p41();
    virtual bool HasPhysicsMeshObject();                       // slot 42 (+0xa8)
    void GetBoundingInfo(C3DVector* centre, float* radius);    // 0x006AF640
};
class CThing;
class CThingFilter {
public:
    bool operator()(CThing* thing) const;                      // 0x006A7C60
};
class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54();
    virtual bool IsSelectable();                               // slot 55 (+0xdc)
    unsigned char   _pad04[0x60 - 0x04];
    CTCPhysicsBase* PhysicsTC;                                 // +0x60
};
class CThingFilter_OverlappingArea {
public:
    const C2DBoxI* Area;                                       // +0x00
    unsigned char  _pad04[4];
    CThingFilter*  Next;                                       // +0x08
    bool operator()(CThing* thing) const;
};

bool CThingFilter_OverlappingArea::operator()(CThing* thing) const
{
    if (!thing->PhysicsTC)
        return false;
    C3DVector centre;
    float radius;
    thing->PhysicsTC->GetBoundingInfo(&centre, &radius);
    float grow = (float)(long)ceil(radius);
    C2DBoxI box = *Area;
    long maxX = (long)(box.X1 + grow);
    long minX = (long)(box.X0 - grow);
    long minY = (long)(box.Y0 - grow);
    long maxY = (long)(box.Y1 + grow);
    long x = (long)centre.X;
    long y = (long)centre.Y;
    if (!(x >= minX && x < maxX && y >= minY && y < maxY))
        return false;
    bool selectable = thing->PhysicsTC && thing->PhysicsTC->HasPhysicsMeshObject() ? thing->IsSelectable() : thing->IsSelectable();
    return selectable && (*Next)(thing);
}
