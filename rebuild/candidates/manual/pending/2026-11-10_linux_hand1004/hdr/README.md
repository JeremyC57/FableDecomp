# Chunk 1004 header overlay (`FABLE_HDR_OVERLAY=$A/hdr`)

Every change is additive and behind a new `#ifdef FABLE_RETAIL_*` opt-in (or is a new file); the default paths of the repo headers
are unchanged. The edited copies are regenerated from `rebuild/include` at HEAD by `python3 $A/apply_hdr.py` (edit list:
`$A/hdr_edits.py`), last re-run after Codex pass 16 (b244ef6c1); both matched rows re-graded RELOCATION_MATCH on the result.

## Used by the matched rows (0x005C90FC, 0x005CACB9)

| file | opt-in | what | evidence |
|---|---|---|---|
| `fable_string.h` | `FABLE_RETAIL_CWIDESTRING_NULLSTRING` | `static CWideString CWideString::NullString;` (0x013BCA24) | Ego_r dynamic initializer / atexit destructor for `CWideString::NullString`; landed 0x00562E10 binds `?NullString@CWideString@@2V1@A` |
| `fable_definition_table.h` | none (verbatim copy) | needed only so its sibling `#include "fable_string.h"` resolves to the overlay copy (MSVC searches the includer's directory first; `#pragma once` is per path) | -- |
| `engine/CPlayerGui.h` | `FABLE_RETAIL_CPLAYERGUI_LIVEGUI` (nested in the existing retail branch) | `bool LiveGuiMode` at +0x2BE splitting the old pad (same size), inline `IsLiveGUIMode()` | Ego_r member list; FableWin `?IsLiveGUIMode@CPlayerGui@@QBE_NXZ`; retail `cmp byte ptr [eax+2BEh],0` at 0x005C9553 |
| `engine/CTCHeroExperience.h` | `FABLE_RETAIL_CTCHEROEXPERIENCE_GETPHEROCOMBATDEF` | inline `GetPHeroCombatDef()` returning the `CDefPointer` by value | FableWin `?GetPHeroCombatDef@CTCHeroExperience@@QBE?AV?$CDefPointer@$$CBVCHeroCombatDef@@@@XZ`; retail `mov esi,[esi+58h] ; inc [esi+4]` at 0x005C974C |
| `engine/CHeroCombatDef.h` | `FABLE_RETAIL_CHEROCOMBATDEF_HEAVYWEAPONS` | `CHeroAbilityStatPackage HeavyWeapons` at +0x13C (pad shortened in both existing branches, sizeof unchanged), offset assert | Ego_r last member; retail walks the map header at `[def+13Ch]` (0x005C9769) |
| `engine/CHeroAbilityStatPackage.h` | new file | `std::map<EHeroTrainableStatType,long> StatThresholds` (STLport 12 bytes), `IsValid` / `AddStatThreshold` declarations | Ego_r class; FableWin / Ego_r method spellings |

## Used only by the parked row 0x005CA68E (differ/005ca68e.cpp)

| file | opt-in | what | evidence |
|---|---|---|---|
| `engine/NInventory_CTCInventoryBase.h` | `FABLE_RETAIL_CTCINVENTORYBASE_ISDYING` | protected inline `IsDying()` returning `Dying` | FableWin `?IsDying@CTCInventoryBase@NInventory@@IBE_NXZ`; retail `cmp byte ptr [ebx+88h],0` at 0x005CA6DF |
| `engine/CTCHero.h` | `FABLE_RETAIL_CTCHERO_WEAPONUSE` (inside `FABLE_RETAIL_CTCHERO_TITLE`) | inline `IsWeaponUseAllowed()` returning `WeaponsUsable` | FableWin `?IsWeaponUseAllowed@CTCHero@@QBE_NXZ` (5-byte body); retail `cmp byte ptr [eax+0Ch],0` at 0x005CA6D5 |
| `engine/CTCWeapon.h` | `FABLE_RETAIL_CTCWEAPON_ISPROJECTILE` | `bool IsProjectile() const;` (0x005DAA70) | Ego_r `?IsProjectile@CTCWeapon@@QBE_NXZ` (0x0063A8A0) |
| `engine/CTCCarrying.h` | `FABLE_RETAIL_CTCCARRYING_CARRYSLOT` | `CThing* GetThingInCarrySlot(long) const;` (0x006A11F0) | Ego_r `?GetThingInCarrySlot@CTCCarrying@@QBEPAVCThing@@J@Z` (0x006C5E80) |
| `engine/CCarryableDef.h` | `FABLE_RETAIL_CCARRYABLEDEF` | retail class over `CSubDefClassBase` with `StatGetTypeName` and the three carry slots (+0x28/+0x2C/+0x30), opaque tail; the generated flat layout stays the default | RTTI `.?AVCCarryableDef@@ : CSubDefClassBase`; the landed 0x00849F70 local model; retail `mov eax,[eax+30h]` at 0x005CA930 |
| `engine/CTCMultiStrike.h` | new file | `CTCMultiStrike : CTCBase`, `StatGetInterfaceType` (TCI_MULTI_STRIKE 135), `ApplyEffectOnWeapon` 0x007CCF50 / `RemoveEffectOnWeapon` 0x007CC9E0 | Ego_r / FableWin spellings; retail `mov edi,87h` inline GetTC |
| `engine/CTCMultiArrow.h` | new file | as above (TCI_MULTI_ARROW 136; 0x007CC060 / 0x007CC120) plus `NumberOfShots` +0x10 and inline `GetShotsRemaining()` | Ego_r layout; FableWin `?GetShotsRemaining@CTCMultiArrow@@QBEHXZ`; retail `cmp dword ptr [edi+10h],ebx` at 0x005CAC43 |
| `engine/CCreatureAction_UnsheatheItemFromInventory.h` | new file (needs `FABLE_RETAIL_CCREATUREACTIONBASE`) | the action class over the retail CCreatureActionBase: both constructors, virtual dtor 0x005CC091, Initialise / PerformAction 0x0069F9E0 / FinishAction, private Clone / GetAnimationToUse, `PUnsheatheItem` +0xA8, `PFollowUpAction` +0xB0, sizeof 0xB8 | RTTI base list; Ego_r / FableWin method spellings; vtable 0x0125CCBC slot 3 = 0x0069F9E0 |
