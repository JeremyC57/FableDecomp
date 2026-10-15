// CPhysicsMesh::GetAllDummyObjectNames(std::vector<CCharString>& names) const -- reserve room for every dummy object (56-byte {name, object} records of the DummyObjects map at +0x20) and append each one's name through a for_each functor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6D090, length 65 bytes.
// Identity: Ego_r.pdb `?GetAllDummyObjectNames@CPhysicsMesh@@QBEXAAV?$vector@VCCharString@@...@Z`;
// retail CPhysicsMesh keeps the PDB member order with 12-byte vectors (DummyObjects at +0x20).
// Callees: vector<CCharString>::reserve(unsigned long) @ 0x004BA7A0 (manifest label
// "CCombatWheel_Resize" is a propagated name), the SGI `for_each<const SDummyObject*, CAddName>`
// @ 0x00A72F40 (`__fastcall`: the functor result's hidden pointer in ecx, `first` in edx, `last` and
// the by-value functor on the stack; its body push_backs each record's leading CCharString).
// Shape: the twin of GetAllHelperPointNames (0x00A6D050); the record count is the byte difference
// times 0x92492493 plus the difference, `sar edx,5` (= / 56); the discarded functor result is
// homed in the dead `names` argument slot (`lea ecx,[esp+14h]`), which needs the functor to have a
// user constructor (hidden-pointer return) and no user copy constructor (pushed as a plain dword).
struct CCharString { void* m_pData; };
struct SDummyObject {                              // pair<CCharString, C3DDummyObject>, 56 bytes
    CCharString   m_name;                          // +0x00
    unsigned char m_object[0x34];                  // C3DDummyObject
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
CAddName __fastcall ForEach_AddDummyObjectName(const SDummyObject* first, const SDummyObject* last, CAddName f);   // 0x00A72F40

struct CPhysicsMesh {
    unsigned char       _pad00[0x20];
    SDummyObject*       m_pDummyObjectsFirst;      // +0x20 (DummyObjects)
    SDummyObject*       m_pDummyObjectsLast;       // +0x24
    void GetAllDummyObjectNames(CCharStringVector& names) const;
};

void CPhysicsMesh::GetAllDummyObjectNames(CCharStringVector& names) const
{
    names.reserve(m_pDummyObjectsLast - m_pDummyObjectsFirst);
    ForEach_AddDummyObjectName(m_pDummyObjectsFirst, m_pDummyObjectsLast, CAddName(names));
}
