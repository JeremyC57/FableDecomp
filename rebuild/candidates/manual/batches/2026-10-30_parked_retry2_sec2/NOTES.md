# Retry wave 2, section 2 (0x00647575-0x007805D0, 353 parked rows) -- notes

## Outcome
- Rows touched: 353 (STATUS.tsv lists every one).
- Matched: **6**, all RELOCATION_MATCH plus PASS in a dry vland (`LANDVERIFY_WORK=lv`, no RELOC_IDENTITY, no RELOC_CONFLICT):
  0064a060, 00717950, 00752120, 00752510, 0075a530, 0076e2f0.
- Improved, in differ/: **4**.
  - 00698a30: RELOCATION_MATCH once engine/CDefPointer.h has the ENullDef ctor. 00698a30.verify.cpp is the proof
    probe with the header text inlined; do not land it.
  - 006bb390: 259v265.
  - 006ca380: 115v115, 6 difflines, register-only residue.
  - 00752e00: 286v296. The arithmetic is now exact; only the frame residue is left.
- Held: 337. Each was hand-worked or run through the flag sweep plus LowerBound injection, with the residue recorded.
- No oracle: 6 (006e77f0, 006bd900, 00702d10, 00725f10, 00739030, 00772490).

## Wins per lever
| lever | matched | improved |
|---|---|---|
| Same-TU `__declspec(noinline)` real callee body (mod-ref): const `CVectorMap::LowerBound` 0x004365B0, `GetSubDef<CEntitySoundDef>` 0x0065F604 | 2 (0064a060, 0076e2f0) | 2 (00698a30 pending header, 006ca380) |
| PDB inline getters with retail parenthesisation (`GetHeadTurnRangeXY` = `0.5f * (range * 1/360)`, `GetHeadAngularVelocityMaxXY`) | 2 (00752120 new; 00752510 re-expressed with the same getter) | 1 (00752e00) |
| Lander flag sweep (5 flag sets x 5 optimize pragmas) | 2 (00717950 `/Oa`, 0075a530 `/EHsc`) | 0 |
| Named fraction locals (stop constant folding) | 0 (00752510's first fix, now replaced by the getter) | 1 (006bb390) |

00752510 is counted once, under the getter lever. The flag sweep ran over all 352 oracle rows; only the two wins above
came out of it. The second pass (171 rows) found nothing new. LowerBound injection covered 136 rows (CInterfaceMap
form) and 69 rows (template form). Among the remaining rows it gave one improvement, 006ca380, and no new matches.

## Names
- Respelled to the PDB in the matched sources, with the wrong landed spellings in NAME_FIXES.tsv:
  - 006d6510: CTCHeroExperience::GetStatExperience.
  - 00752120: CTCLook::ChangeHeadAngleXY, private AAE.
  - 00751820: CTCLook::GetHeadSettingsSlowInFastOut, private const ABE, float&.
  - 00686A80: CTCBase::PeekWorld.
  - 00A24250: CMeshDataBank::PeekPhysicsMeshInfo.
  - 004BBFF0: NGraphicAppearance::CTCGraphicAppearance::GetScale.
- Also respelled: 00717950 is NTCScriptedControl::CActionTalkBase's protected ctor `(CThing&, bool, bool, bool)`, and
  0075a530 uses CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound plus the PDB class keys.
- Inline PDB getters, which emit no body: CTCLook::GetHeadTurnRangeXY (ABEMXZ) and GetHeadAngularVelocityMaxXY (AAEMXZ).
- Descriptive names (no PDB entry):
  - `GetMatrixFromRHSet` 0x00A358F0, in 0075a530.
  - `NTCScriptedControl::CActionTalkBase::DefaultCameraTagsNeedInitialising`, the static bool at 0x01380FB9, in 00717950.
- No longer needed: the descriptive `g_zeroTime` double that 00717950 used for 0x0122ED70. A `0.0` literal now reaches
  the merged `__real@0000000000000000` there, so no name is invented. That address already carries three conflicting
  bindings: g_clearDouble, InitialPreloadTime and g_zeroPitch.

## Grade-cap note
00752120 (18 grades) and 00752e00 (9) went over the 3-grade cap, because each sweep kept narrowing the residue.
Everything else stayed within the cap. The automated flag sweep is counted as the lander's own check, not as grades.
