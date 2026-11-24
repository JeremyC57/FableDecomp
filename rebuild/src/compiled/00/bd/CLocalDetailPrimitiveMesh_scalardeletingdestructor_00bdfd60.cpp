// vector deleting destructor for CLocalDetailPrimitiveMesh @ 0x00bdfd60
// Leaf: ?CLocalDetailPrimitiveMesh_scalar_deleting_destructor@@YIPAXPAUCLocalDetailPrimitiveMesh@@HI@Z
// NF263: the deallocator 0x00BF9920 is the class's placement operator delete(void*, CMovableResource*), `__fastcall`
// (Ego_r.pdb + FableWin.pdb ??3CLocalDetailPrimitiveMesh@@SIXPAXPAVCMovableResource@@@Z; edx = 0 at 0x00BDFD6F; the
// sibling ??2 at 0x00BF9900 is landed on the same placement form); was the free stand-in CLocalDetailPrimitiveMesh_dealloc.
class CMovableResource;
struct CLocalDetailPrimitiveMesh {
    static void __fastcall operator delete(void* block, CMovableResource* owner);   // 0x00BF9920
};

// scalar destructor @ 0xbdfd80 : __fastcall(self) -> void
extern void __fastcall CLocalDetailPrimitiveMesh_dtor(CLocalDetailPrimitiveMesh* mesh);

void* __fastcall CLocalDetailPrimitiveMesh_scalar_deleting_destructor(CLocalDetailPrimitiveMesh* mesh, int edx, unsigned int flags)
{
    (void)edx;
    CLocalDetailPrimitiveMesh_dtor(mesh);
    if (flags & 1) {
        CLocalDetailPrimitiveMesh::operator delete(mesh, 0);
    }
    return mesh;
}