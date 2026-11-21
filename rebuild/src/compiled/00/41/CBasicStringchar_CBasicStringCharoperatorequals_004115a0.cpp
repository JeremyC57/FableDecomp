// CBasicString<char>::operator==(const char* text) const  (0x004115A0)
// Leaf: ??8?$CBasicString@D@@QBE_NPBD@Z
// Name fix (NF202, 2026-11-20, q12262): Ego_r.pdb 0x0040C910 / FableWin.pdb 0x01899D90 ??8?$CBasicString@D@@QBE_NPBD@Z; the landed
//   binders 0x00870FC0 (and the others on engine fable_string.h) already bind this spelling. Was the local stand-in
//   ??8CBasicStringChar@@QBE_NPBD@Z (struct CBasicStringChar { char* m_str; }).
// Body: the retail out-of-line copy (88 bytes) has CBasicString<char>::Compare (0x00411570) expanded in place, then tests the
//   three-way result == 0 on each return path (`xor ecx,ecx ; test eax,eax ; sete cl`). The header's Compare template body
//   (FABLE_RETAIL_CBASICSTRING_COMPARE_BODY) plus the compare-result == 0 spelling (FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO)
//   give it; the pick-any ?Compare@?$CBasicString@D@@KIJPBD0@Z COMDAT this object also carries is byte-identical (cmpbody,
//   relocations equal) to the landed owner 0x00411570.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#include "fable_string.h"

template bool CBasicString<char>::operator==(const char* text) const;
