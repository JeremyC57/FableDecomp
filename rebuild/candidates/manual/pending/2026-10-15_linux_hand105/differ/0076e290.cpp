// CTCWeaponTrail::SetLastProjectileWeaponTrailPoint(const C3DVector&) -- replace the newest projectile-trail entry with the base/tip pair built from the point (nothing when the trail is empty)
// Grade: DIFFER(84v84) -- same instructions, the pair copy is scheduled differently: retail loads two source dwords
// ahead (`mov ecx,[esp+4] ; mov edx,[esp+8] ; mov eax,esi`), the lane loads one at a time through `mov ecx,esi`.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076E290, length 84 bytes (+ int3 padding).
// Callees: GetProjectileTrailElementFromPoint(const C3DVector&) @ 0x0076DEB0 (a file-local `__fastcall` returning the
// pair through the hidden pointer in ecx, the point in edx; manifest label).
// Ego_r.pdb: ?SetLastProjectileWeaponTrailPoint@CTCWeaponTrail@@QAEXABVC3DVector@@@Z.
// Layout: ProjectileWeaponTrail +0x60 (the retail STLport list is one node pointer: node {next, prev, value}); the entry
// is the 24-byte pair<C3DVector, C3DVector>.
// Shape: the empty test compares the sentinel with its successor (`m_pNode == m_pNode->m_pNext` for `cmp eax,ecx`); the
// point is loaded into edx before `push esi`; the destination reference (`&back()`, the sentinel's prev + 8) is taken
// before the call; the result is a named local in the 0x18-byte frame (read back esp-relative, not through eax) and
// copied out by a memberwise operator= (the second vector re-based with `add esi,0Ch`).
// Tried: (1) `back() = Get(point)` with the implicit copy: copies through eax as six dwords (73v84); (2) + memberwise
// operator=: `add esi,0Ch` appears, still through eax and an extra edi (83v84); (3) a named result local: the
// destination is formed after the call (87v84); (4) this form (84v84, scheduling only).
struct C3DVector {
    float X, Y, Z;
};

struct CWeaponTrailEntry {                                  // pair<C3DVector, C3DVector>
    C3DVector first;                                        // +0x00
    C3DVector second;                                       // +0x0c
    CWeaponTrailEntry() { }
    CWeaponTrailEntry& operator=(const CWeaponTrailEntry& other)
    {
        first = other.first;
        second = other.second;
        return *this;
    }
};

CWeaponTrailEntry __fastcall GetProjectileTrailElementFromPoint(const C3DVector& point);   // 0x0076DEB0

struct CWeaponTrailNode {
    CWeaponTrailNode* m_pNext;                              // +0x00
    CWeaponTrailNode* m_pPrev;                              // +0x04
    CWeaponTrailEntry m_data;                               // +0x08
};

struct CWeaponTrailList {                                   // STLport list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>
    CWeaponTrailNode* m_pNode;                              // the sentinel
    bool empty() const { return m_pNode == m_pNode->m_pNext; }
    CWeaponTrailEntry& back() { return m_pNode->m_pPrev->m_data; }
};

struct CTCWeaponTrail {
    void*            __vftable;                             // +0x00
    void*            ParentThing;                           // +0x04
    unsigned char    _pad_0x08[0x60 - 0x08];
    CWeaponTrailList ProjectileWeaponTrail;                 // +0x60
    long             MinProjectileTrailTimer;               // +0x64
    void SetLastProjectileWeaponTrailPoint(const C3DVector& point);
};

void CTCWeaponTrail::SetLastProjectileWeaponTrailPoint(const C3DVector& point)
{
    if (!ProjectileWeaponTrail.empty()) {
        CWeaponTrailEntry& last = ProjectileWeaponTrail.back();
        CWeaponTrailEntry entry = GetProjectileTrailElementFromPoint(point);
        last = entry;
    }
}
