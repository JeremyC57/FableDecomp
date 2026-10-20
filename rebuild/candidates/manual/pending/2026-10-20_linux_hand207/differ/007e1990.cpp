// Grade: DIFFER(214v214) -- scheduling only: retail loads graphic.RenderSizeX before the mesh index and the copied flag word before the first element store
// NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::GetMeshesUsed(vector<CMeshInfo>& meshes) const -- add every mesh of the multi-static-mesh def, each sized like the main graphic and flagged primary
// Leaf: GetMeshesUsed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E1990, length 212 bytes (+ int3 padding). __thiscall, `ret 4`; `this` is the
//         CAGraphicAppearanceComponent subobject at +0x0C (ParentThing read as [ecx-8], PDef as [ecx+10h]).
//         Ego_r.pdb: `?GetMeshesUsed@CTCGraphicAppearanceMultipleStaticMeshes@NGraphicAppearance@@UBEXAAV?$vector@...`.
// Callees: vector<CMeshInfo>::_M_insert_overflow(pos, const T&, const __true_type&, size_type, bool) @ 0x007091A0.
// Shape: an unsigned index loop over PDef->Meshes (56-byte CMultiStaticMeshEntryDef, `size()` = the signed difference
//        divided by 56 through the 0x92492493 multiply, re-evaluated each pass; the byte offset kept in ebx); the main
//        graphic is a reference kept in the frame; each CMeshInfo is filled member by member and pushed through the
//        inline STLport push_back (placement copy, else _M_insert_overflow with the tag in the dead argument slot).
// Layout: Ego_r.pdb CTCGraphicAppearanceMultipleStaticMeshes: PrimHandles +0x10 (12-byte STLport vector in retail), PDef
//         +0x1C; CMultiStaticMeshDef::Meshes +0x28, CMultiStaticMeshEntryDef::Mesh +0x28 (PDB); retail CThing
//         PTCGraphicAppearance +0x64, CTCGraphicAppearance::MainGraphic +0x44 (RenderSizeX +0x4C).
namespace NGraphicAppearance {

class CMeshInfo {
public:
    long Index;                                     // +0x00
    float SizeX;                                    // +0x04
    unsigned char PrimaryMesh : 1;                  // +0x08
};

}

#pragma pack(push, 2)
class CEngineGraphic {                              // 14 bytes
public:
    unsigned long BankIndex;                        // +0x00
    float AnimStep;                                 // +0x04
    float RenderSizeX;                              // +0x08
    bool AdditiveAlpha;                             // +0x0C
    unsigned char Type;                             // +0x0D
};
#pragma pack(pop)

struct __true_type { __true_type() {} };           // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    const T& operator[](unsigned long n) const { return *(begin() + n); }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x007091A0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

class CMultiStaticMeshEntryDef {                    // 56 bytes
public:
    unsigned char _pad00[0x28];
    long Mesh;                                      // +0x28
    unsigned char _pad2c[0x38 - 0x2c];
};
class CMultiStaticMeshDef {
public:
    unsigned char _pad00[0x28];
    CRetailVector<CMultiStaticMeshEntryDef> Meshes; // +0x28
};

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    unsigned char _pad00[0x44];
    CEngineGraphic MainGraphic;                     // +0x44
};
}

class CThing {
public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
};

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
};

namespace NGraphicAppearance {

class CAGraphicAppearanceComponent {
public:
    virtual void GetMeshesUsed(CRetailVector<CMeshInfo>& meshes) const;
};

class CTCGraphicAppearanceMultipleStaticMeshes : public CTCBase, public CAGraphicAppearanceComponent {
public:
    CRetailVector<void*> PrimHandles;               // +0x10 vector<CPrimitive*>
    const CMultiStaticMeshDef* PDef;                // +0x1C CDefPointer<const CMultiStaticMeshDef>::Object
    virtual void GetMeshesUsed(CRetailVector<CMeshInfo>& meshes) const;
};

void CTCGraphicAppearanceMultipleStaticMeshes::GetMeshesUsed(CRetailVector<CMeshInfo>& meshes) const
{
    const CEngineGraphic& graphic = ParentThing->PTCGraphicAppearance->MainGraphic;
    for (unsigned long i = 0; i < PDef->Meshes.size(); ++i) {
        CMeshInfo info;
        info.Index = PDef->Meshes[i].Mesh;
        info.SizeX = graphic.RenderSizeX;
        info.PrimaryMesh = true;
        meshes.push_back(info);
    }
}

}
