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
