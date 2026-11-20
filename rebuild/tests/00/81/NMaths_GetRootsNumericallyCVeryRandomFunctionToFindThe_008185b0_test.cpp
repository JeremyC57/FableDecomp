// Behaviour test for 0x008185B0 NMaths::GetRootsNumerically<CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint>.
// The row is a template instance over an anonymous-namespace class: its symbol carries a per-TU hash of the row's file path
// (`?A0x........`, measured: it changes with the directory and the file name), so no other TU can name it, and a plain link
// drops the unreferenced COMDAT. Two modes, told apart at run time and printed:
//   REAL OBJECT (the row object linked with /OPT:NOREF: tools/trun.py / tools/mut2.py with LINK_EXTRA=/OPT:NOREF): the function is
//     found in the test's own image as the code that refers to DELTA_NOTIONAL_ZERO (only the row object does; its first reference
//     is in the first 16 bytes, 0x008185BB `fsub dword ptr [DELTA_NOTIONAL_ZERO]`) and called through a pointer with the row's
//     calling convention (/Gr: result vector in ecx, the functor in edx, the two floats and two longs on the stack, `ret 10h`).
//     Every case is compared float for float with the reference model.
//   MODEL ONLY (a plain link, as the landing verifier does, like the landed 0x0087AA10 / 0x0087AFF0 / 0x0078B7C0 tests): the
//     reference model is checked against the mathematics (each root brackets a sign change within the accuracy).
// Fixtures: the functor is three floats Ca, Cb, Cc (value = Ca*cos + Cb*sin + Cc*cos*sin of 2*pi*x, TWO_PI 0x0124F2B8); the result
//   is an STLport vector<float> (start, finish, end of storage; malloc / free). DELTA_NOTIONAL_ZERO 0x0129BA3C = 0.0001f.
// Reference model (independent; rounding points read from the retail code): the range is widened by DELTA at both ends
//   (mod start a float, the width a float); steps = roots + asymptotes, then one more a round until that many sign changes
//   (`(a >= 0 && b <= 0) || (a <= 0 && b >= 0)`) are bracketed by consecutive float points start, start + step, ...; each bracket
//   is bisected while end - start > 0.001f (the midpoint exact, stored as a float): the midpoint replaces the end with the same
//   sign side as the first value (`<= 0` / `>= 0`), and that side's flag becomes "the new value is not more than DELTA larger in
//   magnitude than the side's previous one"; a root (the bracket's start) is kept when both flags are set; last, roots outside
//   [range_start, range_end) are dropped. A case is compared only when no decision of the model is within rounding distance of
//   flipping (function values, flag comparisons and the range tests are checked for margin); the others are counted as skipped.
// Checks: hand-written anchors (one and two roots a turn, four with the product term, a root just under range_start that the
//   widened range finds and the last loop drops, asymptote counts, the growing step count, vector growth past 1, 2 and 4 roots)
//   twelve cases found by a script copy of the model that each tell one flag decision apart, and 4000 seeded random cases; the input functor is not written; the vector's storage is exact (finish - start = roots).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
extern "C" {
__declspec(dllimport) void* __stdcall GetModuleHandleA(const char* name);
__declspec(dllimport) void* __stdcall CreateThread(void* attributes, unsigned long stack, unsigned long (__stdcall* start)(void*), void* param, unsigned long flags, unsigned long* id);
__declspec(dllimport) void __stdcall Sleep(unsigned long ms);
__declspec(dllimport) void __stdcall ExitProcess(unsigned int code);
__declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(long (__stdcall* filter)(void*));
}

extern const float DELTA_NOTIONAL_ZERO = 0.0001f;
static const float* g_delta_ptr = &DELTA_NOTIONAL_ZERO;                 // read from data: the test's code never names the constant
static const double kDelta = (double)0.0001f;
static const double kTwoPi = 6.283185482025146484375;

struct Functor { float Ca, Cb, Cc; };
struct VecF { float* start; float* finish; float* eos; };
typedef VecF* (__fastcall *RowFn)(VecF* result, const Functor* func, float range_start, float range_end, long num_roots, long num_asymptotes);

static int g_fail, g_cases, g_compared, g_skipped;
static void Fail(const char* what, double got, double want)
{
    if (g_fail++ < 20) printf("FAIL case %d: %s got %.9g want %.9g\n", g_cases, what, got, want);
}
static unsigned long g_seed = 90210;
static unsigned long Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }
static float RndF(long range, float div) { return (float)((long)(Rnd() % (unsigned long)(2 * range + 1)) - range) / div; }

static double Value(const Functor& f, double x)
{
    double a = x * kTwoPi; double c = cos(a), s = sin(a);
    return (double)f.Ca * c + (double)f.Cb * s + (double)f.Cc * c * s;
}

enum { MAXR = 64 };
struct Model { int n; float root[MAXR]; bool robust; long steps; };
static const double kMargin = 2e-6;
static void Near(Model& m, double v) { if (fabs(v) < kMargin) m.robust = false; }

static void RunModel(const Functor& f, float range_start, float range_end, long num_roots, long num_asymptotes, Model& m)
{
    m.n = 0; m.robust = true;
    float mod_start = (float)((double)range_start - kDelta);
    float width = (float)((kDelta + (double)range_end) - (double)mod_start);
    static float bs[4096], be[4096];
    long steps = num_roots + num_asymptotes - 1, found; int nb;
    do
    {
        ++steps; nb = 0; found = 0;
        float step = (float)((double)width / (double)steps);
        float a = mod_start; float b = (float)((double)step + (double)a);
        double va = Value(f, a); Near(m, va);
        for (long c = 0; c != steps; ++c)
        {
            double vb = Value(f, b); Near(m, vb);
            if ((va >= 0.0 && vb <= 0.0) || (va <= 0.0 && vb >= 0.0)) { ++found; if (nb < 4096) { bs[nb] = a; be[nb] = b; ++nb; } }
            va = vb; a = b; b = (float)((double)b + (double)step);
        }
        if (steps > 400) { m.robust = false; m.n = -1; break; }           // not asked of the row: the caller keeps the count reachable
    } while (found < num_roots + num_asymptotes);
    m.steps = steps;
    float kept[MAXR * 4]; int nk = 0;
    for (int k = 0; k < nb; ++k)
    {
        float a = bs[k], b = be[k];
        double va = Value(f, a), vb = Value(f, b);
        bool fa = true, fb = true; double sa = va, sb = vb;
        while ((double)b - (double)a > (double)0.001f)
        {
            double mid = ((double)a + (double)b) * 0.5;
            vb = Value(f, mid); Near(m, vb);
            bool same = va <= 0.0 ? vb <= 0.0 : vb >= 0.0;
            if (same)
            {
                va = vb; a = (float)mid;
                double d = fabs(va) - (fabs(sa) + kDelta); Near(m, d);
                fa = !(d > 0.0); sa = va;
            }
            else
            {
                b = (float)mid;
                double d = fabs(vb) - (fabs(sb) + kDelta); Near(m, d);
                fb = !(d > 0.0); sb = vb;
            }
        }
        if (fa && fb && nk < MAXR * 4) kept[nk++] = a;
    }
    for (int k = 0; k < nk; ++k)
    {
        float r = kept[k];
        if (fabs((double)r - (double)range_start) < 1e-7 || fabs((double)r - (double)range_end) < 1e-7) m.robust = false;
        if (r >= range_start && r < range_end) { if (m.n < MAXR) m.root[m.n++] = r; else m.robust = false; }
    }
}

// the number of sign changes of the function over the widened range (fine sampling), so that a case never asks for more
static int SignChanges(const Functor& f, float range_start, float range_end)
{
    double lo = (double)range_start - kDelta, hi = (double)range_end + kDelta; int n = 0;
    double prev = Value(f, lo);
    for (int i = 1; i <= 6000; ++i)
    {
        double v = Value(f, lo + (hi - lo) * i / 6000.0);
        if ((prev < 0.0) != (v < 0.0)) ++n;
        prev = v;
    }
    return n;
}

static RowFn g_row;
static void Run(const Functor& f, float range_start, float range_end, long num_roots, long num_asymptotes)
{
    ++g_cases;
    static Model m; RunModel(f, range_start, range_end, num_roots, num_asymptotes, m);
    // the model against the mathematics: every kept root starts a bracket no wider than the accuracy that holds a sign change
    if (m.robust)
        for (int k = 0; k < m.n; ++k)
        {
            double a = m.root[k], va = Value(f, a), vb = Value(f, a + 0.00101);
            if (!((va <= 0.0 && vb >= 0.0) || (va >= 0.0 && vb <= 0.0))) Fail("model root does not bracket a sign change", a, 0);
            if (k > 0 && !(m.root[k] > m.root[k - 1])) Fail("model roots not ascending", m.root[k], m.root[k - 1]);
        }
    if (!g_row) return;
    if (!m.robust || m.n < 0) { ++g_skipped; return; }
    ++g_compared;
    Functor copy = f; VecF v; v.start = (float*)0x11111111; v.finish = (float*)0x22222222; v.eos = (float*)0x33333333;
    VecF* r = g_row(&v, &copy, range_start, range_end, num_roots, num_asymptotes);
    if (r != &v) Fail("returned pointer", (double)(unsigned long)r, (double)(unsigned long)&v);
    if (memcmp(&copy, &f, sizeof f)) Fail("functor written", 1, 0);
    int n = (int)(v.finish - v.start);
    if (n != m.n) Fail("root count", n, m.n);
    else
        for (int k = 0; k < n; ++k)
            if (memcmp(&v.start[k], &m.root[k], 4)) Fail("root value", v.start[k], m.root[k]);
    if (n > 0 && v.eos < v.finish) Fail("end of storage before finish", 0, 1);
    if (v.start) free(v.start);
}

static RowFn FindRow()
{
    unsigned char* base = (unsigned char*)GetModuleHandleA(0);
    unsigned long lfanew; memcpy(&lfanew, base + 0x3C, 4);
    unsigned char* nt = base + lfanew;
    unsigned short nsec, optsize; memcpy(&nsec, nt + 6, 2); memcpy(&optsize, nt + 0x14, 2);
    unsigned char* sec = nt + 0x18 + optsize;                              // section headers, 40 bytes each
    unsigned long want = (unsigned long)g_delta_ptr; unsigned char* first = 0; unsigned char* last = 0; int refs = 0;
    for (int i = 0; i < nsec; ++i, sec += 40)
    {
        unsigned long vsize, va, flags; memcpy(&vsize, sec + 8, 4); memcpy(&va, sec + 0xC, 4); memcpy(&flags, sec + 0x24, 4);
        if (!(flags & 0x20000000)) continue;                               // executable sections only
        unsigned char* p = base + va;
        for (unsigned long k = 0; k + 4 <= vsize; ++k)
        {
            unsigned long v; memcpy(&v, p + k, 4);
            if (v == want) { if (!first) first = p + k; last = p + k; ++refs; }
        }
    }
    if (!first) return 0;
    unsigned char* start = (unsigned char*)((unsigned long)first & ~15ul);
    if (refs < 2 || last - start > 0x800) { printf("unexpected references to DELTA_NOTIONAL_ZERO (%d, span %ld)\n", refs, (long)(last - start)); ++g_fail; return 0; }
    return (RowFn)start;
}

static unsigned long __stdcall Watchdog(void*) { Sleep(25000); printf("008185b0_TEST FAIL (timeout)\n"); fflush(stdout); ExitProcess(3); return 0; }
static long __stdcall Crash(void*) { printf("008185b0_TEST FAIL (exception)\n"); fflush(stdout); ExitProcess(4); return 0; }

static Functor Make(float a, float b, float c) { Functor f; f.Ca = a; f.Cb = b; f.Cc = c; return f; }

static void Anchors()
{
    // sin shifted by an eighth of a turn: roots at 0.375 and 0.875 a turn
    Functor f = Make(0.70710678f, 0.70710678f, 0.0f);
    Run(f, 0.0f, 1.0f, 2, 0); Run(f, 0.0f, 1.0f, 1, 0); Run(f, 0.0f, 1.0f, 1, 1); Run(f, 0.0f, 1.0f, 0, 2); Run(f, 0.0f, 1.0f, 0, 1);
    Run(f, 0.0f, 0.5f, 1, 0); Run(f, 0.5f, 1.0f, 1, 0); Run(f, 0.2f, 0.4f, 1, 0); Run(f, -1.0f, 2.0f, 6, 0); Run(f, -1.0f, 2.0f, 3, 2);
    // a root 0.00005 under range_start: inside the widened range, so it is bracketed and refined, then dropped by the last loop
    Run(f, 0.37505f, 1.0f, 2, 0); Run(f, 0.37505f, 0.9f, 2, 0); Run(f, 0.37495f, 1.0f, 2, 0);
    // a root just over / under range_end
    Run(f, 0.0f, 0.87495f, 2, 0); Run(f, 0.0f, 0.87505f, 2, 0); Run(f, 0.0f, 0.8751f, 2, 0);
    // with the product term: four roots a turn
    f = Make(0.3f, -0.2f, 1.0f);
    Run(f, 0.0f, 1.0f, 4, 0); Run(f, 0.0f, 1.0f, 2, 2); Run(f, 0.0f, 1.0f, 3, 0); Run(f, 0.0f, 1.0f, 1, 0); Run(f, 0.0f, 3.0f, 12, 0);
    Run(f, 0.0f, 3.0f, 9, 0); Run(f, 0.0f, 3.0f, 5, 0); Run(f, -2.0f, 2.0f, 16, 0); Run(f, -2.0f, 2.0f, 8, 8);
    f = Make(-1.0f, 0.25f, 0.0f); Run(f, 0.0f, 1.0f, 2, 0); Run(f, 0.1f, 0.6f, 1, 0); Run(f, 0.1f, 0.6f, 0, 1);
    f = Make(0.0f, 0.0f, 1.0f); Run(f, 0.05f, 0.95f, 3, 0);                                   // cos*sin alone: roots every quarter turn
    // cases found with a script copy of the model (tmp/search), each the only kind that tells one decision apart:
    // the second value of a bracket seeds the end side's flag (a start-side value there drops one of nine roots)
    Run(Make(-0.19479522109031677f, 1.183443546295166f, 2.042926073074341f), 1.2320369482040405f, 3.664968490600586f, 9, 0);
    // the first value seeds the start side's flag
    Run(Make(0.8955506682395935f, -0.14817024767398834f, 4.969690322875977f), -0.5204430222511292f, 1.6877846717834473f, 8, 0);
    Run(Make(-0.7083141207695007f, 1.7687568664550781f, -2.4045968055725098f), 0.7151552438735962f, 2.87939453125f, 1, 0);
    // small amplitudes: successive values differ by less than DELTA, so the sign of DELTA in the flag test decides
    Run(Make(-0.0032344383653253317f, 0.0016274354420602322f, 0.0042548212222754955f), 0.24102911353111267f, 1.946353793144226f, 1, 0);
    Run(Make(0.0010078601771965623f, 0.015005500055849552f, 0.0f), 0.9177811741828918f, 1.6383376121520996f, 1, 0);
    Run(Make(0.010582834482192993f, 0.002921037608757615f, 0.022528668865561485f), -0.7450099587440491f, 0.9935331344604492f, 8, 0);
    // one coarse bracket over several roots: the end side's last value grows, its flag is clear and the root is dropped
    Run(Make(0.1787165403366089f, -0.9419344663619995f, 0.0f), -1.9682754278182983f, 0.02731071598827839f, 1, 0);
    // the mirror image: the start side's flag is clear
    Run(Make(0.1787165403366089f, 0.9419344663619995f, 0.0f), -0.029084112495183945f, 1.9651457071304321f, 1, 0);
    Run(Make(0.1787165403366089f, 0.9419344663619995f, 0.0f), -0.029605001211166382f, 1.9671510457992554f, 1, 0);
    // the same three with the signs turned (the `val_a <= 0` arm of the bisection)
    Run(Make(-0.1787165403366089f, 0.9419344663619995f, 0.0f), -1.9682754278182983f, 0.02731071598827839f, 1, 0);
    Run(Make(-0.1787165403366089f, -0.9419344663619995f, 0.0f), -0.029084112495183945f, 1.9651457071304321f, 1, 0);
    Run(Make(-0.1787165403366089f, -0.9419344663619995f, 0.0f), -0.029605001211166382f, 1.9671510457992554f, 1, 0);
}

static void RandomCases(int n)
{
    for (int c = 0; c < n; ++c)
    {
        float amp = (c % 5 == 4) ? 100000.0f : 1000.0f;                      // every fifth case: amplitudes near DELTA
        Functor f = Make(RndF(2000, amp), RndF(2000, amp), (Rnd() % 3) ? RndF(3000, amp) : 0.0f);
        float rs = RndF(2000, 1000.0f); float re = rs + (float)(1 + Rnd() % 2500) / 1000.0f;
        int k = SignChanges(f, rs, re);
        if (k < 1) { --c; continue; }
        long want = 1 + (long)(Rnd() % (unsigned long)k);
        if (Rnd() % 4 == 0) want = k;
        long na = (Rnd() % 3 == 0) ? (long)(Rnd() % (unsigned long)(want + 1)) : 0;
        static Model pre; RunModel(f, rs, re, want - na, na, pre);
        if (pre.n < 0) { --c; continue; }                                 // needs more than 400 steps (two roots very close): not used
        Run(f, rs, re, want - na, na);
    }
}

int main()
{
    SetUnhandledExceptionFilter(Crash);
    CreateThread(0, 0, Watchdog, 0, 0, 0);
    g_row = FindRow();
    Anchors();
    RandomCases(4000);
    if (g_row && g_compared < 2500) Fail("too few comparable cases", g_compared, 2500);
    if (g_fail) { printf("008185b0_TEST FAIL (%d failures in %d cases)\n", g_fail, g_cases); return 1; }
    if (g_row) printf("008185b0_TEST PASS (REAL OBJECT: %d cases, %d compared float for float, %d skipped as knife-edge)\n", g_cases, g_compared, g_skipped);
    else printf("008185b0_TEST PASS (MODEL ONLY, %d cases: the row's anonymous-namespace instance is not in this link; real-object run needs /OPT:NOREF)\n", g_cases);
    return 0;
}