# Landing nf318 (Account A, 2026-11-23): 88 header-route rows onto the real member of their shared-header class

Hand-back: `pending/2026-11-23_nf318_handback/`; helpers under `naming_batch/agent_tools/nf318/`. Entries: the 122 rejects of
`alias_header.py` (alias1, 93) and `flatten_pilot.py --switch` (flatten-switch1, 29). Outcome: 88 done, 3 held, 31 skipped-port.

- **Landed: 88 rows.** 60 alias rows (`X_Methods::M` -> `X::M`) and 28 flattened rows (CGameScriptInterface 23, CCreatureActionBase 4,
  CAIStateGroupBase 1) now define the real member of the class in `engine/X.h`, through switches that landed rows already define.
  - **No shared header edited.** 5 tests are new or moved: the 4 LINK_FAIL flatten rows and 0x00A19AE0.
  - **Proof (agent, re-done at landing):**
    - precheck; vland HEAD and new; MATCH 38 / RELOCATION_MATCH 51 PASS;
    - objsec with exactly one DEF rename (old -> PDB spelling), equal UND sets, no `??_7` / `??_R`, and every non-debug section equal;
    - `.drectve` equal for 53 rows, or HEAD plus exactly `/DEFAULTLIB:"libcp"` for 36 (the ruling below); a planted DIFFER control was
      flagged;
    - link of the HEAD and new sets: no duplicates, and nothing HEAD resolved goes unresolved. CScriptThing was also linked against its 19
      in-TU NScript rows.
  - **Identity:** the retail RTTI placement names the PDB symbol at every address. At landing, every object defines its PDB symbol (88 of
    88).
  - **Binders:** no landed binder or table line uses an old spelling, so the table is unchanged.
- **Rulings on NF318's questions** (recorded in the standing brief):
  - **Q1 (allowed):** the libcp `.drectve` exception covers header moves of the alias route too, under the same conditions. That is 31 done
    rows; the 32nd is 0x00B6CCF0, held under Q2.
  - **Q2 (held):** 0x00B6CCF0 would be the first landed row to define `FABLE_RETAIL_CLANDSCAPEPATCHTESSELATOR`. Only switches that landed rows
    already define are allowed in this route.
  - **Q3 (allowed):** an enum return written as a literal cast to the PDB enum is the NF307 D3 form.
  - **Q4 (held, T3 units):** respelling a callee off an alias or stand-in onto a name the header declares needs the callee's own second
    source (0x00893EC0, 0x006FECA0, 0x00AA4B50 / B90 / C00).
  - **Q5 (held):** 0x005DBDA0 is byte-exact but loses the COMDAT of a made-up alias accessor. The proof requires equal COMDAT sets.
- **Queued:**
  - T3 lines `nf318` for the 31 ports and 3 holds;
  - the 5 CScriptThing members, which need the nf86 / nf88 pick-any form with their 19 in-TU copies;
  - 0x00692480 as a unit with binder 0x008579D0;
  - NAME_FIXES_QUEUE: 0x013B8790 `CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass`, bound as `g_gameObj` / `g_obj` (already true at
    HEAD).

## Recheck
`logs/recheck.log` (landed tree, 88 rows): 88 MATCH / RELOCATION_MATCH PASS.
