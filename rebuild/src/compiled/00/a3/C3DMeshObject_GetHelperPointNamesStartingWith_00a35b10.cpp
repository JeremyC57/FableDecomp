// C3DMeshObject::GetHelperPointNamesStartingWith(std::vector<CCharString>&, const CCharString&) const -- asks the mesh stats, unsorted
// Retail: Fable.exe 0x00A35B10, 23 bytes: `mov ecx,[ecx+8]` (Mesh.Data) then
// C3DMeshStats::GetHelperPointNamesStartingWith(names, prefix, false) (0x00A2F170), `ret 8`.
// Identity: Ego_r.pdb ?GetHelperPointNamesStartingWith@C3DMeshObject@@QBE...ABVCCharString@@@Z (two arguments; the
// C3DMeshStats overload adds the bool). Its only caller 0x004BDCF4 (CTCGraphicAppearance's helper-point query) calls
// it on a stack C3DMeshObject whose vtable 0x01239860 is .?AVC3DMeshObject@@. PDB layout: MeshInfo +4,
// Mesh (CCountedPointer<const C3DMeshStats>: Data, Info) +8, PositionMatrix +0x10.
// Leaf: ?GetHelperPointNamesStartingWith@C3DMeshObject@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@ABVCCharString@@@Z
class CCharString {
public:
    void* PStringData;
};

namespace std {
    template <class _Tp> class allocator;
    template <class _Tp, class _Alloc = allocator<_Tp> > class vector;
}

class CCPPointerInfo;
class C3DMeshInfo;

template <class T> class CCountedPointer {
public:
    T*              Data;     // +0x00
    CCPPointerInfo* Info;     // +0x04
    T* operator->() const { return Data; }
};

class C3DMeshStats {
public:
    void GetHelperPointNamesStartingWith(std::vector<CCharString>& names, const CCharString& prefix, bool sort) const;   // 0x00A2F170
};

class C3DMeshObject {
public:
    virtual ~C3DMeshObject();
    void GetHelperPointNamesStartingWith(std::vector<CCharString>& names, const CCharString& prefix) const;
    C3DMeshInfo*                         MeshInfo;   // +0x04
    CCountedPointer<const C3DMeshStats> Mesh;       // +0x08
};

void C3DMeshObject::GetHelperPointNamesStartingWith(std::vector<CCharString>& names, const CCharString& prefix) const
{
    Mesh->GetHelperPointNamesStartingWith(names, prefix, false);
}