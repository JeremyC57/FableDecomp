# 0x00839940 -- B24B7 UPDATE: RELOCATION_MATCH with an Inferred device (read this first)
## 0. Inferred device: five dead release ASSERTs (needed for the match)
Source: differ/00839940.cpp = final.cpp = payload.json, overlay hdrf. It defines FABLE_RETAIL_ASSERT_DEAD_ARM, includes fable_assert.h (the
shared header landed rows use; precedent 0x00975BB0 today, 0x00B10B10 landed) and places FABLE_RETAIL_ASSERT on each pointer result that
OnHit dereferences without a null test before the hero block: phit_location->GetArmourDef().Get() (x2: DamageMaterial / BlockMaterial
reads), ptcd_explosion (after GetTCDriver), GetPBaseDef().Get() and phitter->GetPBaseDef().Get() (before their GetSubDef calls).
- Mechanism (c2 trace, inl/gh.py): the dead arms add front-end IL and raise OnHit's running caller size at the hero-experience IS_THING_ALIVE
  from 8927 to 9237 (cap reached at 8992), so IS_THING_ALIVE 0x0040DCE0 and bitset<274>::test 0x00410DE0 are called there as in retail
  (0x0083A825..0x0083A844); the summonable-creature bitset::test (retail expands it) stays at 8892. No code changes elsewhere.
- Graded IL window: +54 DIFFER; +74, +92, +114, +192, +310 (this form), +396 RELOCATION_MATCH; +414 DIFFER. Any of 3..6 of the sites
  above match (f4 = 3 sites, margin only 9 IL; f5 = 4). Chosen: all unchecked dereferenced results before the hero block, not a tuned subset.
- Evidence that asserts exist: FableWin.pdb OnHit 2761i / 12015b at /Od v 1544i / 5801b for this text (/EHsc): +1217i, fwsize "assert?";
  ~42i per debug ASSERT. FableWin is a later build (Ego_r's release OnHit 5631 b v retail 5195), so part of the +1217i is newer live code:
  ~29 ASSERTs is an UPPER bound; five sits well inside it. Conditions are not recoverable (release drops them).
- Second caveat (B24B3 lesson on 0x00F1EED0): FableWin's /Od bodies match VC10 output, which splits `call [reg+X]` into two
  instructions (+1i each). Retail OnHit has 24 virtual calls (dis_all.txt), so that explains only ~24..50i of the +1217i. Contrast:
  0x00F1EED0 PlayWave is +125i with 180 virtual calls -> no assert evidence there (not applied).
- The two GetPBaseDef().Get() asserts build a by-value CDefPointer temporary in the dead arm (+118 IL and four extra inline decisions
  each); fable_assert.h warns that class-typed temporaries in a dead arm can move live register allocation (here: graded MATCH, no
  change). The no-temporary alternative is f4 (asrt/f4.cpp: ptcd_explosion + the two GetArmourDef().Get(); RELOCATION_MATCH, margin
  only 9 IL at the hero site: g 9001 v 8992). The lander picks; final.cpp carries five (f6).
- Not asserted (deliberately): the two CMainGameComponent::Get()-> in the debug-line block (also unchecked); phit_location (null-tested).
- No evidenced non-assert source found (sections below: helper texts, C3DVector forms, Release texts all measured).

# 0x00839940 CThingGameObject::OnHit -- the one residue left (B24B6, 2026-11-24)
Best: differ/00839940.cpp (= e1f.cpp), overlay hdrf (landing form). DIFFER(5187v5195), 7 fg hunks; every hunk is the hero-experience
site 0x0083A825..0x0083A844: retail CALLS IS_THING_ALIVE<CThing*> 0x0040DCE0 and std::bitset<274>::test 0x00410DE0, ours expands them.
DIAG d1h (the same source with only those two calls forced out of line through noinline wrappers) = RELOCATION_MATCH, so nothing else differs.

## Why (c2 trace, inl/run.sh + gh.py)
- Regime-0 caller-size penalty (MATCHING_PLAYBOOK): pen = floor(sz*(a+g)/18000) only when a+g > 9000. At the hero IS_THING_ALIVE
  (decision #567, a=9, f=0.0378) ours has g=8927 -> no penalty -> expanded (score 69 < 74). bitset::test there (a=15, f=1.0 inside
  the forced GetTC) has c1=58 and needs pen ~31, i.e. a+g > 9000 too.
- Window: retail needs g >= 8992 at #567 (+65 over ours) and g <= 8985 at the summonable-creature bitset::test (#547, which retail
  expands; ours 8577 there). So retail's OnHit carries +65..+408 more IL than ours before the hero site with IDENTICAL code.
## What was measured (no code change unless said)
- e6 (DIAG): `= 0` on all 24 TC pointer locals before the hero site: g0 +24, g +24, code unchanged -- each dead init is +1 IL. Not
  evidenced (landed CThingGameObject rows declare TC pointers uninitialised); a tuned subset would be an oracle-forced device: NOT used.
- C3DVector operator+: ADD_XY_GROUP +4 (26 hunks), INITIALISE_ADD +44 (44 hunks), ADD_NAMED_RESULT (44 hunks) -> retail's debug block
  uses the return-by-constructor form.
- CIVCountedPointeeBase::Release RELEASE_COUNT -7; removing CDEFPOINTER_RELEASE_SELFDELETING_POINTEE + PASS22 -241 (code unchanged).
- __stl_throw_out_of_range as STLport's comma expression `puts(msg),abort()`: 0. STLport 4.6.2 texts fetched (stlport462/, Debian
  stlport4.6 4.6.2-7): bitset::test / _Unchecked_test / _Base_bitset helpers / _Rb_tree_iterator / _STLP_DEFINE_ARROW_OPERATOR are
  the repo's texts already. FableWin's STL is Dinkumware (no evidence for STL helper sizes).
- HasTC<CTCActionUseWindow> /Od variants never reach FableWin 60i; one forced site only.
- CDEFPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22: SRC_FAIL with this TU.
## Open
The +65 IL is somewhere in OnHit's own text (pre-optimisation tuples count 1:1, e.g. dead initialisers / named temporaries) or in a
helper's optimised IL, with identical code. No source found that is evidenced. Candidate evidence that would settle it: the FableWin
OnHit body bytes (only its size, 2761i/12015b, a later build, is on disk) or Ego_r's OnHit (5631 bytes, different build).
## Pick-any (pickany_vs_retail.py on e1f / e1p objects)
- GetSubDef<CCreatureDef> 0x00485815: NEQ (182 b) without FABLE_RETAIL_CDEFCLASSBASE_CREATURE_OWNER_SIZE_PASS26, EQ (162 b) with it ->
  the hand-back source must define it (e1p.cpp does; same grade).
- GetDef<CMaterialDef> 0x0066C340 NEQ (116 v 108 b, LEAF-REFERENCED): PRE-EXISTING lane issue of the shared GETDEF_GLOBAL_BODY text --
  landed row 0x008F7950 CreateDustEffectForCreature emits the identical NEQ copy (and GetDef<CEngineThemeDef> 0x0063F3A0). Not row-specific.
- _Rb_global<bool>::_M_increment 0x004292C0 NEQ (not leaf-referenced): shared STLport header copy.
- GetSubDef<CHeroExperienceDef> 0x006DAA90: EQ (182 b) when bound by its PDB symbol (rb_fix.tsv = reloc_bindings + the const template
  ??$GetSubDef@VCHeroExperienceDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCHeroExperienceDef@@@@@Z -> 006daa90; NAME_FIXES.tsv).

## Inline controls (owner decision 2026-11-08 puts a caller-size-cap residue in scope) -- reasoned, not graded
- Chain (CThing.h, ONHIT + GETTC_ORIGINAL_CHAIN): GetTC<T> (forced, depth 1) -> HasTC(type) (forced, 2) -> bitset::test (3);
  GetTC<T> -> GetTC(id) (2) -> CVectorMap::Find (3) -> CKeyPairCompareLess / end (4). Retail at 0x0083A832..0x0083A869 CALLS test and
  EXPANDS Find + the compare (`cmp dword ptr [eax],68h`). `#pragma inline_depth(2)` would call Find too; depth(0) also blocks the forced
  GetTC<T> / HasTC (no out-of-line copy exists). No depth value calls test while keeping Find expanded.
- IS_THING_ALIVE alone could be called with depth(0) on a split `if (IS_THING_ALIVE(phitter))` full-expression, but test still expands
  (g < 8985 there) -- half a fix, so not used.
- /d2inlT<N>: test's c1 is 58 at every GetTC site (f 1.0 inside the forced chain), so a threshold that calls it at the hero site calls
  it at all 30 other sites. The 9000 cap is a c2 constant (`cmp ecx,2328h` at 0x1078D1ED, MATCHING_PLAYBOOK): no flag moves it.

## B24B8 update (2026-11-24): section 0's fitted five replaced by FableWin's eleven (g11/)
The asserts are now RECOVERED (FableWin.exe 0x027EC280, Account A), not fitted. Only their release spelling is inferred: the eleven as
`FABLE_RETAIL_ASSERT(x)` dead arms (+ the kept call for success forms) overshoot the IL window (DIFFER); with the eight success forms as the
kept call alone (VERIFY-style) and the three condition asserts as dead arms the row is RELOCATION_MATCH (g11/payload.json). Lander decides
which release macro to accept; numbers in NOTES.md (B24B8).
