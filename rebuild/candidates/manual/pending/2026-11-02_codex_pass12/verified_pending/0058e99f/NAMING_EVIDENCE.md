# 0058e99f: CTCInventory::OpenInventoryShortcut(long)

The exact symbol is `?OpenInventoryShortcut@CTCInventory@@QAEXJ@Z`: public non-const member, long argument, void return, class key V. Ego_r's name set records it at donor 005f7d45, and FableWin's original procedure records retain `screen` as long, `pentry_sub` as NUISystem::CComponent*, and `pentry_sub_real` as NUISystem::CMenuEntry*. The native nine-way shortcut dispatch and nine menu-entry literals give independent semantic evidence. Native callers listed in ROW.md pass shortcut numbers; the adjacent ContructItemLists implementation is the same inventory translation unit. The selected VC7.1 object symbol table contains the exact Leaf once.

## Correct direct callees

| Native address | Exact symbol | Independent evidence beyond PDB spelling |
|---|---|---|
| 00587a41 | ?DeleteMenuComponents@CTCInventory@@QAEXXZ | Native caller always invokes it before gating/opening; established bound implementation and donor sequence before menu constructors. |
| 00410de0 | ?test@?$bitset@$0BBC@@std@@QBE_NI@Z | Real STLport 4.6 bitset<274>::test; CThing UsingComponents starts at +0x20, and native calls test for interface values111 and20. Existing correct bound spelling agrees. |
| 0058807d | ?OpenPCInventoryMenu@CTCInventory@@QAEXXZ | Donor sequence follows OpenPCSkillsMenu; native shortcut0,1,2 lead here and then select weapons/clothing/items. |
| 00589a30 | ?OpenPCMapMenu@CTCInventory@@QAEXXZ | Native shortcut4 selects map and no entry; PDB donor order after inventory menu. |
| 00587bff | ?OpenPCSkillsMenu@CTCInventory@@QAEXXZ | Native callee stores menu1 and contains PC_SKILLS_LIST. Exact donor005f242c follows DeleteMenuComponents/scoreboard and precedes inventory menu. |
| 0058a763 | ?OpenPCStatusMenu@CTCInventory@@QAEXXZ | Native callee stores menu7 and contains PC_STATUS_LIST at0125119c. PDB donor005f6d96; shortcuts3,6,7 then select quests/personality/logbook. The assigned manifest's OpenPCSkillsMenu hint is wrong. |
| 0099ebf0 | ??0CCharString@@QAE@PBDJ@Z | Original constructor signature and existing correct binding; native pushes literal and default length -1 directly into the by-value outgoing argument object. |

No direct callee is assigned to two addresses in this source. Existing aliases are reported in NAME_FIXES.tsv, not copied into the source.

## Virtual calls and layouts

The complete existing UI class hierarchy is used. Original CClickable TPI introduces OnLeftClicked at byte584 and **OnLeftUnclicked at byte588 (0x24c)**. Native CMenuEntry primary vtable0124662c slot147 points0053d600; this confirms the click-release method independently of its name. Component Update(float) is slot1; FindChild(CCharString) is slot3. Their original argument types are kept.

CTCInventory has genuine NInventory::CTCInventoryBase and NHeroInformationScreens::CBase bases. Original field names are retained through the complete0x228 native size; Components is the real STLport vector at+0xc, and PTop_Level_List is+0x218. The original PDB confirms both fields; retail loads +0xc/+0x10 each loop and tests+0x218. New opt-in header defaults preserve the supplied default text.

## Extent, operand and test audit

The full assigned1498-byte extent is correct: machine code through ret4 is followed by two nine-entry jump tables, and the next function begins at0058ef79. Both tables belong to this function and are retained. RELOCATIONS.json audits all45 operands:16 direct calls,9 exact literals,2 table pointers and18 case entries. Every table/case label offset is compared against its native VA; no padding or invented bytes are used.

The self-contained behavior fixture embeds the exact production method body. It checks768 combinations of shortcut, component availability, top-level presence, matching-child position and current-quests presence, plus an empty-component case. External UI methods are instrumented; the function's own gating, switches, search loops, click-release calls and all ten-update loops are exercised. Both host C++03/no-RTTI and real VC7.1/no-RTTI builds pass.

Main compliance correction: current selected source enables FABLE_RETAIL_UI_ORDINARY_INLINE. Any inherited forced annotations in the relevant helper closure use ordinary inline for this feature; their default branches are retained. Main recompiled the target exactly, checked every operand unchanged, and reran both current source-bound fixtures.

Main ordinary-header review uses the original C3DVector model without unsupported stronger attributes and, where included, the original CDefPointer class with private Object and direct original operator bodies. Both original type sets corroborate the corrected declarations; no invented Assign member remains in that opt-in. Defaults are preserved. Target bytes/operand identities and current-pair tests remain independently verified.

Main ordinary-header review uses the original C3DVector model without unsupported stronger attributes and, where included, the original CDefPointer class with private Object and direct original operator bodies. Both original type sets corroborate the corrected declarations; no invented Assign member remains in that opt-in. Defaults are preserved. Target bytes/operand identities and current-pair tests remain independently verified.

## CResource original field access correction

Both original TPI records prove CResource::ResourceList,PrevResource,NextResource,ResourceSize,LastUsedFrame are private at offsets8,12,16,20,24. The opt-in shared header restores that access, keeping every original field and the entire default branch unchanged. Source includes FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS. Actual VC7.1 preprocessing confirms those fields and the earlier private IVRefCount and private CDefPointer::Object declarations. See RESOURCE_ACTIVE_DECLARATIONS.json and the vendor original signature matrix at workers/vendor/BASE_DECLARATION_AUDIT/REPORT.json. Fresh byte and dual-compiler fixture checks pass; independent main acceptance is pending.

Main ordinary-header review uses the original C3DVector model without unsupported stronger attributes and, where included, the original CDefPointer class with private Object and direct original operator bodies. Both original type sets corroborate the corrected declarations; no invented Assign member remains in that opt-in. Defaults are preserved. Target bytes/operand identities and current-pair tests remain independently verified.
