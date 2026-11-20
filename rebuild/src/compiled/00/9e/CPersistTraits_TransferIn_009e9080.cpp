// CPersistTraits<unsigned char>::TransferIn(CPersistContext& context, CStringParser& parser, unsigned char& value) -- the generic text load
// for an unsigned char: read a long through CPersistTraitsSLONG::TransferIn and store it cast to the byte.
// Leaf: ?TransferIn@?$CPersistTraits@E@@QAEXAAVCPersistContext@@AAVCStringParser@@AAE@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9080, 31 bytes, `ret 0Ch` (the empty traits `this` is unused; the long lives in the
//   dead parser argument slot).
// Port (NF133, q5200): the local CPersistTraits_uchar stand-in and the free PersistByteHelper declaration replaced by the shared
//   engine/CPersistTraits.h: the generic inline CPersistTraits<T>::TransferIn body (SLONG read into a long, then the cast store) explicitly
//   instantiated for unsigned char (Ego_r.pdb 0x00404E70 / FableWin.pdb 0x018BE8D0
//   ?TransferIn@?$CPersistTraits@E@@QAEXAAVCPersistContext@@AAVCStringParser@@AAE@Z; the landed caller 0x009E8C10 binds it), and the
//   callee 0x009E2590 bound as FableWin.pdb ?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCStringParser@@AAJ@Z (the landed
//   self spelling of 0x009E2590).
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"

template void CPersistTraits<unsigned char>::TransferIn(CPersistContext& context, CStringParser& parser, unsigned char& value);
