# agent867 header overlay (`FABLE_HDR_OVERLAY=$S/w22/agent867/hdr`)

Every file here is either a copy of the repo header with an additive `#ifdef FABLE_RETAIL_*` opt-in (the default path is
unchanged: `diff rebuild/include/<path> hdr/<path>` shows only the guarded block and, where noted, one declaration added to an
existing retail block) or a new shared header. "Users" names the chunk-867 rows that define the opt-in (M = matched, P = parked).

## Changed headers

| header | opt-in / change | what it adds | users |
|---|---|---|---|
| engine/CRGBFloatColour.h | FABLE_RETAIL_CRGBFLOATCOLOUR_TO_CRGBCOLOUR | `__forceinline operator CRGBColour() const` = `CRGBColour((uchar)(R*255),(uchar)(G*255),(uchar)(B*255),(uchar)(A*255))` (FableWin ??BCRGBFloatColour@@QBE?AVCRGBColour@@XZ, inline in retail: R,G,B,A in member order; a plain inline is called out of line under /Os, grade v3 of 00427216) | 00427216 M |
| engine/CDefString.h | FABLE_RETAIL_CDEFSTRING_EQUALS | `bool operator==(const char*) const;` 0x005B3440 (declaration) | 0064bfba P |
| engine/CDefinitionManager.h | FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_EXPRESSIONDEF | `GetDef(long, CDefPointer<CExpressionDef>*) const` 0x005F81BE (declaration) | 005f59c7 P |
| engine/CDoorDef.h | FABLE_RETAIL_CDOORDEF | the class on its RTTI bases (CSubDefClassBase), opaque members, sizeof 0x3C kept; so CDefPointer<const CDoorDef> converts (GetSubDef<CDoorDef> 0x00655D28). Needs DEF_CLASS_BASE | 0064bfba P |
| engine/CExpressionSubDef.h | FABLE_RETAIL_CEXPRESSIONSUBDEF | the class on CSubDefClassBase (Ego_r members at retail offsets) | 005f59c7 P |
| engine/CGraphicDataBank.h | FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE | 23 placeholder vslots + `GetGraphicMaxWidth` (slot 23, 0x00A014D0) / `GetGraphicMaxHeight` (slot 24, 0x00A01540), sizeof 0x30C kept | 0064bfba P |
| engine/CHairCardDef.h | FABLE_RETAIL_CHAIRCARDDEF | the class on CSubDefClassBase, opaque, sizeof kept (CDefPointer<const CHairCardDef> conversion) | 005af64e M |
| engine/CHeroTitleDef.h | FABLE_RETAIL_CHEROTITLEDEF | same shape for the hero-title sub-def (GetSubDef<CHeroTitleDef> 0x005B1E5A) | 005af64e M |
| engine/CIVCountedPointer.h | FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT | destructor that drops the count in place (the shape of the text-entry handle release in 0x005A8C66) | 005a8c66 M |
| engine/CIntelligentPointer.h | FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE / _ASSIGN_OUTOFLINE / _COMPARE | the (T*) ctor and operator= as declarations (COMDATs 0x004A96F0 / 0x004A9760 for CThing); `template<class T> bool __fastcall operator!=(const CBaseObject*, const CIntelligentPointer<T>&)` 0x005CBDD2 | 0064bfba P |
| engine/CPlayer.h | (inside the existing FABLE_RETAIL_PLAYER block) | `bool HasMode(EPlayerMode) const;` 0x00634160 next to GetCurrentMode | 0064bfba P |
| engine/CPlayerGuiDef.h | FABLE_RETAIL_CPLAYERGUIDEF_TARGETING | `C2DVector TargetingBorderLeftPos / RightPos` in place of the `_X/_Y` floats; the seven `Highlight*` tables as `std::vector<CRGBColour/float/long>` (same offsets); matching static asserts | 0064bfba P |
| engine/CTCActionUseScriptedHook.h | FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_ISUSABLE | `bool IsUsable() const { return Usable; }` (FableWin ?IsUsable@CTCActionUseScriptedHook@@QBE_NXZ) | 0064bfba P |
| engine/CTCHero.h | FABLE_RETAIL_CTCHERO_TITLE | the hero-title setter the title purchase calls | 005af64e M |
| engine/CTCHeroStats.h | FABLE_RETAIL_CTCHEROSTATS_GETMONEY / _ADDMONEY | GetMoney / AddMoney declarations (0x0057B327) | 005a8c66 M, 005af64e M, 005b01fe P |
| engine/CTCInventory.h | FABLE_RETAIL_CTCINVENTORY_PCMENUS / _QUICKACCESS | the PC menu members / quick-access list the PC inventory rows read | 00590f0b P, 005f59c7 P |
| engine/CTCInventoryAbilities.h | FABLE_RETAIL_CTCINVENTORYABILITIES_EXPRESSIONS | the expressions-list members and helpers | 005f59c7 P |
| engine/CTCInventoryItem.h | FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS / _CATEGORY / _KINDS | GetTattooCardDef / GetHairCardDef, GetInventoryType (0x005D99D0), IsWeapon and the kind predicates | 00590f0b P, 005a8c66 M, 005af64e M, 005b01fe P |
| engine/CTCInventoryStats.h | (default class) | `#include "engine/EHeroAbility.h"` + `void AddHeroAbility(NUISystem::CComponent*, EHeroAbility, float&);` (the row's own out-of-line helper, 0x005D6748 calls it 18 times) | 005d6748 M |
| engine/CTCInventoryTrade.h | FABLE_RETAIL_CTCINVENTORYTRADE (+ _CTCINVENTORYBASE_VTABLE) | the retail class on NInventory::CTCInventoryBase with its members at retail offsets and the private helpers the trade rows call | 005a8c66 M, 005af64e M, 005b01fe P |
| engine/CTCShop.h | FABLE_RETAIL_CTCSHOP_NAME / _SELLPRICE | GetName / GetCurrentSellPrice (0x007743B0) declarations | 005a8c66 M, 005af64e M, 005b01fe P |
| engine/CTCTargeted.h | FABLE_RETAIL_CTCTARGETED_GLOW | `bool IsToUseTargetGlow() const { return UseTargetGlow; }` (FableWin ?IsToUseTargetGlow@CTCTargeted@@QBE_NXZ) | 0064bfba P |
| engine/CTCTargetingBase.h | FABLE_RETAIL_CTCTARGETINGBASE_OFTYPE | `enum ETargetingType;` + `bool IsTargetedThingOfType(ETargetingType) const;` 0x007F5A70 | 0064bfba P |
| engine/CTCWife.h | (inside the existing FABLE_RETAIL_CTCWIFE block) | `bool IsOfferingSex() const;` 0x006CEEA0 | 0064bfba P |
| engine/CTattooDef.h | FABLE_RETAIL_CTATTOODEF | the class on CSubDefClassBase, opaque, sizeof kept | 005a8c66 M |
| engine/CThingAICreature.h | FABLE_RETAIL_CTHINGAICREATURE_INFORMATION | `bool GetHasInformation() const;` 0x008300D0 (declaration; see NAME_FIXES.tsv for the manifest mislabel) | 0064bfba P |
| engine/CUserProfileManager.h | FABLE_RETAIL_CUSERPROFILEMANAGER_TARGETINGSTATUS | `UseNewProfileValues` +0x0C, `ShowTargetingStatus` +0x6C, `ShowTargetingStatusNewProfile` +0x134 with pads (not combinable with the VOLUME block; the two Ego_r offsets shift by -8 / -4 for the retail STLport sizes, see NOTES.md) | 0064bfba P |
| engine/NGraphicAppearance_CTCGraphicAppearance.h | FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HIGHLIGHT | `enum EAppearanceMeshEffectPriority;` + `SetAsHighlighted(float,float,const CRGBColour&,bool,bool,EAppearanceMeshEffectPriority,void*)` 0x004C04C0 and `ClearMeshEffect(void*)` 0x004BFFC0 | 0064bfba P |
| engine/NInventory_CTCInventoryBase.h | FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING / _VTABLE | GetBuildingThing (0x005BC4C4) and the full base vtable the trade rows call through (slots 80/81 AddItemToInventory) | 00590f0b P, 005a8c66 M, 005af64e M, 005b01fe P, 005f59c7 P |
| engine/NPlayerGui_CDrawBase.h | FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES | `PrimitiveHandles` as `std::vector<CEnginePrimitiveHandle>` (8-byte elements; the pointer vector stays the default) + `unsigned char ScaleAlpha(unsigned char) const;` 0x00643D5D | 0064bfba P |
| engine/NTargeting.h | FABLE_RETAIL_NTARGETING_MELEE | `bool __fastcall IsThingVulnerableToMeleeStrikesFromThing(const CThing&, const CThing&);` 0x007F59F0 | 0064bfba P |
| stlport/fable_map.h | FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE / _TREE_ERASE_KEY / _VECTOR_PUSHBACK_OUTOFLINE | the tree clear / erase-by-key and vector push_back as out-of-line declarations (the COMDATs the inventory rows call) | 00590f0b P, 005a8c66 M, 005b01fe P, 005f59c7 P |

## New headers

| header | contents | users |
|---|---|---|
| engine/CExpressionDef.h | CExpressionDef on CParentDefClassBase, Ego_r members at the retail offsets (sizeof 0x90, ExpressionType +0x8C as the landed Transfer 0x004569A7 reads it) | 005f59c7 P |
| engine/CThingObjectDef.h | CThingObjectDef on CThingBaseDef (member-less: only the typed CDefPointer conversion is needed; sizeof unproven, see NOTES.md) | 00590f0b P |
| engine/NGameText_CDataBank.h | NGameText::CDataBank / CTextEntry: EntryExists 0x009CC190, GetTextInfo 0x009C7D00, GetPTextEntry 0x009C9070, GetTextBySymbol 0x009C95E0 (turns FABLE_RETAIL_CBANKFILE_ENTRYQUERY on for its CBankFile include) | 00590f0b P, 005a8c66 M, 005af64e M, 005b01fe P, 005f59c7 P |
| engine/NUISystem_CAction.h | NUISystem::CAction (0x10 bytes with the retail STLport list; ctor 0x0042BE50) | 00590f0b P, 005d6748 M, 005f59c7 P |
| engine/NUISystem_CActionParamState.h | retail model of CActionParamState (FABLE_RETAIL_CACTIONPARAMSTATE required) | 005d6748 M |
| engine/NUISystem_CActionParamULONG.h | retail model of CActionParamULONG (FABLE_RETAIL_CACTIONPARAMULONG required) | 00590f0b P, 005f59c7 P |
| engine/NUISystem_CText.h | NUISystem::CText on its RTTI bases, vtable 0x01249CCC: GetValue slot 142, the three SetValue overloads 143..145 (reverse declaration order), SetValueWithLineBreaks | 00427216 M, 005a8c66 M, 005af64e M, 005b01fe P |
| engine/CTCActionUseBase.h | CTCActionUseBase (TCI_ON_ACTION_USE 13): PActionUseDef +0x0C, Usable +0x10 (`IsUsable()` is a PLACEHOLDER accessor name), sizeof 0x14 | 0064bfba P |
| engine/CTCBattleCharge.h | CTCBattleCharge (TCI_BATTLE_CHARGE 168): PTarget +0x0C, IsZTargetting +0x88, sizeof 0x94; PLACEHOLDER accessor names | 0064bfba P |
| engine/CTCTavernGameMaster.h | CTCTavernGameMaster (TCI_TAVERN_GAME_MASTER 210): TavernGame +0x0C, sizeof 0x10; PLACEHOLDER accessor name | 0064bfba P |
| engine/NPlayerGui_CDrawHighlightingTargetingCursor.h | the class on CDrawBase (PTargetedThing +0x28 .. CursorType +0x44, sizeof 0x48) + `enum EHighlightingCursorType` with PLACEHOLDER enumerator spellings (values 0..4 from the row) | 0064bfba P |
| stlport/fable_algorithm.h | SGI `mem_fun_ref_t` / `mem_fun_ref` and the `for_each` declaration (0x0043F360, /Gr), lifted from the landed CDrawMiniMap::ClearPrimitives model | 0064bfba P |

Withdrawn this session: the `FABLE_RETAIL_CDEFPOINTER_RELEASE_CLEARFIRST` probe block in CDefPointer.h (no row uses it; the overlay
copy was removed so the repo header applies).
