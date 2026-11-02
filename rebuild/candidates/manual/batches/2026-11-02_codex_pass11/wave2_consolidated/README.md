# Codex pass 11 -- wave 2: 28 rows on consolidated per-class headers (2026-11-02)

The 36 verified Codex rows that needed header work went to a consolidation agent (owner decision 2026-11-02). It rewrote Codex's per-feature
header macros as one FABLE_RETAIL_<CLASS> branch per class on the current headers, folded Codex's second models into their existing homes
(engine/CCharString.h -> fable_string.h FABLE_RETAIL_CCHARSTRING; engine/CWideString.h -> fable_string.h FABLE_RETAIL_CWIDESTRING;
engine/CDefStringTable.h -> fable_definition_table.h FABLE_RETAIL_CDEFSTRINGTABLE; stlport46/*.h -> one STLport model stlport/fable_containers.h +
fable_map.h), and reduced every multi-version header to one model. HEADERS_README.md lists every header, its macro and the evidence.

28 rows land (`verify.log`: 28 WINS, all RELOCATION_MATCH + PASS; consolidation_grades.tsv, STATUS.tsv). Headers: 29 existing files changed, 77
new (rebuild/include). Default-path proof (coordinator rerun of hdrcheck.py): all 29 changed existing headers `default=same`; the only existing
retail branches whose text changed are additive (FABLE_RETAIL_CCHARSTRING, CTEXTURE, CSIMPLEMEMORYPOOL, NSCRIPT: CScriptThing gains NullCoord).
Includer rechecks with the overlay (agent): the 4 landed rows defining CCHARSTRING / CTEXTURE / CSIMPLEMEMORYPOOL and all 3,344 landed
FABLE_RETAIL_NSCRIPT sources, 3,344 / 3,344 PASS (134 MATCH + 3,210 RELOCATION_MATCH; includer_recheck_NSCRIPT_SUMMARY.txt); coordinator spot
recheck on the landed repo headers in includer_spot_recheck.tsv. Every `#pragma pack` in the changed headers was already in the repo copy; no
new packing, asm, volatile or inline_depth.

Not done: 0x0047A9F0, 0x00606300, 0x00666EF0, 0x00667450, 0x00667D30, 0x007355F0, 0x0074A5D0, 0x00775C60 (the CThing-family rows: each needs one
merged retail model of CThingPhysical / CThingGameObject / CThingCreatureBase / CTCBase / CWorld / CPlayerManager / CDefinitionManager; NOTES.md
"Header debt"). Their Codex sources stay in pending/2026-11-02_codex_pass11/verified_pending/.

Coordinator decisions (NOTES.md): FABLE_RETAIL_CCOUNTEDPOINTER keeps retail's member test (`if (Data)`) gated (only 0x00626E40 opts in; making it
the default needs a ~7k-includer recheck); FABLE_RETAIL_CPARENTDEFCLASSBASE hides the declared dtor (compiler-generated, as CLEVELSCRIPTBASE);
the three script rows define CScriptThing::operator= inline after the include (the header's contract); the repo's pointer members are kept
(CLevelScriptBase Game, CTCBase ParentThing). 43 second spellings (SECOND_SPELLINGS.tsv) queued PRIORITY as cx11. Labels:
ghidra_out/labels_codex_pass11c_2026-11-02.tsv.
