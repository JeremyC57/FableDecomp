// Grade: DIFFER(175v173) -- register allocation: the vector start is loaded through eax and copied to edi for the scan (retail loads edi directly and frees through edi); the vector dtor's reload goes to eax
// NGraphicAppearance::CTCGraphicAppearance::IsResourcesLoaded(bool beginLoading) const -- whether every mesh the appearance components use is loaded; optionally start loading the missing ones
// Leaf: IsResourcesLoaded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BE3E0, length 173 bytes (+ int3 padding). __thiscall, `ret 4`.
//         Ego_r.pdb: `?IsResourcesLoaded@CTCGraphicAppearance@NGraphicAppearance@@QBE_N_N@Z`.
// Callees: vector<CMeshInfo>::reserve(size_type) @ 0x004C46C0, each component's virtual GetMeshesUsed (slot 2, +0x08),
//          CMeshDataBank::PeekMeshInfo(unsigned long) const @ 0x00A24260 on g_pMeshDataBank @ 0x013B8A04 (the bank's
//          one-line Peek* family; the name follows the Ego_r.pdb declaration order), C3DMeshInfo::IsLoaded() const @
//          0x00A23E40, C3DMeshInfo::BeginLoading(unsigned long, bool, bool) const @ 0x00A256F0, free @ 0x00BFEA14.
// Shape: a local STLport vector (three zero words, reserve(4)) collects the meshes from the component list (end re-read
//        each pass); the scan keeps the result in bl (`mov bl,1`, cleared per missing mesh) and the end re-read; the
//        vector's storage is freed at the end.
// Layout: retail CTCGraphicAppearance::Components +0x3C (STLport list: the sentinel pointer; node value at +8);
//         CMeshInfo {Index, SizeX, PrimaryMesh:1} (12 bytes).
extern "C" void __cdecl free(void* p);              // 0x00BFEA14

namespace NGraphicAppearance {
class CMeshInfo {
public:
    long Index;                                     // +0x00
    float SizeX;                                    // +0x04
    unsigned char PrimaryMesh : 1;                  // +0x08
};
}

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CRetailVector() { if (_M_start) free(_M_start); }
    void reserve(unsigned long count);              // 0x004C46C0
};

class C3DMeshInfo {
public:
    bool IsLoaded() const;                          // 0x00A23E40
    void BeginLoading(unsigned long priority, bool wait, bool loadTextures) const;   // 0x00A256F0
};
class CMeshDataBank {
public:
    const C3DMeshInfo* PeekMeshInfo(unsigned long index) const;   // 0x00A24260
};
extern CMeshDataBank* g_pMeshDataBank;              // 0x013B8A04

namespace NGraphicAppearance {

class CAGraphicAppearanceComponent {
public:
    virtual void v00();
    virtual void v01();
    virtual void GetMeshesUsed(CRetailVector<CMeshInfo>& meshes) const;   // slot 2 (+0x08)
};

template <class T> struct CRetailList {             // STLport list: the sentinel node pointer
    struct Node { Node* Next; Node* Prev; T Value; };
    Node* Head;
};

class CTCGraphicAppearance {
public:
    unsigned char _pad00[0x3c];
    CRetailList<CAGraphicAppearanceComponent*> Components;   // +0x3C
    bool IsResourcesLoaded(bool beginLoading) const;
};

bool CTCGraphicAppearance::IsResourcesLoaded(bool beginLoading) const
{
    CRetailVector<CMeshInfo> meshes;
    meshes.reserve(4);
    for (CRetailList<CAGraphicAppearanceComponent*>::Node* it = Components.Head->Next; it != Components.Head; it = it->Next)
        it->Value->GetMeshesUsed(meshes);
    bool loaded = true;
    for (CMeshInfo* mesh = meshes._M_start; mesh != meshes._M_finish; ++mesh) {
        const C3DMeshInfo* info = g_pMeshDataBank->PeekMeshInfo(mesh->Index);
        if (!info->IsLoaded()) {
            if (beginLoading)
                info->BeginLoading((unsigned long)-1, false, false);
            loaded = false;
        }
    }
    return loaded;
}

}
