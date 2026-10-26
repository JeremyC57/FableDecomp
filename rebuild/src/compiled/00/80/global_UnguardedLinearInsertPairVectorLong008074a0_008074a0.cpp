// _global::UnguardedLinearInsert_PairVectorLong_008074a0(pair<C3DVector,long>*, pair<C3DVector,long>, CCompareTargetedPositionsForFirstPersonTargeting) -- SGI __unguarded_linear_insert for the first-person targeting position sort
// Leaf: UnguardedLinearInsert_PairVectorLong_008074a0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008074A0, length 119 bytes (+ int3 padding).
// Identity: the manifest's `_Copy_backward<CWaterSeaGenerator::CVertexDef*>` label is propagated and wrong -- the body
// is the SGI `__unguarded_linear_insert(last, T val, Compare comp)` (`ret 3Ch` = the 16-byte value + the 0x2C-byte
// comparator by value). Its TU neighbours are the std::pair<C3DVector,long> heap/sort helpers instantiated with the
// anonymous-namespace CCompareTargetedPositionsForFirstPersonTargeting (0x00807620 _Push_heap, 0x008082C0
// _Adjust_heap, 0x00808A30 _Insertion_sort, 0x00808BD0 _Pop_heap), and the two callers (0x0080819A, 0x00808B7A) are
// the matching insertion-sort loops. The comparator wraps a CVectorCompare_BestForTargeting (0x2C bytes; landed
// GetCompareValueForPoint @ 0x007F9660) and orders by the compare value of the pair's position, larger first.
// Shape: /Gr TU (__fastcall, last in ecx). comp(val, *next) evaluates val's value first and parks it in the frame word
// (`fstp [esp+8]`), then `fcomp ; fnstsw ; test ah,5 ; jp` is `a > b`. The pair's implicit operator= copies four
// dwords through a pointer register (`mov ecx,esi`); the final `*last = val` reloads the value from its argument slot.
// Callees: CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector&) @ 0x007F9660 (x2 per step).
class C3DVector { public:
    float X, Y, Z;
};

struct PairVectorLong {                // std::pair<C3DVector, long>, 16 bytes
    C3DVector first;                   // +0x00
    long      second;                  // +0x0C
};

class CVectorCompare_BestForTargeting { public:
    unsigned char _body[0x2c];
    float GetCompareValueForPoint(const C3DVector& point);     // 0x007F9660
};

struct CCompareTargetedPositionsForFirstPersonTargeting {      // anonymous namespace in retail, 0x2c bytes
    CVectorCompare_BestForTargeting VectorCompare;             // +0x00
    bool operator()(const PairVectorLong& a, const PairVectorLong& b)
    {
        return VectorCompare.GetCompareValueForPoint(a.first) > VectorCompare.GetCompareValueForPoint(b.first);
    }
};

void __fastcall UnguardedLinearInsert_PairVectorLong_008074a0(PairVectorLong* last, PairVectorLong val, CCompareTargetedPositionsForFirstPersonTargeting comp)
{
    PairVectorLong* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}