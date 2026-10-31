# Hand chunk 610 (parked same-length rows, CApplyAdditionalExplosiveImpulseIfInsideRing .. CTCCombat): 3 of 10 byte-exact

All ten rows were parked same-length misses from the 2026-10-30/31 retry sweep. Every row was regraded from its best
attempt first (all reproduced), then changed one lever per grade. Three match (all RELOCATION_MATCH, behaviour tests PASS
under g++ and VC7.1); seven are parked in differ/ with the names respelled to the PDB and the residue on line 2.

## What matched (3)
- **0x00C071B0 swd_accept** (LZO 1.08, same-TU static, s in eax / n in ecx): the vendor source's `while (n--) { ... }`
  verbatim (`thirdparty/lzo/lzo-1.08/src/lzo_swd.ch` line 420). The parked `if (n) do { } while (--n != 0)` form made VC7.1
  move n into esi before the test and swap the HEAD2 index registers; the vendor loop gives `test ecx,ecx ; je` before the
  pushes, `dec esi ; jne` and `[eax+ecx]`. Lesson: for third-party rows, diff the parked body against the vendor file first.
- **0x0062B7C0 CWorld::EAControlledCreatureSheatheWeapon** (catalog "CTCHeroRangedAttacks::OnWeaponSwitch" is wrong): two
  changes on top of the parked body: every callee respelled from the PDB name set (protected `CWorld::GetPlayer(unsigned long)`,
  const `CPlayer::HasMode(EPlayerMode)` / `GetPControlledCreature()` / `CTCCarrying::GetThingInPrimarySlot()`, `CThing::GetPlayer()`
  returning `CPlayer&`, `SetCurrentAction(const CCreatureActionBase&)`, the ctor taking `CThingCreatureBase&`, the virtual
  dtor), and the second `GetPlayer(event->Player)` re-assigned to the `player` local exactly as the landed sibling
  `CWorld::EAControlledCreatureUseAbility` 0x00629C40 does. That gave retail's colouring (creature in esi, interface map in edi)
  that the parked file called a register-swap gap. `this` now uses engine/CWorld.h (layout-renamed), CGameEvent the shared
  header, CPlayer / CTCHeroRangedAttacks layout-renamed headers.
- **0x00A10A60 WordWrap_String::TokenizeW**: rewritten in the shape of the landed free-function sibling `WordWrap_TokenizeW`
  0x00A10720: the break test is the inlined PDB helper `bool __fastcall WordWrap_CanBreakLineAt(const wchar_t*, const wchar_t*)`
  called with a null start (its `text == start` is retail's `test edi,edi`), and the loop is
  `for (int remaining = wcslen(text); remaining > 0; --remaining, ++cursor)`. That alone moved `tokens` into ebp, homed the
  count in the dead argument slot and produced retail's `xor esi,esi` zero shared by the count store and the skipped-loop
  return (the parked explicit `if (len <= 0) return count;` gave `xor eax,eax`). Members respelled to Ego_r.pdb
  (`m_pszSource`, `m_iTokens`, `m_pszFirstToken`). Note the retail quirk kept literally: the scan starts ONE BYTE past the
  wide text (`lea edi,[eax+1]`); the test models it with byte offsets.

## Parked for Fable 5.1 (7)
- 0080db70 CApplyAdditionalExplosiveImpulseIfInsideRing::operator() DIFFER(275v275): impulse Y term `fld st(1) ; fmul [esp+0Ch]` where retail has `fld [esp+0Ch] ; fmul st(2)` | tried: s*Y, free operator*(const C3DVector&, float), copy + *= operator*, named-result operator* (fixes Y only, rest worse: no NRVO), body-assignment ctor, const float& ctor, named strength local | next: lane gap: x87 commutative operand choice with the first product kept on the stack
- 004b1a80 CQuestManager::SetBoastAsMade DIFFER(277v277): `this` edi / list head ebx (retail ebx / edi); zero C3DVector temporary stores and `mov ecx,[ecx+60h]` before the AddMessage pushes (retail after `push 32h`) | tried: STLport 4.6 _List_iterator / _List_node_base model with copy ctor and back() = *--end() (279v277), SGI insert through position._M_node with a __prev local, PDB callee spellings | next: lane gap: equal-use register colouring + store sinking past pushes
- 00934f20 CThingCompare_NearestGroupByMember::operator()<CThing> DIFFER(281v281): final `jne 934FA2h` back to the earlier return-b block becomes `mov eax,[esp+24h] ; jne ; mov eax,ebx` | tried: goto to a trailing label, goto to a label inside the nearestA block, flattened `!nearestB -> return a` + ternary, PDB names | next: lane gap: tail-merge / if-conversion choice
- 005b6767 CTCInventoryClothing::GetItemDescriptionArmourRating DIFFER(282v282): `mov eax,[g_pMainGameComponent]` scheduled after `mov esi,[ebp-28h]` (retail before) | tried: split difference with the bank load between, game local between, PTextBank-> in the call (280v282), PDB names (const, CWideString&, nested ArgPasser, CCountedPointer PTextBank) | next: lane gap: two-load scheduling
- 00ab7b00 CStaticFontBank::GetStringExtents DIFFER(287v287): VC7.1 enregisters the line width on the x87 stack where retail enregisters maxHeight (width, height, lineHeight, maxWidth homed; frame 0x14 with the maxima adjacent); ours frame 0x10 and ebx/ebp/esi pushed after the empty-text test | tried: C2DExtentsF line struct, declaration orders, the CStreamingFontBank sibling shape (extents struct first), class C2DExtentsF with copy ctor / Initialise, inline KeepMax(T&, const T&) | next: `maxWidth = Max(maxWidth, width)` with `const T& Max(const T&, const T&)` gives retail's x87 allocation and glyph-sum order but adds an address select (297v287): find a form that homes the width without the select
- 00637c80 NThingMap::CThingMapManager::DebugDraw DIFFER(288v288) (8 lines, was 38): TLX and TLY loaded first (eax/esi), retail TLY, pos.Y, TLX, pos.X | tried: `pos - map->Area.GetTLPos()` (the win from 38), explicit temp, GetTLPos through GetTLX/GetTLY, body-assignment ctor, free operator-, PDB names | next: lane gap: load scheduling of the inlined temporary
- 00733cf0 CTCCombat::UpdateGetHitCounterAttack DIFFER(288v288) (was 285v288): one dead frame word (frame 0x1C v 0x18, all frame offsets +4) and `this` ebp / node ebx (retail ebx / ebp) | tried: final fmul re-read through `found` (gives retail's length and reload), no data local (this in ebx but data loaded late, 286v288), private GetCombatAbilityData / long GFROR13 | next: a form that re-reads at the end but lets the iterator word be reused for the 0x42 key (the parked 285v288 form had the frame)

## Skipped
None.

## Names (two sources each)
- Proven (PDB name set + call-site argument shape / landed siblings): all three matched leaves
  (`?EAControlledCreatureSheatheWeapon@CWorld@@QAEXPBVCGameEvent@@@Z`, `?TokenizeW@WordWrap_String@@QAEHPAPA_W@Z`, `_swd_accept`
  = PDB C name `swd_accept` + the landed find_match binding), every callee in 0x0062B7C0 and 0x00A10A60, and the parked rows'
  leaves: `??RCApplyAdditionalExplosiveImpulseIfInsideRing@@QBEXPAVCThing@@@Z`, `?SetBoastAsMade@CQuestManager@@QAEXJVCCharString@@@Z`,
  `??$?RVCThing@@@CThingCompare_NearestGroupByMember@@QBEPAVCThing@@PAV1@0@Z`,
  `?GetItemDescriptionArmourRating@CTCInventoryClothing@@QBEXAAVCWideString@@J@Z`, `?GetStringExtents@CStaticFontBank@@UBE?AVC2DExtentsF@@PB_W_N@Z`,
  `?DebugDraw@CThingMapManager@NThingMap@@QBEXABVC2DCoordI@@J@Z`, `?UpdateGetHitCounterAttack@CTCCombat@@QAEXABVCThingCreatureBase@@@Z`.
- Descriptive (no PDB/RTTI proof): 0x0062B7C0 / 0x0080DB70 interface-map stand-ins (`CInterfaceMap::LowerBound` 0x0040F020 /
  0x004365B0, as fleet-wide landed code); 0x0080DB70 virtual slot 81 `AddAdditionalImpulse` (vtable 0x0125D2C4 slot 81 =
  0x006B1300, no PDB name; the landed sibling 0x007FD8D0 calls the same slot `ApplyImpulse`, also descriptive);
  0x00637C80 frame counter `g_ThingMapDebugDrawFrame` 0x013BA850 and the landed-majority `g_VirtualWidth/Height` doubles;
  0x00733CF0 world accessor `GetWorld_00686a80` (a this-less call of CTCBase::GetWorld / PeekWorld, unprovable which).
- The inlined helpers `CThing::HasTC/GetTC` (0x0062B7C0) and `CStaticFontBank::FindGlyph` (PDB `IBEPAVCGlyph@@G@Z`, used in the
  0x00AB7B00 differ) are forceinline/inline and emit nothing graded.

## Label corrections
- 0x0062B7C0: catalog "CTCHeroRangedAttacks::OnWeaponSwitch" -> CWorld::EAControlledCreatureSheatheWeapon.
- 0x0080DB70: "CTCPhysicsBase_ApplyExplosiveImpulseFromPoint" -> CApplyAdditionalExplosiveImpulseIfInsideRing::operator().
- 0x00733CF0: "TCCombat_ApplyBlockResponse" -> CTCCombat::UpdateGetHitCounterAttack.
- 0x00637C80: "CThingMapManager::DebugDraw" -> NThingMap::CThingMapManager::DebugDraw (const).
- 0x00487DC0 catalog "CPlayer::GetHeroCreature" -> CPlayer::GetPControlledCreature; 0x0048C950 "~CPhysicsMeshInfo_Derived_0048c950"
  -> CCreatureAction_SheatheItemToInventory::~CCreatureAction_SheatheItemToInventory (vland RELOC_NAME warnings).
- 24 callee spellings in NAME_FIXES.tsv.

## Header debt
- 0x0062B7C0: CCreatureActionBase stays local (engine/CCreatureActionBase.h models the vptr as data; the temporary needs the
  virtual dtor chain); CThing / CTCCarrying / CThingCreatureBase are local method/offset stand-ins (no CThing header).
- 0x00A10A60: no WordWrap_String header; local Ego_r layout.
- 0x004B1A80 (differ): engine/CQuestManager.h carries Ego_r (Dinkumware) list sizes (AcceptedBoasts +0xB0, World +0xD8); retail
  is +0x6C / +0x7C, so the differ keeps a local layout.
- 0x00AB7B00 (differ): engine/CStaticFontBank.h types +0x04 as `long m_maxHeight` and GlyphBanks as opaque bytes; Ego_r.pdb
  has CFontBank::FontHeight (unsigned long) and GlyphBank[1024]; the differ keeps a local model.
- 0x005B6767 (differ) uses engine/CTCInventoryClothing.h + engine/CCountedPointer.h; CMainGameComponent has no header.
