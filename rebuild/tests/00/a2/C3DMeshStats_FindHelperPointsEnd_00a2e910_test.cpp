// Behaviour test for 0x00A2E910 C3DMeshStats::FindHelperPointsEnd (links the source object): the map at +0x38 gets the key by
// reference and its UpperBound result comes back.
#include <cstdio>
#include <cstddef>
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public: float m[12]; };
class C3DHelperPoint {                                     // Ego_r.pdb, 16 bytes
public:
    C3DVector Position;                                    // +0x00
    long      BoneIndex;                                   // +0x0C
};
class C3DDummyObject {                                     // Ego_r.pdb, 0x34 bytes
public:
    CMatrix3x4 Transform;                                  // +0x00
    long       BoneIndex;                                  // +0x30
};

template <class K, class V> class CKeyPairCompareLess { public: };

template <class K, class V, class C> class CPreAllocVectorMap {   // Ego_r.pdb, 16 bytes
public:
    C                    Compare;                          // +0x00
    std::pair<K, V>*     Elements;                         // +0x04
    unsigned long        Size;                             // +0x08
    bool                 Dirty;                            // +0x0C
    std::pair<K, V>* LowerBound(const K& key) const;
    std::pair<K, V>* UpperBound(const K& key) const;
};

class CCharString { public: void* PStringData; };

// Retail layout of the members used (Ego_r.pdb offsets agree up to +0x48): HelperPoints +0x38, DummyObjects +0x48.
class C3DMeshStats {
public:
    virtual ~C3DMeshStats();
    const std::pair<unsigned long, C3DHelperPoint>* FindHelperPointsBegin(unsigned long id) const;
    const std::pair<unsigned long, C3DHelperPoint>* FindHelperPointsEnd(unsigned long id) const;
    const std::pair<unsigned long, C3DDummyObject>* FindDummyObjectsBegin(unsigned long id) const;
    const std::pair<unsigned long, C3DDummyObject>* FindDummyObjectsEnd(unsigned long id) const;
    void*         MemoryPool;                              // +0x04 CMovableResourceMemoryPool*
    long          StatsMemoryHandle;                       // +0x08
    CCharString   Name;                                    // +0x0C
    unsigned char BoundingSphere[0x10];                    // +0x10 C3DBoundingSphere
    unsigned char BoundingBox[0x18];                       // +0x20 C3DBoundingBox
    CPreAllocVectorMap<unsigned long, C3DHelperPoint, CKeyPairCompareLess<unsigned long, C3DHelperPoint> > HelperPoints;   // +0x38
    CPreAllocVectorMap<unsigned long, C3DDummyObject, CKeyPairCompareLess<unsigned long, C3DDummyObject> > DummyObjects;   // +0x48
};

static const void* g_map = 0; static unsigned long g_key = 0; static int g_calls = 0;
static std::pair<unsigned long, C3DHelperPoint> g_entries[4];
template <> std::pair<unsigned long, C3DHelperPoint>* CPreAllocVectorMap<unsigned long, C3DHelperPoint, CKeyPairCompareLess<unsigned long, C3DHelperPoint> >::UpperBound(const unsigned long& key) const
{ ++g_calls; g_map = this; g_key = key; return &g_entries[key & 3]; }
C3DMeshStats::~C3DMeshStats() { }
int main()
{
    C3DMeshStats s;
    const std::pair<unsigned long, C3DHelperPoint>* r = s.FindHelperPointsEnd(6);
    bool ok = g_calls == 1 && g_map == &s.HelperPoints && g_key == 6 && r == &g_entries[2] && offsetof(C3DMeshStats, HelperPoints) == 0x38;
    if (!ok) { std::printf("00a2e910_TEST FAIL\n"); return 1; }
    std::printf("00a2e910_TEST PASS\n");
    return 0;
}
