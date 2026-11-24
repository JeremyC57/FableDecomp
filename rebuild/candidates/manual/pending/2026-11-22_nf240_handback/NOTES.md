# NF240 notes (2026-11-22, round 13): two unprobed held p27 lines + L38 / L39, then the queue

Base: the repo tree with nf238 / nf239 landed. NF241 ran beside me (list<CComponent*> family, its QUEUE_LINES.tsv); none of my rows is
on its list or in `pending/2026-11-22_codex_pass28/RESERVED.tsv` (grep of every address before claiming).
Locks: claimed with plain mkdir + `NF240` (work/locks.log), all new directories; none was held.
Tools: NF238's set re-pathed (tools/), the same vl.sh / sc.sh / mkobj.sh / lnk.py / pk.py / pay.py.
Flags lines added: none. Descriptive names added: none (every new spelling is a PDB name).

## Entry 1 (q12673)
### 0x0143B8D8 ?CurrentRenderFrame@CEngineInternalPrimitiveBase@@0GA -- done, 3 -> 1
Name and access:
- `?CurrentRenderFrame@CEngineInternalPrimitiveBase@@0GA` is in all three PDBs (string grep of Ego_r.pdb, FableWin.pdb, Ego_d.pdb).
  `0` = private static, `G` = unsigned short.
- The p27 witness 0x00B92950 already binds it through the header's PASS27 block (private static + public inline SetAsRendered).
- Access paths for the non-member binders (both inline public accessors of the class, so no friend scaffold):
  - `?SetAsRendered@CEngineInternalPrimitiveBase@@QAEXXZ`: FableWin.pdb + Ego_d.pdb (not Ego_r: inline). The scene renderers store
    `LastFrameRendered = CurrentRenderFrame` in place, as the witness does.
  - `?IncrementFrameCounter@CEngineInternalPrimitiveBase@@SIXXZ` (public static fastcall): FableWin.pdb + Ego_d.pdb, plus the retail
    body as a third source: FrameStart 0x00B3271B `inc word ptr [0x0143B8D8]`.
- Census (absrefs): 9 references, 7 landed rows plus 0x00B31D33 / 0x00B91622 outside any landed row. Spellings at HEAD: `?g_frame@@3GA`
  (0x00B3AAA0), `?g_engineFrameNumber@@3GA` (0x00B32660 0x00B91760 0x00B92500 0x00B92790 0x00B92C20 0x00B943F0), the PDB name (witness).
The change (decision 1 asks about the in-place form):
- Header: one add-only opt-in `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTRENDERFRAME` in the generated (default) class branch
  (hdr/README.md): SetAsRendered, IncrementFrameCounter, the private static. Static members only.
- 0x00B32660 FrameStart (no local model of the class): includes the header with the opt-in; `++g_engineFrameNumber` ->
  `CEngineInternalPrimitiveBase::IncrementFrameCounter()`.
- 0x00B92C20 RenderCellAll (MSVC branch already on the generated header): the opt-in; `p->SetAsRendered()`. Its `#ifndef _MSC_VER`
  portable twin model gets the same three members in place.
- 0x00B91760 / 0x00B92500 / 0x00B92790 / 0x00B943F0: each models CEngineInternalPrimitiveBase locally at HEAD (polymorphic placeholder
  slots; the generated header is a flat pack(1) record, which is why). In-place respell of the HEAD local model: the same class token
  gains SetAsRendered + the private static (the IsLocal precedent, nf238). Per row the symbol diff is the UND data symbol plus a DEF
  pick-any `?SetAsRendered@CEngineInternalPrimitiveBase@@QAEXXZ`, 11 bytes, byte- and relocation-identical to the copy the witness
  0x00B92950 already emits from the shared header (objdump -dr of the three objects), and EQ (pk.py) to its retail out-of-line copy
  0x00B91620 (`mov ax,[0x0143B8D8] ; mov [ecx+36h],ax ; ret`, not in the catalog; one of absrefs' two unowned references).
- 0x00B3AAA0 WasRendered: the self row was a stand-in derived class `CEngineInternalPrimitiveBase_Methods::WasRendered` (int, QAE).
  Ported onto the header (ATTACHED + PASS27, the witness's opt-ins) as the PDB member
  `?WasRendered@CEngineInternalPrimitiveBase@@MBE_NXZ` (protected virtual const; all three PDBs). bool return: RELOCATION_MATCH
  (form 1 failed only on the missing prelude macros: DEF_CLASS_BASE / CIVCOUNTEDPOINTER / CENGINEPERSISTENTPRIMITIVEBASE).
  Census of the self address: 43 vtable references (rdata) only, no code reference; no landed row binds the stand-in spelling (grep),
  so the self respell cannot grow the count. The vtable emitters (about 25 CreateInternalPrimitive / ctor rows) were not compiled.
  New header test: links the row; a never-constructed derived probe makes a qualified call on a raw buffer (no vtable); 4 cases.
- Tests of the other six rows: host models that do not link the row (unchanged, as NF238 left 0x00BEAB40's).
- Table: the held p27 line restored; `?g_frame@@3GA` and `?g_engineFrameNumber@@3GA` retired (TABLE_CHANGES.tsv).

### 0x006A1620 ?IsThingCarryingRangedWeapon@CTCCarrying@@SI_NABVCThing@@@Z -- partial, 8 -> 4
- Name: Ego_r.pdb 0x006C60F0 / FableWin.pdb 0x02214CE0 (Codex p27 NAME_FIXES); the melee predicate is 0x006A15A0 (Ego_r 0x006C6040).
  Several stand-ins called 0x006A1620 "Melee" (the manifest gave both addresses the melee name): wrong, corrected in the comments.
- Binders (callers.py): 0x005C2DF5, 0x00664A10, 0x00664A50, 0x0068F9D0, 0x006AB0E0 (already PDB), 0x00890250, 0x00890D80,
  0x0091EC30 (witness, PDB), 0x0091F050.
- Moved onto engine/CTCCarrying.h with the existing `FABLE_RETAIL_CTCCARRYING_PLAYERHIT_PASS26` opt-in (the witness's; no header change):
  - 0x00664A50 and 0x0091F050: their local empty `enum ETCInterfaceType { }` replaced by the header enum (CTCCarrying.h includes it);
    the free stand-in declaration removed; `CTCCarrying::IsThingCarryingRangedWeapon(*thing)`.
  - 0x0068F9D0: the same, and its local `class CTCCarrying { GetThingInPrimarySlot }` model removed (the header declares the same member).
  - 0x005C2DF5: L39 below.
  Each: RELOCATION_MATCH, the symbol diff is the UND callee only.
- Blocked (stand-in classes):
  - 0x00664A10: its local CThingCreatureBase model has no CThing base, so `const CThing&` needs a cast or a re-base (both forbidden):
    a CThingCreatureBase.h port (the melee sibling 0x006649C0 has the same shape).
  - 0x00890250: the whole row is a stand-in (fake vtable struct, free `EntityWieldsRanged(Entity*)`, leaf a free function): re-authoring.
  - 0x00890D80: `CThingNative*` stand-in argument: a header port.
  Their locks are released again (unchanged).

### L38 0x005C2DED / L39 0x005C2DF5 CTCInventoryWeapons::IsMeleeWeaponDrawn / IsRangedWeaponDrawn -- done, 2 -> 1 each
- Names: Ego_r.pdb 0x00624F7A / 0x00624F82, FableWin.pdb 0x0237C819 / 0x0237C837 (CODEXP27 L38 / L39), public const. The landed
  binder 0x0064B1AC binds both (its paired calls at retail 0x0064B293 / 0x0064B2B7); engine/CTCInventoryWeapons.h declares both under
  the PASS27 opt-in it uses.
- Body (8 bytes each): `mov ecx,[ecx+4]` (CTCBase::ParentThing) ; `jmp 0x006A15A0` / `jmp 0x006A1620`, i.e.
  `return CTCCarrying::IsThingCarrying{Melee,Ranged}Weapon(GetParentThing());` (CTCBase::GetParentThing, FableWin.pdb, inline).
- Self rows were the stand-in forwarder `?Run@T@@QAEXXZ` calling `Sub::Run_006a15a0` / `Run_006a1620`. Now on the headers, so their
  callees take the PDB spellings too: 0x006A15A0 loses one stand-in, 0x006A1620 another.
- New DEF pick-any `?GetParentThing@CTCBase@@QBEABVCThing@@XZ` (4 bytes, the shared header inline; no retail owner).
- HEAD's two `?Run@T@@QAEXXZ` definitions were a LNK4006 pair (link_head.txt); gone.
- Catalog / module / files: RENAMES section 1 (`_global::Forward_IsThingCarryingMeleeWeapon_via0x4_*` -> CTCInventoryWeapons::*).
  vland graded them with payload name / module and oracle name patched, the nf236 / nf238 pattern.
- New header tests (link the row, stub the CTCCarrying predicate): 3 cases each; HEAD tests only printed PASS.

### Proof (entry 1)
- vland --recheck-landed (work/b1.log): 12 / 12 RELOCATION_MATCH + PASS, 0 RELOC_CONFLICT. WARNs are SHARED_HEADER_AVAILABLE on the
  rows' HEAD local models and RELOC_NAME catalog labels, as at HEAD.
- Symbol diffs (sc.sh HEAD v new: work/sym_p[HS]_<addr>.txt, xr_p[HS]_<addr>.txt): per row only the respelled UND symbol, the self leaf where renamed, and the shared-header
  pick-any copies named above. IncrementFrameCounter (8 B) and SetAsRendered (11 B) are EQ (pk.py, work/own_q12673.tsv) to their retail
  out-of-line copies 0x00B31D30 (`inc word ptr [0x0143B8D8] ; ret`) and 0x00B91620: the two 0x0143B8D8 references absrefs found outside
  any landed row are exactly these bodies (a third and fourth retail source for both names; NAME_FIXES). GetParentThing (4 B) is the
  CTCBase inline, not compared.
- Link: the 12 new objects + co-owners 0x00B92950 0x0091EC30 0x006AB0E0 0x0064B1AC: 0 LNK2005 / LNK4006 (HEAD set: 1 LNK4006,
  the ?Run@T@@ pair). work/lnk/.
- Header: equiv_check over the 220 rows whose source or test includes the header's closure (39 headers): 0 differing;
  assert_names_check 0 mismatches; pin_asserts pinned 0 (the generated branch's asserts are all _AT already).
- Census:

  | Address | Before | After | Note |
  |---|---|---|---|
  | 0x0143B8D8 | 3 | 1 | held line restored |
  | 0x006A1620 | 8 | 4 | 3 blocked stand-ins remain |
  | 0x006A15A0 | n | n-1 | Run_006a15a0 gone; the rest is not this entry (NAME_FIXES) |
  | 0x005C2DED | 2 | 1 | self leaf |
  | 0x005C2DF5 | 2 | 1 | self leaf |
  | 0x00B3AAA0 | k | <= k | stand-in self leaf replaced by the PDB spelling; vtable emitters not compiled |

## Entry 2 (q12674): 0x00740040 CModeSitBench ctor onto NCreatureMode_CModeSitFloor.h -- done, 2 -> 1
- The row emits the class vtable 0x01267BFC (absrefs: its only code reference is the ctor's vptr store 0x0074005B). Retail slots
  (vtable read): 0 ??_G 0x007400E0, 1 GetType 0x007400C0, 2 / 3 CModeSitBase AnimUpdate 0x008D6AC0 / FrameUpdate 0x008D6C80,
  4 GetAnimFilter 0x007400D0, 5 CModeSitBase::OnActivate 0x008D6580, 6-11 CModeBase OnDeactivate 0x00739E60, GetMovementSpeed
  0x00739E70, IsIdleMode 0x00739E80, IsAllowingPlayerSpecialMovementMoves 0x00739E90, IsHoldWeaponWithBothHands 0x0073A9F0,
  IsRemoveOnDeactivate 0x00739EA0.
- Port: the PASS27 class + the existing slot opt-ins (CMODEBASE_VIRTUALS, CModeSitBase FRAMEUPDATE / ANIMUPDATE / ONACTIVATE) + the
  filter's INLINE_CTOR (the sibling mode ctor 0x00743600's form). `CModeSitBench(CThing& thing) : CModeSitBase(thing), Filter("SIT_BENCH") {}`.
  The local CModeSitBench / CModeSitBase / CAnimFilterOfGroup / CCharString / CCRC models are gone. RELOCATION_MATCH.
- Form 1 (header as is): its compiler-generated ??_GCModeSitBench called an out-of-line ~CModeSitBench: 30 bytes v the owner's 40.
  Retail 0x007400E0 expands the destructor (`call 0x00739DB0` ~CAnimFilterOfGroup, `call 0x008D71D0` ~CModeSitBase). Form 2: the new
  add-only opt-in IMPLICIT_DTOR (hdr/README.md); the ??_G copy is then EQ to 0x007400E0 (bytes and relocations, with ~CModeSitBase
  bound to its landed self row 0x008D71D0).
- pk.py on the object (work/own_740.tsv): EQ ??_GCModeSitBench (0x007400E0), ??_GCAnimFilterOfGroup (0x0073E970), ??1CAnimFilterOfGroup
  (0x00739DB0), ??0CAnimFilterOfGroup (0x0073E8F0), CCharString::GetLength (0x00403A00) / GetAsCharArray (0x00403A10). NO-OWNER: the
  CAnimFilterBase ctor / dtor, CBasicString PeekData / GetLength. One NEQ: ??_GCAnimFilterBase v 0x0041CCE4 (31 v 31 bytes: retail
  pops the delete argument with `pop ecx`, a size-optimised TU; ours `add esp,4`). It is the shared header's copy: the landed sibling
  0x00743600 emits the identical NEQ copy at HEAD (decision 3).
- Vtable slot census (the vtable is the binder of 12 addresses; HEAD's local model had one slot, ?v00@CModeSitBench@@UAEXXZ):
  - 0x007400C0 / 0x007400D0 / 0x008D6AC0 / 0x008D6C80 / 0x008D6580 / 0x00739E60 / 0x00739E80 / 0x0073A9F0 / 0x00739EA0: the slot
    spelling equals the landed self leaf, or a spelling the header-based sibling vtables (0x00743600, 0x008E3A20) already bind.
  - 0x007400E0: v00 stand-in -> ??_ECModeSitBench@NCreatureMode@@UAEPAXI@Z (the vtable takes ??_E, as the siblings do); count-neutral.
    The self row's own leaf is the stand-in `CModeSitBench_CompositeScalarDeletingDestructor` (NAME_FIXES).
  - 0x00739E70 / 0x00739E90: their self rows were placeholders (?vfunc_7@CClass_00739e70, a global ?IsAllowing...@CModeBase@@QAE), so
    the PDB slot spellings would have been new there. Both self rows are respelled onto CTCCreatureModeManager.h in this hand-back
    (leaf-only, three PDBs each, RENAMES 1); nothing calls them, only vtables reference them. 0x00739E70: 2 -> 1 (local-model vtable
    emitters such as 0x008E3720 declare the nested CModeBase::GetMovementSpeed at slot 7, the PDB spelling: from their sources, not compiled). 0x00739E90: 2 -> 2 (see the swap below).
  - Finding (NAME_FIXES, not acted on): nine landed mode rows with local CTCCreatureModeManager::CModeBase models (0x008E3720,
    0x008E31A0, 0x008E37C0, 0x00741390, 0x00740420, 0x008AC4D0, 0x0073B0D0, 0x0073AEA0, 0x00743FE0) declare slot 9 = IsRemoveOnDeactivate and
    slot 11 = IsAllowingPlayerSpecialMovementMoves. The header, the catalog (0x00739E90 / 0x00739EA0), the CModeStandBase override at
    slot 9 (header-based sibling vtables) and FableWin's code order (GetMovementSpeed 0x01DD6780, IsAllowing... 0x01DD67A0) say slot 9 is
    IsAllowingPlayerSpecialMovementMoves. Their vtable slots 9 / 11 bind the two names swapped.
- Base ctor binding: stand-in ??0CModeSitBase@@QAE@PAVCThing@@@Z -> the PDB ??0CModeSitBase@NCreatureMode@@QAE@AAVCThing@@@Z (already
  the self row 0x008D7CA0's leaf); the stand-in line's witness is 0x0073FF70, so the table line stays.
- The "" literal at 0x0129AAF4 now binds ?NullChar@CCharString@@1DB (fable_string.h's GetAsCharArray; the table already maps it there).
- Test: HEAD's host-model test kept (it does not link the row); a linked test needs about 15 slot stubs (owed, NOTES decision 4).
- New header tests for 0x00739E70 / 0x00739E90 (qualified calls on a raw buffer; link the rows).
- Proof: vland (work/b2.log) 3 / 3 MATCH or RELOCATION_MATCH + PASS. Link with co-owners 0x00742790 0x00938380 0x007400E0 0x008D71D0
  0x0073E8F0 0x00739DB0 0x00743600 0x0073E970: 3 LNK4006, the same 3 at HEAD (the inline ??0CAnimFilterOfGroup copies v its strong owner
  0x0073E8F0; HEAD's local model defined the same symbol). work/lnk2/.
- Census: 0x00740040 2 -> 1; 0x01267BFC 1 -> 1 (respelled line); 0x00739E70 2 -> 1; 0x00739E90 2 -> 2; 0x007400E0 2 -> 2;
  0x008D7CA0 no growth; nothing grows.

## Entry 3 (q12675): the CEngine unit -- step 1 surveyed, not applied
- The four CIEngine.h placeholders are only the `#else` defaults: the header already carries the PDB declarations under opt-ins, each
  with its evidence (RENDER slot 8 ?Render, SCREENPOS slot 11 ConvertScreenPosToWorldPos, CONVERTSCREENPOS slot 12
  ConvertScreenPosToWorldPosAtViewDistance, CONVERTWORLDPOS slot 13 ConvertWorldPosToScreenPos). The CEngine overrides are in Ego_r.pdb
  (0x009B39A0 / 0x009AE850 / 0x009AE730 / 0x009AE5F0) and FableWin.pdb (0x02C9D1D0 / 0x02C9FA10 / 0x02C9F7B0 / 0x02C9F4A0).
- No landed row source spells a placeholder. Ten landed tests override them by name (CWorld_Load 0x004A1840, DrawPyramid 0x008764E0,
  DrawRHSet 0x004BC1A0, StartFrame 0x00AEEBA0, DrawAbilityButton 0x0064AE80, CDrawExperience 0x00646F67, CDrawGuildSeal 0x0064A639,
  CQuestInfoElementTick 0x00650E98, CDrawReceivedItems 0x0064E978, CText::Draw 0x0054EF00); 17 sources / tests use the opt-ins; 148
  sources / tests include CIEngine.h.
- Making the PDB names the default changes the default path of a header with 148 includers (every includer's preprocessed text
  differs, so all 148 need a VC7.1 recheck) and needs the ten tests respelled in the same pass. That is a coordinated header item,
  beyond what was left of this run (decision 5). Steps 2 / 3 (the 83 overrides, both rows) not started.

## Entries 4 / 5 (q12676, q12659) -- not started (context). NF236 NOTES entry 5 still lists each q12676 blocker.

## Slots 9 / 11 of the CModeBase vtable (probe for q12674)
- CModeCarryingWeaponStand::IsAllowingPlayerSpecialMovementMoves 0x008E35F0 (returns true; an override of only that query) has one
  reference, vtable 0x012856E4 + 0x24 = slot 9 (slot 1 there is CModeCarryingWeaponStand::GetType 0x008E35D0). CModeStandBase's
  override 0x0073DA00 is at slot 9 in all 70 vtables that hold it; slot 11 is 0x00739EA0 or a class's own IsRemoveOnDeactivate
  (0x00741FD0, 0x00743CB0, 0x008C36A0, 0x008C37B0). So slot 9 = IsAllowingPlayerSpecialMovementMoves (the header order) is proven
  from retail overrides, not only from header-built vtables; the nine local models have the two names swapped (NAME_FIXES).

## Rule slips
- Two waits used a `while kill -0 <pid>; do sleep 10-15; done` loop in the foreground (PID-based, but a foreground sleep).
- STATUS / APPLIED were written per entry (after the batch grade), not after every row.

## Decisions for the coordinator
1. The in-place form on four rows (0x00B91760 / 0x00B92500 / 0x00B92790 / 0x00B943F0): the HEAD local CEngineInternalPrimitiveBase
   models gain the PDB SetAsRendered + the private static, so they emit the 11-byte SetAsRendered copy identical to the witness's.
   Accept (IsLocal precedent), or hold them for a port onto the header (their models are polymorphic placeholders; a port is
   authoring-sized)?
2. 0x006A1620's held p27 line: restore it now (8 -> 4 spellings in rows; 6 -> 4 table lines with it), or wait for the three blocked
   stand-in rows (0x00664A10 needs CThingCreatureBase.h; 0x00890250 / 0x00890D80 need re-authoring off Entity / CThingNative)?
3. 0x00740040's ??_GCAnimFilterBase copy is NEQ to 0x0041CCE4 (`pop ecx` v `add esp,4`), as the landed sibling 0x00743600's copy
   already is: accept as shared-header debt (nf203), or hold?
4. 0x00740040's test: the HEAD host-model test is kept. A linked header test needs about 15 vtable-slot stubs; queue it?
5. CIEngine.h placeholders (q12675 step 1): switch the default to the PDB declarations as one header unit (148 includers rechecked,
   ten tests respelled), or keep requiring the opt-ins and only retire the placeholder tests?
6. q12674 leaves ?IsAllowingPlayerSpecialMovementMoves@CModeBase@CTCCreatureModeManager@@UBE_NXZ defined at 0x00739E90 (its self row)
   while the nine local-model vtables bind it at 0x00739EA0 (and ?IsRemoveOnDeactivate at 0x00739E90): one name, two addresses,
   which the gate may flag. Land q12674 as is (the retail probe above proves slot 9) and queue the nine rows' swap, or hold q12674
   until the swap lands with it? q12674 (0x00740040 / 0x00739E70 / 0x00739E90) can be held without touching q12673.
