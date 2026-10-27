// NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh::ReplaceMeshWithNext() -- step to the next replacement mesh of the def (wrapping to the first) and switch to it
// Leaf: ?ReplaceMeshWithNext@CTCGraphicAppearanceReplaceableMesh@NGraphicAppearance@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A6B20, 62 bytes, `call ; ret`.
// Identity: Ego_r.pdb `?ReplaceMeshWithNext@CTCGraphicAppearanceReplaceableMesh@NGraphicAppearance@@QAEXXZ`, the donor's next
//   function after ReplaceMeshByIndex (donor 0x7AB780 / 0x7AB850); retail has the landed ReplaceMeshByIndex 0x007A69F0 just
//   before this body, which bumps MeshIndex, wraps it against the def's mesh count and calls ReplaceMeshByIndex(MeshIndex).
//   No direct callers were found (the one hit is a data word). Catalog row was the placeholder `_gapscan::sub_007a6b20`.
// Callees: CTCGraphicAppearanceReplaceableMesh::ReplaceMeshByIndex(unsigned long) @ 0x007A69F0 (landed).
// Layout: as the landed ReplaceMeshByIndex: MeshIndex +0x14, PDef +0x1c; CReplaceableMeshDef::Meshes +0x28 =
//   NGraphicAppearance::CReplaceableMeshes { CArray<CEngineGraphic> Vector; } (Ego_r.pdb), CArray over the STLport vector
//   (12 bytes), CEngineGraphic 14 bytes (pack(2)).
// Shape: `++MeshIndex` stored at once; the count is the vector's `_M_finish - _M_start` divided by 14 (`imul 92492493h ;
//   add ; sar 3`), compared unsigned (`jb`); the index is re-read for the push.
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

namespace std {
template <class _Tp> class allocator { public: };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {      // STLport 4.6 layout (_Vector_base members)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
};
}
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

namespace NGraphicAppearance {
class CReplaceableMeshes {
public:
    CArray<CEngineGraphic> Vector;                  // +0x00
};
}

class CReplaceableMeshDef {
public:
    unsigned char _pad00[0x28];
    NGraphicAppearance::CReplaceableMeshes Meshes;  // +0x28
};

class CThing;

namespace NGraphicAppearance {
class CTCGraphicAppearanceReplaceableMesh {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    bool Replaced;                                  // +0x10
    unsigned long MeshIndex;                        // +0x14
    void* PrimHandle;                               // +0x18 CPrimitive*
    const CReplaceableMeshDef* PDef;                // +0x1C CDefPointer<const CReplaceableMeshDef>::Object
    void ReplaceMeshByIndex(unsigned long index);   // 0x007A69F0
    void ReplaceMeshWithNext();
};

void CTCGraphicAppearanceReplaceableMesh::ReplaceMeshWithNext()
{
    if (++MeshIndex >= PDef->Meshes.Vector.size())
        MeshIndex = 0;
    ReplaceMeshByIndex(MeshIndex);
}
}