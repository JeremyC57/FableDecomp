# NF92 notes (round 7, entries q11206..q11224)

Counts (from STATUS.tsv / APPLIED.tsv): 53 rows: 34 ported, 18 not-a-binder, 1 blocked (0x005BF654); 0x0059C6B0 deferred (held by NF91,
not in rows.txt). 19 entries: 18 done, 1 blocked (q11215). Final recheck: recheck_final.log, one vland run over all 34 ported rows:
5 MATCH + 29 RELOCATION_MATCH, every test PASS.

## Descriptive names
- No new made-up names. The STLport / CVectorMap instances (0x004F99DE, 0x004F9A6E, 0x004F9207, 0x004F927A, 0x004F930A, 0x00527950,
  0x004CCCE0, 0x004CD2F0, 0x004CD5D0, 0x004CD650, 0x004DD21C, 0x004E81C1, 0x006D9D90, 0x005C0C86, 0x005C0965) carry library spellings
  (descriptive-by-library: the instantiation's mangled name, one address each).
- 0x013B8A20 (the thing counter, ++ in CThing::CThing 0x004C9030, -- in ~CThing 0x004C8310) keeps the landed descriptive `g_thingCount`.
  Ego_d.pdb PUB32 `?NoThings@CThing@@2JA` is a candidate (one source; the static block 0x013B8A1C PCurrentThingManager / 0x013B8A20 /
  0x013B8A24 TCsToBeRemoved is consistent with it). It is not in NAME_FIXES, because it has only one source.

## q11218 (CThing::TCsToBeRemoved, a DATA symbol)
- Spelling: `?TCsToBeRemoved@CThing@@0V?$list@U?$pair@V?$CIntelligentPointer@VCThing@@@@PAVCTCBase@@@std@@...@std@@A`, a private
  static. Sources: Ego_d.pdb PUB32, and the Ego_r / FableWin initializer and atexit-destructor names. engine/CThing.h already carries it
  (NF90 FABLE_RETAIL_CTHING_TCSTOBEREMOVED).
- Catalog fix: 0x0121A670 is not `CThing::$E_TCsToBeRemoved`. It initialises the map at 0x013B8A8C =
  NThingManager::UIDsToReplaceWithUID (Ego_d PUB32 map<u64,u64>; Ego_r / FableWin initializer order 0x0121A630 -> 0x0121A650 ->
  0x0121A670). The real TCsToBeRemoved initializer is 0x0121A440, and its atexit destructor is 0x01228CE0. RENAMES s1 has all four.
- Binders ported: 0x004C9840 RemoveTC, 0x004C8050 WorldUpdate and 0x004C8310 ~CThing, all on the shared headers. Not-a-binder:
  0x004C9D60.
- WARN RELOC_CONFLICT `_$E2` on 0x0121A440 / 0x0121A670 (against landed 0x01219F00) is the usual collision of the TU-local `_$E`
  leaves. It is not a real conflict (NF89's 0x0121F0E0 / 0x01229BE0 show the same).

## q11224 (one definition of CModeDragonBase::OnDeactivate)
- Retail expands it. 0x008E74F0 opens with 0x008E72B0's exact body (`push 1 ; call [edx+98h]`, then `mov byte ptr [esi+18h],0`). The
  OnActivate pair (0x008E7290 / 0x008E7390) opens the same way. A same-TU probe (work/probe_008e74f0_expand.cpp: the out-of-line
  definition followed by the qualified `CModeDragonBase::OnDeactivate()` call) grades 0x008E74F0 RELOCATION_MATCH and 0x008E72B0 MATCH.
  0x008E72B0 is referenced only from the CModeDragonBase vtable (.rdata 0x012684F8). No .text call reaches it.
- Applied: the body is inline once in engine/CModeDragonBase.h (opt-in FABLE_RETAIL_CMODEDRAGONBASE_ONDEACTIVATE_INLINE, on NF90's
  inline CModeBase::OnDeactivate). 0x008E72B0 is the nf86 / nf88 pick-any scaffolding (MATCH). 0x008E74F0 makes the qualified call
  (RELOCATION_MATCH).
- Link proof (LINK= cleared, /DLL /NOENTRY /NODEFAULTLIB /FORCE:UNRESOLVED; logs in link/):
  - link/ctl.log: strong owner link/ctl_008e72b0.cpp (selection 1) + new 0x008E74F0 obj -> LNK2005 + LNK1169 (exit 145).
  - link/new.log: new 0x008E72B0 + new 0x008E74F0 objs, both at selection 2 -> exit 0 (LNK4088 only).
  - The first attempt without /NODEFAULTLIB stopped at LNK1104 libcp.lib.
- Caveat for the coordinator: FableWin.pdb puts CModeDragonBase::OnActivate / OnDeactivate at 0x02AD6170 / 0x02AD61C0, beside the
  CModeIntangible .cpp bodies. The header inlines (ctor, GetAnimFilter, dtors) are in the 0x024C0Axx group. So the original was probably a
  .cpp body that VC7.1 auto-inlined within its own TU. The header inline is the repo's one-TU-per-row spelling, which is why the
  scaffolding is marked `// Fidelity: scaffolding, not retail`.
- Follow-up (not in my locks): CModeDragonBase::OnActivate 0x008E7290 / CModeIntangible::OnActivate 0x008E7390 have the same shape.

## Flags for the coordinator
- 0x005BF654 (q11215) is blocked: port. The best draft is work/005bf654_blocked.cpp, at DIFFER(297v297), a register permutation only,
  unchanged over 7 grades. It uses the NF92 opt-in CTCINVENTORYITEM_INVENTORYCATEGORY_MEMBER (no finished row uses it) and a verbatim
  copy of NF91's CHeroTitleDef TYPENAME block, which is now repo text after bacd85eae1. The CHeroTitleDef.h overlay was dropped.
- Row 0x0083AEA0 (not-a-binder, q11207 shape reference): its own name is the stand-in CRetailTreeIterator::Decrement. Its landed
  binders 0x00839100 / 0x0083AFE0 spell the value type `UCWeaponTrailGraphicSet` (struct). Both PDBs spell it `VCWeaponTrailGraphicSet`
  (engine/CWeaponDef.h FABLE_RETAIL_CWEAPONTRAILGRAPHICSET_CLASS). A self rename needs those binders respelled too. Not changed.
- 0x0059C710 / 0x0059C6DD keep binding what the repo has for GetCategoryIndexForTradeType 0x0059B6F4 (NF91 held it back). Each has
  the same-TU `#pragma auto_inline(off)` strong copy, the landed 0x0059C6B0 form. The shared-header inline + noinline form gives
  DIFFER 57v55.
- 0x004CC150 list<CTCBase*>::remove: its own row is still a local STLport model. Callers now bind the PDB spelling (NAME_FIXES).
- Overlays were rebased onto NF91's landing (bacd85eae1) by three-way merge, with no conflicts. orig/ is current for every overlay
  (checked byte-equal to the repo).

## Rule slips
- Two Bash calls exited non-zero:
  - a `[ -n ]` test as the last command of a loop;
  - a `wc` on test files looked up under rebuild/src instead of rebuild/tests (q11224).
- 0x0059C710 was left in progress while later rows were worked (its form depended on NF91's q11204).
- Queue order: q11218's last two binders, 0x004C8050 WorldUpdate and 0x004C8310 ~CThing, were ported after q11224 (they had been
  passed over earlier as large). The brief says q11224 last.
- grades_used in STATUS.tsv was reconciled with grades.log at hand-back. It now counts every compile, including relocation dumps, the
  0x008E72B0 link control and the regrades after the overlay restore. Maximum 9 (0x004C8050); no row is over its limit.
- When adding the 0x004C8050 opt-in, I overwrote hdr/stlport/fable_containers.h (and its orig/) with a fresh repo copy. This
  dropped my earlier FABLE_RETAIL_STLP_BITSET_RESET_DECL block (0x004C9840).
  - Caught by checking that every opt-in used in rows/ exists in rebuild/include or hdr/.
  - The block was restored verbatim from the session transcript, and 0x004C9840 was regraded RELOCATION_MATCH.
  - orig/ is again the repo copy. The final recheck covers both rows.
