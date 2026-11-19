// CEnginePrimitiveMorphedStaticMesh::CEnginePrimitiveMorphedStaticMesh() -- the inline base constructor (type 8, flags, counts, the empty vector and the empty sentinel tree) then the derived type 10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007554C0, length 95 bytes (+ int3 padding).
// Callees: malloc @ 0x00BFEA0E (the 0x14-byte tree header node).
// Identity: built in a frame slot by CTCLook::OnAppearanceDraw (call at 0x0075436C, right after the
// caller zeroes its own locals); no vtable, no arguments. The manifest label
// "CTCBoastingPodium::CTCBoastingPodium" is propagated; the record is named by its user and its
// members descriptively.
// Shape: the first word is written twice (8, then 10 at the very end) = an inline base constructor
// that sets the type, then the derived constructor body that overrides it. The base zeroes its
// scalars with the ebx zero register in declaration order (the words at +0xc..+0x3b and +0x44..+0x4b
// have trivial constructors), constructs a three-pointer vector (+0x4c) and an STLport-shaped
// red-black tree (+0x58): the tree base nulls the header, allocates it (malloc 0x14), the tree sets its
// node count, then `_M_empty_initialize` colours the header red (0) and links root/leftmost/rightmost,
// re-reading the header member after every store through the node (the stores may alias it).
// (The old local model needed a forced-inline base because it held the vector and the tree; on the shared headers they are
// members of the derived class and the plain inline base expands.) The leaf names the derived constructor.
// Leaf: ??0CEnginePrimitiveMorphedStaticMesh@@QAE@XZ
// NF95 2026-11-19 (q11335): the record is CEnginePrimitiveMorphedStaticMesh (Ego_r.pdb 0x007673A0 + FableWin.pdb 0x025B4810
//   ??0CEnginePrimitiveMorphedStaticMesh@@QAE@XZ; engine/CEnginePrimitiveMorphedStaticMesh.h declares this ctor at 0x007554C0 and the
//   dtor 0x00755520; layout TextureMorphs vector +0x4C, HiddenMaterialNames set +0x58, size 0x64 as here) over CEnginePrimitiveStaticMesh
//   (primitive type 8; the derived type is 10). (NF95 respelled only the derived class in the old local model; NF97 below moved
//   the whole row onto the shared headers.)
// NF97 2026-11-19 (port): the class, its base and members are the shared headers (engine/CEnginePrimitiveMorphedStaticMesh.h
//   EYEDRAW_PASS24 over engine/CEnginePrimitiveStaticMesh.h PASS22 / EYEDRAW_PASS24, the landed dtor 0x00755520's opt-ins): the inline
//   CEnginePrimitiveStaticMesh() (CEnginePrimitive(ENGINE_PRIMITIVE_STATIC_MESH, 0), MeshEffect, ThemeChannelDesc), the STLport
//   vector<CEngineTextureMorph> TextureMorphs +0x4C and set<CCharString> HiddenMaterialNames +0x58; the body sets the derived type
//   ENGINE_PRIMITIVE_MORPHED_STATIC_MESH (10, engine/EEnginePrimitiveType.h). No local models.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVESTATICMESH_EYEDRAW_PASS24
#define FABLE_RETAIL_CENGINEPRIMITIVEMORPHEDSTATICMESH_EYEDRAW_PASS24
#include "fable_string.h"
#include "engine/EEnginePrimitiveType.h"
#include "engine/CEnginePrimitiveMorphedStaticMesh.h"

CEnginePrimitiveMorphedStaticMesh::CEnginePrimitiveMorphedStaticMesh()
{
    Type = ENGINE_PRIMITIVE_MORPHED_STATIC_MESH;
}
