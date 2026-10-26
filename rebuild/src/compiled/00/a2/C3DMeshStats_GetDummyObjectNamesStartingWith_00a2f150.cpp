// C3DMeshStats::GetDummyObjectNamesStartingWith(std::vector<CCharString>& names, const CCharString& prefix, bool once) -- UnpackNames(names, true, prefix, once): collect the packed dummy-object names that start with the prefix
// Leaf: GetDummyObjectNamesStartingWith
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2F150, 25 bytes (+ int3 padding), `ret 0Ch`.
// Identity: Ego_r.pdb `?GetDummyObjectNamesStartingWith@C3DMeshStats@@QBEXAAV?$vector@VCCharString@@...@std@@ABVCCharString@@_N@Z`
//   (three arguments, `ret 0Ch`; the catalog's "ConstructString" label is a descriptive guess). The only caller is the
//   landed forwarder 0x00A35B50 (the mesh object passes its statistics object's GetDummyObjectNamesStartingWith(names,
//   prefix, false)). The sibling GetAllDummyObjectNames 0x00A2F0F0 passes an empty CCharString temporary instead of the prefix.
// Callee: C3DMeshStats::UnpackNames(vector<CCharString>&, bool dummyObjects, const CCharString& prefix, bool once) const
//   @ 0x00A2ED10 (landed; `true` selects the dummy-object name table).
// Shape: a plain __fastcall forwarder that cannot tail-jump (four arguments vs three): `once` and `prefix` are loaded
//   first, the constant pushed, then `names` reloaded from its slot; `this` stays in ecx.
class CCharString { public:
    void* m_pStringData;
};

struct CCharStringVector;                  // std::vector<CCharString> (12-byte STLport vector)

class C3DMeshStats { public:
    virtual void v00();
    void GetDummyObjectNamesStartingWith(CCharStringVector& names, const CCharString& prefix, bool once) const;
    void UnpackNames(CCharStringVector& names, bool dummyObjects, const CCharString& prefix, bool once) const;   // 0x00A2ED10
};

void C3DMeshStats::GetDummyObjectNamesStartingWith(CCharStringVector& names, const CCharString& prefix, bool once) const
{
    UnpackNames(names, true, prefix, once);
}