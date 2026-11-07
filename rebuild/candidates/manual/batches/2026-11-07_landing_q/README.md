# Landing 2026-11-07q: expert retry chunk 840 (Fable model; 4 of 10 near-miss parks landed, 6 parked)

| Row | Grade | Note |
|---|---|---|
| 0x0052EA30 NUISystem::CComponent::ChangePositionDelta | **RELOCATION_MATCH + PASS, landed** | 27 attempts; pass `d + Position` (operator+ temporary) to ChangePosition; header opt-in `FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR` (ConvertVector / UnConvertVector) copied in (additive; its landed includer 0x0041CF47 rechecked) |
| 0x009E5C90 CSymbolMap::LoadFromFile | **RELOCATION_MATCH + PASS, landed** | 13 attempts; the whole-file read as a plain inline helper taking the file by reference gives retail's `[eax+esi]` SIB order |
| 0x007BC8FC NThingShot::CTCShotDefinitionTearInGround::InitMovement | **RELOCATION_MATCH + PASS, landed** | 20 attempts; a named `const float speed` flips the IR-numbering tie at the AddTC call |
| 0x00773950 CTCShop::IsOpeningHours | **RELOCATION_MATCH + PASS, landed** | 24 attempts; `if (alwaysOpen) return true;` ahead of the villager block |
| 0x0070C4E0, 0x00C52BB0, 0x0058301A, 0x0073ACC0, 0x0047ED76, 0x007534B0 | DIFFER, 2-4 lines each (27-52 attempts) | constant-return `xor` scheduling at a join block (RTM and QFE-4035 both), K5 compare constant, home-store order, push / fsubp order, copy interleave |

- **Names:** 0x0070C4E0 GetFirstCombinationAnimFromType and 0x00C52BB0 CCpuInfo::GetAMDModelName relabelled (`labels_landing_2026-11-07q.tsv`);
  one new queue line (0x007BEDF0 GetCreatureFireAtPos); its other 15 NAME_FIXES lines were already queued. Unresolved: 0x00A14440 Normalise v
  AccurateNormalise (both bound in landed rows).
