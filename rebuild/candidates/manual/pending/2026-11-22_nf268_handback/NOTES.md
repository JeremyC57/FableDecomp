# NF268 notes (2026-11-22, round 27b)

Base: the repo tree with nf266 / nf264 landed. NF267 runs beside me: none of its rows or entries touched (every row I compiled was
`released` or had no lock directory before I claimed it: work/locks.log); none of my rows is in codex_pass28 RESERVED.tsv (checked).
Tools: NF266's set re-pathed to agentNF268 (tools/). Read-first list: the brief chain NF268 -> NF266 -> ... -> NF215, NF214 and its list
(COORDINATION.md, NF92 BRIEF / LESSONS / NOTES, NF44 BRIEF, NF88-90 LESSONS, GOTCHAS.md / EXTRA_RULES.md in byte chunks without cutting
lines, ASM_RULES.md), the nf266 / nf264 / nf261 landing READMEs and hand-backs (NOTES, LESSONS).

## Entry 1 (q12770): 0x0072CAA0 SetupAssassinRush onto the shared headers -- done (first form)
- Header (hdr/README.md): engine/CCreatureAction_CastAssassinRush.h, add-only `FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_UNTARGETEDCTOR`
  (the (creature, level, final action) ctor; Ego_r 0x009055B0 + FableWin 0x029F91B0; retail 0x008CE100 stores vftable 0x01284254,
  `ret 10h`). pin_asserts: 0 pinned. Whole-tree equiv_check (repo rebuild/include v work/tree_new = repo + this file, every landed source and
  test): 57,125 rows checked, 0 differ (evidence/equiv.log). The six includers 0x0072D990 / 0x008C2550 / 0x008CEF50 / 0x008CD470 /
  0x0096AE50 / 0x009735D0 rechecked on the merged tree: 6 RELOCATION_MATCH + PASS, 0 RELOC_CONFLICT (evidence/vland_includers.log;
  sources and HEAD tests prechecked first).
- Port (rows/0072caa0.cpp): every HEAD row-local model goes (CThing, CPlayer, CThingCreatureBase, the descriptive CTCTargeter,
  CTCSpecialAbilities, CCreatureAction_CastAssassinRush with its conversion operator). CTCSpecialAbilities goes onto its header FULL + USE
  (the 0x0072CF40 define set), not the siblings' row-local `: CTCBase` class (that would re-base a local model): Creature is the PDB
  reference, and the own leaf becomes the private PDB spelling ?SetupAssassinRush@CTCSpecialAbilities@@AAE_NXZ (Ego_r 0x00744040 +
  FableWin 0x0254D9F0). The interface-8 lookup is CThing::PeekTC<const CTCTargetingBase> (expanded, const LowerBound 0x004365B0) and slot 33
  GetPTargetedThing; 0xA9 / 0xA8 are TCI_ASSASSIN_RUSH / TCI_BATTLE_CHARGE; IsDead / GetTypeFlags / PeekPlayer().IsZTargeting() are the
  header accessors (the SetupEnflameSpell 0x0072C790 recipe). INITIALISE is defined as well so a table the TU might emit would bind slot 12
  to 0x008CD470; VC7.1 emits none (objdump: no ??_7 in the object; evidence/vt7_new_objects.txt).
- Grade: RELOCATION_MATCH on the first form. Symbols (evidence/symdiff_0072caa0.txt): leaf QAE -> AAE; the stand-in GetTC<CTCTargeter> and
  the local ??B conversion go; new pick-any copies: the implicit ??1CCreatureAction_CastAssassinRush@@UAE@XZ (75 B, EQ owner 0x0072D990) and
  header inlines; pk (evidence/pk_0072caa0.tsv): every copy with an owner EQ (0x0072D990, 0x0048C5C0, 0x004365B0, 0x00410DE0, 0x004118C0),
  the rest NO-OWNER (IsDead, GetTypeFlags, IsZTargeting, StatGetInterfaceType, the two PeekTC templates, CCountedPointer default ctor,
  bitset / vector helpers). No callee spelling changed (RENAMES section 2 has no 0x0072CAA0 line); the dtor copy now references
  ??1CCombatActionBase@@UAE@XZ (0x0062E900's landed spelling).
- Census: 0x0072CAA0 had 2 spellings (self QAE, binder 0x0072CF40 AAE = the table line); now 1. callers.py: 0x0072CF40 is its only caller.
- Link (evidence/link_e1.txt; LINK unset, /DLL /NOENTRY /FORCE /NODEFAULTLIB): HEAD set (HEAD 0x0072CAA0 + 0x0072CF40 + 0x0072D990 +
  0x008CE040 + 0x008CE100 + 0x0096AE50): 0 duplicates, 0x0072CF40's AAE reference unresolved. New set (new 0x0072CAA0, same controls):
  0 duplicates (the ??1CCreatureAction_CastAssassinRush copies of 0x0072CAA0 / 0x0072D990 / 0x0096AE50 are all pick-any), the AAE
  reference resolved; the two ctor references stay unresolved as at HEAD (0x008CE040 / 0x008CE100 self leaves are stand-ins, queued).
  Positive control (a second copy of the new object): LNK4006 on the AAE leaf.
- Test: the HEAD test is a standalone host model: unchanged, prechecked, PASS (test debt, NAME_FIXES).

## Entry 2 (q12771): slot 12 of the CastAssassinRush tables -- rejected
VC7.1 objects of the HEAD sources of 0x0096AE50, 0x009735D0 and 0x008C2550 (q.py, catalog flags, each RELOCATION_MATCH) define no ??_7
symbol at all (evidence/e2_vc71_heads.txt): nf266's slot-12 census was clang -O0, which also emits the tables of inline ctors / dtors that
VC7.1 drops (nf264 had listed the same three rows among its 14 non-emitters). No emitting row, so no define. REJECTED.tsv.

## Entry 3 (q12772): slot 1 Validate, slot 3 PerformAction -- slot 3 done, slot 1 blocked (port)
- Evidence: retail slot 1 of 0x0125CCBC / 0x0125CEFC = 0x0099A400 = CBaseObject::Validate; slot 3 = 0x0069F9E0 / 0x0069FE60 (nf266 vtmap).
  Name sets: no CCreatureActionBase::Validate in Ego_r / FableWin / Ego_d (only ?Validate@CBaseObject@@UBEXXZ); PerformAction UAE for both
  classes in Ego_r (0x006C4C10 / 0x006C50F0) and FableWin (0x02211A10 / 0x022138D0).
- Slot 3 (rows/0069f9e0.cpp, rows/0069fe60.cpp): the self rows' HEAD row-local class models declare `virtual void PerformAction();`
  (in-place respell of a member of a class each row already defined at HEAD; their base model already has a vptr, so no layout change),
  zero line delta (comment on the same line; neither row has an assert or __LINE__, nothing includes them). VC7.1 HEAD v new: the only
  defined-symbol change is the leaf QAE -> UAE; every function COMDAT byte- and relocation-identical (30 / 21; objsyms); no ??_7 emitted.
  Census: 0x0069F9E0 binders = 0x005CA68E (two E8 sites, UAE: the table witness), the 0x0069F0F0 table (UAE), and the July forwarder
  0x0069FCD0 (placeholder extern "C" ForwardTarget_0069f9e0, unchanged, NAME_FIXES): 3 -> 2 spellings. 0x0069FE60: no E8 caller, the
  0x006A0D50 table (UAE): 2 -> 1. Tests: standalone host models, unchanged, PASS.
- Emitter census (HEAD nothing defined the two UAE symbols, so every table binding them resolves only now): retail (evidence/slot3_retail.txt,
  a Fable.exe scan for the dwords) holds 0x0069F9E0 only at slot 3 of 0x0125CCBC (UnsheatheItemFromInventory) and 0x0125F8D4
  (UnsheatheToShowToSheatheTrophy, which inherits it), 0x0069FE60 only at slot 3 of 0x0125CEFC (UseObject); the other derived class,
  UnsheatheItemFromInventoryWithHasteAction (rtti_bases.tsv), has its own 0x0069FCD0 there; UseObject has no derived class. Landed side:
  clang MS-ABI objects (SLOT=3) of the 61 rows whose source names any of the four classes (evidence/slot3_census_rows.txt /
  slot3_census_clang.txt): the only tables binding either symbol are UnsheatheItemFromInventory (0x0069F0F0, 0x006A0150), UseObject
  (0x006A0D50) and UnsheatheToShowToSheatheTrophy (0x006DF800, 0x006DF860), each at the retail-right address; no WithHasteAction table binds
  the base symbol. The 3 clang failures (0x00694430, 0x0093EA90, 0x00940960) reach the classes through the shared headers, which declare
  PerformAction virtual (their own class's table, right address). VC7.1 objects of 0x0069F0F0 / 0x006A0D50 / 0x006DF800
  (evidence/slot3_vc71_tables.txt) confirm slot 3. Second bodies: no other landed source defines either PerformAction (in-class or out of
  class).
- Link (evidence/link_e3.txt): HEAD 0x0069F9E0 / 0x0069FE60 + HEAD 0x0069F0F0, 0x006A0D50, 0x005CA68E, 0x006DF800, 0x0069FCD0: 0 duplicates,
  4 references to the two UAE symbols unresolved (0x005CA68E, 0x0069F0F0, 0x006DF800, 0x006A0D50). New self rows, same controls: 0
  duplicates, all 4 resolved (each to its one definer). Positive control (a second copy of the new 0x0069F9E0 object): LNK4006 on the UAE
  leaf. (The other unresolved lines in the file are the self rows' own callees, matched by the watch substring; unchanged.)
- Slot 1: the two emitters reach it through row-local CCreatureActionBase models with no base class, so they cannot bind
  ?Validate@CBaseObject@@UBEXXZ without CBaseObject as a base, i.e. re-basing a local model of a shared-header class (not allowed). The fix
  is the queued port of 0x0069F0F0 / 0x006A0D50 onto engine/CCreatureActionBase.h (the nf266 "four entry-1 rows" port; 0x00693B30 /
  0x00693DC0 already bind CBaseObject::Validate there). 10 other landed local CCreatureActionBase models also declare Validate
  (work/validate_models.txt); whether VC7.1 emits a table in any of them is not censused. Decision 2.

## Entry 4 (q12744): the map dtor mirror -- rejected (the claim's own stop condition)
With FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY (one define line each; no asserts, nothing includes the rows) both 0x00AD9770 and 0x00AD9980
stay RELOCATION_MATCH and stop defining ~_Rb_tree<ulong, pair<const ulong, long>> with its base / allocator destructors and the NEQ
_M_erase / clear copies (owners 0x004507A1 / 0x00451046), but they then reference that destructor. Four-object link with HEAD 0x00ADCE10 /
0x00ADC310 (evidence/link_e4.txt): 0 -> 4 unresolved references to it (one per object); nothing in the set defines it, and it has no table
line or known retail owner. Not applied. REJECTED.tsv. Whether some other landed row defines that destructor pick-any was not censused.

## Entry 5 (q12659): open (not started)
Needs the full 469-includer proof; not started (context).

## Whole-pass checks
- New symbol names (each defined by exactly one body: grep of rebuild/src / rebuild/include for the definitions):
  ?SetupAssassinRush@CTCSpecialAbilities@@AAE_NXZ (0x0072CAA0 only), ?PerformAction@CCreatureAction_UnsheatheItemFromInventory@@UAEXXZ
  (0x0069F9E0 only), ?PerformAction@CCreatureAction_UseObject@@UAEXXZ (0x0069FE60 only). The header adds a declaration only.
- Funclets / folds: no `../src/compiled/` include names any of the three changed files (grep of rebuild/src, rebuild/tests, rebuild/include).
- Descriptive names added: none. Flags lines added: none (no changed row has CompilerFlags; none needs one).
- recheck_final.log: one vland --recheck-landed run over the three changed rows (overlay = the merged tree): 3 RELOCATION_MATCH + PASS,
  0 RELOC_CONFLICT (the 0x0069F9E0 / 0x0069FE60 WARN lines are HEAD's local models).
- Locks (work/locks.log): the three hand-back rows stay NF268 (rows.txt); every control / includer / measured row is back to `released`
  (0x0069F9E0 / 0x0069FE60 / 0x0072CF40 / 0x008CE040 / 0x008CE100 / 0x0069FCD0 had no lock directory: created with plain mkdir; all but the two hand-back rows released). Every compiled row checked against codex_pass28 RESERVED.tsv: none reserved.
- Nothing written in the repo (work/root_before.txt v root_after.txt); no git; no sleep; no rm -rf (tools/clean.py).

## Rule slips
- Read-first: the nf258 landing README / hand-back NOTES were read by grep (q12744 lines) only. The whole-tree equiv_check ran under
  `timeout 2700` (tools/eq4.sh; the NF266 brief allows it; it is not a grade).
- The slot-3 link output was printed whole (a watch substring that also matched `referenced in function` text): noisy, not a rule slip.
- Order: entry 2's census compiles, entry 3's slot-3 grades and entry 4's measurement ran while entry 1's equiv_check (8 minutes) was
  running, before entry 1 reached its terminal state (one entry to a terminal state before the next). No cl compile overlapped another
  of mine; equiv_check is preprocessing only.
- One Bash call prechecked six includers with relative paths (pre.sh cds into work/): no input files; rerun at once with absolute paths
  before any grade.

## Decisions for the coordinator
1. q12770: land the UNTARGETEDCTOR opt-in + 0x0072CAA0 (self leaf QAE -> the private PDB AAE that its only binder already binds)?
2. q12772: slot 3 (the two self leaves QAE -> UAE, in place, count-reducing; emitter census + link clean) lands on its own, separate
   from slot 1, or held until slot 1's port? The two touch different addresses and no row in common; slot 1 needs the port of 0x0069F0F0 / 0x006A0D50 onto
   engine/CCreatureActionBase.h (queue it).
3. q12771 rejected (VC7.1 emits no table in the three rows); q12744 rejected (the mirror leaves ~_Rb_tree<ulong,...> without a definer in
   the four-object link). Agree, or should q12744 wait for a whole-tree definer census?
4. q12659 to the next agent.
