// CSampleIndexMap::Add(CLoadLog* log, void* value, unsigned long index) -- register a value under a numeric index: when the index is already present (slot-17 lookup) append "Index <n> already exists!" to a fresh line of the load log, otherwise insert a new {value, index} node (slot 9; a failed allocation inserts null)
// Leaf: ?Add@CSampleIndexMap@@QAEXPAVCLoadLog@@PAXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c3b0e0, 122 bytes (+ int3 padding), __fastcall `ret 0Ch`.
// Library: libpk audio bank loader (no PDB/RTTI); descriptive names. The map is a member of the bank reader (caller
//   0x00C2E25D passes the reader's log, a value and the record's index); its twin instantiation is 0x00C3AB50.
//   The manifest's "Container_AddItem_Checked2" is propagated.
// Callees: the map's own virtual slots 17 (+0x44, lookup by index address) and 9 (+0x24, insert); scalar operator new
//   0x00BFEA1A (8-byte node), the node constructor 0x00C3B0C0 (value, index); the log's NewMessage 0x00C307E0 (landed as
//   "CScriptHandleRegistry::CreateNewInstance": a 12-byte node whose embedded MFC-style string starts empty, returned
//   by address); the library string's __fastcall stream operators 0x00C0DEC0 (text) and 0x00C0DF10 (unsigned number).
// Strings: "Index " @ 0x012B2B10, " already exists!" @ 0x012B54D4.
// Shape: the index is looked up through its argument slot (`lea ecx,[esp+10h]`); the message chain reuses each
//   returned stream pointer; `new` + constructor is null-checked and the result (or 0) goes straight to Insert.
void* __cdecl operator new(unsigned int size);                 // 0x00BFEA1A

// NF53: the log is the shared libpk CLoadLog (libpk/CScriptHost.h: ?AddMessage@CLoadLog@@QAEAAVCString@@XZ 0x00C307E0, returning
//   the new line's CString by reference) and the string the shared engine/CString.h (FABLE_RETAIL_CSTRING_BUILDERS appenders);
//   was the CMessageLog_00c3b0e0 / local CString stand-ins.
// agentNF57 (q10484): the class is the bank processor's sample map, which libpk/CBankProcessor.h already names CSampleIndexMap
//   (CBankProcessor::m_samples at +0x3C; descriptive libpk name): the landed AddSample 0x00C2E190 calls this Add on this+0x3C,
//   and AddDriver 0x00C2E2B0 calls its neighbour Get 0x00C3B160 on the same +0x3C map. Slot 17 is the header's
//   FindByKey(const unsigned long&) and slot 9 PKCTRList<T>::Add(T*). Was the address-suffixed CIndexMap_00c3b0e0 /
//   SIndexNode_00c3b0e0 stand-ins; the node is the header's SSampleIndexNode (opt-in FABLE_RETAIL_CSAMPLEINDEXMAP_ADD).
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#define FABLE_RETAIL_CSAMPLEINDEXMAP_ADD
#define FABLE_RETAIL_CSTRING_BUILDERS
#include "libpk/CBankProcessor.h"

void CSampleIndexMap::Add(CLoadLog* log, void* value, unsigned long index)
{
    if (FindByKey(index)) {
        log->AddMessage() << "Index " << index << " already exists!";
        return;
    }
    Add(new SSampleIndexNode(value, index));
}
