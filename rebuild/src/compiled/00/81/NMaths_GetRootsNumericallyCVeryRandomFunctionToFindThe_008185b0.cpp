// NMaths::GetRootsNumerically<CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint>(float range_start, float range_end, const F& func, long num_roots, long num_asymptotes) -- the roots of the ellipse closest-point function Ca*cos + Cb*sin + Cc*cos*sin (angles in turns) in [range_start, range_end): step the widened range in ever finer equal steps until enough sign changes bracket roots, bisect each bracket to 0.001, keep the roots whose both sides converged, then drop those outside the range
// Leaf: ??$GetRootsNumerically@VCVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint@?A0x
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008185B0, 1360 bytes, __fastcall (/Gr: the result pointer in ecx, func in edx), `ret 10h`.
// Identity: Ego_d.pdb NMaths::GetRootsNumerically<`anonymous namespace'::CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint>
//   (debug length 3168; the leaf is matched by prefix, the anonymous-namespace hash is per TU) and its functor records ::IsValid
//   (13 bytes, `return true`) / ::GetValue (84 bytes, locals sin_x, cos_x). One call: 0x00816650 in 0x00816410 NHitLocations::CPhysicalPrimitiveCylinder::GetClosestPointToPos2D.
// Shape: the template is the text shared with the landed 0x0087AA10 / 0x0087AFF0 / 0x0078B7C0, rewritten statement for statement
//   from Ego_d's C13 line table (ref/008185b0/egod_lines.tsv, lines 32..168) and block-scoped locals:
//   36 / 37 the widened range (12 bytes each); 41 `long found_roots = 0;` (7) before 42 num_steps (13); 46..48 clear (8),
//   found_roots = 0 (7), the increment (9); 50..54 step (12), r_start (6), r_end (9), val_a (21); 58 val_b (21); 63 / 64 the count
//   and the push_back; 67..69 val_a (6), r_start (6), r_end (9); 72 the do condition (15); 79..84 roots_pair (11), r_start (8),
//   r_end (9), accuracy (12); 86 / 87 the two values (24 / 21); 89..94 the two pairs: constructor, first, second (11 / 7 / 12 and
//   8 / 4 / 6); 98..100 r_mid (18) and val_b (24); 105..109 and 113..116: after `val_a = val_b; r_start = r_mid;` ONE statement
//   sets .first (97 bytes on the val_a side, 88 on the val_b side: the 9 bytes are three disp32 operands, so the val_a side reads
//   val_a) and the next stores .second from val_a (12) / val_b (6); 139 / 141 the push of the root; 153..163 the last loop:
//   `if (root >= range_start && root < range_end) { it++; } else { it = roots.erase(it); }` (36, 31 = a post-increment of a
//   checked iterator, 99). Lines 33, 34, 74 and 144 are debug-only asserts (349..381 bytes; func.IsValid() is called nowhere
//   else). VC7.1 /Od gives the bool-assignment form 68 bytes and an if / else 47, against Ego_d's 88: the assignment.
//   Same bytes, no evidence either way: the three post-increments (num_steps++, found_roots++, c1++) and `++it` in the first loop.
//   `c1 != num_steps` is measured (`<` changes the loop: 76 instructions differ). Line 55 is 39 bytes in Ego_d, 30 here (open).
//   GetValue: Ego_d lines 92 / 93 are 15 bytes each = the implicit double-to-float conversion of GFCos / GFSin (12 at VC7.1 /Od + 3
//   for the float argument; an explicit cast costs VC10 6 more); FableWin 0x024EB2C0 computes the cosine first.
// What decided the five inlined GetValue sites (retail factors the sum by sin at four sites and by cos at the fourth, 0x00818841;
//   78 hand4d grades ended 10 lines off with `(Ca*cos_x)` grouped, which forces sin everywhere). Measured in c2.dll 13.10.3077
//   with tools/il (fact.py hooks the tree optimiser's factoring routine 0x1076A6A7; symfree.py logs the symbol pool):
//   1. The sum is three products; the three-factor product's operands are sorted by key (frame symbol number << 5), and the
//      routine factors out the operand at index 1 of two adjacent terms. sin_x numbered above cos_x gives the sin form at once;
//      cos_x above sin_x gives the cos form in a later round, when Ca*cos_x sorts ahead of Cb*sin_x.
//   2. cos_x / sin_x of each expansion take the third and fourth entries of c2's local-symbol free list (LIFO), so the numbers
//      depend on which parameter symbols of every earlier expansion were released, in which order.
//   Retail's pattern S S S C S needs site 2 = sin > cos and site 4 = cos > sin. Plain text gave S - S S S (site 2 cos #181 sin #180).
// Inferred: the do-loop's first value is `func.GetValue(r_start)`, not `func.GetValue(mod_range_start)` (Ego_d line 54 is 21 bytes
//   with either). With r_start the expansion keeps its x_axis symbol, so site 2 gets cos #180 sin #188 = the sin form; with
//   mod_range_start site 2 is not factored (41 instructions differ on this text).
// Inferred: FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE -- STLport 4.6's allocator<T>::deallocate forwarding to
//   __malloc_alloc<0>::deallocate (an existing opt-in of the shared header; same code). Its extra parameter symbols in the
//   push_back / overflow chain leave site 4 with cos #448 sin #383 = the cos form; without it cos #383 sin #448 = sin (38
//   instructions differ). MALLOC_ALLOC_ALLOCATE, ALLOC_PROXY_REBIND and the original overflow scope were measured too: no gain
//   (REBIND undoes it).
// Inferred: GetValue returns through `(float)(...)`. Without it the cos site consumes sin_x at its last use (`fxch st(1) ; fmul`,
//   5 instructions differ); retail copies it and pops it after the sum (0x0081884C `fld st(1)` ... 0x00818853 `fxch st(1)` /
//   `fstp st(0)`). The cast's no-code tuple after the sum stops c2's copy / pop peephole (0x10822304) for sin_x, as in the landed
//   CTanMethod / CCotMethod GetValue. Outer brackets alone give the same bytes; Ego_d's line 94 (39 bytes) fits either.
// Family check (tools/il/fam8.py): the landed 0x0087AA10 (1504 bytes), 0x0087AFF0 (1522) and 0x0078B7C0 (1390) still grade
//   RELOCATION_MATCH with this template text, with and without the deallocate opt-in.
// Callees (names from the object's relocations): malloc 0x00BFEA0E, free 0x00BFEA14, memmove 0x00BFEAE6; globals TWO_PI 0x0124F2B8, DELTA_NOTIONAL_ZERO 0x0129BA3C (0.0001f).
#define FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE
#include "stlport/fable_containers.h"
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#endif
extern "C" { double __cdecl sin(double);double __cdecl cos(double);double __cdecl tan(double);double __cdecl fabs(double); }
#pragma intrinsic(sin,cos,tan,fabs)
const double TWO_PI=6.283185482025146484375;             // 0x0124F2B8 ?TWO_PI@@3NB (ESTABLISHED_SPELLINGS.tsv): a namespace-scope const (internal linkage, decorated name)
extern const float DELTA_NOTIONAL_ZERO;
inline double __fastcall GFSin(float angle){return sin(angle*TWO_PI);}
inline double __fastcall GFCos(float angle){return cos(angle*TWO_PI);}
namespace {
class CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint
{
public:
    CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint(float coef_a, float coef_b, float coef_c) : Ca(coef_a), Cb(coef_b), Cc(coef_c) {}
    bool IsValid() const
    {
        return true;
    }

    float GetValue(float x_axis) const
    {
        float cos_x = GFCos(x_axis);
        float sin_x = GFSin(x_axis);
        return (float)(Ca * cos_x + Cb * sin_x + Cc * cos_x * sin_x);
    }
private:
    float Ca;
    float Cb;
    float Cc;
};
}
namespace NMaths {
template<class F> std::vector<float> GetRootsNumerically(float range_start, float range_end, const F& func, long num_roots, long num_asymptotes)
{
    float mod_range_start = range_start - DELTA_NOTIONAL_ZERO;
    float mod_range_end = range_end + DELTA_NOTIONAL_ZERO;

    std::vector<std::pair<float, float> > roots_between;

    long found_roots = 0;
    long num_steps = num_roots + num_asymptotes - 1;

    do
    {
        roots_between.clear();
        found_roots = 0;
        num_steps++;

        float step = (mod_range_end - mod_range_start) / num_steps;
        float r_start = mod_range_start;
        float r_end = r_start + step;

        float val_a = func.GetValue(r_start);
        for (long c1 = 0; c1 != num_steps; c1++)
        {
            float val_b = func.GetValue(r_end);

            if ((val_a >= 0.0f && val_b <= 0.0f) || (val_a <= 0.0f && val_b >= 0.0f))
            {
                found_roots++;
                roots_between.push_back(std::make_pair(r_start, r_end));
            }

            val_a = val_b;
            r_start = r_end;
            r_end += step;
        }

    } while (found_roots < num_roots + num_asymptotes);

    std::vector<float> roots;
    for (std::vector<std::pair<float, float> >::iterator it = roots_between.begin(); it != roots_between.end(); ++it)
    {
        const std::pair<float, float>& roots_pair = *it;

        float r_start = roots_pair.first;
        float r_end = roots_pair.second;

        const float accuracy = 0.001f;

        float val_a = func.GetValue(r_start);
        float val_b = func.GetValue(r_end);

        std::pair<bool, float> val_a_converging;
        val_a_converging.first = true;
        val_a_converging.second = val_a;
        std::pair<bool, float> val_b_converging;
        val_b_converging.first = true;
        val_b_converging.second = val_b;

        while (r_end - r_start > accuracy)
        {
            float r_mid = (r_start + r_end) * 0.5f;

            val_b = func.GetValue(r_mid);
            if (val_a <= 0.0f)
            {
                if (val_b <= 0.0f)
                {
                    val_a = val_b;
                    r_start = r_mid;

                    val_a_converging.first = !((float)fabs(val_a) > (float)fabs(val_a_converging.second) + DELTA_NOTIONAL_ZERO);
                    val_a_converging.second = val_a;
                }
                else
                {
                    r_end = r_mid;

                    val_b_converging.first = !((float)fabs(val_b) > (float)fabs(val_b_converging.second) + DELTA_NOTIONAL_ZERO);
                    val_b_converging.second = val_b;
                }
            }
            else
            {
                if (val_b >= 0.0f)
                {
                    val_a = val_b;
                    r_start = r_mid;

                    val_a_converging.first = !((float)fabs(val_a) > (float)fabs(val_a_converging.second) + DELTA_NOTIONAL_ZERO);
                    val_a_converging.second = val_a;
                }
                else
                {
                    r_end = r_mid;

                    val_b_converging.first = !((float)fabs(val_b) > (float)fabs(val_b_converging.second) + DELTA_NOTIONAL_ZERO);
                    val_b_converging.second = val_b;
                }
            }
        }

        if (val_a_converging.first && val_b_converging.first)
            roots.push_back(r_start);
    }

    for (std::vector<float>::iterator it = roots.begin(); it != roots.end(); )
    {
        float root = *it;

        if (root >= range_start && root < range_end)
        {
            it++;
        }
        else
        {
            it = roots.erase(it);
        }
    }

    return roots;
}
template std::vector<float> GetRootsNumerically<CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint>(float, float, const CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint&, long, long);
}