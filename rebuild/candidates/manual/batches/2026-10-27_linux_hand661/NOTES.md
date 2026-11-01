# Hand chunk 661 (CThing::TCsToBeRemoved .. CBlurredAttitudesDef): 10 of 10 byte-exact

NA006 of the never-attempted pool (twin-landed rows). 9 RELOCATION_MATCH + 1 MATCH, all behaviour tests PASS under g++ and
the lane's VC7.1; vland read-only run: WINS 10 (only the expected `_$E2` RELOC_CONFLICT / RELOC_NAME warnings of the four
dynamic initializers, GOTCHAS 2026-10-29). Every row matched on its first probe once its own identity was settled, except
0x00C5EE20 (2 probes: same shape, slot names respelled) and 0x0067C370 (probe 1 with the shared CParentDefClassBase header
DIFFER 27v49, probe 2 MATCH). None of the ten twin labels was right for its row.

## What matched (10)

Dynamic initializers (`.text$yc`, Leaf `_$E1`; compiler-emitted from the global's definition, NOT vendor library objects --
the globals and their atexit thunks are engine C++ objects named in both PDBs' `dynamic initializer for` lists, so no
toolchain_library grade applies):
- 0x0121A670 CThing::TCsToBeRemoved -- the inline STLport 4.6 `_Rb_tree()` (malloc(0x20) header, red, root 0,
  leftmost/rightmost = header, count 0; every store re-reads the static header pointer), atexit 0x01228DF0. Model: the
  landed 0x00528E50 STLport text trimmed to the ctor/dtor/clear; the twin's invented `CRetailTree_node0x20` replaced.
- 0x0121AC08 `anonymous namespace'::REACTION_MANAGER_DEF_INSTANTIATION_NAME, 0x0121AE9C TITLE_NAME ("EXPRESSIONS TITLE"),
  0x0121F074 STrapCycle ("TRAP_CONTINUOUS") -- `namespace { CCharString X("lit", -1); }` (fable_string.h).
Implicit destructors / STLport _Destroy (0-caller COMDATs; identity from the ??1 / ??_G / _Destroy triple, below):
- 0x004B6A80 std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>::~pair -- STLport pair + qualified
  explicit dtor call in a harness function.
- 0x006FB8D0 std::_Destroy<pair<CCharString, CSymbolMap::CSymbolInfo> > -- STLport `_Destroy(_Tp*)` explicit instantiation, /Gr.
- 0x007803D0 NFire::CFirePoint::~CFirePoint and 0x00768BF0 AttackParticleAndEndPoints::~AttackParticleAndEndPoints -- Ego_r
  layouts, CIntelligentPointer<CThing> over an out-of-line virtual ~CBaseIntelligentPointer 0x00A01C10.
- 0x0067C370 CBlurredAttitudesDef::~CBlurredAttitudesDef -- engine/CDefClassBase.h opt-in chain + local CParentDefClassBase with
  an implicit dtor + STLport ~_Vector_base frees.
Library audio output:
- 0x00C5EE20 CVtable_012b9174::Uninitialize -- virtual member `{ ReleaseBuffer(); }` -> `mov eax,[ecx] ; jmp [eax+14h]`.

## Proven
- 0x0121A670 `_$E1` = dynamic initializer for 'CThing::TCsToBeRemoved' (global 0x013B8A8C) -- FableWin.pdb order AttitudeSymbols
  (03FDF480) -> REACTION_MANAGER_DEF_INSTANTIATION_NAME (03FDF6E0) -> CThing::TCsToBeRemoved (03FDF810) = retail .CRT$XCU
  0x0121A630 (CCRCSymbolMap 0x013B8A54) -> 0x0121A650 ("OPINION_REACTION_MANAGER_INSTANCE" 0x013B8A7C) -> 0x0121A670, atexit
  thunks 0x01228DD0 / 0x01228DE0 / 0x01228DF0 in the same order; the global sits in the thing-manager .bss block
  (0x013B8A80 / 0x013B8A84 read by CThingManager::GetThingSearchTools / WorldUpdateThings, 0x013B8A88 the copying-things flag)
  and its _M_erase 0x00527950 among the thing-manager STLport instantiations. Retail never reads it.
- 0x0121AC08 `_$E1` = `anonymous namespace'::dynamic initializer for 'REACTION_MANAGER_DEF_INSTANTIATION_NAME'' (0x013B8BC4) --
  33 per-TU PDB copies vs 32 retail initializers of "OPINION_REACTION_MANAGER_INSTANCE"; in the trap TU the PDB order
  REACTION.. -> STrap x4 matches retail literal for literal.
- 0x0121AE9C `_$E1` = `anonymous namespace'::dynamic initializer for 'TITLE_NAME'' (0x013B8C60) -- exactly two TITLE_NAME
  entries per PDB vs exactly two "<SCREEN> TITLE" retail CCharString initializers; "ITEMS TITLE" (0x0121A8C1) is followed by
  Item_Descriptions / Item_Name_Table / Item_Icon = ITEM_DESCRIPTION_STRING / ITEM_NAME_TABLE_STRING / ITEM_ICON_STRING, so this
  one is the other; it ends its TU as FableWin's first TITLE_NAME does; readers = CTCInventoryAbilities expressions screen.
- 0x0121F074 `_$E1` = `anonymous namespace'::dynamic initializer for 'STrapCycle'' (0x013BAE00) -- Ego_r 0x0101196D..0x010119C4 /
  FableWin 03FF46FB..03FF4776: STrapUntriggeredAnimName, STrapTrigger, STrapReset, STrapCycle after REACTION.. and
  BOASTING_CROWD_* = retail 0x0121F000 .. 0x0121F074 (TRAP_UNTRIGGERED / TRAP_TRIGGER / TRAP_RESET / TRAP_CONTINUOUS), 0x1D apart
  in both Ego_r and retail.
- 0x004B6A80 ??1?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@QAE@XZ -- quest-manager TU
  (Ego_r: CRuntimeQuestPersistence's CVectorMap<CCharString,CPersistInfo> instantiations, no CSymbolMap ones; retail __copy over
  the 12-byte pair 0x004B84F0); the TU's triple ??1 0x004B6A80 / ??_G 0x004B77A0 / _Destroy 0x004B86B0 (lane-verified
  0x004B86B0 = _Destroy<pair<..CPersistInfo>> under /Gr); the CSymbolInfo triple lives in the animation-events TU and a COMDAT
  name exists once per image.
- 0x006FB8D0 ??$_Destroy@U?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@std@@@std@@YIXPAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@0@@Z
  -- NAnimationEvents TU (CManager owns a CSymbolMap: FableWin GetEventSymbolMap / CFillEvents); placement among the STLport
  helpers (Ego_r Dinkumware _Dest_val<pair<CCharString,CSymbolInfo>> 0x0042A6B0 = STLport _Destroy), triple ??1 0x006FB460 /
  ??_G 0x006FB730 / _Destroy 0x006FB8D0; the lane emits exactly ??1 + ??_G + _Destroy for this instantiation.
- 0x007803D0 ??1CFirePoint@NFire@@QAE@XZ -- Ego_r NFire::CFirePoint is the only PDB class with CCharString +0 and
  CIntelligentPointer at +0x1C/+0x24; CTCFlammable TU; triple ??1 / ??_G 0x007811D0 / _Destroy 0x00781540 (both lane-verified).
- 0x00768BF0 ??1AttackParticleAndEndPoints@@QAE@XZ -- Ego_r AttackParticleAndEndPoints : BasicParticleAndEndPoints (StartPoint,
  EndPoint, two longs) + ParticleEffect +0x10; FableWin lists its ??1 / ??_G; CTCObjectAugmentations TU (AttackParticleEffects);
  triple ??1 / ??_G 0x007692A0 / _Destroy 0x007699C0 (both lane-verified); its ctor 0x00768BC0 precedes it.
- 0x0067C370 ??1CBlurredAttitudesDef@@UAE@XZ -- the slot-0 ??_G 0x0067C520 of vtable 0x0125B8AC (RTTI .?AVCBlurredAttitudesDef@@)
  inlines the same member sequence; Ego_r CBlurredAttitudesDef = CParentDefClassBase + vector<EOpinionAttitudeType> Attitudes.
- 0x00527950 (callee, not a row) = STLport _Rb_tree::_M_erase of the CUnproven_00527950 instantiation -- lane-verified
  RELOCATION_MATCH from the stl/_tree.c text.

## Descriptive
- 0x00C5EE20 CVtable_012b9174::Uninitialize -- the audio library (no PDB/RTTI). Class spelling = the landed rows of the class
  (ctor 0x00C5EEC0, IsChunkReady 0x00C5EE60, WriteBuffer 0x00C5EEB0, named by vtable 0x012B9174). Slot 2 is the counterpart
  of slot 1 Initialize (DirectSound's slot 2 0x00C57010 calls slot 5 then releases IDirectSound + CoUninitialize); slots 3/4/5
  take the landed DirectSound names CreateAndPlay / CreateLoopingBuffer / ReleaseBuffer (0x00C57380 / 0x00C56D70 / 0x00C57040;
  OpenAL 0x00C579C0 also ReleaseBuffer). The four output vtables (DirectSound 0x012B8D98, OpenAL 0x012B8ED4, waveOut
  0x012B8FCC, silent 0x012B9174) share one slot order; landed slot-5 rows of the waveOut / silent outputs are spelled Close /
  Clear (inconsistent, not respelled here).
- TCsToBeRemoved's element type: unprovable (no retail reader; node 0x20 = 16-byte trivially destructible value); the
  per-instantiation CUnprovenKey/Value/KeyOfValue_00527950 of the landed rows are used; the container wrapper (set/map) is
  unknown, so the data symbol mangles as the raw _Rb_tree.

## Callee names
- malloc 0x00BFEA0E, free 0x00BFEA14, _atexit 0x004012BC, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
  ~CBaseIntelligentPointer 0x00A01C10 (??1CBaseIntelligentPointer@@UAE@XZ, bound), ~CResource 0x009FC550 (engine header),
  CDefPointeeBase vtable 0x01230BA0 (engine header). All checked with calls.py against retail targets.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x0121A670 / 0x0121AC08 / 0x0121AE9C / 0x0121F074 `_gapscan::sub_*` -> the dynamic initializers above.
- 0x004B6A80, 0x006FB8D0, 0x007803D0, 0x00768BF0 `_Dest_val<..pair<CCharString,CSymbolMap::CSymbolInfo>..>` (propagated x1,677)
  -> ~pair<CCharString,CPersistInfo>, _Destroy<pair<CCharString,CSymbolInfo> >, ~CFirePoint, ~AttackParticleAndEndPoints.
- 0x00C5EE20 `_gapscan::sub_00c5ee20` -> CVtable_012b9174::Uninitialize (descriptive).
- 0x0067C370 `CCompressedMemoryDataInputStream::~CCompressedMemoryDataInputStream` -> CBlurredAttitudesDef::~CBlurredAttitudesDef
  (also 0x0067C520 `CCompressedMemoryDataInputStream::vector_deleting_destructor` -> ??_GCBlurredAttitudesDef@@UAEPAXI@Z).
- Landed neighbours (NAME_FIXES.tsv): 0x004B86B0, 0x004B77A0, 0x006FB730, 0x00781540, 0x007811D0, 0x007699C0, 0x007692A0,
  0x00768BC0, 0x00527950, the global 0x013B8A8C and 41 anonymous-namespace CCharString globals (32 REACTION.. copies, STrap x3,
  BOASTING_CROWD x3, TITLE_NAME "ITEMS TITLE", ITEM_* x3) landed as `g_CCharString_<addr>`.
- Likely, unproven: 0x00768B80 (landed CUnprovenRecord_00768b80, two CCharString default ctors) is ??0BasicParticleAndEndPoints
  or ??0ParticleAndEndPoints (identical bodies; FableWin lists both families) -- not in NAME_FIXES.

## Header debt
- engine/CParentDefClassBase.h (opt-in) declares `virtual ~CParentDefClassBase();` out of line (0x00430300). Retail's dtor
  stores no CParentDefClassBase vtable (it is the implicit one) and /O2 TUs inline it (0x0067C370 here, ~CControlsDef 0x004304A0);
  with the header the row calls 0x00430300 (probe 1, 27v49). 0x0067C370 declares CParentDefClassBase locally with the header's
  layout and pack(1) (0x39 bytes) and no dtor declaration. Fix idea: make the header's dtor implicit (or `virtual ~..() {}`
  plus checking that the dead vptr store is dropped) and re-test the 0x00430300 callers (/Os TUs) and its `??_G` 0x004303F0.
- No engine header for CBlurredAttitudesDef, NFire::CFirePoint, AttackParticleAndEndPoints, CSymbolMap, CRuntimeQuestPersistence,
  CBaseIntelligentPointer / CIntelligentPointer: local models with the Ego_r layouts.
