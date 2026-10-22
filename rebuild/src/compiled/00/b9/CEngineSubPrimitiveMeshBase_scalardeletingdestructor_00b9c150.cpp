// Retail 0x00b9c150 CEngineSubPrimitiveMeshBase::`vector_deleting_destructor'
// push esi; mov esi,ecx; call scalar_dtor(this); test [esp+8],1;
// if set { mov ecx,esi; call operator_delete(this); } mov eax,esi; ret 4

struct CEngineSubPrimitiveMeshBase;

// scalar destructor at 0x0099a300 (this in ecx)
extern void __fastcall SubPrim_ScalarDtor(CEngineSubPrimitiveMeshBase* mesh);
// operator delete at 0x00beb550 (this in ecx)
extern void __fastcall SubPrim_Delete(CEngineSubPrimitiveMeshBase* mesh);

void* __fastcall CEngineSubPrimitiveMeshBase_scalar_deleting_destructor(
        CEngineSubPrimitiveMeshBase* mesh, int /*edx*/, unsigned int flags)
{
    SubPrim_ScalarDtor(mesh);
    if (flags & 1)
        SubPrim_Delete(mesh);
    return mesh;
}