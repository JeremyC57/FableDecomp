// CTCWeaponTrail::GetLastWeaponTrailPoints(vector<pair<C3DVector,C3DVector> >&, long) const -- collect the newest points of one weapon trail (a negative index means the first trail); an out-of-range index gives nothing
// Leaf: ?GetLastWeaponTrailPoints@CTCWeaponTrail@@QBE_NAAV?$vector@U?$pair@VC3DVector@@V1@@std@@V?$allocator@U?$pair@VC3DVector@@V1@@std@@@2@@std@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076E5F0, length 45 bytes (+ int3 padding).
// Source correction (FINDINGS chunk 394: "landed 0x76E5F0 could take the `?:` form used here"): the overload call is one
//   call with a `?:` list argument, the form chunk 394 needed where this function is inlined into
//   GetAllTrailsLastWeaponTrailPoints 0x0076EA60 (the two-arm form duplicates the call there; here both grade); and the
//   parameter types are the Ego_r ones, so the symbol is the PDB-exact
//   `?GetLastWeaponTrailPoints@CTCWeaponTrail@@QBE_NAAV?$vector@U?$pair@VC3DVector@@V1@@std@@...@std@@J@Z` (the landed leaf
//   named a local `CPointPairVector`; STLport's `pair` is a struct, `U?$pair`).
// Callees: the private overload GetLastWeaponTrailPoints(vector&, const list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>&)
//   const @ 0x0076E4D0 (Ego_r `?GetLastWeaponTrailPoints@CTCWeaponTrail@@ABE_NAAV?$vector...ABV?$list@VCWeaponTrailEntry@
//   CEnginePrimitiveWeaponTrail@@...@Z`; manifest "WeaponTrail_CollectPoints"), tail-jumped with the trail's address
//   written over the index slot.
// Layout: NoTrails +0x0c; Trail +0x48 (vector<list<CWeaponTrailEntry> >, first word; struct_layouts +0x58 less the
//   retail STLport shrink of the containers ahead of it). The retail list is the 4-byte STLport list (one node pointer).
class C3DVector { public: float X, Y, Z; };

class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry;
};

namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const _Tp& operator[](long __n) const { return _M_start[__n]; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                                          // STLport: the sentinel node pointer (4 bytes)
};
}

typedef std::vector<std::pair<C3DVector, C3DVector> > CPointPairVector;
typedef std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> CWeaponTrailList;

class CTCWeaponTrail {
public:
    void*                           __vftable;              // +0x00
    void*                           ParentThing;            // +0x04
    bool                            ToBeRemoved;            // +0x08
    bool                            IsInGlobalUpdateTCs;    // +0x09
    long                            NoTrails;               // +0x0c
    long                            NoProjectileTrails;     // +0x10
    unsigned char                   _pad_0x14[0x48 - 0x14];
    std::vector<CWeaponTrailList>   Trail;                  // +0x48
    bool GetLastWeaponTrailPoints(CPointPairVector& points, long trail_index) const;
private:
    bool GetLastWeaponTrailPoints(CPointPairVector& points, const CWeaponTrailList& trail) const;   // 0x0076E4D0
};

bool CTCWeaponTrail::GetLastWeaponTrailPoints(CPointPairVector& points, long trail_index) const
{
    if (trail_index < NoTrails)
        return GetLastWeaponTrailPoints(points, trail_index >= 0 ? Trail[trail_index] : Trail[0]);
    return false;
}
