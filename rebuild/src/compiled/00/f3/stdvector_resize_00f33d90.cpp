// std::vector<CFireHeartPatternDef>::resize(size_type new_size) -- STLport one-argument resize: forwards to resize(new_size, CFireHeartPatternDef()) with a default-constructed temporary
// Leaf: ?resize@?$vector@VCFireHeartPatternDef@@V?$allocator@VCFireHeartPatternDef@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F33D90, 44 bytes (+4 int3 padding).
// Name fix (agentNF24, land1011de): was the stand-in CDefRecordVector::Resize over a local CDefRecord34 record. The record is
//   CFireHeartPatternDef: its constructor 0x00F33DC0 stores vtable 0x012CCC50 (slots 18/19 the landed CFireHeartPatternDef::Transfer /
//   Copy; Ego_r/FableWin ??0CFireHeartPatternDef@@QAE@XZ, agentFAM08) and its destructor is the landed ??1CFireHeartPatternDef@@UAE@XZ
//   0x00D34F80; the two-argument resize it calls is the landed vector<CFireHeartPatternDef>::resize 0x00F33DE0. Both PDB name sets
//   hold ?resize@?$vector@VCFireHeartPatternDef@@V?$allocator@VCFireHeartPatternDef@@@std@@@std@@QAEXI@Z.
// Callees: CFireHeartPatternDef::CFireHeartPatternDef 0x00F33DC0, vector::resize(size_type, const value_type&) 0x00F33DE0 (out of
//   line: STLport's body, visible, kept out of line as in the retail TU), CFireHeartPatternDef::~CFireHeartPatternDef 0x00D34F80.
// Shape: STLport 4.6 `void resize(size_type __new_size) { resize(__new_size, _Tp()); }` emitted out of line (speed TU: the count
//   is reloaded into eax and pushed); the temporary is the constructor's returned `this` (`push eax`); no EH frame.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "engine/CFireHeartPatternDef.h"

template void std::vector<CFireHeartPatternDef>::resize(std::vector<CFireHeartPatternDef>::size_type);
