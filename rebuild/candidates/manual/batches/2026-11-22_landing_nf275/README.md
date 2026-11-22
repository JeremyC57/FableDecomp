# Landing nf275 (Account A, 2026-11-22): q12659, the CMainGameComponent ctor onto the shared headers with its dtor family, and the owner-side CVertexGroupBlock dtor (3 rows, 7 headers)

Agent NF275 (both entries done). Hand-back: `pending/2026-11-22_nf275_handback/`.
- **q12659, 0x00418DCA** (CMainGameComponent ctor): two sources each: vtables 0x0122F180 / 0x0122F174 are the PDB pair
  `??_7CMainGameComponent@@6BCBaseClassNonCopyable@@@` / `6BCDeviceResetCallback@@@`; 0x009C0190 is
  `?AddDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z`; retail slots 1 / 4 are CMainGameComponent's own Init / SetQuit
  overrides (missing from the header). The row drops every local model for `engine/CMainGameComponent.h` / `CGameComponent.h` /
  `CDisplayManager.h`, reusing nine member opt-ins and adding seven declaration-only ones (`CMAINGAMECOMPONENT_CTOR`, `_INIT_SETQUIT`,
  `CNETWORKCLIENT_CTOR`, `CGAMEEVENTPACKAGESET_CTOR`, `CROUGHFRAMECOUNTER_CTOR`, `CDISKFILEWIN32_DEFAULTCTOR_OUTOFLINE`, `_DTOR_DECL_ONLY`) and a new
  header `engine/CFrameRateSmoother.h` (size 0x30, layout proven three ways). Final form RELOCATION_MATCH; only three leaf relocations change
  (0x009C0190 and the two vtables); no new copy differs from an owner (the one HEAD mismatch unchanged); all 12 vtable slots point at their retail
  target's PDB member (HEAD's tables had one slot each).
- **0x00418B7A** (its dtor, a family member): `protected:` around its local model's slot 5-8 declarations, so it binds the PDB `MAE` spellings and
  both rows' vtable copies are slot-for-slot identical (without it the port adds a third spelling at 0x00416E78, and the two rows would emit one
  vtable name with different slot symbols).
- **Census:** no address gains a spelling; 0x009C0190 2 -> 1, each vtable 2 -> 1, 0x00415E83 3 -> 2, three slot targets 2 -> 1, 0x00416E78 stays 2.
  Link 0 duplicates; the AddComponent and `v00` stand-ins gone. Whole-tree equiv_check 57,125 rows, 0 differ.
- **Owner-side CVertexGroupBlock dtor, 0x00A96DA0:** new opt-in `FABLE_RETAIL_CVERTEXGROUPBLOCK_DTOR_INLINE` in `engine/CVertexGroupBlock.h` plus
  an nf86 / nf88 anonymous-namespace helper in the row (`// Fidelity: scaffolding`): leaf RELOCATION_MATCH, relocations HEAD's; the dtor COMDAT
  NODUPLICATES -> ANY; LNK4006 1 -> 0; its own whole-tree equiv_check 0 differ. Census (44 rows compiled): 0x00AD94F0 / 0x00AD9B20 define equal
  pick-any copies; 0x00ADC190 / 0x00ADC250 / 0x00ADC4E0 / 0x00ADCB90 define byte-equal copies whose `_M_erase` call binds a stand-in spelling the
  table lacks at 0x004507A1. A HEAD relink with the owner last already keeps 0x00AD94F0's copy over the owner's strong one, so those copies can
  win at HEAD too; pick-any adds no new risk.
- **Table:** three stand-in lines whose only witness was 0x00418DCA removed (`?AddComponent@CComponentList`, `??_7CMainGameComponent@@6BCBaseClass@@@`,
  `??_7CMainGameComponent@@6BCComponentListEntry@@@`); the PDB lines already exist with other witnesses.
- **Recheck** (`logs/recheck_a.log`, `logs/recheck_b.log`, landed tree): the 3 rows + the twelve slot-target rows, 0x009F8B10 and 0x009C0190 (15);
  every includer of CVertexGroupBlock.h / CNetworkClient.h / CGameEventPackageSet.h / CRoughFrameCounter.h and the six census rows above (32):
  47 PASS (40 RELOCATION_MATCH, 7 MATCH), 0 DIFFER, 0 RELOC_CONFLICT (0x004184BD is not a landed row). The 493 CMainGameComponent.h and 60 CDiskFileWin32.h includers are covered by the whole-tree equiv_check (0 differ).

## Coordinator rulings on NF275's questions
1. 0x00418DCA and 0x00418B7A land together with the seven header files. The 12 rows that model CFrameRateSmoother locally become re-port backlog.
2. The eight NAME_FIXES are queued (slot self rows 0x00416E78 / 0x004197A0 / 0x004197B0 / 0x00415E83 / 0x004197B3 on stand-ins; the 0x009F8B10
   catalog label, the CRoughFrameCounter ctor; the `_M_erase` spelling split at 0x004507A1).
3. A linking test for 0x00418DCA is queued as test debt (the HEAD standalone test is kept).
4. 0x00A96DA0 lands now; the four mismatched-copy rows (0x00ADC190 / 0x00ADC250 / 0x00ADC4E0 / 0x00ADCB90, the `_M_erase` split) are queued.
5. 0x00ADCE60 keeps nf273's declaration until it is ported onto the shared header (its local copy is 19 v 84 bytes).
Rule slips (the 44-row census script under an outer `timeout 3600` with `timeout 900` per grade; entry 2's equiv_check alongside its own
compiles): noted.
