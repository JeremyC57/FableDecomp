// CTCWeaponTrail::GetLastWeaponTrailPoints(vector<pair<C3DVector,C3DVector> >&, long) const -- collect the newest points of one weapon trail (a negative index means the first trail); an out-of-range index gives nothing
// Leaf: ?GetLastWeaponTrailPoints@CTCWeaponTrail@@QBE_NAAUCPointPairVector@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076E5F0, length 45 bytes (+ int3 padding).
// Callees: the private overload CTCWeaponTrail::GetLastWeaponTrailPoints(vector&, const list<CWeaponTrailEntry>&) const
// @ 0x0076E4D0 (tail-jumped with the trail's address written over the index slot; manifest "WeaponTrail_CollectPoints").
// Ego_r.pdb: ?GetLastWeaponTrailPoints@CTCWeaponTrail@@QBE_NAAV?$vector@U?$pair@VC3DVector@@V1@@std@@...@J@Z.
// Layout: NoTrails +0x0c; Trail +0x48 (vector<list<CWeaponTrailEntry> >, first word). The retail list is the 4-byte
// STLport list (a single node pointer: the element stride is 4, and ProjectileWeaponTrail +0x60 is followed by
// MinProjectileTrailTimer at +0x64), so the element is modelled as one node pointer.
// Shape: `if (i < NoTrails) { ... } return false;` (the false tail last; the early-return polarity puts it first), the
// `i >= 0` arm first; both arms tail-jump to the private
// overload (`jmp`), the index slot reused for the list reference.
struct C3DVector { float X, Y, Z; };

struct CPointPairVector;                                    // vector<pair<C3DVector, C3DVector> >

struct CWeaponTrailNode;

struct CWeaponTrailList {                                   // STLport list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>
    CWeaponTrailNode* m_pNode;
};

struct CWeaponTrailListVector {                             // vector<list<...> >
    CWeaponTrailList* m_pFirst;
    CWeaponTrailList* m_pLast;
    CWeaponTrailList* m_pEnd;
    const CWeaponTrailList& operator[](long i) const { return m_pFirst[i]; }
};

struct CTCWeaponTrail {
    void*                  __vftable;                       // +0x00
    void*                  ParentThing;                     // +0x04
    bool                   ToBeRemoved;                     // +0x08
    bool                   IsInGlobalUpdateTCs;             // +0x09
    long                   NoTrails;                        // +0x0c
    long                   NoProjectileTrails;              // +0x10
    unsigned char          _pad_0x14[0x48 - 0x14];
    CWeaponTrailListVector Trail;                           // +0x48
    bool GetLastWeaponTrailPoints(CPointPairVector& points, const CWeaponTrailList& trail) const;   // 0x0076E4D0
    bool GetLastWeaponTrailPoints(CPointPairVector& points, long trailIndex) const;
};

bool CTCWeaponTrail::GetLastWeaponTrailPoints(CPointPairVector& points, long trailIndex) const
{
    if (trailIndex < NoTrails) {
        if (trailIndex >= 0)
            return GetLastWeaponTrailPoints(points, Trail[trailIndex]);
        return GetLastWeaponTrailPoints(points, Trail[0]);
    }
    return false;
}
