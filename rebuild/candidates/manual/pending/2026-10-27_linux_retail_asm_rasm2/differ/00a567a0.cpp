// Grade: DIFFER(790v766) -- retail rounds the segment position with an inline-asm fistp helper (ebp frame, fistp/fild at [ebp-8], the x == i+1 bump), which C++ cannot emit (lane gap: inline-asm fistp rounding); the rest (fmod wrap, neighbour selection, Hermite 0x00A56670 __fastcall) is modelled. One structural attempt only.
// Retail-asm: 0x00A568C8..0x00A568E8 `fld [ebp - 0x18] ; fsub [ebp - 0x14] ; fistp [ebp - 8] ; fild [ebp - 8] ; fadd [ebp - 0x10] ; fstp [ebp - 0x1c] ; mov eax, [ebp - 0x18] ; cmp eax, [ebp - 0x1c] ; jne 0xa568e5 ; inc [ebp - 8] ; mov eax, [ebp - 8] ; cdq ` -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A567A0; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. The segment index is the floor block (hence the ebp frame).
#pragma optimize("y", off)
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(fmod)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
struct CPointVector {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    C3DVector& operator[](unsigned long i) const { return *(_M_start + i); }
};
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
__forceinline long RoundDown(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}

class CCardinalSpline {
public:
    CPointVector Points;                          // +0x00
    float        Tension;                         // +0x0c
    bool         Looped;                          // +0x10
    static C3DVector __fastcall Hermite(const C3DVector& p1, const C3DVector& m1, const C3DVector& p2,
                                        const C3DVector& m2, float s);   // 0x00A56670
    C3DVector GetSplinePosition(float t) const;
};

C3DVector CCardinalSpline::GetSplinePosition(float t) const
{
    long count = Points.size();
    if (count < 2) {
        if (count == 0)
            return C3DVector(0.0f, 0.0f, 0.0f);
        if (count == 1)
            return Points[0];
    }
    long segments;
    if (Looped) {
        if (t < 0.0f)
            t = 1.0f - (float)fmod(t, 1.0);
        else
            t = (float)fmod(t, 1.0);
        segments = count;
    } else {
        if (t <= 0.0f)
            return Points[0];
        if (t >= 1.0f)
            return Points[count - 1];
        segments = count - 1;
    }
    float fsegments = (float)segments;
    unsigned long index = RoundDown(fsegments * t) % count;
    unsigned long next = (index + 1) % count;
    C3DVector p1 = Points[index];
    C3DVector p2 = Points[next];
    C3DVector p0;
    if (index > 0)
        p0 = Points[index - 1];
    else if (Looped)
        p0 = Points[index + count - 1];
    else
        p0 = p1;
    C3DVector p3;
    if (next < (unsigned long)(count - 1))
        p3 = Points[next + 1];
    else if (Looped)
        p3 = Points[next - (count - 1)];
    else
        p3 = p2;
    if (Looped && index > next)
        next += count;
    float step = 1.0f / fsegments;
    float start = (float)index * step;
    C3DVector m1 = (p2 - p0) * Tension;
    C3DVector m2 = (p3 - p1) * Tension;
    return Hermite(p1, m1, p2, m2, (t - start) / ((float)next * step - start));
}
