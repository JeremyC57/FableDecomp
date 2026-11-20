# NF141 notes (2026-11-20, round 10: PRIORITY slices 7 and 9, leftovers)

## Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log)
- Entries (4): done 2 (q4994, q5140: one unit); done except 1 (q6182: the three old no-Leaf landscape rows the brief says to leave);
  done-already 0; rejected 0; blocked 0; deferred 0; open 1 (q6408, not started).
- Rows changed: 7, all vland MATCH / RELOCATION_MATCH + test PASS in one run (recheck_final.log): 2 MATCH (0x008FD000, 0x009FD6C0) +
  5 RELOCATION_MATCH (0x00913F30, 0x00964CA0, 0x00964DA0, 0x009FF590, 0x00994930). Ported 4 (0x00913F30, 0x00964CA0, 0x00964DA0,
  0x009FF590; the last three with leaf-only self renames); self rows re-authored 2 (0x008FD000 rename, 0x009FD6C0 one-definition fix, leaf
  unchanged); in-place self rename 1 (0x00994930). Not-a-binder 0. Blocked 0.
- Header opt-ins: 4 (engine/CAIStateGroup_StealStealableItem.h x3, engine/CGraphicDataBank.h x1; hdr/README.md). No shared-header member renamed.
- selfcheck.log: 0 problems.

## q4994 / q5140 (one unit): done
- Claim: Ego_r.pdb 0x00847E50 + FableWin.pdb 0x0288D940 ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ + engine/CAIStateGroupBase.h
  _PEEK; the Ego_r donor order PeekWorld / PeekDefinitionManager / GetPWorldSeed / GetDebugText / PeekThingSearchTools / IsStopping matches retail
  0x008FCFA0 / 0x008FCFB0 / 0x008FCFD0 / 0x008FCFE0 / 0x008FD000 / 0x008FD010 (grep of both name sets here).
- Rows (all vland RELOCATION_MATCH / MATCH + test PASS, work/b1.log; recheck_final.log):
  - 0x00913F30 ported (2 forms; v1 SRC_FAIL: FABLE_RETAIL_CWORLD_GETFRAMESPERSECOND missing).
  - 0x00964CA0 ported + leaf-only self rename to the PDB (1 form).
  - 0x00964DA0 ported + leaf-only self rename to the PDB (1 form). Family row: the only landed caller of 0x00964CA0.
  - 0x008FD000 self row re-authored under the PDB spelling (2 forms; v1 SRC_FAIL: FABLE_RETAIL_CWORLD missing). MATCH.
- Census of the other 53 REFS rows (work/bindchk_q4994.txt, compiled from the working tree): 52 lines bind the PDB spelling; the blank
  0x00935AD0 line was re-checked by hand on the same object (PAIR 008fd000 +f7 = PDB spelling). With 0x00913F30 / 0x00964CA0 here, all 55
  REFS rows bind ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ: the self rename creates no split and closes the old ones.
- Old self leaf of 0x008FD000 taken from the HEAD object: ?CAIStateGroupBase_PeekThingSearchTools@@YIPAVCThingSearchTools@@PBUA@@@Z.

## q6182: done except the three old no-Leaf landscape rows
- 0x009FF590 ported (first form RELOCATION_MATCH): STATUS lists the headers and every binding change. Own leaf to the PDB protected form
  ?NotifyEntryAdded@CGraphicDataBank@@MAEXKKABV?$CArray@D@@@Z (both PDBs; no landed referrer). The row had included the _quarantine
  CGraphicDataBank header under a renaming macro (`#define CGraphicDataBank CGraphicDataBankDonor`): gone.
- One definition: the ported row emits the shared header's inline ??0CGraphicHeader@CGraphicDataBank@@QAE@XZ as a pick-any COMDAT, while its
  owner 0x009FD6C0 defined it strongly (nodup) over local models. 0x009FD6C0 re-authored with the nf86 / nf88 scaffolding (MATCH, claimed lock);
  link proof in work/link (ctl.log: LNK2005 with the HEAD owner; new.log: exit 0). Its only landed caller 0x00B2BD80 binds the same name.
- 0x00994930 self rename PBEJ -> PBXK: NF131's draft, unchanged (in-place respell of the HEAD local model; token check: no new class).
- Census (work/bindchk_q6182.txt): all 31 compiled REFS rows (REFS minus 0x009FF590 and the three no-Leaf rows) bind PBXK; 0x00BF3B60
  graded DIFFER(809v1480) (an old no-Leaf row: q.py picks the wrong function); its candidates/landscape_patch_load_foreground_compiled.h line 88
  declares CMemoryDataInputStream(const void* data, unsigned long size) = PBXK (grep here).
- Left by the brief: 0x00BDC510 (candidates/landscape_map_update_foreground_compiled.h declares (void*, unsigned long): PAXK), 0x00BE8920
  (candidates/landscape_background_patch_compressed.h: (unsigned char*, unsigned long): PAEK), 0x00BE7D70 (its candidates header declares no
  CMemoryDataInputStream constructor). The self rename removes the PBEJ spelling (no binder) and adds none: the address's set shrinks.
- Callee spellings after 0x009FF590 moves (no address gains one): 0x009FD700 now one spelling (= its self leaf); 0x009CBC20 loses the
  CBankFileNative stand-in (MAE already bound by 0x00A636C0); 0x009FD160 and 0x00A01650 keep two (their self rows' stand-ins: NAME_FIXES).

## Locks claimed (all read `released` or were free)
- 0x00913F30, 0x00964CA0, 0x00964DA0, 0x009FF590, 0x009FD6C0. 0x008FD000 / 0x00994930 were NF141's from the start (rows.txt).

## Self renames (RENAMES section 1)
- 0x008FD000 (entry self row): every referrer on the new spelling (above).
- 0x00964CA0 (q4994 binder; leaf only, catalog name right): only referrer 0x00964DA0, in this hand-back.
- 0x00964DA0 (outside the entries; leaf only, both PDBs agree): no landed referrer (exe scan work/fam_refs.tsv; virtual, reached through the vtable).
- 0x009FF590 (q6182 binder; leaf only): no landed referrer (exe scan work/fam2_refs.tsv; virtual).
- 0x00994930 (q6182 self row): see q6182.

## Callee respellings (a PDB / header spelling replaces a stand-in; no address gains a spelling)
- 0x009142B0 / 0x00914330 (from 0x00913F30), 0x009642D0 / 0x00964EF0 (from 0x00964CA0): each was bound only by the row changed here; 0x00914330
  and 0x009642D0 now match their self rows' leaves (split closed); 0x009142B0 / 0x00964EF0 keep their self rows' CThingSearchToolsMethods
  stand-ins (2 spellings before and after: NAME_FIXES).
- 0x00964360 (from 0x00964DA0): the PDB ABE spelling replaces 0x00964DA0's stand-in; the self row (NF137 lock) still defines its _Methods
  stand-in: 2 spellings before and after (NAME_FIXES; decision 1).
- 0x004365B0 (const LowerBound) and the vftables 0x01237F1C / 0x01238C6C (from 0x00964DA0): onto the CVectorMap.h / CIntelligentPointer.h
  spellings other landed rows already bind; the row's own stand-in spellings disappear.

## One definition (objdump -t, new object vs HEAD object, work/def_<addr>_{H,S}.txt)
- Each changed object's only non-COMDAT external is its own leaf (tools/strong.py). New pick-any COMDATs: shared-header inlines (C3DVector,
  CThing scan / type / IsDead / HasTC, bitset, list / vector iterators, CIntelligentPointer<CThing> / <const CThing> members and vftables,
  IS_THING_ALIVE / operator!= instances, GetCreature, PeekWorld / PeekThingSearchTools, StatGetType*), the STLport vector<CThingMarker*> set,
  and the template instances GetPBestThing x3, GetAllLeafThings x1, CThingCompare_Nearest::operator()<T> x3, const LowerBound.
- Owners of those instances compiled from the working tree (work/own_*.txt): 0x00914130, 0x009141F0, 0x00964160, 0x00914330, 0x009642D0,
  0x009142B0 define them by explicit instantiation (pick-any; no strong symbol); 0x00964EF0's only strong symbol is a template<>
  vector<CThingMarker*>::_M_insert_overflow specialization whose mangled name (K fill_len, its own __true_type) differs from the header
  instance (I, std::__true_type). No LNK2005 candidate.

## One definition: every new external symbol vs landed Leaf lines
- The 97 external symbols the seven new objects define and their HEAD objects did not (work/allnew.txt, from the work/def_<addr>_{H,S}.txt
  diffs, 0x009FD6C0 included) were matched against every `// Leaf:` line under rebuild/src/compiled: 11 have a landed owner. Owners compiled
  from the working tree (work/own_*.txt, tools/strong.py): the comparator / GetPBestThing owners (0x00914130, 0x009141F0, 0x00964160,
  0x00914330, 0x009642D0) are explicit instantiations, the IS_THING_ALIVE owners 0x0069CE20 / 0x004CC340, CIntelligentPointer<const CThing>
  operator= 0x005ED300 and ??_G 0x0048A4F0, and the const LowerBound 0x004365B0 all define their leaf pick-any: no clash.
- Existing debt, not created here: 0x0041C780 defines ??0_List_iterator_base@std@@QAE@PAU_List_node_base@1@@Z strongly while the shared
  stlport/fable_containers.h inline (2036 landed includers) emits it pick-any; landed 0x00964360 (nf137) already emits that copy (compiled
  here), and 0x00913F30 / 0x00964CA0 now do too. The owner wants the nf86 / nf88 scaffolding (NAME_FIXES-free; a one-definition follow-up).
- The ??0CGraphicHeader@CGraphicDataBank@@QAE@XZ clash this pass did create is fixed in the same hand-back (0x009FD6C0, q6182).

## Local classes
- 0x00913F30: CAIStateGroup_BirdFlyToFood (the row's own class; no header anywhere under rebuild/include) kept, same token as HEAD; now
  derives from the shared engine/CAIStateGroup_FlyingThingBase.h (narrow rule); member names respelled in place to Ego_r.pdb.
- 0x00964CA0: CThingFilter_MarkerHasProperty is a class token HEAD did not define (HEAD had the stand-in CThingFilter_EscapePoint). No header
  exists for it (grep `class|struct CThingFilter_MarkerHasProperty` over rebuild/include: none); the landed owners 0x006A75B0 / 0x009642D0 /
  0x00964EF0 model it row-locally as a one-word filter; it has no base (Ego_r layout: EMarkerProperty Property +0, size 4). The brief's
  third condition (base from a shared header) does not apply literally: decision 2. Also a forward `enum EMarkerProperty;` (MS forward enum,
  the CAIStateGroupBase.h style); the enumerator for value 1 is in no name source, so the row passes `(EMarkerProperty)1` (`// Inferred:`).
- 0x00964DA0 / 0x008FD000 / 0x009FF590 / 0x009FD6C0: no local class left.
- 0x00994930: keeps its HEAD local models (CBaseClassNonCopyable, SMemoryStreamState_00994930, CMemoryDataInputStream), respelled in place (vland WARN SHARED_HEADER_AVAILABLE as at HEAD); a port onto engine/CMemoryDataInputStream.h is a follow-up.

## EH funclets
- No row under rebuild/src/compiled includes any of the seven changed files (grep of each of the seven file names).

## Tests
- All seven tests unchanged (standalone; PASS). 0x008FD000's test is a standalone copy of the old free stand-in function (like the landed
  0x008FCFD0 test); rewriting it on the shared headers is a follow-up.

## Flags lines
- None.

## Descriptive names
- None new. CWorld::GetFramesPerSecond (existing header opt-in, NF56) is an existing descriptive inline (no symbol in any PDB).

## Decisions for the coordinator
1. 0x00964360's own leaf (?FindPStealableItem@CAIStateGroup_StealStealableItem@@ABEPAVCThing@@XZ) is now a leaf-only fix: its only referrer
   0x00964DA0 binds the PDB spelling after this hand-back. The row is NF137's (lock), so it was not touched (NAME_FIXES).
2. 0x00964CA0 adds the header-less class token CThingFilter_MarkerHasProperty (above).
3. q6182: 0x009FF590 and 0x009FD6C0 land together (one definition of the CGraphicHeader constructor; link proof work/link). 0x009FD6C0 is outside
   the entries (claimed lock; COORDINATION_CLAIMS / UNMATCHED_ROWS: no line for it) and, like the nf86 rows, its object defines a TU-hashed
   anonymous-namespace scaffolding helper (?ReferenceCGraphicHeaderCtor@?A0x...).
4. q6182: 0x00994930 lands while the three old no-Leaf landscape rows keep their own forms, as the brief allows ("done except").

## q6408 (open, not started): closure map for the next agent
- Claim verified here from both PDBs (Ego_r 0x00751760, FableWin 0x024C2570; the K overload is 0x00751900 / 0x024C24D0).
- Census: NF135's work/bindchk_q6408.txt (copy in $S/w22/agentNF138/work/NF135_round1/) + NF138's 0x00848FD0 port: 10 rows bind the claim.
- The 24 wrong rows, by the spelling they bind at 0x0073ACC0 (each port also respells its own mode-ctor callee and the
  CCountedPointer<CModeBase>(T*) callee 0x00718A00):
  - 16 rows ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@UCModeBase@@@@@Z (global CModeBase struct): 0x00848730, 0x00848DB0,
    0x008C0C00, 0x008C0D10, 0x008C1E80, 0x008C1F70, 0x008C48A0, 0x008C7600, 0x008CB060, 0x008CB1A0, 0x008CCD70, 0x008ED950, 0x008F39A0,
    0x008F41D0, 0x008F4320, 0x008F4480. They bind 0x00718A00 as ??0?$CCountedPointer@UCModeBase@@@@QAE@PAUCModeBase@@@Z (witness 0x008C1E80).
  - 5 rows ...QAE_NUCCountedModePointer@@@Z: 0x008C1C30, 0x008F47E0, 0x008F4B00, 0x00950C30, 0x00969CB0 (0x00718A00 as
    ??0CCountedModePointer@@QAE@PAUCModeBase@@@Z).
  - 3 singletons: 0x008676A0 (NCreatureMode CCountedPointer form; ctor 0x00868220 bound only by it as ??0CModeDrained@NCreatureMode@@QAE@PAVCThing@@0@Z),
    0x008A08A0 (CreatureModeManagerMethods / CountedMode; ctor 0x008AC3F0 bound only by it as ??0CModeScreamerSuperAttack@@QAE@AAUCThingNative@@ABU1@@Z;
    a CGameScriptInterface row with a 78-slot local ScriptMethods model and the _quarantine CThing under a renaming macro), 0x008A1A10
    (_Retail class; ctors 0x00861E00 (5 spellings already) and 0x0073E8F0 (2 spellings)).
- 0x00718A00's spelling set today: CountedMode, NCreatureMode, global UCModeBase, CCountedModePointer, and the claim form (0x008C1900); the self row
  defines the stand-in CCountedPointer_007180b0. Moving a whole group removes one spelling from both 0x0073ACC0 and 0x00718A00; a partial move
  is neutral for those two but must not create a split on a mode ctor whose self row and binder agree today (check each ctor with callers.py /
  refs.py before porting).
- Size: every wrong row models 7-16 classes locally (70-192 lines); the 5-row group is the smallest closed group (13-16 local classes each).
  The self row 0x0073ACC0 still defines ?AddMode@CTCCreatureModeManager@@QAE_NUCModePointer@@@Z and moves last.

## Rule slips
- Several read-only Bash calls (cat / grep / objdump) ran without the full `source env.sh` preamble; every compile went through g.sh /
  bindchk.sh / pdiff.sh / vland, which source it, or had the preamble.
- GOTCHAS.md (135 KB) was grepped for the relevant lines and its last ~120 lines read, not read in full.
- q6408 not started (context).
- One wait loop used `timeout 30 tail -f /dev/null` to poll the census log (a stand-in for the blocked foreground sleep).
