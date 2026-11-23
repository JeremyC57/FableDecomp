// Leaf: ?IsDiskFile@CLipSyncDataBank@@MAE_NKK@Z
// Header route (NF319, 2026-11-23): defined on the real class of engine/CLipSyncDataBank.h (was ?IsDiskFile@CLipSyncDataBank_Methods@@QAE_NKK@Z); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CLIPSYNCDATABANK_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CLIPSYNCDATABANK_CLASS
#define FABLE_RETAIL_CLIPSYNCDATABANK_RTTI_DECLS
// CLipSyncDataBank_Methods::IsDiskFile @ 0x00a427c0
// xor al,al; ret 8  -> always returns false (2 stack args, thiscall)
#include "engine/CLipSyncDataBank.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
bool CLipSyncDataBank::IsDiskFile(unsigned long a, unsigned long b) {
    return false;
}