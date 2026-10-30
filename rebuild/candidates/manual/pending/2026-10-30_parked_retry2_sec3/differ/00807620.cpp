// _global::_Push_heap(pair<C3DVector,long>*, long, long, pair<C3DVector,long>, CCompareTargetedPositionsForFirstPersonTargeting) -- heap sift-up for the first-person targeting position sort (larger compare value first)
// Grade: DIFFER(164v164) -- 2 difflines: retail schedules the second GetCompareValueForPoint argument (lea ecx,[esp+1Ch]; push ecx) before the fstp of the first result; tried named va/vb locals, one named local, the PDB const GetCompareValueForPoint (QBE, kept) and a const operator(), /Op /G6 /Oa /Ow: no change
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00807620, 164 bytes, `ret 0x40`.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 16-byte
// std::pair<C3DVector,long> and the 0x2C-byte anonymous-namespace comparator by value
// (4 + 0x10 + 0x2C = 0x40).  Model of the landed TU neighbour __unguarded_linear_insert @ 0x008074A0.
// The comparator wraps a CVectorCompare_BestForTargeting and orders by the compare value of each
// pair's position: GetCompareValueForPoint(a.first) > GetCompareValueForPoint(b.first), the parent's
// value computed first and parked in the `push ecx` frame word (`fstp [esp+14h]`), then the value's
// (`fcomp [esp+10h] ; test ah,5 ; jp`).
// Callee: CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector&) @ 0x007F9660 (x2).
// Shape: SGI/STLport `__push_heap`; the pair's implicit operator= copies four dwords.
struct C3DVector {
    float X, Y, Z;
};

struct PairVectorLong {                // std::pair<C3DVector, long>, 16 bytes
    C3DVector first;                   // +0x00
    long      second;                  // +0x0C
};

struct CVectorCompare_BestForTargeting {
    unsigned char _body[0x2c];
    float GetCompareValueForPoint(const C3DVector& point) const;     // 0x007F9660 (Ego_r ?GetCompareValueForPoint@CVectorCompare_BestForTargeting@@QBEMABVC3DVector@@@Z)
};

struct CCompareTargetedPositionsForFirstPersonTargeting {      // anonymous namespace in retail, 0x2c bytes
    CVectorCompare_BestForTargeting VectorCompare;             // +0x00
    bool operator()(const PairVectorLong& a, const PairVectorLong& b)
    {
        return VectorCompare.GetCompareValueForPoint(a.first) > VectorCompare.GetCompareValueForPoint(b.first);
    }
};

void __fastcall _Push_heap(PairVectorLong* first, long hole, long top, PairVectorLong value, CCompareTargetedPositionsForFirstPersonTargeting comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
