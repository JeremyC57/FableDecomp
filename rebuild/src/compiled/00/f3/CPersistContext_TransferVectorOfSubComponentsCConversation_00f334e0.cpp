// CPersistContext::TransferVectorOfSubComponents<CConversation>(const char* name, std::vector<CConversation>& value) -- persist the conversation sub-defs of a vector by name: text load through the transferable-vector wrapper, binary load (count, resize, each element), binary save (count, each element) or a per-element pass
// Leaf: ??$TransferVectorOfSubComponents@VCConversation@@@CPersistContext@@QAEXPBDAAV?$vector@VCConversation@@V?$allocator@VCConversation@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F334E0, 380 bytes incl. the 5-entry mode jump table at 0x00F33648 (entry 1 is 0), thiscall `ret 8`;
//   speed TU (esp frame). Split: the catalog row (528 bytes, "CPersistContext::Transfer<signed_char>") also covers the wrapper's
//   virtual Size 0x00F33660 / AreDefaultValsApplied 0x00F33680 / SetDefaultValsApplied 0x00F336A0 / Transfer 0x00F336D0 (SPLITS.tsv).
// Identity (proven): Ego_r.pdb / FableWin.pdb ??$TransferVectorOfSubComponents@VCConversation@@@CPersistContext@@QAEXPBDAAV?$vector@VCConversation@@...@Z;
//   (1) element stride 0x5C (`imul` by 0xB21642C9, `sar 6`; `add reg,5Ch`) = sizeof(CConversation) (Ego_r 108 minus the 16 bytes of four
//   12-byte STLport vectors); (2) the default element's out-of-line ctor 0x00F336F0 calls CDefClassBase() 0x0042D9E0 and zeroes the four
//   vectors at +0x2C..+0x58, its dtor 0x00E54D80 destroys four CCharString vectors (Speaker / Dialogue / Animation / AnimLoop); (3) the
//   text wrapper's vtable 0x01301C2C is followed by its CConversation instances (Ego_r ?Size@?$CPersistContext_TransferableVectorOfSubComponents@VCConversation@@@@...);
//   the next function 0x00F337A0 is vector<CConversation>::erase; (4) the only caller 0x00F2F77A passes the key "BookReactions" (0x012FFF90).
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   CConversation::CConversation() 0x00F336F0, std::vector<CConversation>::resize(unsigned int, const CConversation&) 0x00F33730,
//   CConversation::~CConversation() 0x00E54D80, CPersistContext::TransferVectorOfSubComponentsLoadText 0x009BC120, the element's virtual
//   Transfer(CPersistContext&) (CDefClassBase slot 18, +0x48).
// Shape: the shared engine/CPersistContext.h named body (FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS_NAMED) instantiated for
//   CConversation (engine/CConversation.h).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS_NAMED
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CConversation.h"
#include "engine/CPersistContext.h"

template void CPersistContext::TransferVectorOfSubComponents<CConversation>(const char* name, std::vector<CConversation>& value);