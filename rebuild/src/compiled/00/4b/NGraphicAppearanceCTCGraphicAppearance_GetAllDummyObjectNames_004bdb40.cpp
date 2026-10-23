// NGraphicAppearance::CTCGraphicAppearance::GetAllDummyObjectNames(vector<CCharString>& names) const -- gather every component's meshes, build each mesh's C3DMeshObject at unit scale and append its dummy-object names
// Leaf: GetAllDummyObjectNames
//
// Retail 0x004BDB40, 264 bytes (`ret 4`). Ego_r.pdb `?GetAllDummyObjectNames@CTCGraphicAppearance@
// NGraphicAppearance@@QBEXAAV?$vector@VCCharString@@...@std@@@Z`.
// Callees: vector<CMeshInfo>::reserve @ 0x004C46C0 (manifest "VectorReserve"), each component's GetAllMeshes (virtual slot
// 2, +0x08), CBase::CBase @ 0x0099A2D0 / ~CBase @ 0x0099A2E0, CTCGraphicAppearance::GetMeshObject(const CMeshInfo&, float,
// C3DMeshObject&) const @ 0x004BC4F0 (Ego_r.pdb `?GetMeshObject@...@IBEXABVCMeshInfo@2@MAAVC3DMeshObject@@@Z`; manifest
// "CTCGraphicAppearance_CreateAbilityMeshObject"), C3DMeshObject::GetAllDummyObjectNames @ 0x00A35B30 (forwards to C3DMeshStats 0x00A2F0F0 with the dummy-section flag 1;
// the byte-twin 0x004BDA30 is GetAllHelperPointNames), operator delete @ 0x00BFE9BC (the mesh count block), free
// @ 0x00BFEA14 (the vector buffer). C3DMeshObject vtable 0x01239860.
// Layout: retail CTCGraphicAppearance as the landed GetAllMeshes 0x004BD420 (Components list header pointer +0x3c);
// CMeshInfo (Ego_r.pdb, 0xc): Index +0; C3DMeshObject (0x78): Mesh counted pointer +8/+0xc, InversePositionMatrixSetup +0x40.
// Shape: the landed GetAllMeshes inlined at the top (reserve(4), the component list walk re-reading the header); each mesh
// with a non-zero index gets a block-scoped C3DMeshObject (inline default ctor: ~CBase base ctor out of line, vtable, zeroed
// counted pointer, cleared flag; inline dtor: vtable, counted release re-reading the block, both words zeroed, ~CBase); the
// vector's begin stays in eax for the trailing free.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

class CCharString;
template <class T> struct CStdVector;                       // std::vector<T> (the caller's names)

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

namespace N_0099a2d0 { struct CBase {
    CBase();                                                // 0x0099A2D0
    virtual ~CBase();                                       // 0x0099A2E0
}; } using N_0099a2d0::CBase;

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
    void GetAllDummyObjectNames(CStdVector<CCharString>& names) const;   // 0x00A35B30
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
    void GetAllDummyObjectNames(CStdVector<CCharString>& names) const;
};

void CTCGraphicAppearance::GetAllDummyObjectNames(CStdVector<CCharString>& names) const
{
    CMeshVector meshes;
    GetAllMeshes(meshes);
    for (CMeshInfo* mesh = meshes.begin(); mesh != meshes.end(); ++mesh) {
        if (mesh->Index != 0) {
            C3DMeshObject object;
            GetMeshObject(*mesh, 1.0f, object);
            object.GetAllDummyObjectNames(names);
        }
    }
}

}