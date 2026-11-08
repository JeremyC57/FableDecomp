# Hand chunk 873 (CNavigatorAStar .. CAnimationEntry): 2 of 10 byte-exact

All rows grade with `FABLE_HDR_OVERLAY=$S/w22/agent873/hdr` (see hdr/README.md). q.py extents: the four multi-function rows were
graded against their true extent (q2.py CUT table in round 1; the coordinator cut the oracle rows in round 2).

## What matched (2)
- **CPlayer::InitInterfaces 0x00488D10** (RELOCATION_MATCH 4142v4142, test PASS g++ + VC): builds the 66 input processes from one
  16-byte CInputProcessGameBaseInit into `boost::scoped_ptr` members (`InputProcessX.reset(new CInputProcessX(init))`, statement
  order from retail, not member order). Shape: the init block filled World, DefinitionManager, MyPlayer, Component (other orders
  give one scheduling swap); scoped_ptr::reset = old read, new stored, old deleted through slot 0; each derived ctor inline after the
  out-of-line CInputProcessGameBase ctor 0x00687A30; CInputProcessInventoryBase's intermediate vtable store is dead and dropped by
  VC7.1 as in retail. Call targets checked: operator new x66, 0x00687A30 x65, 0x006A9140, 0x00A01B10 x4, malloc x2, 0x00A0E0A0;
  every vtable relocation names the RTTI class. No flag, no device.
- **CAnimationEntry::StatRegisterClass 0x005DD670** (RELOCATION_MATCH 5317v5317, test PASS g++ + VC; ~30 grades): registers 13
  animation-component prototypes in the token map and both CVectorMaps, then sorts the maps. Shape: one block per component through
  a local macro; `CIVCountedPointer<CAnimComponentBase> c(new T)` adopts (no AddRef); after the virtual SetDefaultValue VC7.1 drops
  every later null test on the pointer; `map.insert(std::make_pair(token, c))`; `CVectorMap::Insert(key, value)` in its PDB form
  (const K&, const V&) whose body makes the pair in its own statement (pair released before `Dirty = 1`). Device: Insert
  `__forceinline` (shared header, `#if defined(_MSC_VER)`; `// Inline-budget:` line): with it, VC7.1's caller budget expands
  push_back at exactly the first 17 of 26 sites and calls 0x005E3110 at the rest, as retail. Call targets/strings/vtables checked.

## Parked for Fable 5.1 (8)
Round 2 (coordinator cut the split rows to their true lengths; round-1 overlay merged, kept as hdr_landed_r1/): 0x00BE3970 worked
(19 grades); names established for 0x00A1E5C0 and 0x00804020; the other six rows still have no source (context budget).
- 00be3970 CLandscapePatchTesselator::TesselateEdge DIFFER(2399v2390): loop top -- retail `mov ecx,[esi+8] ; lea eax,[edi+edi*4] ; shl eax,2 ; add ecx,eax ; mov [esp+38h],ecx` (vertex pointer kept) then reloads Verts.Data for the start position; ours keeps Verts.Data in the slot and i*20 in a register, one extra frame dword, recoloured loop (udiff 198 lines, 13 opcode hunks) | tried: helpers forced inline (AddFanBaseVertex / AddDuplicateEdgeVertex / AddCompositeVertex / AddPoly, FindVert), local declaration orders (3), index init forms (=0, if/else, ternary), fan/found scope (2), `<= 1` else-first layout (big win), composite next vertex as vert[1] (second big win), pointer vs reference locals, three GetMainAxisVertexWorldPos spellings, vert declared after start/end, start read through vert | next: a source form that keeps `&strip->Verts[i]` as one pointer value (vert used before start/end, an accessor returning a pointer); GFFloatToLongNear is required at 0x00BE3DD0 / 0x00BE3E1F (bare dword fistp of a homed float), so a match grades retail_asm
- 00a1e5c0 CNavigatorAStar::CalculateAndMoveToPosProximity DIFFER(-v5437): not attempted | tried: none | next: model CNavigatorManager::GetNodeAt 0x00A14F70 / CNavigatorAStar::IsEqual 0x00A1A170 / list<CNavWayPoint>::push_back 0x00A22010; signature (const C3DVector&, const C3DVector&, float, float, C3DVector&, bool x7, float) returning ENavigatorReturn
- 00804020 CTCTargetingPlayer::UpdateTargetList DIFFER(-v5587): not attempted | tried: none | next: CBaseIntelligentPointer traffic (ctor/GetPItem/SetPItem/dtor 0x00A01B10/50/90/0x00A01C10), CThing::PeekPlayer / GetPlayer, std::__find 0x008066A0; engine/CTCTargetingPlayer.h exists
- 00839940 CThingGameObject::OnHit DIFFER(-v5195): not attempted | tried: none | next: 26 inline GetTC (LowerBound 0x0040F020) + IS_THING_ALIVE x15, CHitParameters, CTCFlammable::IsOnFire, CTCEntityEvents::AddEvent; engine/CThing.h FABLE_RETAIL_CTHING_GETTC_EXPANDED
- 00975bb0 CAIStateGroup_JackOfBladesStage1::Update DIFFER(-v4707): not attempted | tried: none | next: AI state machine (GetPItem x40, IsStillRunnable 0x00971B00 x11, Terminate x6, AIAction_PerformActionAndWait x4, CThing::GetPos x17)
- 0078fd60 CTCDivineWrath::FrameUpdate DIFFER(-v9268): not attempted | tried: none | next: largest row; profile first
- 005e4be0 CTCQuestCompletionUI::GetQuestDescription DIFFER(-v4939): not attempted (0x5E4D52..0x5E5090 read: per-reward blocks "PC_GOLD_ICON_AND_NAME" / "PC_RENOWN_ICON_AND_NAME": CreateComponent into a CCountedPointer<CComponent>, child "PC_QUEST_NAME" via slot +0xC, GetTextBySymbol + GFIntToWideString + CWideString operator+ chains, slot +0x240 text setter, parent slot +0xEC) | tried: none | next: landed siblings (CTCQuestCompletionUI_Activate_005e4740 ...) give the local-model style
- 00507c30 CWorldMap::LoadFromFile DIFFER(-v7661): not attempted | tried: none | next: keyword if/else chain over CStringParser::ReadAsSymbol / ReadAsFloat with CBasicString<char>::operator== (x48), _CxxThrowException x16

## Skipped
- none in round 2 (the four multi-function rows were cut to their first function; their extra starts are manifest boundary rows now).

## Label corrections
- 0x005DD670: catalog module `_global` -> `CAnimationEntry` (PDB ?StatRegisterClass@CAnimationEntry@@SIXXZ).
- 0x00488D10: reloc_bindings spells it public `?InitInterfaces@CPlayer@@QAEXXZ`; both PDBs say private `AAE` (NAME_FIXES.tsv).
- 0x013B8C1C / 0x013B8C28 / 0x005E29D0 / 0x005E3AD0: invented reloc_bindings spellings (NAME_FIXES.tsv).
- 0x00A1E5C0 `CNavigatorAStar_MoveTowardGoal` -> CNavigatorAStar::CalculateAndMoveToPosProximity; 0x00804020 `CViewBase_UpdatePlayerTargeting` -> CTCTargetingPlayer::UpdateTargetList (LABELS.tsv, NAME_FIXES.tsv).
