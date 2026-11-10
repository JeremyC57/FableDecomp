# chunk 1003 notes -- CTCInventoryClothing (7 rows)

All grades, c2prio probes and the final gall.sh ran with `export FABLE_HDR_OVERLAY=$A/hdr` (overlay documented in hdr/README.md;
every change is a new `#ifdef FABLE_RETAIL_*` block, default paths byte-identical). Relocation identity of every matched row was
checked with tools/relchk.py (our COFF relocation symbols vs retail call/data targets through rebuild/oracles/reloc_bindings.tsv):
all clean except the bindings that NAME_FIXES.tsv corrects.

## Result: 5 RELOCATION_MATCH, 2 parked
| row | identity (exact PDB spelling, LABELS.tsv) | grade | lever |
|---|---|---|---|
| 0x005B3AEF | `?OnInitialActivate@CTCInventoryClothing@@UAEXXZ` (slot 6) | RELOCATION_MATCH (3 grades) | named `CCharString name("")` in its own block around `CThingObject::Create`; `IsDead()` guard on `SetAsCreatedByOwnerThingCreation`; same-TU GetSubDef/GetDef |
| 0x005B48A2 | `?OnItemSelected@CTCInventoryClothing@@UAEXJAA_N@Z` (slot 33; catalog's CTCShop signature wrong) | RELOCATION_MATCH (1 grade) | ConstructScreenPC devices: same-TU GetDef/GetSubDef + visible `_List_base` ctor |
| 0x005B690B | `?RefreshDescriptions@CTCInventoryClothing@@QAEXXZ` | RELOCATION_MATCH (4th grade) | named `CComponent*` key local + `FABLE_RETAIL_STLP_MAP` |
| 0x005B6A66 | `?ConstructClothingListPC@CTCInventoryClothing@@AAE..` | RELOCATION_MATCH (2 grades) | early `return 0`; Create in both worn arms |
| 0x005B76D1 | `?ConstructScreen@CTCInventoryClothing@@UAEPAVCComponent@NUISystem@@PAV23@@Z` (slot 77; catalog "vfunc_77") | RELOCATION_MATCH (2 grades) | console twin of ConstructScreenPC; `partLists[i]` indexed |
| 0x005B3E4E | `?GlobalUpdate@CTCInventoryClothing@@UAEX_N@Z` | DIFFER(538v543), parked | local model only (see below) |
| 0x005B9641 | `?DrawPreparePrimitivesForRendering@CTCInventoryClothing@@UAEXABVCEnginePrimitivePrepareInfo@@@Z` (slot 11; catalog "vfunc_11") | not built, parked | split + header debt (see below) |

## Identities proven (evidence in LABELS.tsv)
- 0x005B76D1 = ConstructScreen: CTCInventoryClothing vtable 0x0124356C slot 77 (base CTCInventoryBase::ConstructScreen 0x0057A233
  `xor eax,eax ; ret 4`), Ego_r donor 0x005630DB; body = ConstructScreenPC 0x005B8743 shape with the console templates.
- 0x005B9641 = DrawPreparePrimitivesForRendering: vtable slot 11; Ego_r donor order (after ConstructScreenPC, donor 0x005650FF).
- 0x005B48A2 = OnItemSelected: vtable slot 33 (the CTCInventoryBase pure slot; `ret 8`).
- 0x005B358C = CreateMenuDesc (console twin of CreateMenuDescPC, same 0x87-byte shape; donor order 0x0055EE29); resolved by
  ConstructScreen's RELOCATION_MATCH.
- CMenuEntry vtable 0x0124662C slots 152 / 153 = AddActionOnUnselected 0x0053E2A0 / AddActionOnVisible 0x0053E3C0.
- 0x0048524C = CCountedPointer ShareData<CMenuEntry>, 0x004850F9 = ReleaseData<CMenuEntry>; 0x0042BED4 / 0x0042CD84 = CDeletion ctors
  (0x0042CD84 is bound to five different copy-constructor names in reloc_bindings -- ??0CDeletion@NUISystem@@QAE@ABV01@@Z among them;
  ConstructScreen relocation-matches with the CDeletion copy ctor. Not put in NAME_FIXES: the body may be ICF-shared with a CAction
  copy ctor, so prove the owner before respelling).
- 0x00485815 = GetSubDef<CCreatureDef>; CCreatureDef::InitialAppearanceModifiers sits at retail +0x54 (12-byte STLport vectors).
- 0x005B5CF8 = CTCInventoryClothing::GetItemDescription (slot 78 override), called virtually by RefreshDescriptions.

## Name corrections (NAME_FIXES.tsv)
- 0x005B70B8 ConstructClothingList: private AAE, non-const `std::vector<std::pair<long,long> >&` (landed local model: public, const).
- 0x005BDFB1 SelectItemWithDefIndex and 0x005BCCF1 DeselectAll: protected `IAE` members of `NInventory::CTCInventoryBase`.
- 0x005B406D SetThingAsWearingSelectedClothes: private, `CThing&` (landed: public `CThing*`, `CTCInventoryClothing_Retail`).
- Catalog-only: 0x005B48A2 is OnItemSelected (not a CTCShop accessor); 0x005B76D1 / 0x005B9641 are ConstructScreen /
  DrawPreparePrimitivesForRendering, not "vfunc_77" / "vfunc_11".
- Unproven candidate (one source only): 0x005DC1D0, bound as `?GetAnimationTransitionInTime_005dc1d0@CAnimationEntry@@QBEJXZ`, is
  probably `?GetTransitionTime@CAnimationEntry@@QBEJXZ` (Ego_r name set); 0x0070E8B0 probably `?Update@CAnimateThing@@QAEXABVCAnimateThingUpdate@@@Z`
  (12-byte TimePassed / DistanceMoved / PercentageComplete argument matches the call site 0x005B4031..0x005B4050).

## Parked rows -- what they need next
- 0x005B3E4E GlobalUpdate (differ/005b3e4e.cpp, local model, DIFFER 538v543): header debt -- no shared CAnimateThing, CAnimInit (44 B,
  Ego_r layout complete), CAnimSet / CCompleteAnimation (retail 12-byte vectors), CTCAnimationComplex (GetAnimation 0x0070B460 returns
  `CAnimateThing&`), CAppearanceDef (Animation +0x34), CAnimationSet (`GetAnim` 0x005DC340 returns `const CAnimationEntry&`),
  CAnimateThingUpdate, CAnimFilterBase. This session's lever: `if (list.empty()) anim = twirl; else anim = default;` with
  `anim = defaultAnim` initialised gives retail's `sete bl` before `~list` and the default-anim slot re-store. Remaining: frame 0x54 v
  0x58 (retail keeps init / update / frame-info temp in disjoint words), twirl slot -0x20 v -0x1C, `this` reload into eax not edi.
  Next: build those headers from struct_layouts_egor.tsv (retail container stride), then re-run with the real CAnimInit ctor / Init.
- 0x005B9641 DrawPreparePrimitivesForRendering (differ/005b9641.cpp, outline only, 0 grades): the catalog extent is wrong -- the body
  ends at 0x005B9BF0 (0x5AF bytes, SPLITS.tsv); 0x005B9BF0..0x005B9D8C are tiny STLport/CVectorMap helper bodies (names unproven).
  Needs headers for CEnginePrimitivePrepareInfo, CUIMiscThingsDef (+0x1E4..+0x200), CRightHandedSet (ctor 0x00487050),
  C3DOrientationInterpolator (ctor 0x005508D0), CEnginePrimitive2DMorphedAnimatingMesh (0x005BC1B0), the morph-handle map
  (0x005BBFD6), CEngineTextureMorphSet / CSectionSet.

## Header debt left in the overlay (merge at landing): see hdr/README.md (10 opt-ins in 7 headers).
