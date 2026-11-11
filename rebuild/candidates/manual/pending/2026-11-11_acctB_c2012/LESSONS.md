# Chunk 2012 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 2012, 0x006DE280) With CVectorMap::LowerBound inline (FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND) two consecutive plain `for`
  loops give the second loop's body and the code after it the same static frequency (25 = 5 x 5), so a GetTC inside loop 2 and one
  after it score the same (68) and no /d2inlT can split them. Retail's split (loop site called, after-loop site expanded) comes from
  the second loop behind `if (it != end)`: no code (c2 merges the test with the loop guard), body 12.5 / after 15 -> scores 77 / 74,
  closed with `/d2inlT77`. `if (!list.empty())` gives the same frequencies but keeps an extra `cmp [eax],eax ; je`; `&& true` in the loop
  condition (body 2.5x, after 7.5x) also works without a flag but is not plausible source.
- (chunk 2012) FableWin.pdb emits an inline helper's COMDAT right after the first function that references it: the run after a row's
  own FableWin address names its inline accessors (0x0249D470 CWorld::SetRegionLoadConfirmationAsGiven and 0x0249D490
  CTCHeroStats::SetBestTrophyWitnesses after TerminateMinigame 0x0249BE40; 0x02489F40 CTCWife::AppearanceBool after
  CheckAppearanceChanges) -- a two-source name for a bare `mov byte ptr [eax+130h],0`.
- (chunk 2012, 0x006DE2E5) A spill store that retail places inside a test (after the next load) rather than at the definition means
  the frame-resident variable is assigned there: `holder = carrier;` inside `if (carrier && carrier->GetType() == 2)`, with `carrier`
  a separate local.
- (chunk 2012, 0x006D1493) `c = (x & 0x80) ? 7 : 6` compiles to `and eax,80h ; or eax,300h ; shr eax,7`; an if / else-if chain or
  `6 + ((x & 0x80) >> 7)` gives `neg ; sbb ; neg ; add` or `sar/shr ; and ; add`.
- (chunk 2012, 0x006D1356) Retail compares a set find with end() AFTER the call (`call find ; mov edx,[esp+34h] ; cmp edx,[esi]`):
  `s.find(x) == s.end()` evaluates end() first (VC7.1 evaluates the member operator's argument before the object) and hoists the
  header load above the call; a named iterator or `s.end() == s.find(x)` gives retail's order.
- (chunk 2012, 0x006D1382) CDefPointer releases `test ebx,ebx ; je ; dec [ebx+4] ; jne` after GetDef<T>(long, ...) / GetSubDef<T> need
  FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY; declaration-only gives the captured
  form `mov ecx,[eax+4] ; add eax,4 ; dec ecx ; mov [eax],ecx` plus a zero store.
- (chunk 2012, 0x006D181B) `fst [m] ; and dword ptr [m],7FFFFFFFh ; fld [m]` is the inline GFFloatAbs (FableWin ?GFFloatAbs@@YIMM@Z);
  the fabs intrinsic emits `fabs`.
- (chunk 2012) gr.sh-style wrappers: resolve the source path BEFORE `cd` into the repo, and give `grep` the file name -- an empty path
  makes `grep -o "^// Flags:"` read stdin and the script waits forever.

Review at landing:
- 0x006DE280 carries `// Flags: /d2inlT77` with an `// Inline-budget:` line (owner decision 2026-11-08, preference 2) and defines the
  landed CTCTrophy::PlaySoundW 0x006DDCD0 body in its TU (chunk 2007's pattern). It depends on chunk 2007's not-yet-landed headers
  (CTCTrophy.h, CTrophyDef.h FABLE_RETAIL_CTROPHYDEF, CTCCarryable.h NONCONST_CARRIER, CThingAICreature.h STATEGROUPENABLED) -- land with or
  after chunk 2007 -- plus this chunk's new CCreatureAction_SheatheItemToInventory.h and the CWorld / CTCHeroStats / CTCCreatureOpinionOfHero /
  NGameText opt-ins.
- 0x00616580 and 0x00B2EB20 are byte-identical set<long> finds; only 0x00B2EB20 is bound (const member template). Decide which symbol
  0x00616580 is before landing any row that calls it (parked 0x006D1250 binds the const template through the shared set model).
- NAME_FIXES.tsv: 0x00682310 (bound `?Obj_Release@@YIXPAX@Z`) and 0x009C7BA0 (bound as a cdecl varargs ConstructStringInternal).
- Rows not attempted: 0x00949AB0, 0x006414A0 (outlines in NOTES.md); 0x006D1250 parked after one pass only (budget), not three.
