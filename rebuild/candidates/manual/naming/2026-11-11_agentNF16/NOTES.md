# agentNF16 notes: queue land1011bt/bu/bv/bw and the Part 2 LowerBound stand-ins

## Hand-back contents
- **335 changed rows** in `rows/`. No test changed, so there are no `_test.cpp` files.
- **Grades:** every row grades MATCH (1, 00487510) or RELOCATION_MATCH (334).
- **VC7.1 /Od test:** prints the catalog PassPattern for every row.
- **g++ -std=c++11:** all 81 g++ failures (listed below) are PREFAILs. The landed test, built alone without the overlay, already
  fails under g++ (`work/gxxpre.sh`). In 80 of the 81 the cause is `__fastcall`, a rebuild_abi ptr32 assert or a static_assert.
  008d0ba0 builds but prints FAIL, as landed.
- **Grade budget:** at most 4 grades per row (`work/gradecount.txt`).
- **Untouched:** no BUSY.txt address. 00c4f600 (bt/bv) and 00919240 are busy, so their entries are untouched.
- **Overlay:** one header overlay (`hdr/engine/CCreatureActionBase.h`, see `hdr/README.md`).

## Families done (per target counts in STATUS.tsv)
| Family | Target(s) | Rows |
|---|---|---|
| MaxLayers | 013ba848 | 86 (all BAD binders) |
| CAIStateGroupBase::GetWorld | 008fcfc0 | 55 (all) |
| CAnimInit::Init | 0070c050 | 22 |
| CWorld::GetPlayer | 0049d850 | 19 (all) |
| GetEffectFromString | 00669d30 | 19 |
| CDefStringTable::GetString | 009d49b0 | 18 |
| LowerBound / const LowerBound (Part 2) | 0040f020 / 004365b0 | 15 / 16 |
| GetPerceivedThing | 007446a0 | 13 |
| EPlayerMode respells | 00634160 / 00634190 / 006345c0 | 13 |
| PClass\<CPlayerGui\> | 013b8790 | 12 |
| IS_THING_ALIVE\<CThing*\> | 0040dce0 | 9 |
| PeekPDeedEffectsDef | 006c5630 | 7 |
| GetJoystickDeviceNumber | 00487510 | 6 (incl. own) |
| ConstantFPS | 01375550 | 5 |
| IsProjectileWeaponLoaded | 005da980 | 5 |
| Single fixes | 00449970, 004af7a0, 004fc600, 005e4740, 0065be6c, 00681a70, 00686a70, 00856b10, 0099b7d0, 00a01b90 | 1–3 each |

All 13 bw claims are handled: 0041d8e7, 008cb780, 00692980 (+0084a7e0), 005bf654, 00898db0, 007edee0 (+007edfb0) and
006ae1c0 (+006a9ef0) are done. 00842150 and 00415d70/dd0 are rejected. The rest are below.

## Part 2 (0x0040F020 LowerBound stand-ins)
- **Converted:**
  - lb2 batch: 005e4860, 00766550, plus the const-only rows.
  - `work/lb3.py`: lb2 generalised to keep extra inline members (Find/find, end, LowerBoundInline). Applied to 005bf654,
    004c9840, 006594bb, 0065a1fd, 00747da0, 00898c40 and 00929b10.
  - Hand edits:
    - 00693140 and 006cf3b0: value_type → std::pair, plus the third template parameter.
    - 0069e980 and 006d53c0: the CTCMap stand-ins. The invented LowerBoundConst/LowerBound2 become the real const
      overload.
    - 006e7820 and 006e7920: the nested CThing::CInterfaceMap_Retail is moved out to namespace scope.
- **Const LowerBound rows:** 00583a92, 005907ab, 005a8a5d, 005cdccb, 0064a1ae, 0064be45, 0064db2b, 0066ba80, 008a8f20,
  008af5f0 and 00956b00 now bind 0x004365B0 through a `const CInterfaceMap*`.
- **Left (35 BAD binders of 0x0040F020 still, STATUS.tsv lists them):**
  - The 7 non-member-helper rows (per the brief).
  - 004c9d60: a full CTCEntryVector/sort/_M_insert_overflow model with a non-template CKeyPairCompareLess struct.
  - 00783380 / 0089bb30 / 007e5230: the STCEntry `const unsigned long&` key; a different map model.
  - The 0089xxxx CGameScriptInterface one-line models (00895ed0, 008986d0, 00898840/940/b30/bb0, 0089a270, 0089b390,
    0089c4c0, 0089dff0, 0089e4c0, 0089ec60, 008a7980): own-name free stand-ins, part of the 0089c560 reshape.
  - 00899d50, 00899db0, 0089a070, 0089a0e0: converted and graded RELOCATION_MATCH, but their VC71 tests mock the old
    LowerBound and link the row object (LINK_FAIL). Reverted; a test respell is needed with the row.
  - 00927c80: converted but DIFFER(287v287) (a register swap: `mov esi,[eax+18h]` vs retail `mov eax,..; lea esi,[eax+44h]`).
    Reverted.
  - 008385b0, 00857b70, 00859d80, 00915190, 00935160, 009547d0, 00954e30, 00972490, 008af850: bespoke shapes, not reached.

## Reverted (in work/reverted/, not handed back)
- **ConstantFPS (01375550):** 0057cb7e, 006624a0, 006666a0, 006cec90, 006d8580, 0075aec0, 008c6db0. The inline
  `GetConstantFPS()` makes VC7.1 store the value to a temp, giving `fild [esp+4]` where retail has `fild [0x01375550]`, so all
  seven DIFFER. They need a direct read of the protected static. Retail's own callers may have had friend/member access;
  this is unproven.
- **008c9fe0** (GetEffectFromString): the row defines the callee body inline in its TU; the const respell changes codegen
  (DIFFER 302v296).
- **004874d0** (own IsFirstPersonTargeting): see LESSONS. Claim verified, row left on its extern "C" stand-in.
- **007502d0:** no net change.

## Rows handed back with another target still BAD (main fix OK; other binders are outside this pass)
- 005185f0 0049c370
- 00518900 0049c370
- 0057d9fd 005812df
- 0057daf5 005812df
- 005907ab 0040dce0
- 005b37f7 004c9d60
- 005cd767 004c9d60
- 005e4430 004b8c00
- 006295c0 0051ed80
- 0064a1ae 0076d2f0
- 0064db2b 005f4b30 005f9f14
- 006594bb 00410de0
- 006ab300 0122d70e
- 006d0180 00682140
- 006e7820 006af1c0
- 00745730 0085f0c0
- 00746560 0049c370
- 007b0950 00687540
- 007be0f9 004c9d60
- 007e5230 0040f020
- 007e5b90 006644f0
- 007ed060 006644f0 00842150
- 0080a550 0049c370
- 00897630 004365b0
- 008d02c0 0122d70e
- 008d70a0 0070c4e0
- 008d90b0 00a01b50
- 008e0060 0062e900
- 008e7040 00415dd0 004365b0 006ef790
- 00905d10 0049c370
- 00918880 006bb390
- 0092c4e0 006bb390
- 0092c520 006bb390
- 0092c560 006bb390
- 00934920 00934e50
- 009352c0 00a01b90
- 00941550 00a01b50
- 00942390 006a76a0
- 0094fe30 01375550
- 00956b00 00a01b10 00a01b50 00a01b90 00a01c10
- 00962a90 00a01b50

## Queue entries not done (STATUS.tsv `not-done`) and why
- **Mega families:** CCharString (0099eae0/ebf0/ec30/efb0), CWideString (0099aed0/b510), CBaseIntelligentPointer
  (00a01b10/b50/c10), 0122d70e, 004c73d0.
  - Thousands of binder sites, mostly inside row-local class models that need the shared header. Not respell-sized.
- **00404500 CheckCRC (94):** the binders are misidentified CPersistContext::Transfer<T> rows, so those rows need renaming,
  not respelling.
- **00993eb0 / 00993e30:** CDataOutputStreamWriter adapter rows.
- **013b86a0 PMainGameComponent (46):** variants that model CMainGameComponent locally.
- **013b8790:** variants (g_pGameUI-style, and the local CCountedPointer rows 004a0220, 004b3a20, 00636e90).
- **006644f0 SetCurrentAction(const CCreatureActionBase&) (49):** needs a rebase on engine/CCreatureActionBase.h plus
  CThingCreatureBase.
- **0062e900 ~CCombatActionBase (35):** the invented CBase_0062e900 classes.
- **Filter operator() rows** (006a76a0 / 006a76b0 / 006af1c0): pointer → reference parameter reshapes.
- **IS_THING_ALIVE CBaseObject-variant rows:** e.g. 005907ab.
- **Coda SDK names** (00c3xxxx, 0139c988, 0143bb48/58/0143bc28, 012b5ab0): the only source is the Codex "original
  GameCODA SDK". The owner rule forbids leaked sources, so there is no second source. Flagged, not respelled.
- **Remaining smaller targets:** not reached in this pass (time). The binder lists are in STATUS.tsv `left_rows`.
- **bw 0089c560 EntityPostOpinionDeedToAll:** needs the ScriptMethods / CThingNative reshape.
- **bw 005ba7f9 std::find:** an NF15 failed attempt; needs an STLport vector model.
- **bw 008e5670:** verified. 0x008E5F00 is left unnamed (no xref or vtable). Binder 00725c40 needs a CTCPhysicsStandard
  base-class reshape.
- **bw 00487dd0 / 006c5920:** binders not reached.
- **0122ae6a (bt):** the atexit-thunk family, not reached.

## PREFAIL rows (the landed g++ test already fails)
- 00487510 005185f0 00518900 0057d9fd 0057daf5 005bf654 005cdccb 00629c40 00635b30 006378e0 0064a1ae 0064be45 0064fc23 0068fb90 00692980 0069a920 0069e980 006ce300 006d0180 00730c00 00730dd0 00733100 00745730 00746560 0077adf0 007a3ca0 007a9300 007a9580 007b2fd0 007cc2b0 007f0b10 007f4020 007f42a0 007f4500 007f70d0 007f7340 007f9c00 007fdb10 00809a70 0080a550 0080e670 0080ecc0 008139c0 00843fc0 0084a7e0 0084f850 00851490 008786e0 008a8f20 008c0ea0 008c4170 008c8180 008c8380 008ce900 008d0ba0 008dacb0 008dc900 008e3c50 008e4100 008e60d0 008e6690 008e6bb0 008e6c90 008e7a00 00905d10 0090f1c0 0091ba40 0091c3d0 00922750 00937420 0093e090 00947e20 0094bb20 0094fe30 00956460 0095c1f0 0095db90 00969fb0 0096a7a0 0096ff10 00970590
