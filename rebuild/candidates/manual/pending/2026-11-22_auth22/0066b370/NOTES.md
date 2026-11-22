# 0x0066B370 CTCSoundPlayer::PlayAllSoundEvents -- RELOCATION_MATCH (AUTH22, 2026-11-22)
**Status: matched, pending a ruling on Ego_d line 208 and on one pick-any copy (see "Closing checks" at the end).**
Source `0066b370.cpp`, test `0066b370_test.cpp` (vt.py PASS), stock flags (no `// Flags:` line, no inline device), overlay `hdr/` (patch
`hdr.diff`). Strict dry-run lander (LANDVERIFY_STRICT_HEADERS=1 LANDVERIFY_STRICT_RELOCS=1, FABLE_HDR_OVERLAY=hdr): RELOCATION_MATCH PASS, WINS 1
(`land/vland.log`). Grades used: 17 (`LOG.md`).

## Route (recorded draft DIFFER(1805v1808), 278 unmasked lines, `this` in edi)
1. Re-ported the chunk-853 opt-in onto today's CTCSoundPlayer.h (the draft was SRC_FAIL): reproduces 1805v1808.
2. **Ego_d locals and blocks** (eglocals / eglines): `const CAnimSet* panim_set = *it`, `const vector<short>& anim_indeces`, `index_to_use`
   before `best_strength`, `c1`, `strength`, `event_container`, `event_it`, `psymbol_info`, `loop_event_name`, `sound_name`, `voice_name`,
   `extra_tag`, `played_sound`, the four named concatenations, nested `if (GetTC)` instead of an early return. The draft's "no CAnimSet*
   local, hoisted anims / count" (fitted to retail's reload of `*it`) was wrong: with Ego_d's locals VC7.1 gives retail's loop exactly
   (it rematerialises `*it`), including the unsigned `jae` of `c1 < anim_indeces.size()`.
3. **Visible callee bodies** (existing opt-ins FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY):
   GetDef<CMaterialDef> 0x0066C340 is a same-TU COMDAT and the other two are templates, so retail's c2 saw their bodies. With them the three
   def handles stay in registers and release as `dec [reg+4] ; jne` (the draft's load / dec / store releases and the missing
   `mov edi,[psound_def]` before the branch were this). 1836 -> 1798, masked residue 68 -> 19 lines.
4. `GetParentThing().X` (FableWin.pdb inline accessor, as the landed same-TU 0x0066A9E0; Ego_d's lines are 4 B larger than `ParentThing->X`
   at every use) and the implicit `material_name = pmaterial_def->GetInstantiationName();` (retail calls the conversion 0x00415DD0;
   Ego_d line 171 = 88 B): the argument order and `this` in esi follow. 1806v1808.
5. Stop-event arm: `PAttachedEmitter->StopAllCriteriaContainingString(loop_event_name)` through an inline forwarder (new opt-in) instead of
   a named handler pointer: the emitter goes straight to ecx as in retail. RELOCATION_MATCH.
6. Port onto shared headers and PDB names (still RELOCATION_MATCH): CThing virtuals GetThingStandingOn / GetFloorMaterial (slots 38 / 40,
   existing ORIGINAL_VTABLE; Ego_d local `pstanding_on_thing`), CEntitySoundDef.h, CMaterialDef.h, CWorldMap.h (_STRONGESTTHEME),
   CTCAnimationComplex.h, CAnimateThing.h (GetMainAnim), CAnimSet.h + new accessor opt-in, new headers for CSmallVector / CEvent / CManager,
   CEvent's bitfields `Index` / `StopEvent` instead of masks.

## Names and evidence
| name | evidence |
|---|---|
| CTCSoundPlayer::PlayAllSoundEvents | Ego_r.pdb / FableWin.pdb `?PlayAllSoundEvents@CTCSoundPlayer@@QAEXXZ` (Ego_r 0x005749A0, FableWin 0x01E7E870); sole caller FrameUpdate 0x0066BBF0 |
| every local | Ego_d.pdb S_BPREL32 names and types of this function (list in step 2) |
| CAnimSet::GetAnimIndices / GetAnimBlendStrength / GetPercentageComplete / GetLastPercentageComplete | FableWin.pdb name set (`QBEABV?$vector@F..`, `QBEMJ@Z`, `QBEMXZ` x2); Ego_d line sizes 189 (11 B), 195 (15 B), 203+205 (49 B) = our VC10 /Od sizes with these calls |
| CAnimateThing::GetMainAnim | FableWin.pdb `?GetMainAnim@CAnimateThing@@QAEAAVCAnimSet@@XZ`; Ego_d local `CAnimSet& main_anim`, line 181 = 11 B (existing opt-in) |
| NAnimationEvents::CEvent Index / ExternallyImported / StopEvent / Time | Ego_d.pdb TPI field list (30 / 1 / 1 bit unsigned long + float); landed 0x006F9820 uses the same |
| CSmallVector<T,N> (global), Start / Size, begin / end | Ego_d.pdb TPI; FableWin.pdb `?begin@?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@QAE..` (no namespace: the draft's NAnimationEvents::CSmallVector mangled wrong) |
| NAnimationEvents::CManager GetEventSymbolMap / GetAnimEventsBetweenTimes, members MeshDataBank / EventSymbolMap | Ego_d.pdb TPI + FableWin.pdb / Ego_r.pdb name sets |
| NAnimationEvents::GetSoundEventManager | `?GetSoundEventManager@NAnimationEvents@@YIAAVCManager@1@XZ` (both name sets) |
| CThing::GetThingStandingOn / GetFloorMaterial | shared CThing.h ORIGINAL_VTABLE (slots +0x98 / +0xA0); Ego_d local `pstanding_on_thing` (CThing*), `material_index` |
| CEngineThemeDef::MaterialDef +0x8C | Ego_r layout member name; local model = landed 0x007F0300's text (no shared header) |
| g_bSoundEnabled 0x0137E974, g_InvalidSoundHandle 0x0129D25C | reloc_bindings spellings (`?g_bSoundEnabled@@3_NA`, `?g_InvalidSoundHandle@@3KB`) |
| **DESCRIPTIVE** g_bPlayAnimationSoundEvents 0x01375758 | no PDB name; a .data bool read only here (probably a console variable) |
| **DESCRIPTIVE** CThingSoundEmitter::StopAllCriteriaContainingString | TLC-only inline forwarder to CASoundEmitter slot 12 (Anniversary name of the slot); no PDB member; Ego_d has no code for the arm |

## Where the text departs from Ego_d (both forced by retail bytes; Ego_d is a later VC10-era revision here)
1. Stop-event arm: Ego_d lines 216..227 carry no code (the arm is compiled out there); retail has it (0x0066B750..0x0066B77D).
2. `GetAnimEventsBetweenTimes(index_to_use, event_container, panim_set->LastPercentageComplete, panim_set->CurrentPercentageComplete)`:
   Ego_d line 208 (54 B) calls the two by-value accessors. Under VC7.1 an inlined by-value float accessor used as an argument always
   leaves a stored temporary (`mini/m1.cpp`: `mov [$T334],eax` after the push; variants b / e / f / g / h the same, only a `const float&`
   return or a member read is clean); retail 0x0066B6E0..0x0066B6EB has none and the frame is 0x74, not 0x7C. Hence the member reads and
   the `friend class CTCSoundPlayer;` line in the CAnimSet opt-in (unproven: the TLC header may have had public members). The `if` above
   it keeps Ego_d's accessors (same bytes either way).

## Header changes (overlay `hdr/`, patch `hdr.diff`; all add-only)
- engine/CTCSoundPlayer.h: FABLE_RETAIL_CTCSOUNDPLAYER_PLAYALLSOUNDEVENTS (two declarations). pin_asserts: 0 moved.
- engine/CThingSoundEmitter.h: FABLE_RETAIL_CTHINGSOUNDEMITTER_STOPALLCRITERIACONTAININGSTRING (nested in _SOUNDEMITTER). pin_asserts: 1 pinned
  (`FABLE_STATIC_ASSERT_AT(74, ...)`).
- engine/CAnimSet.h: FABLE_RETAIL_CANIMSET_ANIMINDICES (four accessors + friend). pin_asserts: 0 moved.
- NEW engine/CSmallVector.h, engine/NAnimationEvents_CEvent.h, engine/NAnimationEvents_CManager.h.
Proof run here: the three macros have 0 hits in rebuild/src, rebuild/include, rebuild/tests; `g++ -E -P` of each changed header against
its `orig/` copy is byte-identical for the default path and for a retail macro set (md5 pairs equal; assert names included). The full
equiv_check / recheck_rows pass is the coordinator's at landing (no landed text changes, so it should list nothing).
Note for the lander: `class CManager` in a row trips SHARED_HEADER_AVAILABLE (engine/CManager.h is the NUISystem one); the row now
includes engine/NAnimationEvents_CManager.h and defines no class of that name.

## Call targets (cc.py on the graded object: 71 calls v 71, every bound symbol on retail's target)
New bindings (unbound before): 0x006F90C0 `?GetSoundEventManager@NAnimationEvents@@YIAAVCManager@1@XZ`, 0x006F99B0
`?GetAnimEventsBetweenTimes@CManager@NAnimationEvents@@QBE_NKAAV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@MM@Z`, 0x01375758
`?g_bPlayAnimationSoundEvents@@3_NA` (descriptive). `delete [] Start` binds operator delete[] 0x00BFEB1C (the header declares it).

## Closing checks (after the advisor review)
### Pick-any COMDAT copies in the row's object (gobj.py on `final.obj` = the object of the final source)
| copy | retail owner | result |
|---|---|---|
| `??$GetDef@VCMaterialDef@@@CDefinitionManager@@QBE_NJPAV..@Z` | 0x0066C340 (same TU, landed) | RELOCATION_MATCH |
| `??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NJPAV..@Z` | 0x0063F3A0 (landed) | RELOCATION_MATCH |
| `?push_back@?$vector@PAVCAnimSet@@..@std@@QAEXABQAVCAnimSet@@@Z` | 0x0066C4D0 (same TU, landed) | RELOCATION_MATCH |
| `??BCDefString@@QBE?AVCCharString@@XZ` | 0x00415DD0 | RELOCATION_MATCH |
| `??$GetSubDef@VCEntitySoundDef@@@CDefClassBase@@QBE_NPAV..@Z` | 0x0065F604 (landed; "frame-pointer size TU") | **DIFFER(182v162): LAND BLOCKER to rule on** |
The two GetDef copies needed one more existing opt-in, FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD (without it they were 116v108; the
row itself is RELOCATION_MATCH either way; grade 17). The GetSubDef copy cannot equal its owner from this TU: the owner at 0x0065F604
(not 16-aligned) was compiled in a size-optimised frame-pointer TU, this row is /O2; same header text, different TU flags. Retail's
linker kept the other TU's copy. The body must stay visible here (it is what keeps psound_def's Object in edi across the branch,
0x0066B42E). For the coordinator / advisor: the nf201 rule as written makes this a blocker; the retail-faithful reading is that the
/O2 copy is the losing pick-any instance.

### Ego_d rule: which clause the two departures meet (standing criterion, ADVISOR_DECISIONS_B 2026-11-21)
- Stop-event arm (retail 0x0066B750..0x0066B77D): meets clause (1) in the "reverse" direction as far as it can be tested by size: retail's
  block calls CIntelligentPointerBase::GetPItem 0x00A01B50 twice and the slot-12 virtual; Ego_d's lines 216..227 hold zero bytes. The
  forwarder text there is Inferred.
- Event query arguments (Ego_d line 208): **meets neither clause as written.** The Ego_d line decodes exactly (54 B = our VC10 /Od 51 B
  + 3 for the far `event_container`) as calls to the two accessors, but they are inline in release, so "retail never calls them" proves
  nothing. The evidence is of another kind: a VC7.1 codegen fact (an inlined by-value float accessor used as an argument always keeps
  a stored temporary: mini/m1.cpp and five spellings; on the real row r12 gives frame 0x7C and two dead stores, DIFFER(1844v1808)),
  against retail's frame 0x74 and plain `mov ecx,[set+1Ch] ; mov edx,[set+20h] ; push`. So the row is **matched, pending a ruling on
  Ego_d line 208**: either (a) accept the member reads + `friend class CTCSoundPlayer;` as Inferred under this mechanical test (the
  current text), or (b) the row is terminal (A) like 0x0093D270 with r12 (Ego_d-exact, 1844v1808) as the bound text and the final
  source as the oracle. The alternative spelling that also matches the mini TU, a `const float&` return, contradicts FableWin's
  `?GetLastPercentageComplete@CAnimSet@@QBEMXZ` and was not used.
