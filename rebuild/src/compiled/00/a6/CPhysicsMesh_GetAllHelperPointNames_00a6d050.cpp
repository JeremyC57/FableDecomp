// CPhysicsMesh::GetAllHelperPointNames(std::vector<CCharString>& names) const -- reserve room for every helper point (20-byte {name, point} records of the HelperPoints map at +0x10) and append each one's name through a for_each functor
// Leaf: GetAllHelperPointNames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6D050, length 63 bytes.
// Identity: the catalog label "GetAllDummyObjectNames" is propagated from the twin at 0x00A6D090.
// Retail CPhysicsMesh keeps the PDB member order with 12-byte vectors (HelperPoints +0x10,
// DummyObjects +0x20, TotalFaceIndices +0x30, NavigationLines +0x48, RootSegment +0xB4, all
// confirmed by this chunk's bodies); Ego_r.pdb has GetAllHelperPointNames immediately before
// GetAllDummyObjectNames, as here.
// Callees: vector<CCharString>::reserve(unsigned long) @ 0x004BA7A0 (manifest label
// "CCombatWheel_Resize" is a propagated name), the SGI `for_each<const SHelperPoint*, CAddName>`
// @ 0x00A72EE0 (`__fastcall`: the functor result's hidden pointer in ecx, `first` in edx, `last` and
// the by-value functor on the stack; its body push_backs each record's leading CCharString).
// Shape: the record count is the byte difference times 0x66666667 with `sar edx,3` (= / 20); the
// discarded functor result is homed in the dead `names` argument slot (`lea ecx,[esp+14h]`), which
// needs the functor to have a user constructor (hidden-pointer return) and no user copy constructor
// (it is pushed as a plain dword). Arguments right to left: `last` loaded before `first`.
struct CCharString { void* m_pData; };
struct SHelperPoint {                              // pair<CCharString, C3DHelperPoint>, 20 bytes
    CCharString   m_name;                          // +0x00
    unsigned char m_point[0x10];                   // C3DHelperPoint
};
struct CCharStringVector {
    CCharString* m_pFirst;
    CCharString* m_pLast;
    CCharString* m_pEnd;
    void reserve(unsigned long count);             // 0x004BA7A0
};
struct CAddName {
    CCharStringVector* m_pNames;
    CAddName(CCharStringVector& names) : m_pNames(&names) { }
};
CAddName __fastcall ForEach_AddHelperPointName(const SHelperPoint* first, const SHelperPoint* last, CAddName f);   // 0x00A72EE0

struct CPhysicsMesh {
    unsigned char       _pad00[0x10];
    SHelperPoint*       m_pHelperPointsFirst;      // +0x10 (HelperPoints)
    SHelperPoint*       m_pHelperPointsLast;       // +0x14
    void GetAllHelperPointNames(CCharStringVector& names) const;
};

void CPhysicsMesh::GetAllHelperPointNames(CCharStringVector& names) const
{
    names.reserve(m_pHelperPointsLast - m_pHelperPointsFirst);
    ForEach_AddHelperPointName(m_pHelperPointsFirst, m_pHelperPointsLast, CAddName(names));
}