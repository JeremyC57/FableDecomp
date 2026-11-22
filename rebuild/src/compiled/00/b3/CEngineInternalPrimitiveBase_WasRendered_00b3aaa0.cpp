// CEngineInternalPrimitiveBase::WasRendered() const -- true when the primitive was rendered in the current frame
// Leaf: ?WasRendered@CEngineInternalPrimitiveBase@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3AAA0, 17 bytes: compares LastFrameRendered (+0x36) with the frame counter
// @ 0x0143B8D8 (`cmp cx, word ptr [0x0143B8D8]` / `sete al`).
// NF240 (q12673): the real member on engine/CEngineInternalPrimitiveBase.h (was the stand-in
//   ?WasRendered@CEngineInternalPrimitiveBase_Methods@@QAEHXZ). Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?WasRendered@CEngineInternalPrimitiveBase@@MBE_NXZ
//   (protected virtual const; the vtable slot of every internal primitive class, absrefs: 43 vtable references), and the frame
//   counter is the private static ?CurrentRenderFrame@CEngineInternalPrimitiveBase@@0GA (all three PDBs), declared by the
//   header's PASS27 block (the witness 0x00B92950's opt-in).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_SCENEGRID_VISIBILITY_PASS27
#include "stlport/fable_map.h"
#include "engine/CEngineInternalPrimitiveBase.h"

bool CEngineInternalPrimitiveBase::WasRendered() const
{
    return LastFrameRendered == CurrentRenderFrame;
}
