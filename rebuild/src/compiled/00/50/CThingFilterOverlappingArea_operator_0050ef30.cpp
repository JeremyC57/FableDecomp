// CThingFilter_OverlappingArea::operator()(CThing* thing) const -- true when the thing's bounding centre lies in the filter area grown by the thing's radius (rounded up)
// Leaf: operator()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0050EF30, length 242 bytes, `ret 4`, ebp frame.
// Callees: CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*) @ 0x006AF640, __ftol2 @ 0x00BFEA70 (six times).
// Shape: the radius out-value (homed in the dead `thing` argument slot) is rounded up in place by the engine's inline-asm
// ceiling helper and, being address-taken, is read back as a memory operand by the four box-edge conversions; the six `(long)`
// casts are __ftol2 calls.
// Retail-asm: 0x0050EF6A..0x0050EF84 `fld [ebp-10h] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-14h] ; mov eax,[ebp-10h] ; cmp eax,[ebp-14h] ; jne ; dec dword ptr [ebp-4]` -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; dec dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
#pragma optimize("y", off)
class C3DVector { public: float X, Y, Z; };
class C2DBoxI { public:
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
    void GetBoundingInfo(C3DVector* centre, float* radius);    // 0x006AF640
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                                 // +0x60
};
class CThingFilter_OverlappingArea {
public:
    const C2DBoxI* Area;                                       // +0x00
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
bool CThingFilter_OverlappingArea::operator()(CThing* thing) const
{
    if (thing->PhysicsTC) {
        float radius;
        C3DVector centre;
        thing->PhysicsTC->GetBoundingInfo(&centre, &radius);
        radius = (float)CeilToLong(radius);          // rounded up in place: the address-taken out-value stays a memory operand
        C2DBoxI box = *Area;
        box.Grow(radius);
        long x = (long)centre.X;
        long y = (long)centre.Y;
        return x >= box.X0 && x < box.X1 && y >= box.Y0 && y < box.Y1;
    }
    return false;
}