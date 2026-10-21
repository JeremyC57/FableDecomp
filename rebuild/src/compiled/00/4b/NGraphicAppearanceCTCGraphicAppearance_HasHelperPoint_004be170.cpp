// NGraphicAppearance::CTCGraphicAppearance::HasHelperPoint(const CCharString& name) const -- true when any of the appearance's meshes (with mesh info in the bank) carries the named helper point
// Leaf: HasHelperPoint
//
// Retail 0x004BE170, 336 bytes (`ret 4`). Ego_r.pdb `?HasHelperPoint@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@@Z`.
// Callees: vector<CMeshInfo>::reserve @ 0x004C46C0 and each component's GetAllMeshes (slot 2, +0x08) -- the landed GetAllMeshes
// 0x004BD420 inlined; Calc @ 0x004014A0 (`__fastcall(0, chars, length)` CRC of the name, "" @ 0x0129AAF4 for a null string), the mesh
// bank's GetMeshInfo @ 0x00A24270 on g_pMeshDataBank @ 0x013B8A04, C3DMeshInfo::GetMeshStats() const @ 0x00A25530 (counted pointer
// by hidden pointer), C3DMeshStats::HasHelperPoint(unsigned long) const @ 0x00A2EA20, operator delete @ 0x00BFE9BC (the count
// block), free @ 0x00BFEA14 (the vector buffer).
// Shape: the landed sibling HasDummyObject 0x004BE2C0 model, except that this body null-tests the mesh info: the `&&` makes the
// GetMeshStats temporary conditional, so VC keeps a construction flag word (bit 0, zeroed at entry) and releases the counted
// temporary only when the flag is set; the bool lands in bl.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

struct CCharStringData {
    char*         m_pData;                                  // +0
    unsigned long m_length;                                 // +4
};
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetPString() const { return m_pStringData == 0 ? "" : m_pStringData->m_pData; }
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0: CRC

struct SCountBlock {
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};

template <class T> struct CCountedPointer {
    T* m_pObject;                                           // +0
    SCountBlock* m_pCount;                                  // +4
    T* operator->() const { return m_pObject; }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct C3DMeshStats {
    bool HasHelperPoint(unsigned long key) const;           // 0x00A2EA20
};
struct C3DMeshInfo {
    CCountedPointer<const C3DMeshStats> GetMeshStats() const;   // 0x00A25530
};
struct CMeshDataBank {
    const C3DMeshInfo* GetMeshInfo(long index);             // 0x00A24270
};
extern CMeshDataBank* g_pMeshDataBank;                      // 0x013B8A04

namespace NGraphicAppearance {

struct CMeshInfo {                                          // Ego_r.pdb, 0xc bytes
    long          Index;                                    // +0x0
    float         SizeX;                                    // +0x4
    unsigned char PrimaryMesh : 1;                          // +0x8
};

}

template <class T> struct CAllocator { CAllocator() { } };  // STLport allocator<T> (empty)
template <class T> struct CAllocProxy : CAllocator<T> {     // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                     // STLport _Vector_base<T, allocator<T> >
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
    ~CVectorBase() { if (_M_start) free(_M_start); }
};
template <class T> struct CRetailVector : CVectorBase<T> {  // STLport vector<T> (12 bytes), trivially destructible T
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) { }
    T* begin() const { return this->_M_start; }
    T* end() const { return this->_M_finish; }
    void reserve(unsigned long count);                      // 0x004C46C0
};
typedef CRetailVector<NGraphicAppearance::CMeshInfo> CMeshVector;

namespace NGraphicAppearance {

struct CAGraphicAppearanceComponent {
    virtual void v00(); virtual void v01();
    virtual void GetAllMeshes(CMeshVector& meshes) const;   // slot 2 (+0x08)
};

}

struct CComponentListNode {
    CComponentListNode*                               m_next;   // +0x00
    CComponentListNode*                               m_prev;   // +0x04
    NGraphicAppearance::CAGraphicAppearanceComponent* m_value;  // +0x08
};
struct CComponentList {                                     // STLport list<CAGraphicAppearanceComponent*>: header-node pointer
    CComponentListNode* m_head;
};

namespace NGraphicAppearance {

struct CTCGraphicAppearance {
    void*          __vftable;                               // +0x00 (CTCBase)
    void*          ParentThing;                             // +0x04
    unsigned char  _pad08[0x3c - 0x08];
    CComponentList Components;                              // +0x3c
    void GetAllMeshes(CMeshVector& meshes) const            // 0x004BD420 (landed), inlined here
    {
        meshes.reserve(4);
        for (CComponentListNode* node = Components.m_head->m_next; node != Components.m_head; node = node->m_next)
            node->m_value->GetAllMeshes(meshes);
    }
    bool HasHelperPoint(const CCharString& name) const;
};

bool CTCGraphicAppearance::HasHelperPoint(const CCharString& name) const
{
    CMeshVector meshes;
    GetAllMeshes(meshes);
    unsigned long key = Calc_004014a0(0, name.GetPString(), name.GetLength());
    for (CMeshInfo* mesh = meshes.begin(); mesh != meshes.end(); ++mesh) {
        const C3DMeshInfo* meshInfo = g_pMeshDataBank->GetMeshInfo(mesh->Index);
        if (meshInfo && meshInfo->GetMeshStats()->HasHelperPoint(key))
            return true;
    }
    return false;
}

}