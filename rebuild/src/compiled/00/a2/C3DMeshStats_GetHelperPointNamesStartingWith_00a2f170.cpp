// C3DMeshStats::GetHelperPointNamesStartingWith(std::vector<CCharString>&, const CCharString&, bool) const -- unpacks helper-point names with a prefix
// Retail: Fable.exe 0x00A2F170, 25 bytes: forwards (names, prefix, sort) as UnpackNames(names, false, prefix, sort)
// (`push sort ; push prefix ; push 0 ; push names ; call 0x00A2ED10 ; ret 0Ch`).
// Identity: Ego_r.pdb C3DMeshStats has UnpackNames(vector<CCharString>&, bool, const CCharString&, bool) const (private)
// and four public wrappers. 0x00A2F0F0 / 0x00A2F120 pass (names, true / false, CCharString(), sort): the two
// GetAll*Names(names, sort); the bool selects dummy objects (true) or helper points (false). This row passes false and
// a caller prefix, so it is GetHelperPointNamesStartingWith; its only caller 0x00A35B10 is
// C3DMeshObject::GetHelperPointNamesStartingWith (called from CTCGraphicAppearance's helper-point query 0x004BDCF4).
// Leaf: ?GetHelperPointNamesStartingWith@C3DMeshStats@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@ABVCCharString@@_N@Z
class CCharString {
public:
    void* PStringData;
};

namespace std {
    template <class _Tp> class allocator;
    template <class _Tp, class _Alloc = allocator<_Tp> > class vector;
}

class C3DMeshStats {
public:
    void GetHelperPointNamesStartingWith(std::vector<CCharString>& names, const CCharString& prefix, bool sort) const;
private:
    void UnpackNames(std::vector<CCharString>& names, bool dummyObjects, const CCharString& prefix, bool sort) const;   // 0x00A2ED10
};

void C3DMeshStats::GetHelperPointNamesStartingWith(std::vector<CCharString>& names, const CCharString& prefix, bool sort) const
{
    UnpackNames(names, false, prefix, sort);
}