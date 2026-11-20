// LHNetworkLib::LHHttpHeaders::AddHeader(const string& name, const string& value) -- appends one "name: value" header line (a ':' added unless the name already ends in one) to the header list
// Leaf: ?AddHeader@LHHttpHeaders@LHNetworkLib@@QAEXABV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083FC00, 219 bytes, __fastcall, `ret 8`, no EH frame (the LHNetworkLib TU).
// Identity: Ego_r.pdb ?AddHeader@LHHttpHeaders@LHNetworkLib@@QAEXABV?$basic_string@...@std@@0@Z (QAEX: void, two
//   const string&). Next to LHHttpHeaders::Reset (landed 0x0083FBC0). The manifest label "HTTP_Request_BuildPath" is
//   an autoname.
// Callees: STLport basic_string<char>::_M_range_initialize(const char*, const char*, const forward_iterator_tag&)
//   0x00567D00 (the copy constructor, twice), append<const char*>(f, l, const forward_iterator_tag&) 0x00567D60 (":" at
//   0x0124C528, " " at 0x01258C1C), append<char*> 0x00567400 (+= value), vector<string>::_M_insert_overflow(pos, x,
//   const __false_type&, 1, true) 0x00569E30, free 0x00BFEA14 (~header).
// Layout: LHHttpHeaders {vector<string> HdrDocumentHeader +0, string HeaderString +0xc} (landed Reset 0x0083FBC0).
// Shape: `string header(name); if (name[name.size() - 1] != ':') header += ":"; header += " "; header += value;
//   HdrDocumentHeader.push_back(header);` -- the shared engine/LHHttpHeaders_bodies.h body on engine/LHHttp2.h, with the STLport
//   string members of stlport/fable_string_bodies.h in the TU (out of line under auto_inline(off), as 0x008400B0 / 0x00840740):
//   retail keeps `header` unaliased (header._M_start loaded once into edi and reused by the copy and the destructor).
#define FABLE_RETAIL_LHHTTP2
#include "engine/LHHttp2.h"
#include "stlport/fable_string_bodies.h"
#include "engine/LHHttpHeaders_bodies.h"
