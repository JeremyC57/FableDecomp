// CPersistTraits<EOpinion>::TransferIn(CPersistContext& context, CStringParser& parser, EOpinion& value) -- the generic text load for the enum:
// read a long through CPersistTraitsSLONG::TransferIn and store it as the enum
// Leaf: ?TransferIn@?$CPersistTraits@W4EOpinion@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAW4EOpinion@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00679910, 31 bytes, `ret 0Ch` (the empty traits `this` is unused; the long temporary is homed in the
//   parser-argument slot once the parser is in edx). Speed-optimised (default) TU.
// NF136 (2026-11-20, q5199 / q5220): was a local CPersistTraits<EInputKey> specialisation with pointer parameters calling the free
//   TransferIn(CPersistContext*, CStringParser*, long*) declaration of 0x009E2590 (catalog CPersistTraits<EInputKey>::TransferIn, propagated).
//   Identity: its only landed caller 0x00678760 binds ?TransferIn@?$CPersistTraits@W4EOpinion@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAW4EOpinion@@@Z
//   here (exe scan + reloc_bindings.tsv), the FableWin.pdb generic member of CPersistTraits<EOpinion>. Re-authored as the explicit instantiation
//   of the generic inline body of engine/CPersistTraits.h (the 0x009E9080 recipe), on engine/EOpinion.h; 0x009E2590 now binds the Ego_r.pdb /
//   FableWin.pdb ?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCStringParser@@AAJ@Z.
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/EOpinion.h"

template void CPersistTraits<EOpinion>::TransferIn(CPersistContext& context, CStringParser& parser, EOpinion& value);
