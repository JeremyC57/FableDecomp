// CLocalDetailObjectCollectionType::CLocalDetailObjectCollectionType() -- default constructor: the non-copyable base, then no Mesh / ZSpriteMesh / ShadowMesh
// Leaf: ??0CLocalDetailObjectCollectionType@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDD6B0, 29 bytes, thiscall, returns this.
// Identity (probable): stores vtable 0x012A7D94, whose RTTI is .?AVCLocalDetailObjectCollectionType@@ (one slot, the
//   deleting destructor 0x00BDF740 that ends in ~CBaseClassNonCopyable); Ego_r.pdb has
//   ??0CLocalDetailObjectCollectionType@@QAE@XZ (and a virtual ??1 UAE). No direct caller reaches it (the default ctor of a
//   vector element type). The catalog's "CXMVPlayerBuffers" label is wrong.
// Layout: Ego_r.pdb CLocalDetailObjectCollectionType (84 bytes): vptr +0, Mesh +4, ZSpriteMesh +8, ShadowMesh +0xc; the
//   rest (bank indices, PlainDataBlock) is left unset by this constructor. The base's own vptr store is dead and dropped.
// Callee: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0.
class CBaseClass {
public:
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                             // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
class C3DMeshInfo;

class CLocalDetailObjectCollectionType : public CBaseClassNonCopyable {
public:
    CLocalDetailObjectCollectionType();
    virtual ~CLocalDetailObjectCollectionType();
    C3DMeshInfo*  Mesh;                                  // +0x04
    C3DMeshInfo*  ZSpriteMesh;                           // +0x08
    C3DMeshInfo*  ShadowMesh;                            // +0x0c
    long          MeshPolyCount;                         // +0x10
    long          MeshBankIndex;                         // +0x14
    long          ZSpriteMeshBankIndex;                  // +0x18
    long          ShadowMeshBankIndex;                   // +0x1c
    unsigned char PlainDataBlock[84 - 0x20];             // +0x20 (Ego_r.pdb unnamed PlainDataBlock, not touched here)
};

CLocalDetailObjectCollectionType::CLocalDetailObjectCollectionType()
    : Mesh(0), ZSpriteMesh(0), ShadowMesh(0)
{
}