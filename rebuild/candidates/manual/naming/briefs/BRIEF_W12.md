# Naming pass, wave 12 (one agent; two chunk-authoring agents share the lane)

Landed: operator=, waves 1-11, LowerBound. Records in `rebuild/candidates/manual/naming/`; wave 11 is `2026-10-30_wave11/`
(read its README.md, DEFERRED.tsv and DECISIONS_W11.tsv first; the rules in `briefs/BRIEF_W10.md` and `BRIEF_W11.md` are
unchanged and apply here). Since wave 11, chunks 594-642, the parked-row retry sweep (retry1, sec1-6b), the library-signature
passes (libsig/libsig2/libsig3), migration wave 9, the c621 respell pass (n621) and the CDefPointer/LZO passes queued new
NAME_FIXES. Owner decisions in force: wide text is `unsigned short` (`G`); names right the first time (exact PDB spelling, two
sources per name, one retail address = one mangled name); retail's calling convention wins over a PDB spelling when they
differ; established binding spellings for globals are reused, never a third spelling (0x0124F2B8 `?g_twoPi@@3NB`,
`g_pGameScriptInterface` 0x0143E8F8, `pGlobalScriptDef` 0x0143E90C, `g_pMainGameComponent` 0x013B86A0); descriptive class names
already used by landed siblings are the established names (CLibpkObject1D8, CCodaVoice, CDSoundStreamBuffer, CMixKernelsSSE,
CSoundEventManager, PKCTRRedblackMap); libpk/LZO/strmbase/CODA rows take upstream source names, descriptive only when nothing
names them.

Workspace: `$S/naming/w12/` (S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad).
Prepared input: `$S/naming/w12/QUEUE_W12.tsv` (1,072 rows; columns chunk, address, correct_symbol, evidence,
wrong_spellings_seen; 95 of them are REVIEW / OWNER DECISION rows -- scope those, do not respell them). The queue file itself is
`rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv`; rows whose column 5 says `APPLIED` were already respelled at landing.

## Scope, in this order (stop at about 600 edited rows)
1. **Single-callee multi-spelling families proven by a landed PDB row** (cheapest, highest value): addresses where the queue's
   correct_symbol is already the leaf or binding of a landed byte-exact row and the other callers carry stand-in spellings.
   Known families: 0x00A14F70 `?GetNodeAt@CNavigatorManager@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@E@Z` (8 callers, c633);
   0x007EE540 `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ` (8 spellings, c632/c633); 0x00662820
   `?IsAnimCombinationAnim@CThingCreatureBase@@QBE_NJ@Z` (callers 0x00692A20, 0x00694D10); 0x0066C3F0 / 0x0070D0A0-class STLport
   `_M_insert_overflow` IAE (callers 0x0070D100, 0x0070D0A0); 0x00CD3CCB-family CScriptThing / ScriptedThingBase ctor+dtor
   (0x006E7B40, 0x004AA840, 0x007E72A0, 0x007E74D0: callers 0x00DA6D50, 0x00EE8870, 0x00CD2E2D, 0x00E0AD40, 0x00DDD030, 0x00CD2B78,
   0x00D2D060, 0x00E546A0, 0x00D34440, 0x00D246C0); 0x00C19250 `??1CPKListImpl@@QAE@XZ` (17 spellings, c634); 0x008FD010
   `?IsStopping@CAIStateGroupBase@@IBE_NXZ` (12, c637); 0x00686D60 `?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ` (19,
   c637); 0x0099AF00 `??BCWideString@@QBEPBGXZ` (6); 0x00405F00 `?GFIntToWideString@@YI?AVCWideString@@J@Z` (3); 0x00487DD0
   `?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ` (19, c642); 0x009AD410 GetDefGlobalIndexFromName const (16);
   0x0044C6B0 CGameDefinitionManager::Get SI (8); 0x004C7E50 `?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ` (13);
   0x00A01B50 GetPItem returns CBaseObject* (c627/c638/c642 evidence); 0x00A14440 AccurateNormalise (settle the GOTCHAS
   "Normalise" line: the PDB has no out-of-line Normalise); 0x00F35B30 / 0x00CB7940 IsActiveThreadTerminating (CEntityScriptBase
   QBE / CScriptBase); 0x006A4E40 GetHealthPercentage; 0x0070C4E0 GetFirstCombinationAnimFromType; 0x0070C050 CAnimInit::Init.
2. **Chunk rows c600-c642 and 594-599** that are PDB-proven callee/leaf fixes on landed files (access QAE->IAE/AAE, const,
   return types, class nesting, enum parameter types, wrong class owner such as `AnimateThingMethods`, `CThing_Retail`,
   `CWorld_Retail`, `N_006e7b40`, `FablePush_006a3cd0`, `CEngineMeshVisibility`). Landed misspellings flagged in the queue:
   0x00A6CBB0 (AddChildPrimitive -> CPhysicsMesh::GetSingleInstanceCollisionSet), 0x004159B0 CGameEvent fake __fastcall ctor,
   0x0088F620 SetMute int param, 0x0091EA70 IsAbleToFire ABE, 0x00B38ED0 IsVisible private const member (landed as a __fastcall free
   function), 0x005FAD40 / 0x005FBD90 const, 0x0065F116 CIsHornTaskWithThisHorn::operator() const with PHorn as a member, the
   CModeBase slot names OnResume/OnLeave -> OnActivate/OnDeactivate in RemoveMode 0x0073ABA0 / ClearModes 0x0073AE70, the
   0x00AEB6D0 CEmitterPointer stand-in -> CCountedPointer.h, the nested CMemoryAllocatorVariableSize::CMemoryAreaChunk /
   CMemoryArea family (0x00A65060, 0x00A650F0, 0x00A65410, 0x00A65480, 0x00A65720), CWideString::Sprintf / SprintfDynamic
   0x0099BA70 / 0x0099BB80 catalog labels, 0x00ACD960 / 0x00ACD500 COptimised* relabels. Label-only fixes (ghidra labels /
   catalog names, e.g. 0x008ADBA0, 0x008AD950, 0x00CD3DA4 manifest row, 0x00B59710 CEngineComponent ctor) go to LABELS.tsv.
3. **retry1 / sec1-6b / libsig / mig9 / n621 rows** (PDB-proven ones only; libsig rows are CRT/vendor names and need the
   `libsig/lib_verify.py` proof unchanged -- names/catalog only, never the body).
4. **OWNER DECISION rows: scope only, in DECISIONS_W12.tsv with the evidence and a recommendation**, never respell: globals
   0x013B83D0 (CGame object; g_Game / g_InitBase / g_GFInitialiseState_013B83D0), 0x013B8394 (sound manager, ~10 spellings),
   0x0124F2B8 constant name (g_twoPi vs NCameraModes::Scale), the anonymous-namespace binding policy (0x008E8A10; per-TU `?A0x`
   hash, prefix-matched leaves), 0x00C351B0 CVirtualPlayer::Start signature identity, 0x00686D90 PeekThingSearchTools vs five
   GetThingSearchTools spellings, 0x0129BA3C float epsilon (~30 spellings), the duplicate per-TU static / unfolded-COMDAT
   convention (ilog2 x3, SmoothFadeValue x3, CDefString x2, IS_THING_ALIVE x2, LowerBound 0x004365B0 / 0x0040F020 ~33 spellings),
   vtable 0x012B29F8 class name (PKCPlaybackTemplate proposed).

## Rules and hand-back (as BRIEF_W7..W11)
- Idempotent scripts plus verified file lists; a composed overlay (`ov_final/rebuild/...`) re-made from the CURRENT repo before
  the final recheck (the repo moves under you: two chunk agents land every ~30 minutes, so re-copy the source files from the repo
  right before each sub-wave's edit and again before the final recheck); BINDINGS.tsv (action/symbol/target/by_row/note);
  LABELS.tsv (proven names only); DEFERRED.tsv; RENAMES.tsv (git-mv proposals: the coordinator moves); DECISIONS_W12.tsv;
  recheck logs.
- A respelling that changes a row's leaf symbol keeps `// Leaf:` and the header comment in step; one that changes a relocation
  symbol is mirrored in BINDINGS.tsv (RETIRE the wrong spelling, ADD the right one, same target). Full caller sets come from the
  relocation pairs of the compiled candidates, not from reloc_bindings.tsv alone (it keeps only the first caller per symbol).
- Shared headers: when a respell touches a class that has `rebuild/include/engine/<Class>.h`, prefer the header (the
  `#define Class ClassLayout` include pattern is accepted) over a local model unless the header flattens the vptr; say so.
- Purity: never introduce `__asm`, `_emit`, `naked`, `volatile`, `#pragma pack`; if a touched file carries a pre-existing
  `#pragma pack`, leave it and list it in DEFERRED.tsv (purity debt) unless removing it keeps the bytes.
- **Recheck:** every changed landed row must stay MATCH/RELOCATION_MATCH + PASS under
  `tools/decomp_pipeline/linux_lane/lane_tools/vland.py --recheck-landed`, `LANDVERIFY_WORK=$S/naming/w12/lv` (delete lv dirs
  after each batch; use a QTAG of your own on every compile so the concurrent chunk agents' grades do not collide). Rows that
  are `asm_bake`, `retail_asm` or `toolchain_library` are not compiled: only their names/catalog change.
  `bindings_multi_check.py` on your final bindings must print 0.
- Work only in `$S/naming/w12/`. Never run git. Never write under /home/user/FableDecomp. Never kill a process. Never touch
  `$S/w22/agent64*` or `$S/retry/*`. No recursive find/glob over `/` or the scratchpad. `timeout` on every compile; never repeat
  a failing command more than twice. Leave no background job running.
- **Report (SubagentHandback):** rows edited per sub-wave, bindings changed, labels, renames, decisions scoped, deferred items
  with reasons, recheck totals, and the exact overlay path + file list for the coordinator to copy.
