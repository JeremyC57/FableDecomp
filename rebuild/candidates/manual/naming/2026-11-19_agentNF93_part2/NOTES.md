# NF93 notes (round 8, queue part 1 of 3)

## Descriptive names

## Entries
- q11232 0x005C14A6: the default ctor is FableWin.pdb only (0x0235E399); Ego_r.pdb has just the copy ctor. Self row now the nf86 pick-any
  scaffolding on the header inline ctor (`// Fidelity: scaffolding, not retail`), so it no longer clashes (LNK2005) with the COMDAT copies that
  0x005BEDCB / 0x005BEC92 emit through FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA_CTOR. Every retail call site (cs.py) is in those two rows.
- q11242 (0x00527950 map<u64,u64> _M_erase): the three stand-in binders have no retail reference at all (cs.py: no call/jmp, no absolute
  dword in any section). 0x00528E90 / 0x00529EE0 are byte-identical 66-byte ~_Rb_tree bodies (STLport ~_Rb_tree = clear + free header;
  an out-of-line implicit ~map is the same bytes), 0x00528550 is the 51-byte _Rb_tree::clear body. FableWin.pdb has both ??1?$map@_K_K...
  and ??1?$_Tree@...$_Tmap_traits@_K_K... (Dinkumware), so both dtors existed; which address is which is not provable. Blocked: identity.
- q11251 follow-up (not changed): 0x0086BE40's TU defines the PDB-spelled GetRandomTextureMorphs strongly (same-TU body, needed for its
  match), while the self row 0x0086B810 defines a stand-in spelling over a local model. Renaming 0x0086B810 to the PDB spelling would make
  two strong definitions. Fix: move the body into engine/CRandomAppearanceMorph.h as a header inline (opt-in) that 0x0086BE40 uses, then make
  0x0086B810 the nf88 pick-any scaffolding (address-take inside an anonymous derived class, the member is protected); regrade 0x0086BE40
  (/d2inlT49) and 0x0086B9D0. Not attempted (budget).
- q11261 / q11262 (SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900): pilot row 0x008C2980 ported in 3 grades on the
  0x008C2AD0 (WillHealInto) recipe. Recipe for the rest: the 0x008C2AD0 define/include set + FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR and
  engine/CSoundPair.h when an action header has the GetSoundPair inline ctor; the action temporary's class goes on its generated
  engine/CCreatureAction_<X>.h through a `_CLASS` opt-in (retail class on CCombatActionBase, rtti_bases.tsv, ctor from Ego_r.pdb, generated
  flat class kept under `#else`), the owning action's FinishAction through a declaration opt-in. Next: 0x008C2A80 (WillEnflameInto, header
  already has a _MAZE retail block; CCreatureAction_CastWillEnflameSpell ctor (CThingCreatureBase&, long) 0x008BB450), then 0x008C29F0
  (WillShieldInto: ShieldUpTime / ShieldHealth members, CMainGameComponent::ConstantFPS).
- Stopped here (context): entries q11261 and q11262 are open with the rows left listed in APPLIED; q11264 onward not started.

## Descriptive names
- No new made-up names. STLport instances named descriptive-by-library: 0x005C08D3 find_if<CCategory*, CIsInventoryCategory>, 0x005C06C3
  __find_if<CCategory*, CIsInventoryCategory>, 0x004DB88A _Rb_tree<long, pair<const long, CDefString>>::clear (and the NAME_FIXES
  0x004DA27F _M_erase).

## Rule slips
- One duplicated grade: 0x005BF654 was graded twice with the same source back to back (a grep count and a head of the same command); both
  are in grades.log and counted in STATUS (4).
- 0x0086B810 (q11251 self row) was left as a stand-in spelling (one-definition follow-up above), not ported.
- Rule slips (added at hand-back): four Bash calls exited non-zero: the first `ls` of $S/w22/nftools + $S/w22/agentNF92 (missing
  directory), the CCategory overlay `diff ... && echo` (diff exit 1 by design), and two `cat .../owner` reads of unclaimed locks (005c06c3 check,
  006655a0 check). grades_used counts g.py compiles from grades.log (including baselines and the smoke test of 0x005BEDCB); vland runs are
  not counted.
- Section 2 "old" symbols derived from the old source, not a baseline compile: 0x005BDFB1 (find_if), 0x005BF3C6 RemoveItem, 0x004DB88A
  (AutoTinyFreeListAllocation_004da27f), the 0x005C06C3 old leaf, and the 0x0086BF70 old callee spellings. The 0x005BEC92, 0x005BF3C6
  find_if and 0x008C2980 pairs come from baseline grades.
- Header comment slip seen (not changed): engine/NInventory_CTCInventoryBase.h lines 610 / 612 attach 0x005BF654 to the (long, long, ..)
  overload and 0x005BF32D to the (CThing&, ..) one; the landed rows define the opposite (0x005BF654 = CThing&, 0x005BF32D = long itemDefIndex).
- Tests still spelling stand-ins (self-contained models, they pass, unchanged): 0x005BEC92, 0x005BDFB1, 0x005BF3C6 (FindCategoryWithDefIndex_005c08d3),
  0x00753B40, 0x004DB88A, 0x005C06C3.
- Sibling overlays at hand-back: NF94 hdr has CCategory.h (its REMOVEITEM block is byte-identical to my copy), NF95 hdr has CCategory.h and
  NInventory_CTCInventoryBase.h among others; neither declares SelectItemOfType, GetRandomMorphSet, the CastWillForceSpell class or the
  WillForcePushInto FinishAction. My changed addresses have no UNMATCHED_ROWS.tsv line and no EH funclet includes them.

# Part 2 (NF93 continued, same workspace)

## Entries
- q11261 / q11262 (SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900): 14 rows ported in part 2 on the 0x008C2980 recipe and its
  siblings (0x008C2AD0, 0x008DF3A0, 0x009210A0, 0x007EB9B0, 0x008A05A0), plus the binder 0x008E10E0 (access-only respell). Three wrong
  temporary classes found by reading the ctor bodies' vtable stores (rdis.py + RTTI): 0x008DF640 builds CCreatureAction_ScorpionRecover
  (0x008E4770), not GoUnderground; 0x008663D0 builds CCreatureAction_BeDazed (0x00865A00), not AlertToPlayer; 0x008A9320 builds
  CCreatureAction_ForcePushInto (0x008ADFE0), not SheatheWeapons. 0x0072D990 (unattributed) is ~CCreatureAction_CastAssassinRush.
  Seven rows left blocked: port (dozen-class local models, not drafted; classes in STATUS). Both entries end blocked: port.
- Self renames in part 2 (RENAMES s1): 0x008C2A80, 0x008C29F0, 0x008C5370, 0x008663D0 (QAE -> UAE), 0x008DF640 (QAE -> AAE), 0x0072D990
  (stand-in -> ??1CCreatureAction_CastAssassinRush@@UAE@XZ). The 0x0072D990 self rename moves its file from
  unattributed_CBase0062e900Derived0072d990_0072d990.cpp to a CCreatureAction_CastAssassinRush name; no EH funclet includes any of the
  part-2 rows (grep of rebuild/src/compiled/01/21 for each file name: none).
- Row-local classes kept (no shared header exists; each on a shared base): CCreatureAction_WaspQueenDie (0x008C5370),
  NCreatureMode::CModeScorpionKingExhausted (0x008DF640, as its landed siblings), CAIStateGroup_CreatureIdleUnderground (0x009208E0),
  CTCActionUseKickChicken (0x007ECE20), CTCCarriedActionUseDropGenericBox (0x007ED060), CCreatureAction_ForcePushInto (0x008A9320),
  CCreatureAction_CastWillDrainLifeSpell (0x008B9010, as 0x006F5C10), CCreatureAction_BeStunned (0x008663D0).
- 0x0072D990 is `// Fidelity: scaffolding, not retail`: a qualified call of the header class's implicit destructor in an anonymous-namespace
  helper makes the TU emit it as a pick-any COMDAT (selection 2 checked with objdump). With `#pragma inline_depth(0)` the member
  ~CCountedPointer was not expanded (DIFFER 22v75); without it VC7.1 emits the destructor out of line (as at its three retail callers).
- Not changed, recorded in NAME_FIXES: 0x008C2810 (NF94 lock) still binds the old QAE FinishAction of 0x008663D0; 0x0072CAA0 binds
  ??1CCreatureAction_CastAssassinRush@@QAE@XZ (should be UAE).
- q11272 needs header claim: engine/CTexture.h FABLE_RETAIL_CTEXTURE_LEVELBYTELENGTH (egod e13) declares `unsigned long GetLevelByteLength(unsigned
  long level) const` (?..QBEKK@Z, in no PDB); Ego_r / FableWin spell ?GetLevelByteLength@CTexture@@QBEJJ@Z (the header's own
  FABLE_RETAIL_CTEXTURE_GETLEVELBYTELENGTH block already declares the long form). The one user 0x009FB750 defines the body in its TU (strong;
  it was a second strong definition of the same KK symbol as the self row 0x009F9E80, which now spells JJ). Unblocks: 0x009FB750 respell.
- q11270: tests of 0x0085EE10 / 0x00803B20 are self-contained models and still spell IsCreatureButNotHero / IsControlledCreature (pass, unchanged).

## Part 2 rule slips
- 0x0085EE10 was graded once without the cpp #error precheck: engine/CDefString.h needs CDEFSTRINGTABLE, Wine cl spun on the #error until I killed my own cl/wine processes (18032 / 18034 / 18035; no other agent's processes touched). From then on every compile went through tools/gg.sh (cpp precheck first).
- One tool call tried a foreground sleep (blocked by the harness; no effect).
- Context grew heavy after q11295; entries q11296 onward left open (not started).
