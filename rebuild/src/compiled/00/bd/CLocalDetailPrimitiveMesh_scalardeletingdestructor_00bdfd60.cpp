// vector deleting destructor for CLocalDetailPrimitiveMesh @ 0x00bdfd60
// Leaf: ?CLocalDetailPrimitiveMesh_scalar_deleting_destructor@@YIPAXPAUCLocalDetailPrimitiveMesh@@HI@Z
// NF263: the deallocator 0x00BF9920 is the class's placement operator delete(void*, CMovableResource*), `__fastcall`
// (Ego_r.pdb + FableWin.pdb ??3CLocalDetailPrimitiveMesh@@SIXPAXPAVCMovableResource@@@Z; edx = 0 at 0x00BDFD6F; the
// sibling ??2 at 0x00BF9900 is landed on the same placement form); was the free stand-in CLocalDetailPrimitiveMesh_dealloc.
// Naming (NF456, r454b unit with 0x00BDFD80): the destructor call is the real one, ??1CLocalDetailPrimitiveMesh@@UAE@XZ (Ego_r + FableWin name sets;
//   virtual), written as the qualified call `mesh->CLocalDetailPrimitiveMesh::~CLocalDetailPrimitiveMesh()` (direct, not through the vftable 0x012A7E04);
//   was the free stand-in CLocalDetailPrimitiveMesh_dtor. The self stays the free stand-in: retail's ??_GCLocalDetailPrimitiveMesh@@UAEPAXI@Z (FableWin
//   name set) is compiler-generated and only emitted beside the vftable (the ctor-owned thin-row route), which is not this row's object.
class CMovableResource;
struct CLocalDetailPrimitiveMesh {
    virtual ~CLocalDetailPrimitiveMesh();                                            // 0x00BDFD80
    static void __fastcall operator delete(void* block, CMovableResource* owner);   // 0x00BF9920
};

void* __fastcall CLocalDetailPrimitiveMesh_scalar_deleting_destructor(CLocalDetailPrimitiveMesh* mesh, int edx, unsigned int flags)
{
    (void)edx;
    mesh->CLocalDetailPrimitiveMesh::~CLocalDetailPrimitiveMesh();
    if (flags & 1) {
        CLocalDetailPrimitiveMesh::operator delete(mesh, 0);
    }
    return mesh;
}
