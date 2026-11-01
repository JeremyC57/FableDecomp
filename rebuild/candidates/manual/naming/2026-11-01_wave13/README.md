# Naming pass wave 13 (2026-11-01): 314 landed rows respelled to the 2026-11-01 owner decisions

Inputs: brief `rebuild/candidates/manual/naming/briefs/BRIEF_W13.md`, `2026-10-31_wave12/DECISIONS_W12_RESOLVED.tsv`,
`naming/ESTABLISHED_SPELLINGS.tsv`, QUEUE_W13.tsv (153 rows: NAME_FIXES_QUEUE c643-c650 + codex50 plus one row per decided item).
One Opus 5.5 agent authored the overlay in `$S/naming/w13` (scripts/ here: compose13.py recomposes every stage on fresh copies of
the current repo; fam_glob.py / fam_gptr.py are the new global-rename family tools; hand13/ the per-family hand stages); the
coordinator copied ov_final, applied RENAMES (git mv + build_candidates.ps1 Module/Source/TestSource), BINDINGS and LABELS, and
rechecked every changed row against HEAD: **314/314 RELOCATION_MATCH + PASS** (`logs/coordinator_recheck.log`; agent's own run
`logs/recheck_final.log`, also 314/314). `bindings_multi_check.py` 0. Name-only: no byte changed anywhere.

Families, committed in this order (row lists `<fam>.rows.txt`; pmgr and eps share rows, the union is 314):
1. ggame (4): GGame 0x013B83D0, GSystemManager 0x013CA618; 0x00412F60 is `CGame::Uninitialise` (was TailJmp_Uninitialise),
   0x01223FF0 is `??__EGSystemManager@@YAXXZ` (renamed file), 0x009A7440 is `??0CSystemManager@@QAE@XZ`.
2. twopi (4): `extern const double TWO_PI;` (0x00A0C450 local g_twoPi renamed; 0x00853830 NCameraModes::Scale moved to global scope).
3. stools (6): `CTCBase::PeekThingSearchTools() const` 0x00686D90; the templated callees made const to match.
4. cds (5): 0x00415DD0 is `?GetString@CDefString@@QBE?AVCCharString@@XZ` (renamed file); `??BCDefString@@QBE...` retargeted to 0x00415D70.
5. pmgr (138): PSoundManager 0x013B8394 (`class CASoundManager; extern CASoundManager* PSoundManager;`), PDisplayManager 0x013B8390,
   PInputManager 0x013B8388, PGraphicBankManager 0x013B837C, PFontManager 0x013B838C. 0x00B7AD20's g_pDisplayManager relocates to
   0x01436E1C, a different engine global: left alone (DEFERRED).
6. eps (140): `extern const float EPSILON;` 0x0129BA3C (the `@@3MA` rows now spell `@@3MB`).
7. sfv (1): 0x00AF9000 renamed to `NParticleEngine::GFSmoothFadeValue` (`?GFSmoothFadeValue@NParticleEngine@@YIMM@Z`), file moved.
8. ita (1): 0x0069CE20 `IS_THING_ALIVE<CIntelligentPointer<const CThing> >`; the CThingPhysical instantiation 0x006961F0 deferred
   (member type unproven).
9. pkc (5): PKCDeferredBank (ctor 0x00C14E40, DeleteData, ApplyTo, SetOptionCC, SetOptionCD; vftable 0x012B29F8), files renamed.
10. anon (2): GetDataFromEventString 0x00870C90 bound as `?GetDataFromEventString@?A0xaf422eae@@YI_NABVCCharString@@JAAV2@@Z`;
    39 anonymous-namespace bindings that match a PDB name modulo the hash normalised to the PDB spelling (the CThingCompare
    comparators in two TUs: 0x00978790 / 0x00979370 `?A0xffab493a`, 0x0097ABF0 / 0x0097B2C0 `?A0xeaff0e08`); 175 bindings differ
    beyond the hash and stay (DEFERRED).
11. queue (14): const/access-only PDB respells (IsPointInside QBE, IsPermanentlyStocked QBE, PlayAnimation AAE, InitialiseTextures
    AAE, GetDefName QBE).

Bindings (BINDINGS.tsv): 156 RETIRE (157 table rows), 52 ADD, 5 KEEP (still named by an unedited row: `??BCDefString@@QAE` by
0x00CBB4D0, the two GetThingSearchTools QBEPAV spellings by 0x007E92D0 / 0x00799900, `CInitBaseClass::Uninitialise` at 0x0099A360,
`??_7CTCDiceGame@@6B@` at 0x0124093C). Labels: LABELS.tsv (22; also `ghidra_out/labels_naming_wave13_2026-11-01.tsv`), catalog
names corrected for the 11 function rows. Renames: RENAMES.tsv (8 rows, src + test each).
Decisions scoped, not applied (DECISIONS_W13.tsv, owner): GetPWorld 0x008FCFC0 (44 spellings / 72 rows; PDB is the protected
reference getter `?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ`, so the respell changes `->` to `.` at every use), VIRTUAL_SCREEN_*
0x013961E8 / 0x013961F0 (3NA vs the GFAXToVX volatile model), vector<CCharString> dtor 0x00414EA0 (16 spellings / 54 rows; header-based
family or table-only unification), GetCurrentWorldFrame 0x004167C8 (one source; CMainGameComponent getter + PMainGameComponent).
Deferred: DEFERRED.tsv (18 items with reasons; the CDefString implicit-conversion rows change bytes, LowerBound needs per-row CVectorMap
models, the queue rows IsPerformingAnyAction / GetWorld IAE / Initialise UAE need per-row modelling, CBaseIntelligentPointer and the
class-static globals are mega families). Wave 12 DEFERRED carried as DEFERRED_W12_INPUT.tsv in the agent workspace (not applied).
