// 004014a0 CCRC::Calc, reflected CRC-32 with a caller-supplied seed.
// Leaf: ?Calc@CCRC@@SIKKPBXK@Z
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_CCRC_ORIGINAL_TYPED_LOOP_PASS27
#define FABLE_RETAIL_CRC32TAB_PASS29
#include "engine/CCRC.h"

// Fidelity: scaffolding, not retail. FableWin marks CCRC::Calc inline (S_FRAMEPROC `marked inline`, main.obj and
// Engine_Water_Sea_Generator.obj), so the body is the class-inline member in engine/CCRC.h (NF86/NF88 form).
// The isolated VC7.1 owner build requires a reference to emit this class-inline member.
// This private reference has no executable body.
#if defined(__GNUC__) || defined(__clang__)
__attribute__((used))
#endif
static unsigned long (FABLE_FASTCALL &StandaloneCalcDefinition)(
    unsigned long, const void*, unsigned long) = CCRC::Calc;
