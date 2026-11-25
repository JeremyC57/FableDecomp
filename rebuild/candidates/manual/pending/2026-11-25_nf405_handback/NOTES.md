# NF405 hand-back (2026-11-25): T3 nf398 items 1-3 (CTCCreatureGroup::Members family)

## Summary
- **b1, 0x004E5C71 ~CTCCreatureGroup: ACCEPT.** Ported onto engine/CTCCreatureGroup.h. Retail's TU used the **compiler-generated** destructor;
  a new add-only, default-off opt-in `FABLE_RETAIL_CTCCREATUREGROUP_IMPLICIT_DTOR` drops the `virtual ~CTCCreatureGroup();` declaration and the
  row emits the implicit COMDAT through the CMessageEvent 0x00687210 helper form, `#pragma optimize("s", on)`. RELOCATION_MATCH.
  TABLE_CHANGES: `?inner@FableDestVal_004e5c71_t@@QAIXXZ -> 004e350f` replaced by `??1?$_Rb_tree@...` (the Members call), `?outer@... -> 00686830` deleted.
- **b2, 0x004DBE39: ACCEPT.** Self respelt to `??_G?$pair@$$CB_KVCGroupMemberData@@@std@@QAEPAXI@Z` in the nf86/nf88 pick-any form,
  `#pragma optimize("s",on)` kept in source. No table change (the line already binds this name). Self and table now agree.
- **Item 3:** both rows can be authored and grade RELOCATION_MATCH against retail (evidence/item3/); both need a catalog boundary add first (T3.txt).
- Two independent batches (batches/b1, batches/b2); either can land alone. No RULINGS_NEEDED.

## Item 1: what retail's TU looked like (measured; evidence/p*.cpp, probe header evidence/probe_hdr)
Retail 0x004E5C71 = `56 8b f1 8d 4e 18 e8 93 d8 ff ff 8b ce 5e e9 ac 0b 1a 00` (push esi; mov esi,ecx; lea ecx,[esi+18h]; call 0x004E350F; mov ecx,esi; pop esi; jmp 0x00686830). No vptr store.
| candidate | /Os | /O2 | retail evidence |
|---|---|---|---|
| user-declared empty out-of-line dtor (NF398 probe) | DIFFER 25v19 (stores ??_7 first) | DIFFER 83v19 | - |
| user-declared inline `virtual ~CTCCreatureGroup() {}` | DIFFER 25v19 | DIFFER 83v19 | - |
| `__declspec(novtable)` + user dtor | dtor RELOCATION_MATCH | - | **excluded**: the same compile's ctor loses `mov [esi],??_7`; retail ctor 0x004E5C2F keeps it (`c7 06 6c 32 24 01`; p3s v p4s, evidence/novtable_ctor_objdump.txt; p4s ctor = retail bytes) |
| base without virtual dtor | - | - | **excluded**: vftable 0x0124326C slot 0 = ??_G 0x004E75BD, CTCBase vftable 0x0125BE0C slot 0 = 0x00687050 |
| empty inline base dtor | - | - | **excluded**: ~CTCBase 0x00686830 = `c7 01 0c be 25 01 e9 ..` (stores its own vptr, tail-jumps on) |
| **compiler-generated dtor** | **RELOCATION_MATCH** | DIFFER 77v19 (inlines ~_Rb_tree) | chosen |
Second source for "compiler-generated": FableWin.pdb's module stream defines ??1CTCCreatureGroup@@UAE@XZ in `debug\tc_register.obj`, and Ego_d.pdb has
CTCCreatureGroup::~CTCCreatureGroup in `FableLib\debug\tc_register.obj`. The Ego_r and FableWin module streams of `tc_creature_group.obj`, which holds every
out-of-line member of the class (12 in FableWin), contain no destructor. Ego_r's `release\tc_register.obj` holds the inline ctor 0x00507F55, GetClassTypeName and the ??_E.
Precedent: CMessageEvent 0x00687210 and the `*_IMPLICIT_DTOR` opt-ins (CArenaRoundDef, C3DPositionInterpolator): a user-written body stores the vptr; the implicit one does not.
/Os is the mode of the tc_register rows 0x004E5C2F / 0x004E5C61 / 0x004E75A0 / 0x004E75BD.

## Names, two sources each
- **??1CTCCreatureGroup@@UAE@XZ (0x004E5C71).** (1) The FableWin.pdb spelling (public and module stream), already table-bound here (witness 00784bd0); (2) Ego_d's
  tc_register.obj destructor, plus the retail callers ??_G 0x004E75BD (vftable 0x0124326C slot 0) and the Tag's ??_G 0x004E764A (vftable 0x012432F4 slot 0).
  That the Tag's ??_G calls 0x004E5C71 directly is observed; whether by a COMDAT fold or by inlining the implicit Tag destructor is left open.
  Ego_r.pdb's name sets have no ??1CTCCreatureGroup entry.
- **??1?$_Rb_tree<..CGroupMemberData..> (0x004E350F, new table line).** (1) The landed nf398_b1 row's self; (2) this compile names it at the Members (+0x18) call and its copy is byte-
  and reloc-identical to that row's object. pk_all: EQ at 004e350f, reach y.
- **??_G?$pair@$$CB_KVCGroupMemberData@@@std@@QAEPAXI@Z (0x004DBE39).** (1) The existing table line (nf398_b1, witness 004dfb8f), whose object and 0x00813460's emit the same 31 bytes (EQ);
  (2) the only retail references are `push 0 ; call 0x004DBE39` in _M_erase 0x004DFB8F (0x004DFBAE) and the _Destroy 0x004DD6F3 (0x004DD6F5), the VC7.1 form of
  STLport `_Destroy(p){ p->~T(); }`; the body destroys PMember at +8 (after the 8-byte key) and then deletes on bit 0.
- **??1?$map<..CGroupMemberData..> (0x004E409B, item 3 / b1 extra).** (1) Emitted by every ~CTCCreatureGroup compile beside ~_Rb_tree (NF398, NF405); (2) retail 0x004E409B is the only jump
  to 0x004E350F in the image and is itself unreferenced (rawref). STL spellings are the repo STLport's (descriptive-by-library), as in nf398.
- **??$_Destroy<pair<..>> __fastcall (0x004DD6F3, item 3).** NF398's evidence (unique call-resolved masked match), and the item-3 row grades RELOCATION_MATCH there.

## Extra COMDATs (R1 / R10)
- **b1:** ~_Rb_tree, ~_Rb_tree_base, ~_STLP_alloc_proxy, ~allocator<node> and the node deallocate are TIER1, byte- and reloc-identical to the landed 0x004E350F object (evidence/tier1_004e5c71_vs_004e350f.txt).
  ~map is new to the build. Its TIER2 home is unique: 0x004E409B, an off-catalog hand boundary (0x004E409A ends `leave ; ret`, and the next function starts at 0x004E40A0), with no landed leaf calling it. This is the nf398 Q3 precedent.
  The leaf defines no vftable, and clear / _M_erase are declaration-only (0x004E350F's define set).
- **b2:** ~CIntelligentPointer<const CThingAICreature>, ~CGroupMemberData and ~pair are TIER1 (identical in the 004dfb8f, 00813460 and 00813010 objects). Probe q1 (a _Destroy helper) also matched but added a _Destroy COMDAT, so I chose q2.

## Gates (both rows)
- **vland --recheck-landed,** HEAD and new at one LANDVERIFY_WORK path per pair (work/wd_b1, work/wd_b2; FABLE_HDR_OVERLAY=hdr on both sides; logs in batches/*/logs):
  - RELOCATION_MATCH PASS on both sides; the new tests PASS (same PassPatterns); no WARN LITERAL_DIFF; no grade drop.
  - b1 new: WARN RELOC_NAME = the catalog label StdList_Dtor_004e350f (relabel already queued).
  - b2 new: HEAD's WARN SHARED_HEADER_AVAILABLE is gone.
- **TABLE_CHANGES:** apply_table.py --dry: b1 replace 1, delete 1, 0 misses, multi-target 1 -> 1 (evidence/apply_dry_b1.txt); b2 empty. hdrcheck2 on b1's overlay: default, _CTCBOASTINGCROWDCONTROL and the new macro all `same`.
- **pk_all + pickany_vs_retail** (--leaf given), HEAD on the repo table and new on both the repo table and the TABLE_CHANGES table (work/troot): 0 NEQ, 0 reach flips (HEAD had no reached copy). The 4 census objects
  (004e350f 004dfb8f 00813460 00813010) give identical pk on both tables.
- **/FORCE link** of the header-reach rows plus the 4 other definers of the old 0x004DBE39 stand-in, run three ways (b1 alone, b2 alone, both;
  evidence/link_single_swaps.txt, link_lk1_diff.txt). The two objects share no COMDAT name, so the batches are independent.
  - Duplicates: b1 alone 10 -> 10; b2 alone 10 -> 9; both 10 -> 9 (the stand-in duplicate goes; nothing new).
  - Unresolved (watch set) 392 -> 389: ??1CTCCreatureGroup, ?inner and ?outer are now resolved or gone. ??1CTCBase was unresolved at HEAD too (1 -> 2 references).
  - Nothing HEAD resolved is left unresolved (evidence/link_lk1_diff.txt).
- **Census by header reach** (tools/reachset.py, guards of CTCCreatureGroup.h and CGroupMemberData.h plus the stand-in names: 55 rows).
  - A clang prefilter, then VC7.1 of all 55 (008127f0 failed in clang and is covered by VC7.1).
  - Plus the 7 rows that mention CTCCreatureGroup as a word outside that reach (`grep -rlw`; 004d4789 004e5c5b 004e5c61 004e5f18 00784600 00812f20 00934920), all VC7.1. Of the 7 headers that mention the name, only engine/CTCCreatureGroup.h defines the class.
  - No other definer of ??1CTCCreatureGroup or ~map. ??_G pair definers: 004dfb8f and 00813460, both EQ with b2's copy.
  - The old 0x004E5C71 self, ?inner and ?outer are used only by that row and its test.
  - The old 0x004DBE39 self is a Leaf of 4 other rows (their own bodies, strong; T3). No table line or caller uses it.
- **equiv_check** for the header (all 57158 sources and tests, scratch copies work/eq/old v work/eq/new): 11 lines differ.
  - All 11 differ only by the `FABLE_STATIC_ASSERT(sizeof(CTCCreatureGroup)==36)` __LINE__ typedef name (the EQUIV_LOOSE rerun gives 0).
  - VC7.1 of those 10 rows with the original and the new header: every COMDAT EQ.
  - Test 0x00934410 under vland with the overlay: PASS.

## Wineserver note (coordinator message)
- The probes p0s..p2t were compiled at 01:47:46-51, at the edge of the 01:45-01:47 `wineserver -k` window. I reran all six.
- p0s hung on the rerun (01:56, a fresh wineserver whose winedevice children held the pipe; cl.exe defunct). timeout killed it, and the follow-up rerun gave DIFFER 25v19 again.
- All reruns reproduce the first results (evidence/probes_rerun_after_wineserver.txt). Every other compile ran after 01:51, and every link ran later still.
- I never ran wineserver -k.
- After the row and header comments were corrected (source attributions), I regraded b1 (RELOCATION_MATCH), reran its vland pair (logs replaced) and reran hdrcheck2. The header edit was a comment with an unchanged line count, so the equiv_check result stands.

## Locks / deviations
- 004e5c71 and 004dbe39 stay locked NF405 (both ACCEPT). The item-3 addresses (004dd6f3, 004e409b) have no catalog rows and were never claimed.
- Census and definer compiles were read-only HEAD compiles of other rows (no edits, no locks claimed).
- Nothing was written to the repo. vland was dry (--recheck-landed), run from work/rsroot. equiv_check ran from the repo root and wrote only to my folder and $S/stubinc_equiv.
- Deviation: one shell line ended with `git -C /dev/null status 2>/dev/null`, a stray no-op. /dev/null is not a repository, so it read and changed nothing, but it was a git invocation against the brief.
- The first TABLE_CHANGES draft used `del`. apply_table.py takes `delete`; fixed before the dry run.
