// CThingFilter_OverlappingArea::operator()(CThing* thing) const -- true when the thing's bounding centre lies in the filter area grown by the thing's radius (rounded up)
// Leaf: operator()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005115E0, length 299 bytes (+ int3 padding), `ret 4`, ebp frame.
// (A combined-filter instantiation: the area test, then the thing's slot-55 predicate (reached through either arm of the
// physics slot-42 test) and the second filter @ 0x006A7C60 on this+8.)
// Callees: CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*) @ 0x006AF640, __ftol2 @ 0x00BFEA70 (six times).
// Shape: the radius out-value (homed in the dead `thing` argument slot) is rounded up in place by the engine's inline-asm
// ceiling helper and, being address-taken, is read back as a memory operand by the four box-edge conversions; the six `(long)`
// casts are __ftol2 calls.
// Retail-asm: 0x0051161D..0x00511637 `fld [ebp-14h] ; fadd [ebp-10h] ; fistp dword ptr [ebp-8] ; fild [ebp-8] ; fsub [ebp-0Ch] ; fstp [ebp-18h] ; mov eax,[ebp-14h] ; cmp eax,[ebp-18h] ; jne ; dec dword ptr [ebp-8]` -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; dec dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
#pragma optimize("y", off)
struct C3DVector { float X, Y, Z; };
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI() { }
    C2DBoxI(const C2DBoxI& o) : X0(o.X0), Y0(o.Y0), X1(o.X1), Y1(o.Y1) { }
    // grow the box by a float amount on every side (the four __ftol2 calls are the `(long)` casts)
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
    bool OverlapsArea(CThing* thing) const;
    bool operator()(CThing* thing) const;
};

// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
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
// the area test of the plain instantiation 0x0050EF30, expanded in place as one term of the filter's && chain
__forceinline bool CThingFilter_OverlappingArea::OverlapsArea(CThing* thing) const
{
    float radius;
    C3DVector centre;
    thing->PhysicsTC->GetBoundingInfo(&centre, &radius);
    radius = (float)CeilToLong(radius);              // rounded up in place: the address-taken out-value stays a memory operand
    C2DBoxI box = *Area;
    box.Grow(radius);
    long x = (long)centre.X;
    long y = (long)centre.Y;
    return x >= box.X0 && x < box.X1 && y >= box.Y0 && y < box.Y1;
}
bool CThingFilter_OverlappingArea::operator()(CThing* thing) const
{
    return thing->PhysicsTC && OverlapsArea(thing)
        && (thing->PhysicsTC && thing->PhysicsTC->HasPhysicsMeshObject() ? thing->IsSelectable() : thing->IsSelectable())
        && (*Next)(thing);
}
