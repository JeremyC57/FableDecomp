// LHNetworkLib::LHHttpHeaders::Reset() -- empties the prepared header string and erases every stored document header line
// Leaf: ?Reset@LHHttpHeaders@LHNetworkLib@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083FBC0, 37 bytes, __fastcall, plain `ret`. Ego_r.pdb ?Reset@LHHttpHeaders@LHNetworkLib@@QAEXXZ.
// Callees: STLport basic_string<char>::assign(const char* first, const char* last) 0x005693B0 (the empty literal 0x0122D70E
//   as both bounds, as the landed LHHttp2::ResetRequest 0x0083F980); vector<basic_string<char> >::erase(iterator first,
//   iterator last) 0x00841FC0 (manifest "std::vector_move_and_cleanup": moves the tail down, frees the dead strings).
// Callers: 0x0055FB45, 0x00569C33, 0x00569D6D, 0x008409DD.
// Layout: retail LHHttpHeaders (Ego_r.pdb names, 12-byte STLport containers): HdrDocumentHeader vector +0, HeaderString +0xc.
// Shape: HeaderString = "" first, then the inline clear() as erase(begin(), end()). The manifest label
//   "SetStaticCast<NUISystem::CComponent>" is propagated and wrong.
// NF123 (2026-11-20, q4193): the local CRetailString / CRetailStringVector stand-ins replaced by the shared engine/LHHttp2.h
//   LHHttpHeaders (STLport std::vector<std::string> / std::string); its SCOREBOARD opt-in declares Reset out of line, as retail has it.
//   The erase call now binds the STLport spelling of 0x00841FC0 (was ?erase@CRetailStringVector@@QAEPAUCRetailString@@PAU2@0@Z).
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_SCOREBOARD
#include "engine/LHHttp2.h"

namespace LHNetworkLib {

void LHHttpHeaders::Reset()
{
    HeaderString = "";
    HdrDocumentHeader.clear();
}

}
