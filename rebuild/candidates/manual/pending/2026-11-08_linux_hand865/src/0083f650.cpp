// LHNetworkLib::LHHttpHeaders::GetHeader(const std::string& name) -- the value of the first received header line that starts (case-insensitively) with name: the text after the first ':' with leading spaces skipped, the whole line when it has no ':' past column 0, or "" when no line matches
// Inline-budget: flag /d2inlS11 -- `return line`'s STLport range initialise (_M_range_initialize<char*> 0x00567D00): retail calls it out of line there and expands it at the loop copy and at `return ""`; VC7.1's caller IL-size cap (default S9) expands all three sites; RELOCATION_MATCH for S11..S20 (S10 = default shape; /d2inlT60 also matches, T50/T70 do not). The __find_if 0x00841900 body is visible in the TU (stlport/fable_string.h opt-in, auto_inline(off), never writes through its tag reference) so the two dispatch-tag temporaries share the result-pointer argument slot as in retail (declared only: the second tag took the name slot, one lea off)
// Leaf: ?GetHeader@LHHttpHeaders@LHNetworkLib@@
// Flags: /d2inlS11
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083F650, 443 bytes (the catalog row stops at 442: the int3 after the trailing noreturn
//   `call abort` at 0x0083F80A belongs to the function), thiscall (hidden result pointer, then name), `ret 8`.
// Identity: Ego_r.pdb ?GetHeader@LHHttpHeaders@LHNetworkLib@@QAE?AV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@ABV34@@Z
//   (the debug build's 0x0084E820); called by LHHttp2::GetHeader 0x00840B60 (landed). The catalog's "ProcessTextWithRegex" is descriptive.
// Callees: malloc 0x00BFEA0E / memmove 0x00BFEAE6 / free 0x00BFEA14 (the inline string copies and destructor),
//   LHNetworkLib::StartsWith 0x0083EE10 (landed; body from engine/LHStringFunctions_bodies.h), search<const char*, const char*,
//   _Eq_traits> 0x00567120 (find(":")), __find_if<const char*, _Not_within_traits> 0x00841900 (find_first_not_of(" "), landed),
//   basic_string::substr 0x00569330 (landed), _M_range_initialize<char*> 0x00567D00 (`return line`'s copy), puts 0x00BFEAF2 +
//   abort 0x00BFEAEC (STLport's length error with exceptions off, inline after the loop copy).
// Data: ":" 0x0124C528, " " 0x01258C1C, "basic_string" 0x0124C5B0.
// Layout / headers: engine/LHHttp2.h (FABLE_RETAIL_LHHTTP2: LHHttpHeaders {vector<string> HdrDocumentHeader +0, string HeaderString
//   +0xc}; the GetHeader declaration is the FABLE_RETAIL_LHHTTP2_GETHEADER opt-in), stlport/fable_string.h (FABLE_RETAIL_STLP_STRING_FIND:
//   find / find_first_not_of and their STLport predicates and algorithms; FABLE_RETAIL_STLP_STRING_THROW_INLINE: the length-error
//   helper in class, expanded here as retail does -- so fable_string_bodies.h is not included).
// Shape: `for (it ...) { string line = *it; if (StartsWith(line, name)) { int colon = line.find(":"); if (colon > 0)
//   return line.substr(line.find_first_not_of(" ", colon + 1)); return line; } } return "";` -- the loop's copy is the inline
//   STLport range initialise (the length-error arm inline), `return line` calls it out of line (the S11 flag), `return ""` folds it
//   (malloc(1), terminator). line's block pointer stays in esi across StartsWith and substr because both bodies (and toLowerCase
//   under StartsWith) are in the TU and never store the string's address.
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_GETHEADER
#define FABLE_RETAIL_STLP_STRING_FIND
#define FABLE_RETAIL_STLP_STRING_THROW_INLINE
#include "engine/LHHttp2.h"
#include "engine/LHStringFunctions_bodies.h"

namespace LHNetworkLib {

std::string LHHttpHeaders::GetHeader(const std::string& name)
{
    for (std::vector<std::string>::iterator it = HdrDocumentHeader.begin(); it != HdrDocumentHeader.end(); ++it) {
        std::string line = *it;
        if (StartsWith(line, name)) {
            int colon = line.find(":");
            if (colon > 0)
                return line.substr(line.find_first_not_of(" ", colon + 1));
            return line;
        }
    }
    return "";
}

}
