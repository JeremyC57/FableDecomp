// std::vector<long>::resize(size_type n, const long& x) -- STLport 4.6: shrinking erases [begin()+n, end()) (the inline copy-down:
// memmove of the empty tail, then `last` stored), growing calls _M_fill_insert(end(), n - size(), x) out of line (0x00440F00).
// Leaf: ?resize@?$vector@JV?$allocator@J@std@@@std@@QAEXIABJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004428F0, 87 bytes, `ret 8`. Callees: memmove 0x00BFEAE6, _M_fill_insert 0x00440F00.
// Naming (agentNF32, 2026-11-12; was the placeholder CRetailVector_004428f0::Resize_CRetailVector_004428f0): DESCRIPTIVE-BY-LIBRARY.
//   Retail is STLport 4.6; FableWin.pdb's ?resize@?$vector@JV?$allocator@J@std@@@std@@QAEXIJ@Z is its later Dinkumware build (value
//   by copy), so no PDB carries this exact spelling. Evidence: the body is the shared stlport/fable_containers.h resize (const&
//   value, STLport 4.5.2/4.6 form); the landed 0x004433F0 binds this address as ?resize@?$vector@JV?$allocator@J@std@@@std@@QAEXIABJ@Z
//   (rebuild/oracles/reloc_bindings.tsv), and 0x0055F710's vector<char> resize 0x004122D0 is the same header instantiation.
#include "stlport/fable_containers.h"

template void std::vector<long>::resize(unsigned int, const long&);
