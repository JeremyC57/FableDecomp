// CPersistContext::TransferVector<std::pair<CCharString, long> >(const char* name, std::vector<std::pair<CCharString, long> >& value) -- persist one vector member ("GiftsReceived"): CRC-check the key, then by mode reset to empty / binary save / binary load / text save / text load
// Leaf: ??$TransferVector@U?$pair@VCCharString@@J@std@@@CPersistContext@@QAEXPBDAAV?$vector@U?$pair@VCCharString@@J@std@@V?$allocator@U?$pair@VCCharString@@J@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D4C00, 188 bytes incl. the 5-entry jump table, `ret 8`.
// Name (NF118, PRIORITY q4703): the PDB member template ??$TransferVector@<T>@CPersistContext@@QAEXPBDAAV?$vector@<T>V?$allocator@<T>@std@@@std@@@Z
//   (Ego_r.pdb / FableWin.pdb, engine/CPersistContext.h) for T = std::pair<CCharString, long>: the landed caller 0x006D2BD0 binds
//   exactly this instance for its "GiftsReceived" member (Ego_r struct_layouts: vector<pair<CCharString,long> >). Was a local
//   CPersistContext / vector model instantiated on an invented element type CNamedLong (TransferVector<CNamedLong>).
// Body: the shared FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY (the 0x006C9EE0 / 0x00F333B0 instances): CheckCRC 0x00404500,
//   closed switch on Mode with arms in retail order 4,3,2,1,0; the reset is the inline vector::clear() calling the out-of-line
//   erase(first, last) 0x006D44F0 (result dropped); GFSerialiseVectorBinaryOut<T> 0x006D3A30 / GFSerialiseVectorBinaryIn<T> 0x006D4B40
//   (`__fastcall` stream, vector) and the CPersistContext_TransferableVector<T> wrapper (vtable 0x0125ED48) to TransferVectorSaveText
//   0x009BAC80 / TransferVectorLoadText 0x009BB110.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/fable_nscript_forward.h"
#include "fable_string.h"
#include "engine/CPersistContext.h"
template void CPersistContext::TransferVector<std::pair<CCharString, long> >(const char*, std::vector<std::pair<CCharString, long>, std::allocator<std::pair<CCharString, long> > >&);
