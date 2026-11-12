// std::map<long, CEngineStaticMapFileBlockManager::CFileBlock >::map() -- STLport 4.6 container default constructor: the member _Rb_tree built with _Compare() and allocator_type() (header node allocated, red, self-linked)
// Leaf: ??0?$map@JUCFileBlock@CEngineStaticMapFileBlockManager@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJUCFileBlock@CEngineStaticMapFileBlockManager@@@std@@@4@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B442E0, 54 bytes; node 0x24.
// Identity (proven, agentFAM07): the next COMDAT 0x00B44320 is this container's destructor and calls _M_erase 0x00B441F0, landed as the map<long, CEngineStaticMapFileBlockManager::CFileBlock> (engine/CEngineStaticMapFileBlockManager.h: Blocks)
//   _M_erase; this constructor's symbol is in the Ego_r / FableWin PDB name sets; node 0x24.
//   Value layout: Ego_r.pdb member names and types at the retail sizes (only sizeof matters here).
//   Was the placeholder CUnprovenContainer_00b442e0::CUnprovenContainer_00b442e0 (name-only change: the same STLport text, the per-instantiation placeholder key/value
//   types replaced by the proven ones).
// Model (agentNF24 port of agentFAM07's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CEngineStaticMapFileBlockManager.h (nested CFileBlock) replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CCPPointerInfo;
#include "stlport/fable_map.h"
#include "engine/CEngineStaticMapFileBlockManager.h"

template std::map<long, CEngineStaticMapFileBlockManager::CFileBlock>::map();
