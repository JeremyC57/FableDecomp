# Landing nf220 (Account A, 2026-11-21): 0x0099C120 onto fable_string.h, and the size-TU operator== copies (3 rows)

Agent NF220 (owner: "we can start a new agent when one lands / finishes"). Hand-back: `pending/2026-11-21_nf220_handback/`.

- 3 rows land, all RELOCATION_MATCH + PASS: 0x0099C120 (q12401), 0x005A4CDD and 0x005A5B31 (q12383, part).
- Recheck (`logs/recheck_summary.txt`): 4,257 rows = the direct includers of fable_string.h, the rows and the CWideString / operator==
  owners: 3,662 RELOCATION_MATCH, 593 MATCH, 2 DIFFER = the known bare-CompilerFlags artifact (0x00DA8510 / 0x00E0A510).
- Header (add-only, fable_string.h, 7 opt-ins; default path preprocesses identically): CWIDESTRING_COPYCTOR_INLINE, _DTOR_INLINE,
  _ADDCHAR, _MAKEOWNCOPYOFDATA, CWIDESTRINGDATA_CTOR_TEXT, _APPEND_CHAR, CBASICSTRING_TEXT_EQ_OWNER_BODY (size-pragma scaffolding,
  EXTRA_RULES 2026-11-20; the rows' row-head optimize("s") sits after the include, the nf189 placement).
  - Coordinator change: CWIDESTRING_DTOR_INLINE uses the owner row 0x0099B510's own text. Its pick-any copy then equals retail (68 = 68);
    NF220's shorter body gave a differing 65-B copy. 0x0099C120's bytes are the same with either.
- q12401: 0x0099C120 is ported off its local CWideString model; its UnassignString copy is now the pick-any 62-B body equal to
  0x0099B4D0, so the link loses that duplicate (`logs/link_check.txt`: 423 -> 422). Its strong same-TU operator+= / AllocStringData
  copies already differed from their owners at HEAD and still do (EXTRA_RULES nf203: debt). MakeOwnCopyOfData is unchanged.
- q12383: the two rows' CBasicString<char>::operator==(const char*) pick-any copy goes from 19 B to the owner 0x004115A0's 88-B body
  (equal to retail). `??8CCharString@@QBE_NPBD@Z` grows 35 -> 39 B in both; it is referenced only by .debug$F, with no retail copy to
  compare (debt). `__copy_trivial` (cdecl, unbound) differs from retail before and after; NF220's note that it now equals 0x00405BA0
  is wrong under the catalog flags (0x00405BA0 is a 30-B fastcall function). `logs/pickany_vs_retail.txt`, `logs/comdat_diff.txt`.
- reloc_bindings: no change.
- Not landed / queued (land1121nf220): q12226-q12229 (the ReleaseData family is about 40 rows, one component; NF220's census and
  five-step plan in its NOTES), the CComponent ??1 census (4 rows bind ??1 at 0x004291DE; the table line `??1 -> 0x004291DE`, witness
  0x00570332, is stale), the remaining q12383 rows 0x005A04F7 (compile error when fable_string.h goes first), 0x00CBFB7D, 0x0065A075.
