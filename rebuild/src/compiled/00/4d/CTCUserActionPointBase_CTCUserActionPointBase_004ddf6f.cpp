// CTCUserActionPointBase::~CTCUserActionPointBase() -- the compiler-generated destructor: destroy PThing (+0x2C), free the BlockedActionPointIndexes (+0x18) and FreeActionPointIndexes (+0x0C) vectors' blocks, then ~CTCBase (tail jump)
// Leaf: ??1CTCUserActionPointBase@@UAE@XZ
// Name (agentNF20, 2026-11-11): Ego_r.pdb + FableWin.pdb ??1CTCUserActionPointBase@@UAE@XZ; retail callers are the slot-0 ??_G of
//   CTCUserActionPointBase 0x007FEFA0, CTCSingleUserActionPoints 0x004E037C and CTCNamedUserActionPoints 0x004E241C (rtti-vtables.tsv;
//   both derive from it). Was "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" over the invented non-virtual base
//   CChunkedFileChunkBase (??1CChunkedFileChunkBase@@QAE@XZ bound to 0x00686830): the base is engine/CTCBase.h's retail branch, so
//   the tail is ??1CTCBase@@UAE@XZ 0x00686830 (Ego_r.pdb / FableWin.pdb).
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DDF6F, 47 bytes. Pragma: optimize("s") (`pop ecx` cleanup after each free).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10, free @ 0x00BFEA14 (CRT), ~CTCBase @ 0x00686830.
// Layout (Ego_r.pdb names at retail offsets, 12-byte STLport vectors): FreeActionPointIndexes vector<long> +0x0C,
//   BlockedActionPointIndexes vector<long> +0x18, NumberOfActionPoints +0x24, FrameOfNextCheck +0x28, PThing CIntelligentPointer<CThing> +0x2C.
// Shape: no vptr store -- the class declares no destructor (the implicit ??1); members die in reverse declaration order: PThing,
//   then the two vectors (STLport: `if (_M_start) free(_M_start)`), then the base. DestroyOne makes VC7.1 emit the implicit ??1.
#include "fable_compiler.h"
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCUserActionPointBase : public CTCBase {
public:
    std::vector<long>              FreeActionPointIndexes;      // +0x0c
    std::vector<long>              BlockedActionPointIndexes;   // +0x18
    long                           NumberOfActionPoints;        // +0x24
    long                           FrameOfNextCheck;            // +0x28
    CIntelligentPointer<CThing>    PThing;                      // +0x2c
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CTCUserActionPointBase* object)
{
    object->CTCUserActionPointBase::~CTCUserActionPointBase();
}
