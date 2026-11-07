# chunk 826 -- six parked retries (all six re-parked, none matched)

Every row was re-graded unchanged first (same grades as parked). All attempts are in ATTEMPTS.log / STATUS.tsv notes;
masked diff lines (md) use agent824's md.py metric. differ/<addr>.cpp is the best source of each row with new lines 2-3;
differ/<addr>.test.cpp is the parked test, unchanged (all six PASS under g++).

| row | parked | now | what moved it |
|---|---|---|---|
| 0x00960120 CAIStateGroup_DeedReactionRoot::Update | 1576v1600 md 438 | 1580v1600 md 135 | layout fix, inline CDefPointer release, IsEmpty opt-in, creature ref, HasTC forms |
| 0x00ED5F50 CV_SwordInTheStoneScript::CSwordCrowd::Main | 4971v5024 md 381 | 4971v5024 md 22 | explicit CCharString("") region name, rand() counts as statements |
| 0x00951FB0 CAIStateGroup_JackDragon::UpdateHover | 2229v2272 md 394 | 2248v2272 md 357 | named bool break test (stops the x87 sinking) |
| 0x00850A80 CContextSensitiveItemDef::IsValidOverride | 2684v2576 md 300 | 2692v2576 md 301 | frame 0x30 = retail (b4); edi-zero / tail merge open |
| 0x00E8EF80 CGhostGrannySon::Main | 6038v5996 md 59 | unchanged | blocker (per-site STB dtor split; /d2inlT72 only) |
| 0x00D7FE90 CQ_HeroSoulsArenaScript::WatchForRoundsInArena | 2105v3148 md 398 | unchanged | blocker (per-site ~vector<CScriptThing> split) |

## Per row
- **0x00960120** (18 attempts). The parked source had a layout bug: `CThingCreatureBase`'s pad started at 0x92 but sizeof(CThing)
  is 0x94 there, so PCurrentAction was read at +0x104 (retail +0x100) -- fixed (pad from 0x94). The three CDefPointer
  temporaries of PeekPDeedEffectsDef were called out of line: retail expands their release; the forced-inline Release()
  holding the pointee in a local (the shared engine/CDefPointer.h shape, local model kept because the row's def classes are
  local) gives retail's `mov esi,[eax+4] ; ... dec esi ; ... mov [eax],esi` (md 415 -> 269). `animation->IsEmpty()` is tested in
  place in retail: new opt-in FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY (hdr/fable_string.h; md -> 185). A
  `const CThingAICreature& creature = GetCreature();` before the attitude if stops the jump threading (source in esi, the
  GetCreature loads in the test block as retail). Retail's standalone `HasTC(0x0f)` is `mov ecx,[eax+20h] ; test ch,ch ; jns`:
  mini-proved that only a non-bool mask or the if-return form (`if (word & mask) return true; return false;`) emits that
  (every `!= 0` / shift / cast spelling gives `shr ecx,0Fh ; test cl,1`), while the PeekTC/GetTC presence must stay a stored
  bool: c17 writes HasTC as the if-return form and PeekTC/GetTC's presence as the mask expression in place (md 135).
  Remaining: the bit-16 presence (0x50) load narrowed to `mov cl,[eax+2Ah]` (only the `const bool&` form keeps retail's dead
  spill, and it narrows; the Hawking 0x0094BB20 plain-bool form folds into `test byte` here), frame slots +4, the GetSex operand
  order, `mov esi,32h` registerised in the IsInterruptable loop, and the LookAtThing CIntelligentPointer argument (retail
  inlines the copy body at that site and calls 0x00654D58 at the PerformAction site; with FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
  both are called; /d2inlS40 inlines more = function-budget side, diagnostic only).
- **0x00ED5F50** (7 attempts). `AddSpawnedFunction(new ..., "")` builds the implicit "" temp before the new-expression; the
  explicit `CCharString("")` (the landed RegisterMain rows 0x00D61C60 family) builds it after and destroys it first, as retail.
  Retail calls rand() before building each PlayLoopingAnimation name temporary: each count as its own statement
  (`long count1=rand()%2+1;` ... count4; descriptive names) gives retail's order and every frame slot (md 22; chunk 812 had the
  same rand lesson for an angle). The one residue is the function-final ~CScriptThing (Sword): retail expands it at 0x00ED71C8,
  this build calls 0x004AA840. A row-level `inline CScriptThing::~CScriptThing() { }` (g5) expands it there but also expands
  the by-value Par1 temporary's dtor inside the forced-inline CSpawnedFunc1 ctor, which retail calls (0x00ED6AA1): this is the
  case INLINE_DEPTH_REVISIT.md lists as still unexplained (CScriptThing-holding special members). A plain-inline CSpawnedFunc1
  ctor (trial opt-in, g6) is then not inlined at all; dropped.
- **0x00951FB0** (7 attempts + 2 diagnostics; f5 declaration order: no change; f7/f8 Left / both ctors declared-only: 394 / 324, but retail expands Forward at the first arm, so f2 stays the best shape). mini probes: VC7.1 sinks the toEnemy products and sqrt below the break test
  (partial dead code) when the loop holds a destructible temporary and the comparison is the if's own expression; a named
  `bool enemyAboveLanding = ...; if (enemyAboveLanding) break;` keeps them where retail has them (also: distance read after
  the loop does, retail has no such read). Now the loop head matches structurally. Remaining: frame 4 short with slots shifted,
  and the fire-blast arms: retail expands the Forward ctor at the GoLeft/facing arm and calls 0x00953780 (Left) and 0x00953750
  (Forward) at the other two, this build expands all three (per-site split). Checked: Ego_r.pdb has a
  CCreatureAction_DragonFireBlastRight type, but the exe has RTTI vtables only for Base/Forward/Left, so the third arm is
  Forward, not Right.
- **0x00850A80** (10 attempts). b4 (from the parked a-series): vector declared before radius/box, visible same-TU noinline
  C2DBoxI(const C2DCoordI&, long) body copied from the landed row 0x00434740 (reloc binding ??0C2DBoxI@@QAE@ABVC2DCoordI@@J@Z
  at 0x00434740, consistent), `C3DVector::ToC2DCoordI` forced inline (PDB ?ToC2DCoordI@C3DVector@@QBE?AVC2DCoordI@@XZ),
  filter through operator(): frame 0x30 = retail. The residue is retail's edi = 0 register for the whole body and the
  release/return-false tails it lets VC7.1 cross-jump (16 v 20 `ret 0Ch`).
- **0x00E8EF80** and **0x00D7FE90**: the brief's documented per-exit inline-split blockers. New tries not in GOTCHAS:
  e1 a row-local cost-only ~CCountedPointer<STB> specialisation (no-op: the inliner costs the optimised callee), e2 the
  while(!T) loop form; d0 the plain-inline ~vector form plus /d2inlT20..40 and /d2inlS10 diagnostics (threshold side, no value
  reproduces retail's site pattern). Parked early.

## Names / labels
All six names were already proven; no new names. No label corrections (LABELS.tsv and NAME_FIXES.tsv carry headers only).
Open question, not proven: 0x00851490 is labelled
`GetAllThingsInRoughArea<CThingObject,std::list<...>,CFilter_Bind2<CThingFilter_In2DRange,CThingFilter_CanBeHitByEpicSpell>>`
(propagated catalog name), while the 0x00850A80 caller passes a C2DBoxI, a HasTC(0xEF) filter and a std::vector<CThing*>;
the row calls it as a descriptive stand-in `GetAllThingsInRoughArea<CThing, vector, CThingFilter_HasTC>`. Either ICF folded
two instantiations or the propagated label is the wrong one; needs the body compared before anyone renames it.

## Headers
hdr/fable_string.h: one new opt-in (hdr/README.md). Rediffed against rebuild/include before hand-back: only the opt-in lines differ.
No callee binding was added (reloc_bindings checked for 0x00434740).
