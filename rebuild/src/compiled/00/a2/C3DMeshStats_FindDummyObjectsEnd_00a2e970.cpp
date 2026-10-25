// C3DMeshStats::FindDummyObjectsEnd(unsigned long id) const -- the first entry whose key is greater than `id` in the DummyObjects map at +0x48 (DummyObjects.UpperBound)
// Leaf: ?FindDummyObjectsEnd@C3DMeshStats@@QBEPBU?$pair@KVC3DDummyObject@@@std@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E970, 16 bytes, `ret 4`. First landed as a padded-struct forwarder
//   (catalog _global::Forward_UpperBound_member0x48_00a2e970; manifest CTCInventoryAbilities::AddLearnedExpression, a propagated label).
// Name: Ego_r.pdb runs FindHelperPointsBegin 0x00B8E0C0 / FindHelperPointsEnd 0x00B8E0D0 / GetDummyObject 0x00B8E0E0 /
//   FindDummyObjectsBegin 0x00B8E130 / FindDummyObjectsEnd 0x00B8E140 and retail runs 0x00A2E900 / 0x00A2E910 / GetDummyObject
//   0x00A2E920 (landed) / 0x00A2E960 / 0x00A2E970; this one is 0x00B8E140 (?FindDummyObjectsEnd@C3DMeshStats@@QBEPBU?$pair@KVC3DDummyObject@@@std@@K@Z).
// Callee: CPreAllocVectorMap<unsigned long,C3DDummyObject,CKeyPairCompareLess<unsigned long,C3DDummyObject> >::UpperBound(const unsigned long&) const
//   @ 0x00A317E0 (landed), on the map at +0x48; the key goes by address (`lea eax,[esp+4]`).
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

template <class K, class V> struct CKeyPairCompareLess { };

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

const std::pair<unsigned long, C3DDummyObject>* C3DMeshStats::FindDummyObjectsEnd(unsigned long id) const
{
    return DummyObjects.UpperBound(id);
}
