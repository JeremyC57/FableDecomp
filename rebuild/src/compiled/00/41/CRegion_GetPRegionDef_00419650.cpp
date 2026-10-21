// CRegion::GetPRegionDef() const -- a counted copy of the region's definition pointer
// Leaf: GetPRegionDef@CRegion
// Retail: Fable.exe @ 0x00419650, 19 bytes, `ret 4` (hidden result pointer). Catalog label CThing::GetPBaseDef is wrong
// (CThing's PBaseDef is far deeper, Ego_r.pdb +0x80). Evidence: the only caller, CWorld::UpdateRegionLoad 0x004A5297, passes
// the same local it hands to CRegion::GetDisplayName 0x006BBF20; Ego_r.pdb ?GetPRegionDef@CRegion@@QBE?AV?$CDefPointer@$$CBVCRegionDef@@@@XZ.
// Layout: retail CRegion (no engine header) = Ego_r.pdb with 12-byte STLport vectors: Maps +0x00, VisMaps +0x0c, Name +0x18,
// DisplayName +0x1c, RegionDefName +0x20, PDef +0x24 (PDB +0x2c). CDefPointeeBase::RefCount +0x04.
class CDefPointeeBase {
public:
    void* __vftable;
    long  RefCount;                                      // +0x04
};
class CRegionDef;
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer();
};
class CCharStringData;
class CCharString { public: CCharStringData* PStringData; };
template <class T> struct CRetailVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
class CRegion {
public:
    CRetailVector<long>            Maps;                 // +0x00
    CRetailVector<long>            VisMaps;              // +0x0c
    CCharString                    Name;                 // +0x18
    CCharString                    DisplayName;          // +0x1c
    CCharString                    RegionDefName;        // +0x20
    CDefPointer<const CRegionDef>  PDef;                 // +0x24
    CDefPointer<const CRegionDef> GetPRegionDef() const;
};

CDefPointer<const CRegionDef> CRegion::GetPRegionDef() const
{
    return PDef;
}