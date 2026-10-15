// CEngineInternalPrimitiveMorphInfo::~CEngineInternalPrimitiveMorphInfo() -- free the hidden-material byte array, erase and free the hidden-material-name set, destroy and free the composite-surface vector map, then destroy the texture-decal vector map out of line
// Leaf: ??1CEngineInternalPrimitiveMorphInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAA2B0, length 128 bytes.
// Label correction: catalogued as CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (propagated). Callers: the
// scalar deleting destructor @ 0x00BAA380 of CEngineInternalPrimitiveMorphedStaticMesh (vtable 0x012A51B4) on its
// member at +0xac, which Ego_r.pdb names `CEngineInternalPrimitiveMorphInfo Morph`, plus two calls at 0x00BD9A49/0x00BD9A79.
// Callees: free @ 0x00BFEA14 (x4), the set<CCharString> tree erase @ 0x004ABA70 (catalogued as the propagated
// "LTextTreeWalkThrough::BuildTreeArray"), _Destroy_range<pair<long,CCompositeSurfaceInfo>> @ 0x00BA9FD0 (`__fastcall`
// first/last, the iterator tag by reference at `lea [esp+0Bh]`), the texture-decal vector destructor @ 0x00BAA1D0
// (catalogued "DestroyTesselationEdgeList").
// Layout: Ego_r.pdb CEngineInternalPrimitiveMorphInfo (PDB 72 bytes, retail 0x38 with 12-byte STLport containers):
// TextureDecals +0x00 (vector + compare), CompositeSurfaces +0x10 (vector + compare), HiddenMaterialNames +0x20
// (set: header, count, compare), HiddenMaterials +0x2c (CArray<unsigned char>).
// Shape: members destroyed in reverse order; the set's destructor inline (as the landed ~CRetailTree in 0x00755520:
// erase the root, reset the head links, zero the count, free the head); the composite vector's destructor inline
// (destroy range, free); the decal vector's destructor out of line; no vtable (a plain class); `ret`.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };

struct CCompositeSurfaceInfo;
struct SCompositeSurfacePair;                      // pair<long, CCompositeSurfaceInfo>
void __fastcall DestroyRange(SCompositeSurfacePair* first, SCompositeSurfacePair* last, const SNonscalarPtrIteratorTag& tag);   // 0x00BA9FD0

struct CTextureDecalVector {                       // vector<pair<pair<long,long>,CEngineTextureDecal>>
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    ~CTextureDecalVector();                        // 0x00BAA1D0
};
struct CCompositeSurfaceVector {                   // vector<pair<long,CCompositeSurfaceInfo>>
    SCompositeSurfacePair* _M_start;
    SCompositeSurfacePair* _M_finish;
    SCompositeSurfacePair* _M_end_of_storage;
    ~CCompositeSurfaceVector()
    {
        DestroyRange(_M_start, _M_finish, SNonscalarPtrIteratorTag());
        if (_M_start)
            free(_M_start);
    }
};
struct CTextureDecalMap {                          // CVectorMap<pair<long,long>,CEngineTextureDecal,...>
    CTextureDecalVector m_vector;                  // +0x00
    long                m_compare;                 // +0x0c
};
struct CCompositeSurfaceMap {                      // CVectorMap<long,CCompositeSurfaceInfo,...>
    CCompositeSurfaceVector m_vector;              // +0x00
    long                    m_compare;             // +0x0c
};
struct CCharStringSet {                            // set<CCharString>: the retail red-black tree {head, size, compare}
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node*         head;
    unsigned long size;
    long          compare;
    void Erase(node* root);                        // 0x004ABA70
    ~CCharStringSet()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
};
struct CByteArray {                                // CArray<unsigned char>: first / last / end
    unsigned char* m_pFirst;
    unsigned char* m_pLast;
    unsigned char* m_pEnd;
    ~CByteArray() { if (m_pFirst) free(m_pFirst); }
};

struct CEngineInternalPrimitiveMorphInfo {         // retail 0x38 bytes
    CTextureDecalMap      TextureDecals;           // +0x00
    CCompositeSurfaceMap  CompositeSurfaces;       // +0x10
    CCharStringSet        HiddenMaterialNames;     // +0x20
    CByteArray            HiddenMaterials;         // +0x2c
    ~CEngineInternalPrimitiveMorphInfo();
};

CEngineInternalPrimitiveMorphInfo::~CEngineInternalPrimitiveMorphInfo()
{
}