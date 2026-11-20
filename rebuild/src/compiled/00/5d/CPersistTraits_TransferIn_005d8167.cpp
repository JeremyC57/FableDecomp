// CPersistTraits<long>::TransferIn(CPersistContext& context, CStringParser& parser, long& value) -- the generic text load for a long: read it
// through CPersistTraitsSLONG::TransferIn and store it
// Leaf: ?TransferIn@?$CPersistTraits@J@@QAEXAAVCPersistContext@@AAVCStringParser@@AAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D8167, 30 bytes, `ret 0Ch` (size-optimised TU; the empty traits `this` is unused; the long lives in the
//   dead parser argument slot).
// NF136 (2026-11-20, q5199 / q5220): was the free stand-in CPersistTraits_TransferIn(CPersistTraits*, int, CPersistContext*, CStringParser*,
//   EInputKey*) calling the free Persist_ReadValue declaration of 0x009E2590 (catalog CPersistTraits<EInputKey>::TransferIn, propagated).
//   Identity: its only landed caller 0x009E8C10 binds ?TransferIn@?$CPersistTraits@J@@QAEXAAVCPersistContext@@AAVCStringParser@@AAJ@Z here
//   (exe scan + reloc_bindings.tsv), the FableWin.pdb generic member of CPersistTraits<long>. Re-authored as the explicit instantiation of the
//   generic inline body of engine/CPersistTraits.h (the 0x009E9080 CPersistTraits<unsigned char> recipe); 0x009E2590 now binds the
//   Ego_r.pdb / FableWin.pdb ?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCStringParser@@AAJ@Z.
#pragma optimize("s",on)
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"

template void CPersistTraits<long>::TransferIn(CPersistContext& context, CStringParser& parser, long& value);
