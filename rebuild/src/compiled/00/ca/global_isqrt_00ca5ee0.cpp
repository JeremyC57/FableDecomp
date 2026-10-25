// isqrt(int x) -- DirectShow BaseClasses renbase.cpp: cheap integer square root for the IQualProp statistics (doubling start guess, three Newton-Raphson steps)
// Leaf: ?isqrt@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA5EE0, 101 bytes (+11 int3). Callee-pops `ret 4`: renbase.cpp declares a plain
//   `int isqrt(int x)`, and strmbase is built with the BaseClasses project's default __stdcall (/Gz), so the free function is
//   `__stdcall` (written explicitly here; the mangled name `?isqrt@@YGHH@Z` is the /Gz one).
// Caller: CBaseVideoRenderer::GetStdDev 0x00CA5FEF (`*piResult = isqrt((LONG)x)`).
// Shape: renbase.cpp verbatim; `while (s*s<x)` is guarded by the `x > 1` entry test, the three NR steps share one register
//   chain and the `s>=0` tests jump to the common epilogue.
// Label: manifest "Math_IntegerSquareRoot" is a descriptive placeholder; the real name is the renbase.cpp function.
int __stdcall isqrt(int x)
{
    int s = 1;
    // Make s an initial guess for sqrt(x)
    if (x > 0x40000000) {
        s = 0x8000;     // prevent any conceivable closed loop
    } else {
        while (s * s < x) {    // loop cannot possible go more than 31 times
            s = 2 * s;       // normally it goes about 6 times
        }
        // Three NR iterations.
        if (x == 0) {
            s = 0; // Wouldn't it be tragic to divide by zero whenever our
                   // accuracy was perfect!
        } else {
            s = (s * s + x) / (2 * s);
            if (s >= 0) s = (s * s + x) / (2 * s);
            if (s >= 0) s = (s * s + x) / (2 * s);
        }
    }
    return s;
}