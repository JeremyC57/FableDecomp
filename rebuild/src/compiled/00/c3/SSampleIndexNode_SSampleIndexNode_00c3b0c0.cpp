// SSampleIndexNode::SSampleIndexNode(void* value, unsigned long index) -- constructs an 8-byte sample-index-map node: the value at +0, the index at +4
// Leaf: ??0SSampleIndexNode@@QAE@PAXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3B0C0, 18 bytes, __thiscall `ret 8`, returns `this`:
//   mov edx,[esp+8] ; mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],ecx ; mov [eax+4],edx ; ret 8
// Identity (NF59, round 7, queue q10547): libpk ships no symbols; libpk/CBankProcessor.h's descriptive node SSampleIndexNode (opt-in
//   FABLE_RETAIL_CSAMPLEINDEXMAP_ADD, "ctor 0x00C3B0C0") is the node its only caller, CSampleIndexMap::Add 0x00C3B0E0 (call 0x00C3B13D),
//   now binds (NF57 q10484). Re-authored on that header: the landed row defined the address-suffixed stand-in SIndexNode_00c3b0e0
//   (sweep1 naming; earlier a propagated CDateAndTime label). The twin 0x00C3AB30 belongs to the other index-map copy.
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#define FABLE_RETAIL_CSAMPLEINDEXMAP_ADD
#define FABLE_RETAIL_CSTRING_BUILDERS
#include "libpk/CBankProcessor.h"

SSampleIndexNode::SSampleIndexNode(void* value, unsigned long index)
    : m_pValue(value), m_index(index)
{
}
