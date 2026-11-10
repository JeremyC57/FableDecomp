# agentNF3 notes: what is left and why

## Done
- Verified all 68 queue spellings against two sources each: the Ego_r and FableWin PDB name sets; the Fable Anniversary 360 publics for the LH audio module; the reloc bindings of landed callers; the STLport 4.6 source shape. Verdicts are in `STATUS.tsv`.
- Every re-authored row is in `rows/<addr>.cpp`. Each grades MATCH or RELOCATION_MATCH with `FABLE_HDR_OVERLAY=$S/w22/agentNF3/hdr`. The full regrade log is `work/final.tsv` (columns: grade, BAD-binding count, HARD count, test results, self symbol).
- New or changed tests are in `rows/<addr>_test.cpp`. All of them pass under g++ -std=c++11 (64-bit) and VC7.1 /Od. Rows without a test file keep their landed test unchanged; those tests are self-contained models and do not link the row.
- Rows whose own name changed are listed in `RENAMES.tsv`.

## Left (not re-authored), and why
These binder rows still bind a wrong spelling of a queued address. All of them are stand-in or local-model rows where a
one-line respell does not keep the bytes. Each needs a per-row rewrite onto the shared headers, which is more than this
wave could do.

| Target | Rows left | Why |
|---|---|---|
| 0x00A01C10 `??1CBaseIntelligentPointer@@UAE@XZ` | ~90 | The rows model the pointer with an explicit `void*` vptr member and a non-virtual destructor. Making it virtual shifts the layout, and in STLport `_Destroy` it turns the call into a virtual dispatch (0/31 kept in the batch). They need engine/CIntelligentPointer.h. |
| 0x00A01B10 `??0CBaseIntelligentPointer@@QAE@XZ` | 33 | Same family. Many use invented classes or namespaces (`CBaseIntelligentPointerCtor`, `FableCons_004aa030`, `NRemap006a23e0`). |
| 0x0073A6E0 `?GetCurrentMode@CTCCreatureModeManager@@QBEAAVCModeBase@1@XZ` | 57 | The retail return type is the nested `CTCCreatureModeManager::CModeBase&`. The rows declare a global `CModeBase` / `CMode` / `CCreatureModeBase`, which has to be moved inside the manager class row by row. |
| 0x00430370 `??0CParentDefClassBase@@QAE@XZ` | 26 | 25 rows bind `??0CBase_00430370`, an address-suffixed stand-in. They are def constructors and need the shared CParentDefClassBase.h model. |
| 0x00430300 `??1CParentDefClassBase@@UAE@XZ` | 12 | Def destructors and `??_G` stand-ins (`realdtor`, `dtor@Pair`, `CPatchStripBase`, `CDefinitionBaseModel`). Their classes are unidentified and their catalog names are propagated garbage. The pattern for doing them properly is rows/00430300.cpp, 00478706.cpp and 00477460.cpp. |
| 0x004C9D60 `?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z` | 21 | Eight different wrong signatures (`PAX`, `JJ`, typed returns `CTCHitOnCollision*`, `void` returns, `CThingNative`, an invented namespace). Each needs its call site retyped. |
| 0x00993EB0 `?WriteSLONG@CDataOutputStream@@QAEXJ@Z` | 32 | The batch fixed 21. Skipped: 24 rows that do not name 0x00993EB0 next to the declaration (`Write(void*)`, `ShadowSink::Push`, `HandlerB::Do` stand-ins). Reverted: 8 rows where the rename did not compile or changed bytes. |
| 0x006A1F80 `?PutThingInCarrySlot@CTCCarrying@@QAEXAAVCThing@@J_N@Z` | 8 | The batch fixed 12. Left: 5 with other declaration shapes (`AddThing`, ...) and 3 that failed to compile. |
| 0x00993CA0 `?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z` | 3 | 0x007565E0 / 0x007ADBF0 sit in invented namespaces `X_007565e0` / `X_007adbf0` with stand-in `CCharString` and `map`, and the default engine/CMemoryDataInputStream.h does not derive from CDataInputStream. 0x006C8780 is mostly in the intelligent-pointer family. |
| 0x00703210 `?Create@CThingObject@@SI...` | 3 (0x0078B070, 0x00848500, 0x008C64F0) | They also bind other big-family targets. 0x00848500 was respelled for PutThingInCarrySlot; Create is still the old spelling there. |

No BUSY address was touched.

## Row debt kept, and noted in the rows
- 0x006B36xx: a local `class CEnvironment` model (no complete shared header).
- 0x00478722 / 0x00478706: a local CAbilityTrackDef and STLport `_Vector_base` model. The shared engine/CAbilityTrackDef.h pulls the inline `~vector`, but retail keeps `~_Vector_base` out of line (0x00478743).
- 0x005D8D50: local CThing / CTCInventoryItem / NInventory::CTCInventoryBase models. CThing.h has no SendMessage, and the `CThing&` AddItemToInventory overload is protected in NInventory_CTCInventoryBase.h.
- 0x00667B30: `SAnimationSetEntry` stands in for `std::pair<unsigned long, CAnimationEntry>` (reached through a pointer cast).
- 0x008916A0: invented `CGSISheatheHeroWeapons_*` classes.
- 0x00733100: self is the free function `CTCCombat_IsUsingUnarmedCombatIdle`.
- 0x00BACC50: self is `CEnginePrimitiveOcclusionGroupManager_Methods::RenderPrimitive`.
- 0x0093DCD0: self is `CCombatStateMethods::IsEnemyCreatureInCombatPosition`.

All of these were pre-existing. Only their callee spellings were fixed.

- 0x004C7A70 is still spelled `GetDisplayEngine` (made `const`) in the DrawIsDrawable rows. It has several landed spellings (`PeekComponent@CThing@@QBEABVCMainGameComponent@@XZ`, ...). It is not a queue entry, so it was left.

## Descriptive names (not found in any PDB or other source)
- `CASoundManager::GetPSoundBank(const CLipSyncDataBank&) const` 0x00991690: no Anniversary counterpart. It is spelled as a third GetPSoundBank overload.
- The `CASoundManager::CRegisteredBank` member names `PSoundBank`, `PLipSyncBank` and `SymbolName`. The class name itself is from the Anniversary publics.
- `ESubPrimitiveType` / `EDamageAttribute` enumerators are given by value where the header has none.

## Respell waves for the coordinator
- CRegisteredBank: 0x00992510 / 0x00992630 / 0x00993030 are bound by ~20 rows under the invented `CSoundBankRecord` / `FableHeap_00992b80` spellings (`NAME_FIXES.tsv`). rows/009918f0.cpp keeps CSoundBankRecord until that wave lands.
- `NAME_FIXES.tsv` also lists the landed self rows whose spelling is now proven wrong: the two-argument vector resizes, `_Vector_base` 0x00478743, 0x00445940, 0x005F4B30, 0x005BF2ED and 0x00868860.

## Not done (optional)
- Moving 0x00472A8D, 0x0067D6B0, 0x00632E00 and 0x007D2740 onto the shared TransferVectorOfSubComponents body. Their WriteSLONG binding was fixed here (batch); the move itself was not attempted.
