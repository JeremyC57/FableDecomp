# Chunk 759 notes (retry of 40 parked same-length rows, 395-486 bytes)

## Results (incremental; see STATUS.tsv)
| address | name | status | grades | what fixed it / residue |
|---|---|---|---|---|
| 00871290 | NCreatureAction::CActionEventExplosion::SetDataFromString | RELOCATION_MATCH | 5 | shared engine/NCreatureActionEvents.h + fable_string.h (FABLE_RETAIL_CCHARSTRING, new sub-macro FABLE_RETAIL_CCHARSTRING_SUBSTRING for FindCharPos / GetSubString, existing FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ) + CGameDefinitionManager.h; the esi/ecx residue goes once the template body of CBasicString<char>::operator==(const char*) (0x004115A0) is visible in the TU (explicit specialization, no pragma), as the landed same-TU sibling 0x00870FC0 has it: VC7.1 then keeps the type's string data in esi across the three calls. The three type tests are plain `if (type == "...") ... else if ...`. |
| 00870e00 | NCreatureAction::CActionEventTriggerParticle::SetDataFromString | RELOCATION_MATCH | 3 | twin of 00871290, same fix. |
| 00984d90 | CAIStateGroup_OpinionReactionRoot::FleeIsRunnable | RELOCATION_MATCH | 26 | every creature access through the inline CAIStateGroupBase::GetCreature() (`*PBrain->Creature`, FableWin IBEAAVCThingAICreature@@XZ): VC7.1 loads the creature straight into esi and forms the map with `add esi,44h`; PeekTC(ETCInterfaceType, T**) const over the real engine/CVectorMap.h with the key copied into a local and an explicit LowerBound / end() test. |
| 00687730 | CMessageEventManager::AddMessage(const CMessageEventDef&, ...) | RELOCATION_MATCH | 11 | PDB signature (def / pos are const references); 0x49 is TCI_ENEMY (Ego_r CTCEnemy::PFaction +0x28 with 12-byte lists = retail +0x18); PeekTC over the real CVectorMap; `event.Pos = pos` straight after the time stamp puts the 0x49 key temporary in the dead `pos` slot as retail does; callee 0x0049D870 is CWorld::GetFrame (static __fastcall). |
| 006aeea0 | CTCRegionFollower::IsCreatureFollowingHeroAcrossRegions | RELOCATION_MATCH | 13 | PDB signature (const CThingAICreature&); the three lookups as PeekTC over the real CVectorMap (removes the eax/ecx/edx rotation); a same-TU `__declspec(noinline)` GetSubDef<CCreatureDef> body in the landed CTCWound::AddWound stand-in shape (an external declaration splits the def release, 21 lines); fable_string.h CCharString. No pragma. |
| 00838ec0 | CThingGameObject::GetShotHitParameters | RELOCATION_MATCH | 7 | PDB signature (NHitLocations::CHitLocation*, CThing&, const C3DVector&, CHitParameters&) const and the NThingShot::CTCShotDefinitionBase / CHitParameters PDB member names; both TCI_SHOT_DEFINITION lookups as PeekTC over the real CVectorMap (25 -> 2 lines); `params.Localised = true` between the Position and Direction copies. |
| 007751a0 | CTCShop::SetShopKeeper | RELOCATION_MATCH | 15 | PDB signature (CThingCreatureBase*); engine/CBaseObject.h + engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN); the FableWin inline __fastcall templates operator==(const CIntelligentPointer<T>&, const CBaseObject*) and IS_THING_ALIVE(const T&) (VC then reloads ParentThing after GetPItem as retail does); the keeper test and the tavern test as one `||` over one SetStockState(0); PeekTC over the real CVectorMap + std::bitset bits. |

## Parked rows (residue + next idea; incremental)
Full residues are line 2 of each `differ/<addr>.cpp` and STATUS.tsv. Grades per row are the LOG.tsv counts (18 rows got only the
baseline grade this session: 004888c0 004feec0 00537776 0056f30b 0057aeb4 00649e8b 0065c5bb 00713d50 00753e90 007e8a40
0087bd70 008e5d40 00920dc0 009faf70 00b81820 00b847d0 -- context ran out; they still need their 10 attempts; 00838ec0 matched
 on its 7th grade and 008cdd40 got one PeekTC try, still 24 lines: its residue is the frame layout).
Next ideas, by lever that worked this chunk:
- Rows with component lookups through a local CInterfaceMap model (0057aeb4, 0062bef0, 00649e8b, 0065c5bb, 00713d50, 00753e90,
  00838ec0, 008cdd40, 0097f870): swap in std::bitset + the real engine/CVectorMap.h with the PeekTC(ETCInterfaceType, T**) shape
  (four matches this chunk came from it), and the PDB signatures (several drafts have pointer-for-reference or wrong const).
- Rows with CIntelligentPointer locals (00774730, 008bf500, 0089b5b0, 0095b820, 00687540): engine/CIntelligentPointer.h and the
  FableWin inline __fastcall operator== / IS_THING_ALIVE templates.
- 00b50370: constant-register hoisting (8.0 / 0.1 / 1.0 in ecx / edx / eax with source store order) not reproduced by 17 spellings;
  next: an inline member that takes the values as parameters (e.g. a SetShadowDistances(float, float)), or the PDB InitialiseTextures
  being called through the real header.
- 00872110 / 00b9d730 / 00b4b8b0 / 00697490: x87 operand order; 00872110 keeps 2 lines with the draft shape, the shared C3DVector
  spelling fixes the side test but flips the first dot product.
- 0089b5b0: the CreateNullHandle temporary's ~CCountedPointer reloads Info into ecx (retail eax) for operator delete.
- 006aeea0-style GetSubDef stand-ins: 00774730 (GetSubDef<CShopDef> 0x005130A0) now uses one; residue is the loop esi/edi swap.

## Header overlays (hdr/)
- `fable_string.h`: new sub-macro `FABLE_RETAIL_CCHARSTRING_SUBSTRING` (inside FABLE_RETAIL_CCHARSTRING) declares
  `long FindCharPos(char) const` 0x0099E5A0 and `CCharString GetSubString(unsigned long, unsigned long) const` 0x0099EC70
  (Ego_r.pdb ?FindCharPos@CCharString@@QBEJD@Z / ?GetSubString@CCharString@@QBE?AV1@KK@Z). hdrcheck2: default=same and every macro same.

## Name fixes (NAME_FIXES.tsv)
- 0x00B4FC10 is CEngineShadowRenderer::ChangeVideoOptionShadowQuality(long, float) -> bool (both PDBs; landed as SetShadowMapSize(long, long)).
- 0x00A01B50 / 0x00A01B90 take / return CBaseObject* (both PDBs); two landed bindings spell GetPItem with CThing*.
- 0x00485815 is the template GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*) const.
- Draft names corrected in the sources: interface 0x49 "CTCFactionMember" is CTCEnemy (TCI_ENEMY), 0x21 "CTCBuildingContents" is
  CTCThingOwner (TCI_THING_OWNER), 0x0049D870 "GetCurrentTime" is CWorld::GetFrame, CTCFollowed::IsCreatureValidFollower takes CThing*.

## GOTCHAS one-liners
- Component lookups: PeekTC(ETCInterfaceType, T**) over the real engine/CVectorMap.h + std::bitset<274> bits (key copied into a local,
  explicit LowerBound / end() test) fixes eax/ecx/edx key-address rotations that local CInterfaceMap models leave (0x006AEEA0, 0x007751A0).
- A dead parameter slot reused for a key temporary follows the source order of the parameter reads: copy the parameter that retail's
  slot belongs to early (0x00687730 `event.Pos = pos` right after the time stamp).
- `ip == thing` through the FableWin inline __fastcall template operator==(const CIntelligentPointer<T>&, const CBaseObject*) reloads
  the compared member after GetPItem; `ip.Get() == thing` keeps it in a register (0x007751A0).
- STLport vector operator[] as `*(begin()+n)` stops VC7.1 CSE-ing i*8 across GetPItem calls (`_M_start[i]` does not; 0x008BF500).
- A same-TU `__declspec(noinline)` GetSubDef<T> body (landed AddWound stand-in) is needed for retail's single def-release block;
  `__declspec(noinline)` alone replaces the old `#pragma auto_inline(off)` wrapper (0x006AEEA0).

## Open name conflict (not resolved here)
- 0x00662000: 00984d90 calls it as Ego_r ?GetCreatureType@CThingCreatureBase@@QBE?AW4ECreatureType@@XZ (the PDB has that member, and
  the row compares the result with 1); the catalog label is the descriptive CThingCreatureBase::GetMovementMode (chunk 218, no PDB
  symbol) and the landed body file models it as a CountReferences stub (`[this+0xE0]->[+0xC4]`). vland warns RELOC_NAME on it.
