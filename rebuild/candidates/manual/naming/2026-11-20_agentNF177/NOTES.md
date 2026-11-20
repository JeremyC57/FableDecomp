# NF177 notes (w22, 2026-11-20)

28 entries, all terminal: done 13, done-already 5, rejected 1, blocked 8, deferred 1, open 0 (counted from APPLIED.tsv).
STATUS.tsv: 33 rows ported (rows/), 11 not-a-binder, 6 blocked, 1 rejected (0x00B4B830, unchanged).
34 row files in rows/ (33 rows + the 0x00BDC180 test), every one RELOCATION_MATCH or MATCH and PASS under vland --recheck-landed
(recheck_final.log). Header opt-ins and includer checks: hdr/README.md. Renames: RENAMES.tsv (s1 = self leaves / catalog names,
s2 = caller respellings).

## Decisions for the coordinator
- **q5054 (0x00C4D240)**: blocked, needs your decision. No row was changed. Per your call, a split into recorded descriptive names is not
  acceptable. The GameCODA 2.10 CODAApiDbg.lib names fit the body:
  - ?AllocateVoice@CSalContext@@AAEHPBVCCodaSource@@AAVCCodaObjectList@@AAH2W4SAL_VOICETYPE@@PAPAVCSalVoice@@@Z takes six arguments, matching
    `ret 18h`.
  - The pool calls are CCodaObjectList::GetFree / Add / RemoveFree.

  These are better than the descriptive CreatePooledEvent / UpdateSlot, but a CODA name still needs a second source before it lands (APPLIED
  q5054 lists the evidence). Proposal: NAME_FIXES.tsv 0x00C4D240.
- **q11918 (0x00B4B830)**: rejected. The claimed name belongs to the next function.
  - 0x00B4B830 is the private ?UninitialiseTextures@CEngineShadowRenderer@@AAEXXZ, and the virtual OnReleaseDefaultPoolResources is 0x00B4B880.
  - Evidence: vtable 0x012A2374 slot 11 = 0x00B4B880, which is a 5-byte `jmp 0x00B4B830`. In FableWin.pdb, UninitialiseTextures is 105 bytes
    and OnReleaseDefaultPoolResources is a 19-byte wrapper; Ego_r.pdb has the same order.
  - The landed shared-header opt-in FABLE_RETAIL_CENGINESHADOWRENDERER_ONRELEASEDEFAULTPOOLRESOURCES (NF152) pins the virtual to 0x00B4B830.
    Its two users, 0x00B4D210 and 0x00B4FC10, therefore bind the wrong name.
  - The fix is a 3-row move plus a private opt-in. Proposal: NAME_FIXES.tsv 0x00B4B830 / 0x00B4B880.
- **0x005DA7F0 GetWeaponClass**: already bound under three spellings by landed rows (0x005DAB10, 0x008D9530, 0x008E7040).
  - The q11929 self row keeps its old stand-in callee through a `static_cast<const CTCWeapon_Getters*>(this)` downcast.
  - This casts a real type to a stand-in, the reverse of the case the brief names. I am reporting it so you can respell the whole family
    (NAME_FIXES.tsv 0x005DA7F0).
- **0x013CAA70 lock**: the q12001 self row now binds g_allocatorLock, the spelling of 0x009F2000 and 0x009F2310 (it was g_perfStatsObject).
  0x009F26B0 is now the only row on g_perfStatsObject (NAME_FIXES.tsv).

## Entry outcomes (APPLIED.tsv has the full text)
- **done**:
  - q4391.
  - q5090: 0x00894E60.
  - q6927: 0x009D56C0 and 0x009D5820, a port onto the shared headers as you required. No in-place respell.
  - q11775: 5 rows.
  - q11929: self + 5 binders. New opt-in CTCWEAPON_ISUSABLEBY.
  - q11968: self row moved onto NParticleEngine::CParticleEmitter, MATCH.
  - q11978: self + 1 binder.
  - q12001 / q12097: one row, 0x009F26D0 CMemoryAllocator::NewFrame.
  - q12085: self row onto the RadialBlur header's retail branch.
  - q12088: D1 family, 6 rows (details below).
  - q12095: self + 3 binders, pointer form to reference form.
  - q12096: catalog-only; NAME_FIXES.tsv.
- **done-already**: q11794, q12084, q12086, q12087, q12089.
- **blocked**:
  - q4392: family. 0x009A7D40 left, waiting on the 0x009AAEB0 insert_unique family.
  - q5054: needs your decision (above).
  - q11666 / q11969 / q11987: port, family. Rows 0x006E11F0 + 0x006E13E0 + 0x006E2640, plus a private nested
    CTCDParticleEmitter::CParticleCreationParameters opt-in and the default-axes datum (no PDB name).
    - q11969 cast note: the 0x006E11F0 self row passes its X_006e11f0 C3DVector / CRightHandedSet stand-ins through casts to the global types
      and casts the emitter result (`// Fidelity: cast, not retail`). That goes away only with the family port. I added no casts.
  - q11917: port. 0x00418DCA, the CMainGameComponent ctor on a dozen local models.
  - q11988 / q11989: need a vtable TU. ??_G is emitted only by a TU that emits the class vtable. These rows have vtable references only and
    no landed referencers.
- **deferred**: q5089, held by NF176 (0x006A23E0). Its other referencer, 0x008A99F0, needs a full port.
- **rejected**: q11918 (above).

### q12088 details
- 0x00BE0930 takes the D1 member-template const find (the NF172 draft, MATCH).
- 0x00BE04C0 gets FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE.
- 0x00BE0480 IsBlockLoaded moves off the CRetailBlockSet / SBlockNode stand-ins onto engine/CEngineStaticMapFileBlockManager.h (new opt-in
  ISBLOCKLOADED).
  - Its own leaf goes from QAE to the PDB QBE.
  - Two referencers already bound QBE. The other three (0x00BDC180 via a candidates-header opt-in, 0x00BF8030, 0x00BF84C0) were respelled, so
    0x00BE0480 now has one name.
  - 0x00BDC180's test needed the macro and a const stub: rows/00bdc180_test.cpp.

## Local models (narrow rule) and pre-existing local models
- **Narrow rule**: 0x00644644 / 0x0064464C / 0x00645821 (q11775) model NPlayerGui::CDrawHealth / CDrawStamina / CDrawMeleeActionButtonEffects
  locally, in the landed 0x00645605 sibling's shape, on the shared NPlayerGui_CDrawBase.h. None of those classes has a shared header.
- **0x00BDBFF0 / 0x00B3EDC0 CEngineLandscapeMap**: on engine/_quarantine/CEngineLandscapeMap.h with NF160's opt-in, the include the caller
  0x00B40230 uses.
  - My first form used a one-member local class, as the landed sibling 0x00BDBFE0 does. selfcheck flagged it NEWLOCAL (the header lives under
    _quarantine/), so I replaced it.
  - The sibling 0x00BDBFE0 still has that local class. It was not touched.
- **In-place respells on pre-existing row-local models** (no new local model added):
  - 0x005DABE0, 0x0073A9F0, 0x008E9950: local CTCWeapon models. engine/CTCWeapon.h exists.
  - 0x00701940 + 0x00681940 / 0x00681980 / 0x006819D0: local COpinionReactionManager. engine/COpinionReactionManager.h exists.
  - 0x00B3EDC0: local CEngineMap. vland warns SHARED_HEADER_AVAILABLE.
  - 0x00BF8030 / 0x00BF84C0: local CEngineStaticMapFileBlockManager.
  - 0x00BE0930: local STLport tree model, the same as its landed twin 0x00BE08E0.

## Self renames that move file names (RENAMES s1)
- 0x00644644, 0x0064464C, 0x00645821: placeholder forwarders become NPlayerGui::CDraw*::ClearEffects.
- 0x0064528A: CDrawAbilityButtons::ClearEffects.
- 0x00AED090: CParticleEmitter becomes NParticleEngine::CParticleEmitter (file name as the sibling 0x00AEDE60).
- 0x00BDBFF0: E2_E2_00bdbff0.cpp becomes CEngineLandscapeMap_OnGameMapUnloaded_00bdbff0.cpp.
- 0x009F26D0: CMemoryAllocator_ClearPerformanceStats_009f26d0.cpp becomes CMemoryAllocator_NewFrame_009f26d0.cpp.
- Leaf only, file name unchanged: 0x009A7CA0 / 0x009A7E20 / 0x009A7ED0, 0x005DAB10, 0x00B5BD70, 0x00BE0930, 0x00BE0480, 0x00701940.

## Bonus
- 0x0064BE45 (q11775): HEAD defined CVectorMap::LowerBound const (0x004365B0) as a strong COMDAT (`comdat 1`). The port emits only the header's
  pick-any copy (`comdat 2`), which removes a potential duplicate-strong-definition hazard.
- 0x0064BE45's own leaf keeps the global-namespace class. The PDB has NPlayerGui::CDrawMeleeActionButtonEffects::Update: NAME_FIXES.tsv.

## Descriptive (made-up) names
None introduced. Every new spelling here is from Ego_r.pdb + FableWin.pdb, or is an existing landed spelling (g_allocatorLock).

## Rule slips
1. **Wrong lock**: I claimed lock 00438230 by mistake and set it back to `released` right away. I never edited that row.
2. **Preamble**: some read-only Bash calls skipped the `export S=...; source env.sh` preamble.
3. **Exit code**: one Bash call exited 1 (cat of lock files that did not exist).
4. **git**: one read-only check accidentally contained `git -C /home/user/FableDecomp status` (late in the run, during the overlay-conflict
   check). The permission layer denied it and it did not run. No other git command was issued.
