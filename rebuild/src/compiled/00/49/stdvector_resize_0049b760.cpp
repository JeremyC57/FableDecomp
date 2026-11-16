// std::vector<CCharString>::resize(size_type) -- STLport one-argument resize: resize(n, CCharString()) with the empty fill value
//   destroyed on both arms (erase the surplus when shrinking, else fill-insert at the end)
// Leaf: ?resize@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049B760, 101 bytes, `ret 4`. Callees: CCharString() 0x0099E4B0, the range erase
//   0x0043336A and _M_fill_insert 0x004333B3 (both out of line, ICF-shared by the CCharString vector), ~CCharString 0x0099EAE0.
// Name: descriptive-by-library (STLport 4.6; both PDBs are Dinkumware builds): the landed binders 0x009CE5E0 CBankFile::AddEntryMultipleFiles
//   and 0x00CBFB7D RunCutsceneMacro bind this symbol. Emitted from the shared stlport/fable_containers.h vector (NF33 2026-11-16; was the
//   stand-in ?resize@CCharStringVector@@QAEXI@Z), like vector<CTexture>::resize 0x0049B7F0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
template void std::vector<CCharString, std::allocator<CCharString> >::resize(std::vector<CCharString, std::allocator<CCharString> >::size_type);
