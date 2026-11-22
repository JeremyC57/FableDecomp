// CIEngine::CIEngine() -- init-base class, own vtable, the Active flag byte at +8 cleared
// Leaf: ??0CIEngine@@IAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23750, 22 bytes (+ int3 padding); no code or data reference reaches it (an unreferenced
//   COMDAT copy of the inline constructor; retail's CEngine ctor 0x00B260B0 expands the same body).
// Callees: CInitBaseClass::CInitBaseClass() @ 0x0099A310 (??0CInitBaseClass@@QAE@XZ). Vtable 0x012A0D24 is CIEngine's (rtti-vtables.tsv).
//   The manifest label "CAIStateGroup_MoveGoodsBase ctor" is propagated from a byte-identical shell and rejected.
// Names: FableWin.pdb + Ego_d.pdb ??0CIEngine@@IAE@XZ (protected; no PDB has the public QAE spelling), engine/CIEngine.h
//   FABLE_RETAIL_CIENGINE_CTOR (NF249: `protected: CIEngine() : Active(false) { }`, inline in retail).
// NF251 (q12701): was a strong ??0CIEngine@@QAE@XZ over local CInitBaseClass / CIEngine models; now the header's inline constructor,
//   emitted here as a pick-any COMDAT (as in every TU that uses it: the CEngine ctor / dtor rows carry an equal copy), so no object
//   defines the constructor strongly. The CIEngine vtable / ??_GCIEngine copies this TU now emits are the header's (equal to the
//   CEngine rows' copies; ??_GCIEngine equal to its owner 0x00B23770).
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_CTOR
#define FABLE_RETAIL_CIENGINE_INLINE_DTOR
#define FABLE_RETAIL_CIENGINE_RENDER
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CIEngine.h"

namespace {
// Fidelity: scaffolding, not retail: the smallest use of the header's protected inline constructor (a derived class's out-of-line
// constructor in the anonymous namespace; CIEngine is abstract, so it is never instantiated), so this TU emits ??0CIEngine@@IAE@XZ as a
// pick-any COMDAT. Internal to this TU (hashed names).
class CIEngineConstructorReference : public CIEngine {
public:
    CIEngineConstructorReference();
};
CIEngineConstructorReference::CIEngineConstructorReference()
{
}
}
