#include "engine/C3DMeshFileMaterialChunk.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
// C3DMeshFileMaterialChunk_Methods::HasExtendedProperties @ 0x00abfcd0
// push STR; call helper; xor ecx,ecx; cmp ecx,eax; sbb eax,eax; neg eax; ret
// Return whether a named property lookup returned a non-null result.
// Literal (NF418, 2026-11-25): was extern data `kExtPropName_abfcd0` (table ?kExtPropName_abfcd0@@3QBDB -> 0x0129EDCC); now spelled at the use site:
//   "MTLE", retail's pooled literal at 0x0129EDCC (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).
int FABLE_STDCALL FableHasExtHelper_abfcd0(const char *name);
struct C3DMeshFileMaterialChunk_Methods : C3DMeshFileMaterialChunk {
    bool HasExtendedProperties();
};
bool C3DMeshFileMaterialChunk_Methods::HasExtendedProperties()
{
    return (unsigned int)FableHasExtHelper_abfcd0("MTLE") > 0;
}