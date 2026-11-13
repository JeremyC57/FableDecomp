## 0x004093E0

## 0x004093E0 CUserProfileManager::SetDefaultValuesVideo -- DIFFER(841v841), 9 hunks, block placement only (wf21 author, ~100 grades)
Best source: differ/004093e0.cpp (= v68). Same instructions and registers as retail; the only residue is that retail lays the def
arm (else of the null test, 0x004095D4..0x00409676, ends `jmp 0x004094AD`) after the inlined ClampResolution loop's back-jmp, while
VC7.1 here keeps it straight after the null arm; the shifted offsets then flip three loop-alignment pads. The SAME layout appears in
SetDefaultValues 0x0040C790 and SetDefaultValuesForNewProfile 0x00409B70 (both carry a copy of this code), so it is a property of the
source, not a fluke.
What got it from 921 to 841 / same code:
- port onto fable_user_profile.h FABLE_RETAIL_CUSERPROFILEMANAGER (+ VIDEODEFAULTS overlay), engine/CDefPointer.h, CSystemManager
  GetDisplayManager(), FableWin inline CDisplayManager::GetBitDepth()/GetRefreshRate() (overlay), real PDB reference signatures;
- GetConfigOptionsDefaultsDef body in the TU (sibling lever; without it the def gets its own frame word / registers move);
- refresh pick loop as do { } while (++i < 4): a for loop is fully unrolled by VC7.1 (921 bytes) -- that alone fixed the
  register allocation (edi = def, ebx/ebp = width/height, depth in the frame slot);
- mode search through `const std::vector<CDisplayModeDesc>& modes = GetDisplayModes();` with GetDisplayModes' body in the TU
  (gives retail's `xor edx,edx ; mov [esi+48h],edx ; mov eax,[esi+80h] ; cmp eax,[esi+84h]` head);
- ClampResolution / ClampRefreshRate real bodies in the TU (auto-inlined like retail); unrotated loops only with early-return
  FABLE_FORCEINLINE predicates (IsModeWiderThan / IsRefreshRateAbove: MADE-UP names, as in the landed ClampResolution row;
  FableWin.pdb lists no such helper, so the real spelling is open). A while-loop mode search stops ClampResolution being inlined.
Tried without effect on the placement: if(!pDef)/if(def.IsNull())/def.Get()/==0 tests, arm order swapped (worse: def falls
through), else-if complementary tests, two separate ifs, per-call if/else and ternaries (threaded or 825 bytes), Antialiasing
outside/after the if-else (guarded or ternary), ClampAntialiasing duplicated into both arms (tail-merged, no move), locals defaulted
to -1 with one def if (795), hoisted max-effects local, const def / Gamma order / assignment init, ClampVideoOption real body
visible (auto_inline(off) or noinline), GetConfigOptionsDefaultsDef body absent, CDisplayModeDesc desiredMode local, /G5 /G7 /Oa
/GS /Ow /Ob1 /d2inlT20..200, QFE-4035 compiler (identical), toy probes (VC never moved an else arm in any toy).
Best next idea: find what makes c2 treat the def arm as the cold successor (it is laid out like an unlikely block): candidates are a
retail-only statement in the def arm that folds away (an assert-like construct / an inline accessor with its own test), or the
mode/clamp section being spelled with retail's real helpers (look for FableWin debug-build line info or the debug copy 0x01881E20
to read the original statement structure).
Globals: g_MemoryA/B are `unsigned long` in GFConfigDetection's landed source but retail tests them with `jle` (signed) here;
declared `long` in this row (type mismatch to resolve at landing; names reused). [Superseded by the repair section below: NAME_FIXES.tsv now handed back.]

## 0x004093E0 attempt 2 (wf21 author #2) -- still DIFFER(841v841), 9 hunks, same block-placement residue (~30 grades + toy probes)
Best source: differ/004093e0.cpp (= t1.cpp). New, kept: the clamp loops no longer need the made-up FABLE_FORCEINLINE predicates
(IsModeWiderThan / IsRefreshRateAbove): `unsigned long index = x; while (index >= 0 && <elem> > max) --index; x = index;` (folded
unsigned guard) gives retail's unrotated `cmp; jle exit; dec; sub; jmp top` loop in the row AND MATCHES the landed standalone
ClampResolution 0x00406D90 (q.py MATCH with that one-line change, $W/cr/cr1.cpp) -> suggest re-spelling the landed row at landing
and dropping its made-up helper. `for (i = x; i >= 0; --i) if (<= max) { x = i; break; }` gives the same bytes.
Tried without moving the def arm (do not repeat): per-call if/else, !-test per call, per-call arg ternaries (825); locals set by
if/else then one call set (795/809, no tail duplication); comma-expression ?: , do{..break..}while(false), goto forms; bool-local
test, raw pointer ==NULL, IsNull()!=false; inline-helper IsNull bodies with early return; __assume (release-ASSERT) at arm start,
end of null arm, in the clamp loop; whole if/else, null arm only, def arm only in __forceinline helpers; ClampAntialiasing in both
arms; ClampResolution / ClampRefreshRate / GetDisplayModes / GetConfigOptionsDefaultsDef bodies defined AFTER the row; folded
`< 0 ||` range tests in the Clamp bodies; for(;;)/break loop forms (rotated: wrong); flags /EHsc (EH frame), /Ox, /Oi- (same),
/Op (921). Probes ($W/toy2): VC7.1 lays out if/else arms in IL order (then-arm falls through from the test, the LAST arm falls into
the join) for every spelling tried, including folded `|| u<0` terms (those only swap which arm comes first); nothing ever put an
arm after a later loop. Removing ClampResolution / the whole second half / ApplyValues from the row does not move the def arm either.
Model of retail's layout: chain layout where null->join is the chosen fall-through edge and the def arm becomes its own chain,
emitted at the first hole (after the CR loop's back-jmp; same in the 0x00409B70 / 0x0040C790 copies). Our compile always picks
def->join. So in retail's c2 the null arm out-weighs the def arm, or its IL had the def arm later than the join.
Best next idea: work out which source fact makes c2 weight the null arm higher. Candidates not yet tried: a real Lionhead
CDefPointer operator-> / IsNull with a debug-check macro that leaves a folded branch in release (look in other landed rows with
CDefPointer null tests whose retail puts the non-null arm out of line, and copy what made them match); LTCG (/GL) codegen for this
TU (would need a link-time harness); an EH-on TU where every callee is known nothrow.

## 0x004093E0 attempt 3 (wf21 author #3) -- RELOCATION_MATCH (strict headers too), test vt.py PASS, addmatch.py done
What made it match: the block-placement residue was a missing STATEMENT (wf8 lesson): the null arm does NOT set Antialiasing;
ONE guarded statement after the detail if-else, `if (def.IsNull()) Antialiasing = 0;`, is threaded by c2 into the null arm. The
join then has the null arm as fall-through predecessor and the def arm stays behind the inlined ClampResolution loop, jumping back
(retail 0x004095D4..0x00409672). The def arm keeps `Antialiasing = def->Antialiasing;`. (Tried and wrong: AA via phi local, AA
ternary after the if-else, `if (!def.IsNull()) AA = def->AA` after, both AA assignments after as if/else; goto/late-IL def arm,
inline helper with early return, /Os /O1 /EHsc.)
Also: the local CNewFrontendGameComponent model was replaced by engine/CNewFrontendGameComponent.h; that needs
engine/CMainGameComponent.h instead of fable_game.h (fable_game.h's FABLE_RETAIL_CGAMECOMPONENT class collides with
engine/CGameComponent.h). Headers: overlay hdr/ (fable_user_profile.h VIDEODEFAULTS, engine/CDisplayManager.h VIDEODEFAULTS). Flags: none.
Clamp loops use `while (index >= 0 && ...)` (no made-up helpers); landed ClampResolution 0x00406D90 also matches that way ($W/cr/cr1.cpp).
Globals g_MemoryA/B declared `long` here (retail `jle`); GFConfigDetection's landed source has `unsigned long` -- resolve at landing.

## 0x004093E0 repair (verifier findings 1-7) -- still RELOCATION_MATCH (strict headers too), test vt.py PASS, addmatch.py rerun
- Source: dropped the unjustified CDefinitionManager::GetDef template body (0x00410890 stays out of line through the header's
  template declaration; landed row CDefinitionManager_GetDef00410890_00410890.cpp, named in the Callees comment); abs pragma comment
  now cites retail's inline `cdq ; xor ; sub` at 0x0040967A..0x004096C8; GetConfigOptionsDefaultsDef body now has the landed row's
  explicit `CCharString name("CONFIG_OPTIONS_DEFAULTS_DEF",-1)` temp, and the comment states the retail fact (Ego_r.pdb: same source
  file, 0x00406C90 ahead of SetDefaultValuesVideo 0x004073D0). It keeps the PDB accessors CMainGameComponent::Get() /
  CNewFrontendGameComponent::Get() instead of the landed row's made-up g_pMainGameComponent / g_pFrontendRuntime: those bind the
  real statics PMainGameComponent 0x013B86A0 / m_pNewFrontendGameComponent 0x013B871C under second names (NAME_FIXES).
- Globals: one name per address = GFConfigDetection 0x004017B0 spellings (descriptive, no PDB symbol). TYPE of 0x01375084 /
  0x01375088 / 0x0137509C / 0x013750A0 is `long` (3JA): signed compares in this row (jle on MemoryA/B), GetMaxResolution 0x00408900
  and GetMaxRefreshRate 0x00406890; GFConfigDetection only stores them, so its `unsigned long` (3KA) is the spelling to fix.
  Spelling conflicts: 0x01375080 g_MaxAntialiasing vs g_ConfigLevel (0x00409090), 0x01375084 g_MaxResolutionWidth vs
  g_FableConfigWidth (0x00408900), 0x01375088 g_MaxRefreshRate vs g_maxDisplayRefreshRate (0x00406890). All in NAME_FIXES.tsv.
- NAME_FIXES.tsv (10 rows): ClampVideoOption 0x004068D0 (?..@AAEXAAMMMMM@Z, float&), ClampAntialiasing 0x00409140 and
  ClampResolution 0x00406D90 (?..@AAEXAAKJ@Z, unsigned long&): the landed rows use pointers / public and bare or missing Leaf lines,
  and this row's header declares the PDB reference forms, so they must be respelled when this lands; plus the two component
  statics and the five globals above.
- Test: added a 1024x768 16-bpp mode beside the 32-bpp one (def BitDepth 16 -> mode 1, no def -> mode 2) and display rate 74 Hz
  (75 is nearer but above -> 70). Mutants: removing the `<= refreshRate` guard FAIL, removing `depth = def->BitDepth` FAIL
  ($W/rp_mut_guard.cpp, $W/rp_mut_depth.cpp).
- Verifier findings not applied: none (finding 5 applied for the statements; the landed row's global names are respelled the
  other way, via NAME_FIXES, because the shared headers carry the PDB statics).
- Note: $W/r1.cpp (an old author variant) was overwritten by the first repair variant.

## 0x004093E0 repair 3 (verifier findings 1-5) -- still RELOCATION_MATCH (strict headers too), test vt.py PASS, addmatch.py rerun
Work dir: $W/r3 (base.cpp = the matching source before this repair; backup of source/test/hdr/NAME_FIXES/NOTES/payload in repair3_backup/).
- Test (1): the override case is now def 640x480 + g_MemoryA/B = 1280x1024 -> Resolution 2 (losing the override gives the def's
  missing mode -> clamped 0). Mutant r3/mut_override.cpp (width override deleted): FAIL (bad 8).
- Test (2): the same case is uncapped (g_MaxRefreshRate -1) with display rate 88 -> RefreshRate 3 (written against a wrong table {60,70,75,85}; in retail 0x0122DE30 = {60,75,85,100}, so
  85 Hz is the THIRD entry, index 2; corrected in Repair 4). Mutant
  r3/mut_loop3.cpp (`while (++i < 3)`): FAIL (bad 256). Original test: PASS under vt.py and native g++.
- Source (3): header comment rewritten: per carried body the retail fact (call / no call, expansion range), the landed row and where
  this row's spelling differs, and its own-address grade. Graded THIS source against each address's oracle
  ($S/batch/wf21r3_callees_oracle.tsv, made with oracle_from_exe.py from r3/rows.tsv):
    0x004083E0 GetConfigOptionsDefaultsDef  RELOCATION_MATCH (was DIFFER(203v194) before this repair)
    0x00410890 GetDef<CConfigOptionsDefaultsDef> RELOCATION_MATCH
    0x004069A0 ClampRefreshRate             RELOCATION_MATCH (out-of-line copy, 52 bytes; no landed row)
    0x00406D90 ClampResolution              MATCH
    0x00406880 GetDisplayModes body         MATCH (bytes only; ICF fold, no name claim)
  What fixed 0x004083E0: GetDef's template body visible (shared opt-in FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, noinline).
  Found by probing the landed 0x004083E0 row: removing its auto_inline(off) GetDef body gives exactly our DIFFER(203v194); its
  dtor / mutable / __forceinline differences do not matter. The header's GetDef body then needed SetDynamicCast to re-read the
  source handle (new overlay opt-in FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD) to match 0x00410890 itself (Assign() caches it:
  DIFFER(116v108)). Tried and not it for 0x004083E0: CDEFPOINTER_RELEASE_POINTEE / RELEASE_COUNTREF (203v194), RELEASE_SELFDELETING
  (182v194, drops the Delete), each + CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES. 0x004093E0 matches with or without the GetDef body.
- Source (4): GetDisplayModes: two-source name (Ego_r 0x004054D0 + FableWin 0x01889A50); comment says the TU never references
  0x00406880 and binds no symbol to it (ICF fold landed as CModeBlock::GetAnimFilter). NAME_FIXES.tsv: ClampResolution row reworded
  (reference parameter, no made-up helper, no local CRetailVector), new rows 0x004083E0 (callee spellings) and 0x00410890 (landed
  made-up GetDef_00410890); 0x013B86A0 / 0x013B871C rows kept.
- Source (5): duplicate `ClampVideoOption 0x004068D0 (x4 per arm)` removed.
- Headers: hdr/engine/CDefPointer.h added (full copy of the current repo header + the one #ifdef). Flags: none.

## 0x004093E0 repair 4 (second round, verifier findings 2-6) -- still RELOCATION_MATCH (strict headers too), test vt.py PASS, addmatch.py rerun
Work dir: $W/r4 (backup of source/test/hdr/NOTES/NAME_FIXES/payload/oracle in repair4_backup/). Row code unchanged (comments only).
- Test (2): refresh table now retail's {60, 75, 85, 100} (Fable.exe bytes at 0x0122DE30 read directly). Cases: no def 78 Hz -> index 1
  (75); def 90 Hz picks 85 (index 2), capped at 75 -> index 1; uncapped 120 Hz -> index 3 (100, fourth entry); 84 Hz: 85 nearer but
  above -> 75 (index 1). vt.py PASS + native g++ PASS. Mutants r4/mut_loop3.cpp (`while (++i < 3)`): FAIL (bad 256);
  r4/mut_override.cpp (width override deleted): FAIL (bad 8). The row source only declares the table extern (no values modelled);
  its header comment now states the retail values.
- Headers (3): hdr/engine/CDisplayManager.h rebuilt = current repo header (keeps FABLE_RETAIL_CDISPLAYMANAGER_RENDERTARGET for
  0x00F39730) + only the VIDEODEFAULTS #ifdef. `diff <repo> <overlay> | grep -c '^<'` = 0 for all three overlay headers.
- Names (4): fable_user_profile.h GetDisplayModes comment no longer gives 0x00406880 (PDB addresses + expanded at 0x00409548..0x0040954E;
  the ICF fold is landed as CModeBlock::GetAnimFilter, no symbol of this row bound there); hdr/README.md likewise.
- NAME_FIXES (5): 0x013B86A0 / 0x013B871C rows list every spelling seen with file counts (r4/name_counts.txt, from reloc_bindings.tsv +
  extern declarations commented with the address; 31 wrong spellings for 86A0, 8 for 871C).
- Comments (6): do/while cites the rolled `inc ecx ; cmp ecx,4 ; jb 0x409694` at 0x00409694..0x004096C8; the unsigned guard cites the
  unrotated `cmp ; jle ; dec ; jmp` loops at 0x004095C6..0x004095D2 and 0x004096F0..0x004096FA (checked in dis.txt).
- Grades (r4/final.cpp == 004093e0.cpp): 0x004093E0 RELOCATION_MATCH (strict too); carried 0x004083E0 RELOCATION_MATCH, 0x00410890
  RELOCATION_MATCH, 0x004069A0 RELOCATION_MATCH, 0x00406D90 MATCH, 0x00406880 MATCH (each also with LANDVERIFY_STRICT_HEADERS=1).

