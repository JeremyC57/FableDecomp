// NGraphicAppearance::CTCGraphicAppearance::HasDummyObject(const CCharString& name) const -- true when any of the appearance's meshes carries the named dummy object
// Leaf: HasDummyObject
//
// Retail 0x004BE2C0, 288 bytes incl. padding (`ret 4`). Ego_r.pdb `?HasDummyObject@CTCGraphicAppearance@NGraphicAppearance@@
// QBE_NABVCCharString@@@Z`.
// Callees: vector<CMeshInfo>::reserve @ 0x004C46C0 and each component's GetAllMeshes (slot 2, +0x08) -- the landed
// GetAllMeshes 0x004BD420 inlined; Calc @ 0x004014A0 (`__fastcall(0, chars, length)` CRC of the name, "" @ 0x0129AAF4 for a null
// string), the mesh bank's GetMeshInfo @ 0x00A24270 on g_pMeshDataBank @ 0x013B8A04, C3DMeshInfo::GetMeshStats() const
// @ 0x00A25530 (counted pointer by hidden pointer), C3DMeshStats::HasDummyObject(unsigned long) const @ 0x00A2EA60, operator
// delete @ 0x00BFE9BC (the count block), free @ 0x00BFEA14 (the vector buffer).
// Layout: retail CTCGraphicAppearance Components list header +0x3c (landed GetAllMeshes); CMeshInfo Index +0.
// Shape: the GetAllDummyObjectNames family (0x004BDB40) with the name CRC taken after the mesh walk through the null-first
// CCharString accessors (as the landed GetFirstPrimitiveWithDummyObject 0x004BC410); the bank's mesh info is a named local
// fetched before the hidden-return temporary, whose counted temporary is released inline before the result test; the `return true` inside the loop duplicates the
// vector's free.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

class CCharStringData { public:
    char*         m_pData;                                  // +0
    unsigned long m_length;                                 // +4
};
class CCharString { public:
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

template <class T> class CCountedPointer { public:
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

class C3DMeshStats { public:
    bool HasDummyObject(unsigned long key) const;           // 0x00A2EA60
};
class C3DMeshInfo { public:
    CCountedPointer<const C3DMeshStats> GetMeshStats() const;   // 0x00A25530
};
class CMeshDataBank { public:
    const C3DMeshInfo* GetMeshInfo(long index);             // 0x00A24270
};
extern CMeshDataBank* g_pMeshDataBank;                      // 0x013B8A04

namespace NGraphicAppearance {

class CMeshInfo { public:                                          // Ego_r.pdb, 0xc bytes
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

class CAGraphicAppearanceComponent { public:
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

class CTCGraphicAppearance { public:
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
    bool HasDummyObject(const CCharString& name) const;
};

bool CTCGraphicAppearance::HasDummyObject(const CCharString& name) const
{
    CMeshVector meshes;
    GetAllMeshes(meshes);
    unsigned long key = Calc_004014a0(0, name.GetPString(), name.GetLength());
    for (CMeshInfo* mesh = meshes.begin(); mesh != meshes.end(); ++mesh) {
        const C3DMeshInfo* meshInfo = g_pMeshDataBank->GetMeshInfo(mesh->Index);
        if (meshInfo->GetMeshStats()->HasDummyObject(key))
            return true;
    }
    return false;
}

}