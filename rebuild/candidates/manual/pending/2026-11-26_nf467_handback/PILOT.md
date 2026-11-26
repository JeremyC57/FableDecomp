# NF467 pilot (2026-11-26): r459a short-vftable port, CAIStateGroup_StandStill, then the AIStateGroup ctor family

## Why this pilot, not the top of the ranking
Ranking (evidence/rank_by_name.tsv, NF461 q1 ONLY_SHORT / MAX_SHORT with a locator, joined with NF465's 197 vftable targets):
- `??_7T@@6B@` (185 rows): the 37 shared local-`struct T` names; left last as the brief says (list in README).
- 60 `CInputProcess*` names x 61 rows: ONE real source (0x00488D10 CPlayer::InitInterfaces) plus 60 thin `??_G` includers. The rows are
  already on the shared headers; the headers lack the virtual lists (`Virtuals other than the inherited destructor are not modelled`).
  A header-only shape (add the virtual lists to ~61 FABLE_RETAIL_INPUTPROCESS headers), not a local-model port. Not done.
- CACompressor (3 short + 1 binder 0x009C7060 closing 2 NF465 targets) probed first (49 s): blocked twice. (1) engine/CACompressor.h
  declares `virtual ~CACompressor();` but 0x009C6F40 proves an implicit destructor (no vptr store, 48-byte ??_G); with the header the ctor
  TUs emit a 30-byte ??_G calling an unbound ??1. (2) the header pulls stlport/fable_containers.h: 0x00A3AE70 / 0x00A3AEA0 would gain
  `/DEFAULTLIB:"libcp"` (r459b). Locks released.
- q1 retail sizes are overcounted for 38 names (evidence/vtaudit.tsv): retail_vt counts any following dword that lands in .text.
  Example: CThreadedFileCallback "16" is 12 + the string "XMV\0" (0x00564D58) at 0x0129B394; its PDB has 3 virtuals, and 4 of its 5 copies
  are already right. Check func_slots, not q1 retail_size.
- None of the ported classes is an NF465 target, so no naming-tracker move is expected (measured: none).

## Method (per class; scripted for the family: tools/genfam.py, famcheck.sh)
1. Retail table at the RTTI va (tools/vtdump.py, retail_vt.py): slot 0 the deleting dtor, slots 1..13 the CAIStateGroupBase bodies
   unless overridden, slot 12 Update.
2. Header: add-only opt-in `FABLE_RETAIL_<CLASS>` on the generated engine/<Class>.h (or a local class on engine/CAIStateGroupBase.h when no
   header exists, r462a): `class X : public CAIStateGroupBase` with the ctor and one declaration per overridden slot, spelling and access
   letter from the PDB name sets (the override must have exactly one PDB spelling with the base signature, else HOLD). Generated default
   branch under `#else`, byte-identical after `g++ -E -P` under three macro sets; moved asserts pinned with pin_asserts.py.
3. Row: `#pragma optimize("s", on)` + the three macros + the header + the unchanged ctor body. The pragma is retail's TU mode, not
   scaffolding: retail ctors of this TU sit at unaligned addresses (0x008FDE1B; a /O2 COMDAT is 16-aligned, optimize("s") 1-aligned) and
   only under it are the companions the full table brings equal to retail (??_G 28 bytes = slot-0 target; under /O2 it is 30 bytes).
4. Gates: vland --recheck-landed HEAD v new (runpair_hdr.sh); objproof_keyed + secdiff (ctor data/relocs equal, only the alignment flag);
   cdump (.drectve, COMDAT list); ceq (??_G and ??1CAIStateGroupBase byte- and relocation-equal to retail); slotproof (14/14 slots at retail
   targets: EQ / COMDAT_IN_OBJ / NO_ROW / SELF_MISSPELT); link HEAD v new with every slot self's HEAD object in both sets; sandbox
   (apply_table --dry, bindings_multi_check, descriptive_check, naming_status dry run in a shadow, sandbox467.sh).

## Per-step times (tool wall clock, work/times.log; authoring/reading time not included)
| step | time |
|---|---|
| CACompressor probe (HEAD objects + COMDAT dump) | 49 s |
| StandStill: header opt-in + row + compile + dump | 75 s |
| ??_G mismatch found, size-pragma test, alignment proof | 191 s |
| vland HEAD v new, 1 row | 4 s |
| slot selves HEAD objects (17) + link | ~100 s |
| family generator, 41 rows | < 40 s |
| 41-row compile + per-row checks (famcheck.sh) | 43 s |
| vland HEAD v new, 41 rows | 47 s (+ 4 rows re-run after a comment fix) |
| 118 slot-self HEAD objects | 25 s |
| links (all, accept set, hold set) | ~60 s |
| sandbox (2 shadows, tracker, checks) | ~4 min (background) |

## Result (after the final review: everything is HOLD)
- Pilot 0x008FDE1B: ??_7CAIStateGroup_StandStill@@6B@ 4 -> 56 bytes (retail 56), every slot at retail's target, ctor bytes unchanged.
- Wave 1: 41 more ctor rows built the same way, all RELOCATION_MATCH, 41/41 tables at retail size, 82/82 added COMDATs equal to retail.
- Blocker found last (r467a): the added `??1CAIStateGroupBase@@UAE@XZ` pick-any copy collides with the landed STRONG (sel 1) definer
  0x008FD288 (link dup 2 -> 3 once 0x008FD288 is in the set). Every one of the 42 rows has it, so all 42 are HOLD r467a.
- 22 of the 41 also have a wrong class model (r467f): 19 headers already carry a retail view (NF330 `_VIRTUAL_MODEL` etc.) and/or the PDB
  class has members (0x18..0x3c), and 3 headerless classes are not 0x14 in Ego_r. Their opt-ins were withdrawn
  (b3/hdr_rejected/); redo them on the existing views (add a ctor declaration under a sub-macro).
- Ready once r467a is ruled: b1 (1) + b2 (19) = 20 rows, 3 headers in b0_hdr. r467b also applies to the rows with misspelt-self or
  row-less override slots (named in RESULTS).
- 179 other AIStateGroup names held for wave 2 (evidence/plan_aisg_wave1.tsv).

## Lessons for the next agent (the method is right, check these first)
1. Grep the landed definers of every COMDAT the full table drags in (here 0x008FD288 `??1CAIStateGroupBase`, sel 1) before building a family.
2. Read every target header's existing opt-ins and the PDB size before generating a view.
3. Use func_slots (vtaudit), not q1 retail_size.
