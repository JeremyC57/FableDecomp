# NOTES (agent NF34 name-fix pass, tags land1115ga .. land1116p23, 2026-11-16)

## Result
- Queue: 645 entries. done 33, done-already 145, partial 16, rejected 1, deferred 0, carry 450 (APPLIED.tsv; 73 binders respelled).
- 83 changed rows in rows/ (no test changed). recheck_final.log: one vland --recheck-landed run over all 83 with the overlay:
  7 MATCH + 76 RELOCATION_MATCH, every test PASS. STATUS.tsv carries those grades; grades_used counts every q.py grade of the row
  (max 3: 0x00A23B50, 0x00929820; 2: 0x00535CD0, 0x007DCCB0, 0x0057B3E2, 0x007A09B0, 0x007C6760, 0x00929820, 0x00927C80; plus the one probe compile
  of every landed caller row in work/probe.tsv).
- RENAMES.tsv: 18 section-1 self renames, 71 section-2 caller respellings.
- Header overlay: 3 opt-ins (hdr/README.md).

## Method
- callers.py over every BINDERS_WRONG / SELF_WRONG address, then work/probe.py compiled each landed caller row once (private work dir
  pS/, never $S/wine/q) and recorded the symbol each relocation binds at the retail target (work/probe.tsv, 1,000+ row/target pairs for
  the two-PDB (EF) BINDERS_WRONG targets and the self-renamed addresses). work/analyze.py joins that with the claims; this, not
  reloc_bindings, is the binder list each fix used. work/selfprobe.py compiled every two-PDB SELF_WRONG row to read its emitted symbol.
- Every claim acted on is in BOTH Ego_r.pdb and FableWin.pdb name sets (checked per entry), plus body/layout evidence where the name moved
  between addresses (IsSellable +0x5C, AddToLoadingQueue +0x40, GetNoAllocatedAreas = GetTotalMemoryAllocated + 0x100 in Ego_r and retail,
  RemoveBoastsForScript / RemoveAcceptedBoastsForScript PDB order).
- ALREADY_OK (116): 5 spot-checked (lines 30, 167, 300, 409, 585): self Leaf and reloc_bindings equal the claim; all marked done-already.
  NO_BINDER (10): done-already, nothing to do. SELF_WRONG rows whose compiled symbol already equals the claim (19, probe): done-already.

## Self renames (section 1)
0x00535CD0 ContainsPoint long->bool (moved onto engine/C2DBoxF.h + C2DVector.h, MATCH); 0x00989700 UpdateAmbient->UpdateCameraPos
(private); placeholders -> PDB members on shared headers: 0x00A23B50 SetTextureBank, 0x00A23B60 SetDP3BumpEnable, 0x009886F0
SetSoftwareVertexShaderEnable, 0x00988700 SetSoftwareFixedFunctionEnable (all MATCH); access/signature: 0x005E3EA0, 0x007C3EB0,
0x00813B80 (private), 0x00A892E0 (private, unsigned long), 0x009BAA50 (CDataOutputStream&), 0x0074DD20 / 0x00842C70 (ctor owner by
reference); names: 0x007021F0 GetTeleporterRegionName (was made-up GetMarkerRegionName), 0x009F2550 GetNoAllocatedAreas, 0x00E53670
IsHeroWearingAnyBeard; plus 0x0075C420 TransformPosition (protected) and 0x006CB840 GetNearestFleePosition (const, C3DVector&),
the self rows of two BINDERS_WRONG targets. Every retail caller of these addresses that sits in a landed row was probed: all bind the new leaf (0x009E1280,
the one that did not, is respelled). File names still carry the old names (coordinator renames files/catalog).

## Gaps (partial entries; reasons also in APPLIED.tsv)
- 0x00404310 GetCRC: 0x0065C781 models it as `const char* GetString()` feeding LookupValue(const char*) 0x009CC410; respelling needs that
  callee too.
- 0x005EDE93 GetAbilityLevel: 0x0057770A / 0x005778CA bind a CTCHeroAbilities stand-in class.
- 0x008FCFB0 PeekDefinitionManager: 4 rows use a GetPDefinitionManager (pointer result) stand-in.
- 0x00B8D090 / 0x00B8D0A0: 0x00B906C0 uses stand-in class CEngineRenderer_00b906c0.
- 0x009FC3D0: 0x00A25BD0 (CResourceBase stand-in) and 0x00A42980 (ResourceBankNative stand-in).
- 0x00625930 IsActive (4-byte getter, ICF-folded): 0x00583BD5 / 0x0068FF50 call other classes' IsActive; not respelled.
- 0x0075C280: its binders 0x0075CCB0 / 0x0075CCF0 are CEnvironmentSpellEffectInterface stand-ins; caller identity unproven.
- 0x005CCC91, 0x006B8310, 0x00701750, 0x008F8F50, 0x00E1FEB0, 0x005BFB96: need re-models or cascade self renames (APPLIED notes).
- 0x00802390 GetMaxTargetingDistance: the engine/CTCTargetingPlayer.h opt-in chain hung one compile (killed by the 240 s timeout); left.
- Line 91 (AddResource at 0x009FC3D0) rejected: superseded by line 461 (AddToLoadingQueue; landed self row already MATCH).
- Not reached (carry, 450): the remaining BINDERS_WRONG / SELF_WRONG entries without two PDBs, the TransferVector / Rb_tree / STLport
  template families (20+ stand-in binders each), the NEngineInternal GP* globals, NOT_MANGLED decisions, the Coda/libpk entries.

## Rules
- Local models: no NEW local model of a class with a shared header. Pre-existing local models were kept and only their declarations
  respelled (NF32 precedent). Three local stand-ins were removed in favour of shared headers (CCardinalSpline in 0x007DCE40; empty
  `enum EHeroAbility { }` in 0x007A09B0 / 0x007C6760; the 0x00535CD0 C2DBoxF / C2DVector models).
- `class CThingCreatureBase;` / `class CThingAICreature;` forward declarations (no layout) were added where a PDB parameter type had none.
- Made-up names: none. No `__asm` / `naked` / `_emit` / `register` keyword / bare `__forceinline` added (scan: only pre-existing comment
  words). No git, nothing written under the repo.
- Locks: 82 nflocks owned by NF34 (0x00802390 claimed but not changed: no row handed back). No row was held by NF33.
- Rule slip: one grade (0x00802390) hung until the 240 s timeout; its Bash call ran into the 120 s tool limit and was backgrounded, then
  exited 0.

## Verification added at the end (advisor review)
- Self rows of every BINDERS_WRONG target acted on were compiled (work/selfprobe2.tsv). Wrong self rows found and fixed: 0x0075C420,
  0x006CB840. Wrong self rows left (entries downgraded to partial): 0x005D9B30 (_FableIsBuyable_5d9b30 placeholder), 0x00B8D090 /
  0x00B8D0A0 (placeholders), 0x009FC3D0 (CResourceBank_Retail stand-in), 0x005EDE93 (QAEHH). 0x009AD390's landed row is the ICF twin
  COptimisedPrimitive::ActualToUniqueIndex; the CDefinitionManager callers keep GetGlobalIndexFromClassIndex.
- Old names grepped in rebuild/src/compiled and reloc_bindings.tsv: only the landed self rows themselves and an unrelated
  CV_BordelloScript::IsHeroWearingBeard (0x00E44A40) carry them. Neither PDB has IsHeroWearingBeard on CV_BeardyBaldyScript.
- Unverified: retail calls 0x00989700 from 0x00BE6F70, 0x00BEACA0, 0x00BF4570, but the probe found no relocation at the target in the
  function it graded (their leaf is a bare `Render`/`RenderForeground`); those three rows were not checked further.
- 0x00929820 passes 0x12 as `(EOpinionAttitudeType)0x12`: 18 is the enum's LAST value, so no source enumerator is claimed.
- The tests of self-renamed rows are self-contained models and still use the old names (e.g. 0x00535CD0 models `long ContainsPoint`);
  they pass unchanged and were not edited.
- Additional rule slips: one `sleep 60` Bash call was blocked by the harness (no command ran); the first IsOpeningHours grades (7 rows,
  before the private pS/ dir existed) ran in the shared $S/wine/q/<addr> dirs of rows NF34 had claimed.
