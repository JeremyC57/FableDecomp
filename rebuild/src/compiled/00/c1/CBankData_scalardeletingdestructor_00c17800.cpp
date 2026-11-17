// CBankData::`scalar deleting destructor'(unsigned int flags) -- run ~CBankData, free the block when bit 0 of the flags is set, return this
// Leaf: ??_GCBankData@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17800, 30 bytes (`ret 4`).
// Name (round 7, NF53): the callee 0x00C2EF30 is the landed ??1CBankData@@QAE@XZ (descriptive libpk name; libpk/CBankProcessor.h declares
//   CBankData with its out-of-line constructor and destructor); VC7.1 emits this scalar deleting destructor for the non-virtual destructor
//   when a bank data is deleted (checked with a scratch TU), so the row forces it with a Kill_ helper (the 0x00ADBE50 pattern). Was the free
//   stand-in CLandscapeBackgroundPatch_vector_deleting_destructor over a local CLandscapeBackgroundPatch and CLandscapeBackgroundPatch_dtor_00c2ef30.
//   Descriptive libpk names (no libpk PDB): one address, one name with the self row and the header.
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/CBankProcessor.h"

void Kill_00c17800(CBankData* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
