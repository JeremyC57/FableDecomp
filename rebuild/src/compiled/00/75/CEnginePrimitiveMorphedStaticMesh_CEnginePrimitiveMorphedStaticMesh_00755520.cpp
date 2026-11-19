// CEnginePrimitiveMorphedStaticMesh::~CEnginePrimitiveMorphedStaticMesh() -- the implicit member teardown: HiddenMaterialNames (+0x58,
//   set<CCharString>: erase, head links reset, head freed), TextureMorphs (+0x4c, the out-of-line vector<CEngineTextureMorph> destructor),
//   then the base's MeshEffect (+0x3c, CIVCountedPointer release)
// Leaf: ??1CEnginePrimitiveMorphedStaticMesh@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00755520, length 103 bytes.
// Identity (NF93 q11336): Ego_r.pdb 0x00767440 / FableWin.pdb 0x025B48B0 ??1CEnginePrimitiveMorphedStaticMesh@@QAE@XZ (non-virtual);
//   engine/CEnginePrimitiveMorphedStaticMesh.h (TextureMorphs +0x4c, HiddenMaterialNames +0x58, ctor 0x007554C0, this destructor
//   0x00755520); retail 0x00754B3E (CTCLook::OnAppearanceDraw) destroys the same stack mesh its constructor call 0x0075436C built. Was
//   the address-named SAppearanceDrawRecord_00755520 stand-in (the catalog label CHitLocation was wrong: that class is 24 bytes).
// Callees: the set's _M_erase 0x004ABA70, free 0x00BFEA14, ~vector<CEngineTextureMorph> 0x005BC0D3 (out of line,
//   FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE), the mesh effect's virtual slot 1 at a zero count.
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
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "fable_string.h"
#include "engine/CEnginePrimitiveMorphedStaticMesh.h"

CEnginePrimitiveMorphedStaticMesh::~CEnginePrimitiveMorphedStaticMesh()
{
}
