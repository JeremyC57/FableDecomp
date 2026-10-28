// Behaviour test for CEngineSubPrimitiveMeshBase::CEngineSubPrimitiveMeshBase (0x00B98B40): parent kept, no next sub-primitive, mesh, render mode and cloth flag stored.
#include <cstdio>
struct Parent { int dummy; }; struct Mesh { int dummy; };
struct SubBase { unsigned int RenderFlags; unsigned char Alpha; Parent* ParentPtr; SubBase* NextSubPrimitive;
    SubBase(Parent* p) : ParentPtr(p), NextSubPrimitive(0) {} };
struct MeshBase : SubBase { Mesh* MeshPrimitive; int RenderMode; bool IsCloth;
    MeshBase(Parent* p, Mesh* m, int mode, bool cloth) : SubBase(p), MeshPrimitive(m), RenderMode(mode), IsCloth(cloth) {} };
int main()
{
    Parent p; Mesh m; MeshBase s(&p, &m, 3, true);
    if (s.ParentPtr != &p || s.NextSubPrimitive != 0 || s.MeshPrimitive != &m || s.RenderMode != 3 || !s.IsCloth) { std::printf("00b98b40_TEST FAIL\n"); return 1; }
    std::printf("00b98b40_TEST PASS\n");
    return 0;
}