// CEnginePrimitive2DAnimatingMesh::~CEnginePrimitive2DAnimatingMesh() -- destroy Anim (+0x58), then Orientation (+0x24, its inline
//   destructor reduced to the CBaseClass base destructor, a tail jump)
// Leaf: ??1CEnginePrimitive2DAnimatingMesh@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00550A00, 20 bytes, thiscall: `lea ecx,[esi+58h] ; call 0x004C2E90` (~C3DAnimationInterpolator),
//   `lea ecx,[esi+24h] ; jmp 0x0099A2E0` (~CBaseClass). Compiler-emitted COMDAT (an implicit destructor; Ego_r.pdb / FableWin.pdb keep it
//   at odd COMDAT addresses 0x00565947 / 0x01E1E023).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ??1CEnginePrimitive2DAnimatingMesh@@QAE@XZ; the body destroys exactly the Ego_r.pdb
//   members Anim (C3DAnimationInterpolator, +0x58) and Orientation (C3DOrientationInterpolator : CBaseClass, +0x24) of
//   engine/CEnginePrimitive2DAnimatingMesh.h. Its two callers agree: NUISystem::CMesh 0x005506BF runs it on the animating-mesh primitive
//   local, and 0x005BC216 tail-jumps to it after destroying TextureMorphs (+0x80, vector<CEngineTextureMorph>) and +0x8C, i.e. Ego_r.pdb
//   CEnginePrimitive2DMorphedAnimatingMesh (sizeof 0xA0: the 0x80 CEnginePrimitive2DAnimatingMesh base, TextureMorphs +0x80,
//   HiddenMaterialNames +0x90 in Ego_r = +0x8C retail), its derived class. One identity, not a fold; the catalog label
//   _Dest_val<...CMusicManager...> was propagated.
// Was: the free stand-in FableDualDestval_00550a00 (rebuild/src/compiled/00/55/global_Destval_00550a00.cpp).
// Header: engine/CEnginePrimitive2DAnimatingMesh.h, FABLE_RETAIL_CENGINEPRIMITIVE2DANIMATINGMESH_IMPLICIT_DTOR_PASS23 (the implicit
//   destructor over the typed Anim member) and engine/C3DOrientationInterpolator.h FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_IMPLICIT_DTOR
//   (its destructor is the implicit one: retail destroys Orientation with a direct ~CBaseClass jump and no vptr store, as at 0x00AEB80D).
#define FABLE_RETAIL_CENGINEPRIMITIVE2DANIMATINGMESH_IMPLICIT_DTOR_PASS23
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_IMPLICIT_DTOR
#include "engine/CEnginePrimitive2DAnimatingMesh.h"

namespace {
// Fidelity: scaffolding, not retail: an explicit destructor call in a size-optimised helper, so this object emits the implicit destructor
// out of line as a pick-any COMDAT (retail's 0x00550A00; the nf86 / nf88 pattern, NF90's #pragma optimize("s") form: an inline_depth(0)
// helper would also generate the implicit destructor without inlining ~C3DOrientationInterpolator). Internal; never called.
#pragma optimize("s", on)
void ReferenceAnimatingMeshDestructor(CEnginePrimitive2DAnimatingMesh* mesh)
{
    mesh->~CEnginePrimitive2DAnimatingMesh();
}
#pragma optimize("", on)
}
