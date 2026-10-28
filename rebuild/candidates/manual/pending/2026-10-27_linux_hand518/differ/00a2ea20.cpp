// C3DMeshStats::HasHelperPoint(unsigned long id) -- is a helper point stored under this id?
// Grade: DIFFER(63v63) -- only the second end(): retail loads Size into edx and Elements into ecx (`mov edx,[esi+8] ; mov ecx,[esi+4] ; lea edx,[edx+edx*4] ; lea ecx,[ecx+edx*4]`), ours Size into ecx and Elements into edx
// Leaf: ?HasHelperPoint@C3DMeshStats@@QBE_NK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2EA20, 63 bytes, `ret 4`. Name: Ego_r/FableWin PDB ?HasHelperPoint@C3DMeshStats@@QBE_NK@Z
//   -- the map at +0x38 with 0x14-byte entries is HelperPoints (pair<ulong, C3DHelperPoint>, 4 + 16 bytes); the twin at
//   0x00A2EA60 (landed) walks DummyObjects (+0x48, 0x38-byte pairs) and is HasDummyObject, matching the Ego_r donor
//   order HasHelperPoint 0x00B8E300 / HasDummyObject 0x00B8E340. The catalog label "C3DMeshStats::HasDummyObject" here is
//   propagated from that twin.
// Callee: CPreAllocVectorMap<ulong, C3DHelperPoint, CKeyPairCompareLess<ulong, C3DHelperPoint> >::LowerBound(const ulong&)
//   const @ 0x00A316B0 (PDB spelling, returning const std::pair*).
// Layout: Ego_r.pdb C3DMeshStats: vptr, MemoryPool +4, StatsMemoryHandle +8, Name +0xC, BoundingSphere +0x10, BoundingBox
//   +0x20, HelperPoints +0x38 (CPreAllocVectorMap: Compare +0, Elements +4, Size +8, Dirty +0xC), DummyObjects +0x48.
// Shape: the map's const Find() inlined (lower bound; end() when it is end() or the key sorts before it), compared with a
//   recomputed end(); the `? true : false` ternary gives retail's bare `setne al` (a plain `!=` gives xor/setne dl/mov al,dl).
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
class C3DVector { public: float X, Y, Z; };
class C3DHelperPoint {
public:
    C3DVector Position;    // +0x00
    long      BoneIndex;   // +0x0c
};
template <class K, class V> class CKeyPairCompareLess { };

template <class K, class V, class C> class CPreAllocVectorMap {
public:
    C                    Compare;    // +0x00
    std::pair<K, V>*     Elements;   // +0x04
    unsigned long        Size;       // +0x08
    bool                 Dirty;      // +0x0c
    const std::pair<K, V>* LowerBound(const K& key) const;   // 0x00A316B0
    const std::pair<K, V>* end() const { return Elements + Size; }
    const std::pair<K, V>* Find(const K& key) const
    {
        const std::pair<K, V>* it = LowerBound(key);
        if (it == end() || key < it->first)
            it = end();
        return it;
    }
};

class C3DMeshStats {
public:
    virtual ~C3DMeshStats();
    void*         MemoryPool;          // +0x04
    long          StatsMemoryHandle;   // +0x08
    void*         Name;                // +0x0c CCharString
    unsigned char BoundingSphere[0x10];   // +0x10
    unsigned char BoundingBox[0x18];      // +0x20
    CPreAllocVectorMap<unsigned long, C3DHelperPoint, CKeyPairCompareLess<unsigned long, C3DHelperPoint> > HelperPoints;   // +0x38
    bool HasHelperPoint(unsigned long id) const;
};

bool C3DMeshStats::HasHelperPoint(unsigned long id) const
{
    return HelperPoints.Find(id) != HelperPoints.end() ? true : false;
}
