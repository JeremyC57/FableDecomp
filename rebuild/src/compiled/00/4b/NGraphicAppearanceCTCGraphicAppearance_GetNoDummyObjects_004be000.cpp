// NGraphicAppearance::CTCGraphicAppearance::GetNoDummyObjects(const CCharString& name, float scale) const -- gather every component's meshes, build each mesh's C3DMeshObject at the given scale and return the first non-zero count of dummy objects with that name (0 when none has any)
// Leaf: GetNoDummyObjects
//
// Retail 0x004BE000, 356 bytes (+12 int3 padding, `ret 8`). Byte twin of GetNoHelperPoints 0x004BDE90: Ego_r.pdb source order after GetDummyObjectNamesStartingWith (retail 0x004BDD70) is GetNoHelperPoints,
// GetNoDummyObjects, GetHelperPoint, GetDummyObject, ..., HasHelperPoint, and the two twins differ only in the C3DMeshObject
// counter they call (0x00A36480 here, 0x00A36140 there). Ego_r.pdb `?GetNoDummyObjects@CTCGraphicAppearance@NGraphicAppearance@@QBEJABVCCharString@@M@Z`.
// Callees: vector<CMeshInfo>::reserve @ 0x004C46C0, each component's GetAllMeshes (virtual slot 2), CBase::CBase @ 0x0099A2D0 /
// ~CBase @ 0x0099A2E0, CTCGraphicAppearance::GetMeshObject @ 0x004BC4F0, C3DMeshObject::GetNoDummyObjects(const CCharString&) const
// @ 0x00A36480, operator delete @ 0x00BFE9BC (the mesh count block), free @ 0x00BFEA14 (the vector buffer).
// Shape: the landed GetAllHelperPointNames 0x004BDA30 model (GetAllMeshes inlined, block-scoped C3DMeshObject with inline
// ctor/dtor); the early return of a non-zero count duplicates the object and vector destructors on its own exit.
// Layout: as 0x004BDA30 (Components list header pointer +0x3c; C3DMeshObject vtable 0x01239860, Mesh counted pointer +8/+0xc).
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

class CCharString;

struct SCountBlock {
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};

struct CCountedPointer {
    void* m_pObject;                                        // +0
    SCountBlock* m_pCount;                                  // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
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

struct CBase {
    CBase();                                                // 0x0099A2D0
    virtual ~CBase();                                       // 0x0099A2E0
};

struct C3DMeshInfo;
struct C3DMeshObject : CBase {                              // vtable 0x01239860, 0x78 bytes
    const C3DMeshInfo* MeshInfo;                            // +0x04
    CCountedPointer    Mesh;                                // +0x08 CCountedPointer<const C3DMeshStats>
    unsigned char      PositionMatrix[0x30];                // +0x10 CMatrix3x4
    bool               InversePositionMatrixSetup;          // +0x40
    unsigned char      InversePositionMatrix[0x30];         // +0x44 CMatrix3x4
    float              Scale;                               // +0x74
    C3DMeshObject() : CBase(), Mesh(), InversePositionMatrixSetup(false) { }
    virtual ~C3DMeshObject() { }
    long GetNoDummyObjects(const CCharString& name) const;   // 0x00A36480
};

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
    void GetMeshObject(const CMeshInfo& info, float scale, C3DMeshObject& out) const;   // 0x004BC4F0
    long GetNoDummyObjects(const CCharString& name, float scale) const;
};

long CTCGraphicAppearance::GetNoDummyObjects(const CCharString& name, float scale) const
{
    CMeshVector meshes;
    GetAllMeshes(meshes);
    for (CMeshInfo* mesh = meshes.begin(); mesh != meshes.end(); ++mesh) {
        if (mesh->Index != 0) {
            C3DMeshObject object;
            GetMeshObject(*mesh, scale, object);
            long count = object.GetNoDummyObjects(name);
            if (count != 0)
                return count;
        }
    }
    return 0;
}

}