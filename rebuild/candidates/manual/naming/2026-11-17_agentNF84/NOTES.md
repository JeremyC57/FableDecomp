# NF84 notes (round 7: the nf83 follow-ups q11044..q11055 and 23 rows NF64 left blocked)

Totals (from STATUS.tsv / APPLIED.tsv): 51 rows (the 45 locked rows plus 6 extra rows claimed after the compile scan and appended
to rows.txt; rows_all.txt is the original list): 16 ported, 8 not-a-binder, 27 blocked (14 authoring, 13 port), 0 deferred.
12 entries: 4 done (q11044, q11045, q11046, q11054), 3 done-already (q11047, q11049, q11055), 5 blocked (q11048, q11050..q11053;
every one of these has its self row right and is blocked only by binder rows: the slice ones in STATUS, the outside ones from a
compile scan in work/outside_binders.tsv and NAME_FIXES.tsv). All 16 ported rows: RELOCATION_MATCH + vland PASS in one recheck_final.log run over the final files.
RENAMES.tsv: 8 section-1 renames, 19 section-2 respellings (old spellings from probes of the landed rows, new from the final compiles).

## Ported rows
- 0x00789358 CHitParameters::~CHitParameters (self rename; Ego_r + FableWin + the header's _CTOR block + layout); binds the
  EDamageAttribute ~_Rb_tree 0x00451AC6. Unblocks the dtor half of 0x008E7E70.
- 0x004585BD CArmourDef::~CArmourDef (self rename; only caller is CArmourDef's ??_G 0x004585A1); NF84 opt-in FABLE_RETAIL_CARMOURDEF_DTOR.
- 0x0085FF10 NCreatureMode::GetUpFromSitting ported onto the shared headers; RemoveModeWithAction now binds the namespaced PDB
  symbol. The CCreatureAction_GetUpFromSitting 4-arg ctor body is defined `inline` in the row (retail's TU holds its COMDAT
  0x00860150; VC7.1 calls it on the temporary and inlines it on the named local, as retail): the object carries
  ??0CCreatureAction_GetUpFromSitting@@... as a pick-any COMDAT. When 0x00860150's stand-in row is re-authored it must not define a
  nodup copy of that symbol (NAME_FIXES).
- 0x009387F0 CAIStateGroup_UsesSeat::OnReplace ported onto engine/CAIStateGroup_UsesSeat.h (+ base / brain / creature headers);
  binds the PDB GetUpFromSitting. Members now use the header names (GotSeatPos, SeatPosIndex, SittingOnBench).
- 0x0059B576 deque<NUISystem::CComponent*>::_M_push_back_aux_v (self, STLport, fable_deque.h shared body).
- 0x0059AAE3 _Deque_iterator<CComponent*, _Nonconst_traits>::operator- (moved onto fable_deque.h; catalog name fixed, leaf unchanged).
- 0x0059B4FE deque<NUISystem::CComponent*>::deque(const deque&) (self rename from _unattributed::PointerDeque); NF84 opt-in
  FABLE_RETAIL_STLP_DEQUE_COPY_CTOR.
- 0x006DC020 CPersistTraits<CComboMultiplierData>::~CPersistTraits (q11054; see below).
- 0x00981EA0 CAIStateGroupBase::AIAction_SpeakAISpeechToTarget: leaf respelt to Ego_r 0x0084D220 (IAE, const CCharString&);
  NF84 opt-in FABLE_RETAIL_CAISTATEGROUPBASE_SPEAKAISPEECH_WAIT for the 7-argument callee 0x00981350.
- 0x0053CB9B map<unsigned long, NUISystem::CUIState>::map(const map&) (self rename); NF84 opt-in FABLE_RETAIL_STLP_TREE_COPY_CTOR_SMINMAX.

- Six extra q11050 binders (0x0057B783 0x006644F0 0x00928E20 0x009621F0 0x0097D8B0 0x0097DB70), found by the compile scan: the
  pre-existing local CThingCreatureBase models' IsFreeToPerformAction respelt in place to the PDB form (const, by const reference;
  NF83's in-place precedent), the call passing the action by reference. Their other stand-in (the 0x00668D90 Reset, where present)
  is unchanged: it needs the shared CCountedPointer<T>.

## Split check of my own renames
Every landed caller of the eight section-1 addresses was compiled once (work/scan2.tsv): none binds an old self spelling (all were
row-specific stand-ins), so no rename splits a caller anew. Stand-in binders that remain (NAME_FIXES): 0x004585A1 (CArmourDef ??_G),
0x00812110 / 0x00812150 (CHitParameters dtor), 0x0053CCD0 (map copy ctor), 0x005959AB (deque copy ctor, missing allocator argument).

## NF64 entries whose state changed
- q10659 (0x0053CB9B): its self row is now the map copy constructor its callers bind (ported).
- q10656 (0x00981EA0): its self row now defines the Ego_r spelling; the callers 0x0091D190 / 0x0095EE70 stay blocked.

## q11054 (name collision) -- judgement call, stated for the coordinator
0x00451AC6 keeps ~_Rb_tree<EDamageAttribute, ...> (it has the callers). 0x006DC020's body only destroys a map<EDamageAttribute,float>
at +0, which fits three names equally: ~map<EDamageAttribute,float>, ~CComboMultiplierData, ~CPersistTraits<CComboMultiplierData>
(VC7.1 emits all three at 66 bytes when it compiles the landed 0x006DCFA0 TU). Bytes cannot decide. TU placement does:
- FableWin.pdb keeps ??0 / ??1 ?$CPersistTraits@VCComboMultiplierData@@ adjacent inside the tc_hero_experience TU (0x01F00A40 /
  0x01F00A60), whereas its ??1CComboMultiplierData (0x01C189B8) and ~map<EDamageAttribute,float> (0x0194A593, armour-def TU) came
  from other objects;
- Ego_d.pdb tc_hero_experience.obj emits GetDefaultVal<CComboMultiplierData>, then the traits ctor and dtor adjacent;
- retail has one ctor/dtor pair here (0x006DBFE0 54 bytes / 0x006DC020) right after TransferBinaryIn<CComboMultiplierData>, and the
  same pattern for CHeroStatIncreaseData (0x006DC1B0 / 0x006DC1F0 after 0x006DC070).
So the name is the best-supported one, not byte-proven; the alternatives are listed. NAME_FIXES proposes the matching ctor and the
CHeroStatIncreaseData pair (not graded).

## Descriptive names (marked per the 2026-11-10 rule)
- STLport leaves (0x0059B576, 0x0059AAE3, 0x0059B4FE, 0x0053CB9B, and the new bindings ~_Rb_tree<EObjectAugmentationType,float>
  0x00451A85, __uninitialized_copy(.., __true_type) 0x0059B49B, _M_reserve_map_at_back IAE 0x0059B4D8): descriptive by library
  (retail is STLport 4.6; both PDBs are Dinkumware).
- 0x00981350's seven-argument AIAction_SpeakAISpeechToTarget spelling: no PDB has the overload (descriptive, sibling pattern).
- 0x006DC020: see above.

## Second sources found
- q11055: FableWin.pdb 0x02833100 ?SetAsPerformAction@CBrainOutput@@QAEXABVCCreatureActionBase@@@Z (NF83 had it as Ego_d-only).
- 0x005BEAD5 / 0x005BF30D (NF64 "fragments"): complete wrappers; Ego_r TU order names them CTCInventoryBase::GetBoughtPrice and
  ::GetAmountOfItemsContainable, and shows the landed 0x005BE8D3 "GetBoughtPrice" is GetPerItemDataForItem (NAME_FIXES; not graded).

## Blocked rows (STATUS gives each reason)
- q11048: the five ??_G binders need their class vtable (constructor TU); ten more ??_G rows outside the slice bind 0x00BEB550 under
  stand-ins. 0x004861C3 (CTCInGameMenu ??_G) has the same blocker.
- 0x0074ACE0: one port form failed (C2259, abstract CCreatureActionBase under the row's local action models); needs
  CCreatureAction_UndeadRiseUp as a real class header and CTCCreatureGenerator.h opt-ins first.
- The other 20 NF64 rows were probed (all RM, unchanged) and kept blocked with NF64's reasons plus the updates noted in STATUS
  (0x008E7E70, 0x0091D190, 0x0095EE70, 0x00A86EA0, 0x005BEAD5, 0x005BF30D); no new port form was graded for them in this pass.

## Headers
Four overlays (hdr/README.md): engine/CArmourDef.h, engine/CAIStateGroupBase.h, stlport/fable_deque.h, stlport/fable_map.h. Base
copies in orig/ equal the repo at hand-back (NF82's landing 830629f2dc did not touch them). Token identity without the opt-ins:
tokid.log (10 includers, all IDENTICAL). pin_asserts.py pinned nothing in any of the four.

## Corrections to earlier records (no queue claim was rejected; REJECTED.tsv has the header only)
- NF83: 0x00860130's name is not Ego_d-only (FableWin.pdb 0x02833100).
- NF64: 0x005BEAD5 / 0x005BF30D are complete wrappers, not mid-function fragments.

## Rule slips
- One check of a claimed outside binder (0x0057B783) and a 43-row probe scan of outside callers were compiled (read-only, nothing
  changed or handed back for them); the scan script initially misparsed its own output (re-parsed from the saved dumps, no regrade).
- Two claims (0x009498B0, 0x0096CFC0) were taken and released without changes.
- 0x005BEAD5 / 0x005BF30D and most NF64 rows end blocked after a probe only (no new form), for budget.
- The brief named agentNF84/LESSONS.md as reading; the coordinator corrected it to NF82's (read).
