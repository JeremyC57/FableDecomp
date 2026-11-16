// CPersistContext::TransferVector<CDefString>(const char*, std::vector<CDefString>&) -- the named vector transfer: CheckCRC(name), then the
// Mode switch (clear / binary save / binary load / text save / text load through a CPersistContext_TransferableVector<CDefString> wrapper)
// Leaf: ??$TransferVector@VCDefString@@@CPersistContext@@QAEXPBDAAV?$vector@VCDefString@@V?$allocator@VCDefString@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F333B0 (right after CScriptDef::Transfer).
// NF39 (2026-11-16): moved off its local stand-in (CPersistContext::TransferVector<long> spelling) onto the shared engine/CPersistContext.h
//   (FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY, whose comment names this instance 0x00F333B0): Ego_r.pdb / FableWin.pdb
//   ??$TransferVector@VCDefString@@@CPersistContext@@...; its 12 call sites in CScriptDef::Transfer 0x00F2D6E0 pass std::vector<CDefString>
//   members (engine/CScriptDef.h) and the landed 0x00F2D6E0 already binds this symbol. Preamble from the landed 0x00F2D6E0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS_NAMED
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_EMPTY_DTOR
#define FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS_CONST_DEFAULTVAL
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"
#include "fable_string.h"
#include "engine/CDefString.h"
#include "fable_persist_context.h"

template void CPersistContext::TransferVector<CDefString>(const char* name, std::vector<CDefString, std::allocator<CDefString> >& value);
