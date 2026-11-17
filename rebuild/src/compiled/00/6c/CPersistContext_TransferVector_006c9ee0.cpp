// CPersistContext::TransferVector<std::pair<long, COpinionDeed*> >(const char* name, std::vector<std::pair<long, COpinionDeed*> >& value) -- persist one vector member ("OpinionDeedMap"): CRC-check the key, then by mode reset to empty / binary save / binary load / text save / text load
// Leaf: ??$TransferVector@U?$pair@JPAVCOpinionDeed@@@std@@@CPersistContext@@QAEXPBDAAV?$vector@U?$pair@JPAVCOpinionDeed@@@std@@V?$allocator@U?$pair@JPAVCOpinionDeed@@@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C9EE0 (the switch table follows the code), `ret 8`. Called from CDeedManager::TransferState (0x006C7370) with the key "OpinionDeedMap".
// Name (NF70, q10748): the PDB member template ??$TransferVector@<T>@CPersistContext@@QAEXPBDAAV?$vector@<T>V?$allocator@<T>@std@@@std@@@Z
//   (Ego_r.pdb / FableWin.pdb, engine/CPersistContext.h) for T = std::pair<long, COpinionDeed*>, the vector element the caller builds
//   with std::__copy 0x006C9B70 from its map<long, COpinionDeed*> (back_insert_iterator<vector<pair<long, COpinionDeed*> > >).
//   Was a local stand-in CPersistContext::Transfer<COpinionDeedRecord> on a local vector model.
// Body: the shared FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY (the 0x00F333B0 / 0x00412E80 instances): CheckCRC 0x00404500,
//   closed switch on Mode with arms in retail order 4,3,2,1,0, `value.clear()` out of line (0x006C85E0),
//   GFSerialiseVectorBinaryOut<T> 0x006C7BA0 / GFSerialiseVectorBinaryIn<T> 0x006C9E40 (`__fastcall`), and the
//   CPersistContext_TransferableVector<T> wrapper (vtable 0x0125E9DC) to TransferVectorSaveText 0x009BAC80 / TransferVectorLoadText 0x009BB110.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/fable_nscript_forward.h"
#include "engine/CPersistContext.h"
class COpinionDeed;
template void CPersistContext::TransferVector<std::pair<long, COpinionDeed*> >(const char*, std::vector<std::pair<long, COpinionDeed*>, std::allocator<std::pair<long, COpinionDeed*> > >&);
