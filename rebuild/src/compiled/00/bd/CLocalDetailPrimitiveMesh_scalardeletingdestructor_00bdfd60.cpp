// vector deleting destructor for CLocalDetailPrimitiveMesh @ 0x00bdfd60
struct CLocalDetailPrimitiveMesh;

// scalar destructor @ 0xbdfd80 : __fastcall(self) -> void
extern void __fastcall CLocalDetailPrimitiveMesh_dtor(CLocalDetailPrimitiveMesh* mesh);
// deallocator @ 0xbf9920 : ecx=self, edx=0
extern void __fastcall CLocalDetailPrimitiveMesh_dealloc(CLocalDetailPrimitiveMesh* mesh, int edx);

void* __fastcall CLocalDetailPrimitiveMesh_scalar_deleting_destructor(CLocalDetailPrimitiveMesh* mesh, int edx, unsigned int flags)
{
    (void)edx;
    CLocalDetailPrimitiveMesh_dtor(mesh);
    if (flags & 1) {
        CLocalDetailPrimitiveMesh_dealloc(mesh, 0);
    }
    return mesh;
}