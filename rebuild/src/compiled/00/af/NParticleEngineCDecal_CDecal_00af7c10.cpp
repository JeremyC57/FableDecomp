// NParticleEngine::CDecal::~CDecal() -- particle decal dtor (empty user body): ~ParentHandleList (the primitive handles), then ~PrimIDList
// Leaf: ??1CDecal@NParticleEngine@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF7C10.
// Identity: Ego_r.pdb ??1CDecal@NParticleEngine@@QAE@XZ; sizeof 0x74 (overlay FABLE_RETAIL_CDECAL).
// Callees: ~vector<CArray<long>> (out of line, as retail), ~CEnginePersistentPrimitiveBase.
// Shape: empty user body.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CDECAL
#include "engine/CDecal.h"

namespace NParticleEngine {
CDecal::~CDecal()
{
}
}