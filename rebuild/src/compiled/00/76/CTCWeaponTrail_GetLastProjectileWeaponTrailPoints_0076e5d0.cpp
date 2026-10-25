// CTCWeaponTrail::GetLastProjectileWeaponTrailPoints(vector<pair<C3DVector,C3DVector> >& points) const -- collect the newest points of the projectile trail
// Leaf: ?GetLastProjectileWeaponTrailPoints@CTCWeaponTrail@@QBE_NAAV?$vector@U?$pair@VC3DVector@@V1@@std@@V?$allocator@U?$pair@VC3DVector@@V1@@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076E5D0, 17 bytes, thiscall, `ret 4`.
// Identity: FableWin.pdb ?GetLastProjectileWeaponTrailPoints@CTCWeaponTrail@@QBE_NAAV?$vector@...@Z; callers are
//   CTCShotDefinitionBase::OnCollision (0x007BF5E8), UpdateCollision (0x007BFF01) and OnCollisionProper (0x007C12C4),
//   each right after HasWeaponTrailPoints (landed 0x0076E4A0). Ego_r.pdb CTCWeaponTrail::ProjectileWeaponTrail (list
//   at +120) sits at retail +0x60: the landed sibling 0x0076E5F0 puts Trail at +0x48 and HelperNames follows at +0x54,
//   each container 12 bytes, the STLport list 4. Parameter spelling (namespace std, `U?$pair`) follows the landed
//   sibling 0x0076E5F0, which is PDB-exact. Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: CTCWeaponTrail::GetLastWeaponTrailPoints(vector&, const list<CWeaponTrailEntry>&) const @ 0x0076E4D0
//   (private overload; manifest "WeaponTrail_CollectPoints").
// Shape: `return GetLastWeaponTrailPoints(points, ProjectileWeaponTrail);` -- `this` stays in ecx for the private
//   overload, the list address is pushed first (`lea eax,[ecx+60h]`).
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
    long                            NoTrails;               // +0x0C
    long                            NoProjectileTrails;     // +0x10
    unsigned char                   _pad_0x14[0x60 - 0x14];
    CWeaponTrailList                ProjectileWeaponTrail;  // +0x60
    bool GetLastProjectileWeaponTrailPoints(CPointPairVector& points) const;
private:
    bool GetLastWeaponTrailPoints(CPointPairVector& points, const CWeaponTrailList& trail) const;   // 0x0076E4D0
};

bool CTCWeaponTrail::GetLastProjectileWeaponTrailPoints(CPointPairVector& points) const
{
    return GetLastWeaponTrailPoints(points, ProjectileWeaponTrail);
}