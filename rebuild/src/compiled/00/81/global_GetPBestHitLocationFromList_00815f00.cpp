// GetPBestHitLocationFromList(const THitLocationIntersectionElement* elements, unsigned long count, const C3DVector& origin, C3DVector& outPos, C3DVector& outNormal) -- of the sorted hit-location intersections that share the last element's key, pick the one closest to origin; copy its intersection point and outward normal out and return its hit location (null for an empty list)
// Leaf: ?GetPBestHitLocationFromList@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00815F00, 255 bytes, /Gr __fastcall (elements in ecx, count in edx), `ret 0Ch`.
// Identity: Ego_r.pdb ?GetPBestHitLocationFromList@?A0x5ebd2a70@@YIPAVCHitLocation@NHitLocations@@PBUTHitLocation
//   IntersectionElement@1@KABVC3DVector@@AAV5@2@Z (anonymous namespace of the CTCHitLocations TU); callers
//   CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithFiniteLine 0x008168B0 and ...WithTriangle 0x008169B0
//   (landed) after their std::sort.
// Callees: std::__equal_range<const THitLocationIntersectionElement*, long, THitLocationIntersectionElementPred, int>
//   0x00817E30 (landed; __fastcall, hidden pair result in ecx, `ret 10h`).
// Layout: Ego_r.pdb THitLocationIntersectionElement, 32 bytes: key +0, hit_location +4, intersection_pos +8,
//   outward_normal +0x14 (the landed 0x00817E30 model).
// Shape: `equal_range(first, last, key, Pred())` inline over a named copy of (last - 1)->key (the value-initialised
//   empty predicate is the zeroed byte pushed as a dword); the squared distances are inline C3DVector differences kept
//   on the FPU stack, the first summed x-first (`fld st(2) ; fmul st(3)` leads) and the loop's z-first (`fld st(0) ;
//   fmul st(1)` leads) -- the two retail accumulation orders, each reproduced only by its own statement form; the running best distance lives in the dead `origin` argument slot; `d < best` is
//   `test ah,5 ; jp`; the winner's two vectors are copied member-wise and its hit location returned.
namespace NHitLocations { class CHitLocation; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { float s = Z * Z; s += Y * Y; s += X * X; return s; }
};

namespace {
struct THitLocationIntersectionElement {               // 32 bytes
    long                         key;                  // +0x00
    NHitLocations::CHitLocation* hit_location;         // +0x04
    C3DVector                    intersection_pos;     // +0x08
    C3DVector                    outward_normal;       // +0x14
};
struct THitLocationIntersectionElementPred {
    bool operator()(const THitLocationIntersectionElement& a, long b) const { return a.key < b; }
    bool operator()(long a, const THitLocationIntersectionElement& b) const { return a < b.key; }
};
}

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class ForwardIter, class Tp, class Compare, class Distance>
pair<ForwardIter, ForwardIter> __fastcall __equal_range(ForwardIter first, ForwardIter last, const Tp& val,
                                                        Compare comp, Distance*);   // 0x00817E30
template <class ForwardIter, class Tp, class Compare>
inline pair<ForwardIter, ForwardIter> equal_range(ForwardIter first, ForwardIter last, const Tp& val, Compare comp)
{
    return __equal_range(first, last, val, comp, (int*)0);
}
}

namespace {
NHitLocations::CHitLocation* __fastcall GetPBestHitLocationFromList(const THitLocationIntersectionElement* elements,
                                                                    unsigned long count, const C3DVector& origin,
                                                                    C3DVector& outPos, C3DVector& outNormal)
{
    if (count == 0)
        return 0;
    const THitLocationIntersectionElement* last = elements + count;
    long key = (last - 1)->key;
    std::pair<const THitLocationIntersectionElement*, const THitLocationIntersectionElement*> range =
        std::equal_range(elements, last, key, THitLocationIntersectionElementPred());
    const THitLocationIntersectionElement* best = range.first;
    C3DVector offset = best->intersection_pos - origin;
    float bestDistance = offset.X * offset.X;
    bestDistance += offset.Y * offset.Y;
    bestDistance += offset.Z * offset.Z;
    for (const THitLocationIntersectionElement* it = range.first + 1; it != range.second; ++it) {
        float distance = (it->intersection_pos - origin).GetLengthSquared();
        if (distance < bestDistance) {
            bestDistance = distance;
            best = it;
        }
    }
    outPos = best->intersection_pos;
    outNormal = best->outward_normal;
    return best->hit_location;
}
}

NHitLocations::CHitLocation* (__fastcall *g_keepGetPBestHitLocationFromList)(const THitLocationIntersectionElement*,
    unsigned long, const C3DVector&, C3DVector&, C3DVector&) = &GetPBestHitLocationFromList;   // keeps the body emitted