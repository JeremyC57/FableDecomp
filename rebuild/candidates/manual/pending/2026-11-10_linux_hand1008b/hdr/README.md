# chunk 1008b header overlay (`FABLE_HDR_OVERLAY=$A/hdr`)

Copies of repo headers (rebuild/include, HEAD at the start of the chunk) plus three new headers. Every change sits behind a new
`#ifdef FABLE_RETAIL_*` opt-in or is a new file nobody includes yet, so the default path of every existing TU is byte-identical.

| file | kind | opt-in | what | used by |
|---|---|---|---|---|
| engine/CTCSummonSpell.h | NEW | FABLE_RETAIL_CTCSUMMONSPELL (forces FABLE_RETAIL_CTCBASE) | `class CTCSummonSpell : public CTCBase`: inline `StatGetInterfaceType()` = TCI_SUMMON_SPELL (145, `mov ebx,91h` 0x005EE66A) and `long GetResetCreatureManaCost() const` 0x0079A610 (Ego_r + FableWin `?GetResetCreatureManaCost@CTCSummonSpell@@QBEJXZ`). No data members. Without the opt-in only names the class. | 0x005EE49A |
| engine/CSpecialAbilitiesFireballSpellDef.h | NEW | (new file; DEF_CLASS_BASE forced like its siblings) | the 4 Ego_r.pdb members, CParentDefClassBase base: ReleaseDelay +0x3C, Inclination +0x40, `std::vector<float> StaminaCost` +0x44, TargettingArcAngle +0x50 (retail 0x58). Read: StaminaCost at 0x005EE54E. | 0x005EE49A |
| engine/CSpecialAbilitiesDivineWrathDef.h | NEW | (new file) | the same 22 Ego_r members as CSpecialAbilitiesUnholyPowerDef (same order, spelling, offsets; size 0x100). Read: EnvChangeSecs +0xB8, StaminaCostPerSec +0xDC (0x005EE607..0x005EE625). | 0x005EE49A |
| engine/CTCInventoryAbilities.h | copy + block | FABLE_RETAIL_CTCINVENTORYABILITIES_BUTTONS (nested in FABLE_RETAIL_CTCINVENTORYABILITIES) | declarations `void SetAbilityToButton(EHeroAbility, long, bool)` 0x005F6423 and `long GetNumAssignableSpecialMoveButtons() const` 0x005EDCF0 (PDB spellings). | 0x005F6BD6 |
| engine/CGamePlayerInterface.h | copy + block | FABLE_RETAIL_CGAMEPLAYERINTERFACE_SPECIALMOVEBUTTONS | declaration `long GetNumberOfAssignableSpecialMoveButtons() const` 0x00445940 (Ego_r + FableWin; landed as GetNumAssignableSpecialMoveButtons). Layout unchanged. | 0x005F6BD6 |
| stlport/fable_algorithm.h | copy + block | FABLE_RETAIL_STLP_FIND | declaration of STLport 4.6 `find(first, last, const T&)` as FABLE_FASTCALL (retail COMDAT find<EHeroAbility*,EHeroAbility> 0x005F8150, ecx/edx + stack, `ret 4`). | 0x005F6BD6 |

Only the matched row 0x005EE49A (src/) needs the three new headers; the others serve the parked 0x005F6BD6 (differ/).
