# Landing nf261 (Account A, 2026-11-22): seven CCombatActionBase slot rows on PDB members, the pooled 0.5f on its compiler spelling (9 rows); 0x00692890 HELD

Agent NF261 (q12740, q12741, q12742 done; q12743 / q12744 / q12659 open). Hand-back: `pending/2026-11-22_nf261_handback/` (evidence/ = symdiff,
vtmap, xref, pk, link, census, locks).
- **q12740:** 0x0062EA40 IsToUseRotationInAnimation, 0x0062EA50 IsToAlignWithGroundNormal, 0x0062EA60 IsToFaceTarget, 0x0062EA70
  IsToFaceTargetAtStart, 0x0062EA80 GetActionMovementVector (`UBE?AVC3DVector@@XZ`), 0x0062EAA0 IsToPerformActionOnStrike, 0x0062EAB0
  IsToHitProneThings onto their `CCombatActionBase` `UBE` spellings (FableWin.pdb + retail vftable 0x01259E44 slots 71-77 read directly +
  the bodies); 0x0062EAC0's define block; all MATCH on the first form. vtmap NOLEAF -> SAME (0x00955620 slots 71-73 / 75-77; slot 74,
  overridden there, on 0x008B83C0). The HEAD link left 13 references to the seven symbols unresolved; now 0. New tests link the row objects.
- **q12742 (0x0122F59C, the pooled 0.5f):** 0x00661F90 `g_scale` -> the literal `0.5f`; 0x00B0A750 `g_cosTableScale` was a crossed binding (VC7.1
  put the extern on the 0.5f site; the name meant the pooled 1024.0f at 0x01230010): form 1 `x * 0.5f * 1024.0f` folded to 512.0f (DIFFER
  444v456); form 2, two statements (the landed GFSmoothFadeValue 0x00AF9000 spelling), RELOCATION_MATCH. Every 0x0122F59C site binds
  `__real@3f000000`, both 0x01230010 sites `__real@44800000` (xref); census over 498 reader rows: only these two names ever sat on
  0x0122F59C; both table lines deleted.
- Link (new objects + HEAD controls): 0 duplicates (positive control LNK4006). pk.py: every new non-leaf COMDAT NO-OWNER.
- **Recheck** (`logs/recheck.log`, landed tree): the 9 changed rows + 0x0062EAC0, 0x00955620, 0x008B83C0, 0x00AF9000 (13 rows): 7 MATCH + 6
  RELOCATION_MATCH, all PASS; 0 RELOC_CONFLICT.

## Coordinator rulings on NF261's questions
1. **0x00692890 (q12741, `?AllowsCombinationAnims@CCreatureActionBase@@MBE_NXZ`, RELOCATION_MATCH): held.** 0x006F5C10's local-model
   CCombatAction_* tables bind that symbol at slot 66, where retail has 0x0062EA10 (CCombatActionBase::AllowsCombinationAnims); at HEAD
   nothing defines it, so those slots are unresolved; once 0x00692890 defines it they would resolve, silently, to the wrong body. It lands
   as one unit with the 0x006F5C10 respelling and the 0x0062EA10 self row (`?AllowsCombinationAnims@CCombatActionBase@@UBE_NXZ`, FableWin
   0x01D916B0): queued. The draft is in the hand-back rows/.
2. 0x00B0A750 as a crossed-binding repair: accepted.  3. FableWin + the retail slot + the body for the seven q12740 spellings: accepted (as
   queued).  4. NAME_FIXES queued (0x0062EA10; 0x00693B30's slot 65-67 `UBE` v the PDBs' `MBE`; the CastAssassinRush family 0x008CEAE0 /
   0x008CE040 / 0x008CE100; TableCos01 = the expanded GFSmoothFadeValue; pool-literal candidates 0x01266300 (0.001f) / 0x01230A5C (0.01f)).
5. q12743, q12744, q12659 to the next agent, in that order.
Rule slips (GOTCHAS / EXTRA_RULES read with long lines cut, EXTRA_RULES lines 251-261 not displayed; the first link without the lane env,
rerun; the controls' HEAD tests not prechecked): noted.
