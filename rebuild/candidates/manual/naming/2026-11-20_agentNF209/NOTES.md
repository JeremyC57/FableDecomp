# NF209 notes (round 10, 2026-11-20; single agent; entries in the brief's order; handed back at the q11772 / q4389 entry boundary)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 8 entries: 4 done (q11692, q12295, q11879, q4860), 3 blocked (q11760, q11873:
port; q11772: pick-any), 1 open (q4389, not started). Rows in rows/: 4 ported (0x008E9850, 0x008D9530, 0x009A0AE0, 0x00E2AB30), all
RELOCATION_MATCH PASS in recheck_final.log. Held outside rows/: 0x0047DB73 (RELOCATION_MATCH PASS draft, work/held/, pick-any decision);
blocked draft 0x006D41F0 (work/006d41f0_blocked.cpp, DIFFER 307v295). Not-a-binder: 0x008E9950. Header opt-ins: none (no overlay).
Self renames (RENAMES s1): 0x009A0AE0, 0x00E2AB30; caller respellings (s2): 8 lines. No made-up names: every new spelling is a PDB name,
a compiler-generated ??_G, or a repo STLport spelling.

## Coordinator decisions needed
1. q11772 / 0x0047DB73 (held draft, blocked pick-any): its object newly defines the inline COMDAT ?HasTC@CThing@@QBE_NW4ETCInterfaceType@@@Z
   (8 bytes: `call test`), which reloc_bindings maps to 0x004118C0 (witness 0x00488620). 0x004118C0 is a retail out-of-line CThing::HasTC with
   the bitset test expanded (no catalog row); landed 0x00488620 defines that 58-byte copy (cmpbody False). Held under the pick-any rule; it
   needs a way to keep HasTC's COMDAT equal (or a ruling). NAME_FIXES adds the 0x004118C0 catalog name.
2. q4860 / 0x00E2AB30 (in rows/): one LNK2005 on ??0CQS_ScytheInfoScript (identical 113-byte copy vs 0x00E299F0's strong definition;
   accepted-debt rule) and the stand-in vtable / ??_G copies 0x00E299F0 emits; queue 0x00E299F0's port (details in the q4860 section).
3. q11879 / 0x009A0AE0 (in rows/): queue 0x0122A9A0 and 0x009F2B20 to stop emitting their differing ??_G / ?Destroy byproducts (advisor
   ruling applied; 0x009F2B20 also spells ?Destroy@CMemoryAllocator@@KAXXZ, not the PDB KIX; no LNK2005 with either, link/ctl2 / new2).


## q11692 + q12295 (0x0073DFF0 CAnimFilterOfGroup::SetCurrentGroupName; held draft 0x008E9850)
- 0x008E9850: NF205's held draft landed verbatim after the advisor checks (nf205 amendment): cmpbody of SetCurrentGroupName vs owner 0x0073DFF0's
  object True/True; GetCRC vs owner 0x00404310 bytes True, relocs differ only by the owner's stand-in ?g_EmptyString@@3QBDB for the same retail
  address 0x0129AAF4 (both external, both bound there in reloc_bindings); 0x00403A10 / 0x00403A00 owners define free-function stand-ins (no
  symbol clash); q.py of the row's source at 0x00404310 / 0x00403A10 / 0x00403A00 / 0x0073DFF0: RELOCATION_MATCH / RELOCATION_MATCH / MATCH /
  RELOCATION_MATCH. All six new inline copies are byte- and reloc-identical to the ones landed 0x0073DFF0's object already defines.
- New binding at 0x008E9850+0x86: ?NullChar@CCharString@@1DB at 0x0129AAF4 (HEAD: a "" literal COMDAT, no binding). NullChar is already bound
  there (witness 0x00AE8D50). 0x0129AAF4 is an existing many-spelling split (18 spellings in reloc_bindings); NF205 already queued the
  0x00404310 / 0x00403A10 respells (its NAME_FIXES), so no new NAME_FIXES line here.
- 0x008D9530 (CModeBerserk::FrameUpdate): ported in one form. PDB spelling replacing a stand-in on a callee (count does not grow):
  0x0099EC30 ??0NativeStringValue@@QAE@ABU0@@Z -> ??0CCharString@@QAE@ABV0@@Z (Ego_r / FableWin; already bound, witness 0x0046381B).
  0x008DA690 / 0x008DA890 still declare the NativeStringValue / AnimationGroupFilter stand-ins but bind neither 0x0099EC30 nor 0x0073DFF0
  (their HEAD objects, binds.py), so both stand-in spellings leave the binding set.
- vland WARN SHARED_HEADER_AVAILABLE on 0x008E9850 (CThing, CTCWeapon, C3DAnimationInfo, CMeshDataBank, CMainGameComponent): HEAD-local models,
  unchanged; no class token was added to either row (0x008D9530 keeps its HEAD ModeStandNative / ThingNative / NativeStringValue / CModeBerserk).
- EH funclets: no landed row includes either changed file.

## q11879 (0x009F1F90 ??3CMemoryAllocator; last binder 0x009A0AE0)
- 0x009A0AE0 re-authored as the owner ??_GCMemoryAllocator@@IAEPAXI@Z (NF205 draft unchanged; `// Fidelity: scaffolding, not retail`, helper
  in the TU-hashed anonymous namespace). Self rename (RENAMES s1); its old leaf is the HEAD object's ?CMemoryAllocator_scalar_deleting_destructor
  (no Leaf line at HEAD). The 10 other exe-scan referrers of 0x009F1F90 include engine/CMemoryAllocator.h and spell no stand-in.
- Advisor ruling conditions, each checked: link/ (LINK= cleared) ctl.log / new.log no LNK2005, pos.log positive control LNK2005; 0x0122A9A0's
  pairs bind neither ??_G nor ?Destroy; refs.py 0x009A0AE0 = 0 referrers.
- Debt for the coordinator to queue: landed 0x0122A9A0 (DEALLOCATOR opt-in) emits unbound byproduct copies ??_GCMemoryAllocator@@IAEPAXI@Z
  (35 bytes, VirtualFree expanded) and ?Destroy@CMemoryAllocator@@KIXXZ (44) that differ from this owner's (28 / 37). Pick-any: the linker
  may keep either; the fix is to stop 0x0122A9A0 emitting them (or make its operator delete declaration-only there).
- EH funclets: none include the file. The catalog-name change (`scalar_deleting_destructor' -> `scalar deleting destructor') may move the file
  name at landing; nothing includes it.

## q4860 (0x00E2AB30 NScript::CQS_ScytheInfoScript ??_G)
- New local model (all three conditions hold): NScript::CQS_ScytheInfoScript has no header anywhere under rebuild/include (grep `class
  CQS_ScytheInfoScript`); landed siblings 0x00E267C0 / 0x00E299D0 model their CQS_* level script the same way; the base CLevelScriptBase<T>
  comes from engine/CLevelScriptBase.h. Master data CQ_SunnyvaleMasterData: Ego_r.pdb types the nested CScytheMarker / CScytheNearOracle
  PMasterData (+0x18) as CQ_SunnyvaleMasterData*; the landed entity rows cast to it.
- Member names from Ego_r.pdb (MissionSucceeded, cs_args, cs_chars); retail offsets from the body (+0x4C / +0x58) and the Alloc's 0x64.
- Coordinator decisions / debt:
  1. One LNK2005 (link/q4860/new.log; ctl.log has none): the inline PDB ctor copy equals 0x00E299F0's strong definition (113 bytes,
     cmpbody True/True; q.py of this source at 0x00E299F0 RELOCATION_MATCH). Accepted-debt rule (identical copy vs strong owner).
     Queue: port 0x00E299F0 onto this model as pick-any (its HEAD form is a local CScriptBase / CRetailMap stand-in).
  2. Vtable ??_7CQS_ScytheInfoScript@NScript@@6B@: this row's copy follows retail 0x012E22C8 slot for slot; 0x00E299F0's copy is a 4-byte
     one-slot stand-in (SELECT_ANY data). 0x00E299F0 also emits a 30-byte ??_GCQS_ScytheInfoScript (calls ??1 out of line) that differs from
     this owner's 48 bytes: HEAD debt (nf203). Both clear with the queued port of 0x00E299F0.
  3. Slot 2 ?Main@CQS_ScytheInfoScript@NScript@@UAEXXZ (0x00E29B20) has no landed row (unresolved until authored). Slot 4 binds 0x00CBD4E0 as
     ?OnPersist@CScriptBase@@UAEXAAVCPersistContext@@@Z (FableWin.pdb + engine/CScriptBase.h, as the sibling CLevelScriptBase vtables do); that
     row's own leaf is an old stand-in (T::Hook, no Leaf line): existing split, not grown.
  4. ??1?$CLevelScriptBase@VCQ_SunnyvaleMasterData@NScript@@@@UAE@XZ (5-byte jmp) is table-bound at 0x00CBD510 by 0x00D80C00, whose body is
     ~CScriptBase: under nf181 the copy has no owner; it equals the sibling 0x00E267C0's copy. Queue the 0x00D80C00 binding.
- The old self leaf ?CThingSwitch_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z is also the Leaf of many other rows (other addresses,
  e.g. 0x00565870, 0x00D67240): no reloc_bindings line binds it; those rows are their own stand-in debt.
- A FABLE_FORCEINLINE ctor probe was tried and dropped (it changed nothing: VC7.1 emits the inline ctor COMDAT anyway, .debug$F only).
- The build_candidates Module is still `CVtable_012e22c8`; RENAMES s1 gives NScript::CQS_ScytheInfoScript. No EH funclet includes the file.

## q11760 + q11873 (0x006D41F0 CPersistTraits<pair<CCharString,long>>::TransferIn; referrer 0x006D4180): blocked, port
- Claim verified: Ego_r.pdb 0x00708E10 + FableWin.pdb 0x02490FA0 carry the exact symbol; the body reads "(" string "," long ")".
- The self row cannot keep its N006d41f0 stand-ins under a respelled leaf (turning N006d41f0::CPersistTraits / CCharString into the real
  tokens is the nf95b case), so it was ported onto fable_string.h + engine/CStringParser.h + the full engine/CPersistTraits.h with the
  existing FABLE_RETAIL_CPERSISTTRAITS_PAIR body. Best DIFFER(307v295): a per-call-site inline split of CBasicString<char>::operator==(const char*)
  (retail expands it at the "(" test, calls 0x004115A0 at "," / ")"). 3 forms (16 compiles: form 1, a /d2inlS sweep, a /d2inlT sweep).
- 0x006D4180 was not changed: it must move in the same hand-back as the self rename (std::pair from stlport, reference parameters).

## q11772 (0x00410DE0 std::bitset<274>::test; last wrong binder 0x0047DB73): blocked, pick-any decision (above)
- Draft: CThing::HasTC(TCI_IN_GAME_MENU) (0xC6, engine/ETCInterfaceType.h) with FABLE_RETAIL_STLP_BITSET_TEST_DECL defined before the
  first include (the first try defined it after fable_containers.h was already included and emitted the whole test body); `// Inferred:`
  retail discards the result. Test rewritten (a std::bitset<274>::test double replaces the D double). GlobalGui::Notify left as is.

## q4389 (FP-COMBATMAP leftovers): open, not started.

## Rule slips
- One Bash call exited 1: the side-by-side diff script after the first /O2 grade of 0x006D41F0 (IndexError on the log split); rerun fixed. grades_used was reconciled with work/grades.log (counts compiles of the row's own source at any address).
- 0x006D41F0 took 16 compiles inside 3 forms (two flag sweeps of 3 and 8 values); the round-10 budget counts forms, not compiles.
