# NF89 notes (q11163..q11176, the nf87 follow-ups)

All numbers below were counted from STATUS.tsv, APPLIED.tsv, RENAMES.tsv, NAME_FIXES.tsv and recheck_final.log at hand-back.

## Counts
- STATUS.tsv: 60 rows = the 51 in rows.txt + 9 claimed extras (extra.txt: 0x01229BE0, 0x01229C40, 0x00A71880, 0x00A71910, 0x00A70020,
  0x00A70320, 0x00A70490, 0x005BD328, 0x0059D9AD).
  - ported 29 (27 RELOCATION_MATCH, 2 MATCH), all PASS in recheck_final.log (one vland run over the 29 row files in rows/).
  - not-a-binder 29; blocked 2 (0x005BF77D, 0x005A30A1); deferred 0.
- APPLIED.tsv: 12 done, 1 done-already (q11169), 1 blocked (q11171: 0x005BF77D, 0x005A30A1).
- RENAMES.tsv: 27 section-1 lines, 64 section-2 lines. NAME_FIXES.tsv: 14 lines. REJECTED.tsv: 2 lines (stale WORKLIST claims).
- Read-only probes of landed rows outside the slice (graded in my pS/, not claimed, nothing changed), to read their actual bindings:
  0x0059B6F4 (own leaf for NAME_FIXES), 0x005BDCB4 (tail-jumps to 0x005BD054 under the stand-in: NAME_FIXES), 0x0056EC79 (tail-jumps to
  0x0056E5AB, binds it right). The tail jumps came from an E9 / Jcc32 scan over every section-1 address (e8.py sees only E8 calls).
- Claims taken and released unchanged: 0x00A355F0 and 0x005BD1CF (both NAME_FIXES; their lock directories were mine and are removed).

## Per entry
- q11163 / q11164: 0x005A8A5D and 0x005A3581 moved off all-local models onto CTCInventoryTrade.h (retail branch), CThingPlayerCreature.h
  (retail chain), CTCShop.h, CTCHeroStats.h, the PASS22 CCategory. 0x005A8A5D's own leaf changes (the STLport vector parameter):
  section 1, leaf-only; its only retail caller (0x005AD140) is unlanded.
- q11172 / q11173: the four static rows now define `std::map<CCharString, bool> CTCAIScratchpad::ActiveGossipCategories` /
  `IsGossipForPlayer` (GOSSIP_BOOL_MAPS), as the landed Gossip pair 0x0121F160 / 0x01229CA0 does: each `$E1` / `$E2` row defines the same
  static, so VC7.1 emits both functions from one definition (the Gossip precedent; the two rows of a pair must not be linked together).
  Catalog: CTCAIScratchpad::`dynamic initializer for '...'` / `dynamic atexit destructor for '...'` (sibling spelling). The
  RELOC_CONFLICT warnings on `_$E2` are the generic static-function name (same as the Gossip pair).
- q11174..q11176: the introsort loops and their partitions / median are explicit STLport instantiations (INTROSORT_LOOP, SORT_INTERNALS).
  The sort front ends 0x00A355F0 / 0x00A73B20 / 0x00A73BA0 already bound the loops under SElement16 stand-ins and still do; with the
  insertion-sort helpers 0x00A34D10 / 0x00A33870 (std::CKeyPairCompareLess stand-in leaves) they are the next family port (NAME_FIXES).
- q11165..q11171 (inventory): seven self rows (0x005BD1AE, 0x005BC4B2, 0x005BC8AC, 0x005BF482 renamed; 0x005BCE3C / 0x005BCE2F /
  0x005BCACE already right) and their binders. Follow-through renames: 0x0061A040 is CTCInventoryQuests::GetSelectedCategoryAssociation,
  0x005BD328 GetNumberOfSlotsForCategory, 0x0059D9AD ResetTradeLists; several binders' own leaves were stand-ins too (section 1).
- q11168: 0x005BD054 HighlightNextItem (the one binder still on 0x005BC8AC's old leaf in the caller sweep) moved onto the PASS22
  CCategory with NF90's GetPDef() / quarantine CInventoryCategoryDef. Grade 1 was DIFFER(163v163): retail reads the def's flag byte
  before the Release `dec [def+4]`, mine after. The repo opt-in FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE (engine/CDefPointer.h, written
  for exactly this order) made it RELOCATION_MATCH. 1 probe + 2 grades.

## Catalog CompilerFlags that change (the rows' `// Flags:` lines; vland carried them)
- 0x00A34260, 0x00A35490, 0x00A71880, 0x00A73910, 0x00A73A00: `/Gr` (the algorithm TU; the old rows spelt __fastcall by hand).
- 0x00A71910: `/Gr /d2inlT66` with an `// Inline-budget:` line (owner rule 2026-11-08, preference 2). Retail calls the by-value pivot's
  implicit ~pair out of line (0x00A6F390); the default /d2inlT74 expands it. Sweep: T40 DIFFER(128), T60 / T65 / T66 MATCH, T67 / T68 /
  T70 / T73 DIFFER(174). The rule says "the smallest N that matches"; that wording is for raising the budget. Here the budget is lowered,
  so I took the largest matching value (66), the one closest to the default, and say so in the row.
- 0x00A70020: `/Gr /Oa` (was `/Oa`).

## Tests
- 0x0059D9AD: test rewritten on a neutral recorder struct (the old test modelled a CMemoryAllocator); PassPattern `CP_59d9ad_OK` kept.
- All other tests are the landed ones (standalone) and PASS unchanged.

## Descriptive names (no original found anywhere)
- STLport instance names are descriptive-by-library (template name from STLport 4.6, instance proven by callers / callees): every
  __introsort_loop, __unguarded_partition and __median row here.
- `g_TrainableStatTable` (0x0137C8E8, 0x0056E632) and `g_TradeTypeCategories` (0x0137CCDC, the same-TU helper in 0x0059C6B0, the
  landed 0x0059B6F4 spelling) are file statics with no name in Ego_r / FableWin / Ego_d / labels / FSE; kept as landed (descriptive).
- No other made-up name was introduced: every own name is Ego_r.pdb + FableWin.pdb (or Ego_r + Ego_d for the gossip statics).

## Same-TU bodies kept
- Only 0x0059C6B0 defines GetCategoryIndexForTradeType (landed separately at 0x0059B6F4) under `auto_inline(off)`, as its old row
  did (VC7.1 must see it touches only eax/ecx). This is a strong copy of a separately landed row: a one-definition case for the
  coordinator (the same pattern NF88 queued for 0x00739E60 / 0x008E74F0). 0x0059CF49 only declares it: its old row declared it, and the
  body I had added there was removed and the row regraded (RELOCATION_MATCH).
- 0x005A8A5D defines the GetDef<T> template body in its TU (pick-any COMDAT), as the console twin 0x005A29F4 does.

## Blocked (full reasons in STATUS.tsv; drafts in work/)
- 0x005BF77D GetResurrectionItemInInventory: 3 failed forms (1 probe + 4 grades). try1 SRC_FAIL. try2, with the opt-ins
  CTCINVENTORYBASE_RESURRECTIONITEM and CRESURRECTIONITEMDEF (retail class over CSubDefClassBase), DIFFER(359v334). try3, adding the
  header template bodies GetDef<T>(long) / GetSubDef<T> that retail's TU also saw, DIFFER(324v334), with every relocation right
  (work/005bf77d_try3.cpp, diff work/005bf77d_try3.diff; the two opt-ins are in work/hdr_blocked/, not hdr/). Residue: retail keeps the
  thing def in edi across GetSubDef and homes the element address; mine does the reverse. The cause is NOT isolated; the 3-form limit
  stopped me. Two untested candidates, cheapest first:
  (a) the template bodies are `__declspec(noinline)` only, while the old row's GetDef / GetSubDef stand-ins (which its Shape note credits
      with "def kept in edi") and the repo's GETDEF_INDEX_BODY sit under `#pragma auto_inline(off)`;
  (b) the GetAllItems TU-mate (the old row credits its fake same-TU GetAllItems body with "element address homed"). Retail 0x005BF482 is
      a non-template TU-mate, so (b) needs a same-TU strong copy of the landed body: the coordinator's one-definition decision.
- 0x005A30A1 InitialiseBuyableItems: the header form (0x005A4CDD's same-TU body) grades DIFFER(917v917): two `stock[key] = 1` stores
  tail-merged, temporaries one slot off (work/005a30a1_try1.cpp / _try2.cpp).

## recheck_final.log warnings (14, all read)
- RELOC_NAME / STANDIN_LEAF lines name catalog labels that section 1 of this hand-back respells (0x005BC4B2, 0x005BD1AE, 0x005BD328,
  0x0061A040, 0x0059D9AD, the static rows) or are checker artifacts with the right name (GetDef<CUIMiscThingsDef>, swap<pair<unsigned
  long, C3DMeshVolume> >). 0x00A6F390's catalog label is in NAME_FIXES.
- RELOC_CONFLICT `_$E2` (2): the generic static-function name, as in the Gossip pair.

## Rule slips (work/slips.txt)
- Slip 1: one st.py note for 0x005BD054 sat in double quotes with backticks; bash tried to run them (command not found; the Bash call
  still exited 0; nothing else changed). The note was rewritten from a quoted heredoc.
